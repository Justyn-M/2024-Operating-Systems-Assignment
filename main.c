#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mssv.h"
#include "sudoku_reader.h"

// Sudoku grid and counter
int sudoku[GRID][GRID];
int Row[GRID], Col[GRID], Sub[GRID];
int Counter = 0;

// Thread States
int Last_Thread_ID = 0;
int thread_State[TOTAL_THREADS] = {0};
int finished_threads = 0;

// Mutex and Condition
pthread_mutex_t mutex;
pthread_cond_t cond;

// MAIN FUNCTION
int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        // Printing usage if the number of arguments is incorrect
        printf("Usage: %s <filename> <delay> \n", argv[0]);
        return 1;
    }

    int delay = atoi(argv[2]);
    read_sudoku(argv[1]);

    // Initialize mutex and condition
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create threads
    pthread_t threads[TOTAL_THREADS];
    for (int i = 0; i < TOTAL_THREADS; i++)
    {
        if (i < TOTAL_THREADS - 1)
        {                                                                              // cannot do &i below because can lead to race condition
            pthread_create(&threads[i], NULL, row_subgrid_validator, (void *)(long)i); // (void *)(long)i) pass i straight to the thread
        }
        else
        {
            pthread_create(&threads[i], NULL, col_validator, (void *)&delay);
        }
    }

    //   Wait for all threads to finish
    pthread_mutex_lock(&mutex);
    while (finished_threads < TOTAL_THREADS)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Join threads
    for (int i = 0; i < TOTAL_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Output results to terminal
    for (int i = 0; i < TOTAL_THREADS; i++)
    {
        if (thread_State[i] == 1)
        {
            printf("Thread %d: Valid\n", i + 1);
        }
        else
        {
            printf("Thread %d: Invalid\n", i + 1);
            if (i == 3)
            {
                // Check for invalid columns
                for (int j = 0; j < GRID; j++)
                {
                    if (Col[j] == 0)
                    {
                        printf("Column %d ", j + 1);
                    }
                }
            }

            else
            {
                // Check for invalid Rows and Subgrids
                for (int j = i * 3; j < (i + 1) * 3; j++)
                {
                    if (Row[j] == 0)
                    {
                        printf("Row %d ", j + 1);
                    }
                }
                for (int j = 0; j < GRID; j++)
                {
                    if (Sub[j] == 0)
                    {
                        printf("Subgrid %d", j + 1);
                    }
                }
            }
            // Print 'are invalid' after the invalid Row, Cols & Subgs
            printf(" are invalid\n");
        }
    }

    //  Print the final result
    printf("There are %d valid sub-grids, and thus the solution is ", Counter);
    // If there are 27 valid sub-grids, then the solution is valid
    if (Counter == 27)
    {
        printf("valid.\n");
    }
    // Otherwise, sudoku solution is invalid
    else
    {
        printf("invalid.\n");
    }

    // Print the last thread ID
    printf("Thread ID-%d is the last thread.\n", Last_Thread_ID);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    // Exit the program
    return 0;
}