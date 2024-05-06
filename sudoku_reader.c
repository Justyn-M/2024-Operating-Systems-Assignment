#include <stdio.h>
#include "sudoku.h"

// Function that reads a sudoku from input file
void read_sudoku(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    for (int i = 0; i < GRID; i++) {
        for (int j = 0; j < GRID; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }

    fclose(file);

}

