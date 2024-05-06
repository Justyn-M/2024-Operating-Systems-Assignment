#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "sudoku.h"

// Declaring the sudoku grid and counter
extern int sudoku[GRID][GRID];
extern int Row[GRID], Col[GRID], Sub[GRID];
extern int Counter;

// Declaring thread synchronisation and thread functions
extern int Last_Thread_ID;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern int thread_State[TOTAL_THREADS];
extern int finished_threads;

// Function to validate the grid
bool grid_validator(int *set) {
    bool seen[GRID + 1] = {0};
    for (int i = 0; i < GRID; i++) {
        if (seen[set[i]] || set[i] < 1 || set[i] > GRID)
            return false;
        seen[set[i]] = true;
    }
    return true;
}

// Function to validate sudoku rows and subgrid
void* row_subgrid_validator(void* param) {
    // Thread Index
    int index = *(int*)param;
    int row_start = index * 3;
    bool _valid = true;

    // Validating rows
    for (int i = row_start; i < row_start + 3; i++) {
        if (grid_validator(sudoku[i])) {
            pthread_mutex_lock(&mutex);
            Row[i] = 1;
            Counter++;
            pthread_mutex_unlock(&mutex);
        }
        else {
            _valid = false;
            break;
        }
    }

    // Validating subgrids start here
    for (int x = index * 3; x < (index + 1) * 3; x++) {
        // Subgrid starting row and column
        int xRow = (x / 3) * 3;
        int xCol = (x % 3) * 3;
        int subgrid[GRID];
        int p = 0;

        // Extracting subgrid
        for (int i = xRow; i < xRow + 3; i++) {
            for (int j = xCol; j < xCol + 3; j++) {
                subgrid[p++] = sudoku[i][j];
            }
        }
        // subgrid validation
        if (grid_validator(subgrid)) {
            pthread_mutex_lock(&mutex);
            Sub[x] = 1;
            Counter++;
            pthread_mutex_unlock(&mutex);
        }
        else {
            _valid = false;
            break;
        }
    }

    // Thread state update
    pthread_mutex_lock(&mutex);
    thread_State[index] = _valid ? 1 : 0;
    finished_threads++;

// if all threads are finished, signal the main thread to wake it up
    if (finished_threads == TOTAL_THREADS) {
        Last_Thread_ID = index + 1;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);

// Let the thread exit
    return NULL;
}

void* col_validator(void* param) {
    // delay sleep time that happens after thread finshes validatoin
    int delay = *(int*)param;
    bool _valid = true;

    // Validating columns
    for (int i = 0; i < GRID; i++) {
        int col[GRID];
        for (int j = 0; j < GRID; j++) {
            col[j] = sudoku[j][i];
        }
        // column validation
        if (grid_validator(col)) {
            pthread_mutex_lock(&mutex);
            Col[i] = 1;
            Counter++;
            pthread_mutex_unlock(&mutex);
        }
        // if column is invalid, break loop
        else {
            _valid = false;
            break;
        }
        // Sleep for delay time
        sleep(delay);
    }

    // Thread state update
    pthread_mutex_lock(&mutex);
    thread_State[TOTAL_THREADS - 1] = _valid ? 1 : 0;
    finished_threads++;
    // if all threads are finished, signal the main thread to wake it up
    if (finished_threads == TOTAL_THREADS) {
        Last_Thread_ID = 4;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);

 // Thread Exit
    return NULL;
}

