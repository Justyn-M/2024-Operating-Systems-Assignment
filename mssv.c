#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "mssv.h"

// Declaring the sudoku grid and counter
extern int Sudoku[GRID][GRID];
extern int Row[GRID], Col[GRID], Sub[GRID];
extern int Counter;

// Declaring thread synchronisation and thread functions
extern int Last_Thread_ID;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern int thread_State[TOTAL_THREADS];
extern int finished_threads;

// Function to validate the grid
bool validate_set(int *set) {
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
    int idx = (int)(long)param;
    int rowStart = idx * 3;
    bool isValid = true;

    // Validating rows
    for (int i = rowStart; i < rowStart + 3; i++) {
        if (validate_set(Sudoku[i])) {
            pthread_mutex_lock(&mutex);
            Row[i] = 1;
            Counter++;
            pthread_mutex_unlock(&mutex);
        } else {
            isValid = false;
        }
    }

    // Validating subgrids start here
    for (int sg = idx * 3; sg < (idx + 1) * 3; sg++) {
        // Subgrid starting row and column
        int sgRow = (sg / 3) * 3;
        int sgCol = (sg % 3) * 3;
        int subgrid[GRID];
        int k = 0;

        // Extracting subgrid
        for (int i = sgRow; i < sgRow + 3; i++) {
            for (int j = sgCol; j < sgCol + 3; j++) {
                subgrid[k++] = Sudoku[i][j];
            }
        }
        // subgrid validation
        if (validate_set(subgrid)) {
            pthread_mutex_lock(&mutex);
            Sub[sg] = 1;
            Counter++;
            pthread_mutex_unlock(&mutex);
        } else {
            isValid = false;
        }
    }
// Thread state update
    pthread_mutex_lock(&mutex);
    thread_State[idx] = isValid ? 1 : 0;
    finished_threads++;

    // if all threads are finished, signal the main thread to wake it up
    if (finished_threads == TOTAL_THREADS) {
        Last_Thread_ID = idx + 1;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);

    // Let the thread exit
    return NULL;
}

void* col_validator(void* param) {
        // delay sleep time that happens after thread finshes validatoin
    int delay = *(int*)param;
    bool isValid = true;
    // Validating columns
    for (int i = 0; i < GRID; i++) {
        int column[GRID];
        for (int j = 0; j < GRID; j++) {
            column[j] = Sudoku[j][i];
        }
        // column validation
        if (validate_set(column)) {
            pthread_mutex_lock(&mutex);
            Col[i] = 1;
            Counter++;
            pthread_mutex_unlock(&mutex);
        } else {
            isValid = false;
        }
        // Sleep for delay time
        sleep(delay);
    }

    // Thread state update
    pthread_mutex_lock(&mutex);
    thread_State[3] = isValid ? 1 : 0;
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
