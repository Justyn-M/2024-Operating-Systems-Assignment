#ifndef MSSV_H
#define MSSV_H

#include <pthread.h>

#define GRID 9
#define TOTAL_THREADS 4

extern int Sudoku[GRID][GRID];
extern int Row[GRID], Col[GRID], Sub[GRID];
extern int Counter;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

void* row_subgrid_validator(void* param);
void* col_validator(void* param);

#endif
