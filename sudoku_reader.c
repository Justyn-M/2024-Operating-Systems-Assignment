#include <stdio.h>
#include "mssv.h"


// Function that reads a sudoku from input file
void read_sudoku(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            fscanf(file, "%d", &Sudoku[i][j]);
        }
    }

    fclose(file);
}

