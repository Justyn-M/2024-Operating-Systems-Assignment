#ifndef SUDOKU_H
#define SUDOKU_H

// pthread library
#include <pthread.h>

#define GRID 9
#define TOTAL_THREADS 4

// Sudoku grid size
extern int sudoku[GRID][GRID];

// Arrays to store the status of each row, column and subgrid
extern int Row[GRID], Col[GRID], Sub[GRID];

// Counter
extern int Counter;

// Thread synchronization mutex and condition
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

// Thread functions
void* row_subgrid_validator(void* param);
void* col_validator(void* param);

#endif