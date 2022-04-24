#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#define IN_FILE "0.matrix"
#define OUT_FILE "1.matrix"

/*
Checks and returns count of alive/dead neighbors for givin cell

usage: check_neighbors(cells, x, y)
Parameters:
    cells       - IN - the matrix
    x           - IN - the X positon of the cell to check
    y           - IN - the Y positon of the cell to check
    m           - IN - the width of the matrix
    n           - IN - the height of the matrix
Returns:
    neighbors           - OUT - a count of alive and dead neighbors(index 0 is for alive and index 1 is for dead)
*/
void check_neighbors(Cell* prev, Cell* curr, int x, int cols, int j){
    curr = prev;
    int i, t;
    int alive = 0;
    int length = sizeof(prev)/sizeof(prev[0]);
    //Loops should check all eight neighbors of the cell starting at the bottom right, checking all in the row and moving up
    for (i = 0; i < 2; i++){
        for (t = 0; t < 2; t++){
            if ((x + 1 - t)*cols + (j + 1 - i) < length && (t != 1 && i != 1)){
                if (curr[(x + 1 - t)*cols + (j + 1 - i)].state == 1)
                {
                    alive++;
                }
            }
        }
    }

    //Check rules
    if(curr[(x)*cols + (j)].state == 1){
        if(alive != 2 || alive != 3){
            curr[(x)*cols + (j)].state = 0;
        }
    }else{
        if(alive == 3){
            curr[(x)*cols + (j)].state = 1;
        }
    }

   return;
}

// CHECK INDEXING!!!
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

int main(int argc, char* argv[]){
    int NTHREADS = omp_get_num_threads();
    int ITERATIONS = 3;
    int rows, cols, iter, i, j;
    char intToString[2];//for converting int to strong
    char outPath[] = ".out";
    Cell* previous = read_matrix("initial.matrix", &rows, &cols);
    Cell* current = (Cell*) malloc(rows * cols * sizeof(Cell));
    Cell* temp = (Cell*) malloc(rows * cols * sizeof(Cell));
    int length = sizeof(previous)/sizeof(previous[0]);
    
    int ID = omp_get_thread_num();
    int p=omp_get_num_threads();
             
    int div = ceil(length/p);

    //make sub array

    for(iter = 0; iter<ITERATIONS; iter++){
    #pragma omp parallel for collapse(2)
        for (i = 0; i < rows; i++){
            for (j = 0; j < cols; j++){
                check_neighbors(previous, current, i, cols, j);
            }
        }
        temp = previous;
        previous = current;
        current = temp;
        char output[] = "out/final";
        sprintf(intToString, "%d", iter);
        strcat(output,intToString);
        strcat(output,outPath);
        write_matrix(output, rows, cols, current);
    }

    
    

    write_matrix("final.matrix", rows, cols, current);

    return 0;
}