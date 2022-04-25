#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define IN_FILE "Glider.matrix"
#define OUT_NAME "out/Glider"
#define ITERATIONS 100


/*
identify if coords are valid within matrix

usage: is_valid(x, y, rows, cols)
Parameters:
    x           - IN - the X positon of the cell to check
    y           - IN - the Y positon of the cell to check
    cols           - IN - the width of the matrix
    rows           - IN - the height of the matrix
Returns:
    True - if coords are within the matrix
    False - if coords are outside of matrix 
*/
int is_valid(int x, int y, int rows, int cols){
    return 0 <= x && x < rows && 0 <= y && y <= cols; 
}

/*
Checks and updates cells that are to remain alive, become alive, stay dead, or become dead

usage: check_neighbors(previous, current, i, j, cols, rows);
Parameters:
    prev       - IN - the matrix
    curr       - IN - the matrix
    x           - IN - the X positon of the cell to check
    y           - IN - the Y positon of the cell to check
    cols           - IN - the width of the matrix
    rows           - IN - the height of the matrix
*/
void check_neighbors(Cell* prev, Cell* curr, int x, int y, int cols, int rows){
    int id = omp_get_thread_num();
    int i, j, index, nx, ny;
    int alive = 0;
    int length = cols * rows;
    //Loops should check all eight neighbors of the cell starting at the bottom right, checking all in the row and moving up
    for (i = -1; i <= 1; i++){
        for (j = -1; j <= 1; j++){
            nx = (x - i);
            ny = (y - j);
            index = nx*cols + ny;

            if (is_valid(nx, ny, rows, cols) && !(j == 0 && i == 0)){
                alive += (prev[index].state == DEAD)?0:1; // Relies on ALIVE being 1, dead being 0
            }
        }
    }

    // Check rules
    if(prev[x*cols + y].state == ALIVE){
        curr[x*cols + y].state = (alive == 2 || alive == 3);
    }else{
        curr[x*cols + y].state = alive == 3;
    }
    return;
}

/*
Read a matrix file containing initial data. 1 for alive, 0 for dead.

usage: read_matrix(filepath, cells, m, n)
Parameters:
    filepath    - IN  - A string containing the filepath
    m           - OUT - the width of the matrix
    n           - OUT - the height of the matrix
Returns:
    A new cell array
*/
Cell* read_matrix(const char* filepath, int* rows, int* cols){
    FILE *fp;
    int m, n, i, j;

    fp = fopen(filepath, "r");
    fscanf(fp, "%d %d", rows, cols);
    m = *cols;
    n = *rows;


    Cell* cells = (Cell*) malloc(m * n * sizeof(Cell));

    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){

            fscanf(fp, "%d", (int*) &cells[i*m + j].state);
            cells[i*m + j].x = i;
            cells[i*m + j].y = j;
        }
    }

    return cells;
}

/*
Write a matrix file containing initial data. 1 for alive, 0 for dead.

usage: read_matrix(filepath, cells, m, n)
Parameters:
    filepath    - IN  - A string containing the filepath
    m           - IN - the width of the matrix
    n           - IN - the height of the matrix
    cells       - IN - the matrix
*/
void write_matrix(const char* filepath, int m, int n, Cell* cells){
    int i, j;
    FILE *fp;
    fp = fopen(filepath, "w");
    fprintf(fp, "%d %d\n", m, n);

    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            fprintf(fp, "%d ", cells[i*m + j].state);
        }
        fprintf(fp, "\n");
    }
}


void print_matrix(int m, int n, Cell* cells){
    int i, j;

    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            printf("%d ", cells[i*m + j].state);
        }
        printf("\n");
    }

}

/*
Generate a file name given the iteration number

usage: get_name(iter, filename)
Parameters:
    iter       - IN  - the suffix of the file
    filename   - OUT - An empty buffer
Results:
    filename holds a generated file name
*/
void get_name(int iter, char* filename){
    filename[0] = 0; // Clear the buffer
    char str_int[9]; // Means that at most 10^8 iterations can be done before buffer overflow
    sprintf(filename, OUT_NAME);
    sprintf(str_int, "%d", iter);
    strcat(filename, str_int);
    strcat(filename, ".matrix");
}


int main(int argc, char* argv[]){
    int rows, cols, iter, i, j;
    char outpath[100];

    Cell* previous = read_matrix(IN_FILE, &rows, &cols);
    Cell* current = (Cell*) malloc(rows * cols * sizeof(Cell));
    Cell* temp;
    
    get_name(0, outpath);
    write_matrix(outpath, rows, cols, previous);

    for(iter = 1; iter <= ITERATIONS; iter++){
        #pragma omp parallel for collapse(2)
        for (i = 0; i < rows; i++){
            for (j = 0; j < cols; j++){
                check_neighbors(previous, current, i, j, cols, rows);
            }
        }
    
        get_name(iter, outpath);
        write_matrix(outpath, rows, cols, current);

        temp = previous;
        previous = current;
        current = temp;
    }

    free(previous);
    free(current);
    return 0;
}