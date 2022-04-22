#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>

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
void check_neighbors(Cell* cells, int x, int y, int m, int n){
    static int neighbors[2];
    int alive = 0;
    int dead = 0;
    int length = sizeof(cells)/sizeof(cells[0]);
    //Loops should check all eight neighbors of the cell starting at the bottom right, checking all in the row and moving up
    for (int i = 0; i < 2; i++){
        for (int t = 0; t < 2; t++){
            if ((x + 1 - t)*m + (y + 1 - i) < length && (t != 1 && i != 1){
                if (cells[i*m + j].state == 1)
                {
                    alive++;
                }
                else
                {
                    dead++;
                }
                
            }
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
    int m, n;

    fp = fopen(filepath, "r");
    fscanf(fp, "%d %d", rows, cols);
    m = *cols;
    n = *rows;


    Cell* cells = (Cell*) malloc(m * n * sizeof(Cell));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){

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
    FILE *fp;
    fp = fopen(filepath, "w");
    fprintf(fp, "%d %d\n", m, n);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            fprintf(fp, "%d ", cells[i*m + j].state);
        }
        fprintf(fp, "\n");
    }
}

int main(int argc, char* argv[]){
    int rows, cols;
    Cell* data = read_matrix("initial.matrix", &rows, &cols);

    write_matrix("final.matrix", rows, cols, data);

    return 0;
}