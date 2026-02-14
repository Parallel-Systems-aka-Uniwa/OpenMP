/*
 *  === File: omp.c ===
 *
 *  Full Name: Athanasiou Vasileios Evangelos
 *  Student ID: 19390005
 *  Degree Program: PADA
 *  
 *  Compilation: gcc -o omp omp.c -fopenmp
 *  Execution: ./omp A.txt B.txt
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// The number of threads is provided by the user
#define T 2
#define N 10
#define CZ 2

void create2DArray(int **Array, int size);
void print2DArray(FILE *fp, int **Array, int size);

int main(int argc, char *argv[]) 
{
    int **A;                                    // Matrix A to be tested for strict diagonal dominance
    int **B;                                    // Matrix B: Bij = m – |Aij| for i<>j and Bij = m for i=j 
    int *M;                                     // Array M to be used for the binary tree algorithm
    int threads, size, chunk;                   // Number of threads, matrix size, and chunk size (iterations per thread)
    FILE *fpA, *fpB;                            // Output files for storing matrices A and B
    int i, j, k;                                // Loop indices
    int rowSum;                                 // Sum of the elements of a row in matrix A
    int flag, tid;                              // Flag to check if A is strictly diagonally dominant, thread ID
    int loc_sum, loc_flag, loc_index, loc_min;  // Local variables for each thread
    int incr, temp0, temp1;                     // Variables for the binary tree algorithm
    int m;                                      // Maximum value on the diagonal of matrix A
    int min_val;                                // Minimum value of matrix B
    double all_time_start, all_time_end;        // Total execution time of the parallel program
    double loc_time_start, loc_time_end;        // Execution time for each parallel task

    /*
     *  Set the number of threads and open the output files 
     */
    // Confirm that the parameters are integers
    threads = T;
    size = N;
    chunk = CZ;

    omp_set_num_threads(threads);

    if (argc != 3) 
    {
        printf("Usage: %s A.txt B.txt\n", argv[0]);
        exit(1);
    }

    fpA = fopen(argv[1], "w");
    if (fpA == NULL) 
    {
        printf("Cannot open file %s\n", argv[1]);
        exit(1);
    }

    fpB = fopen(argv[2], "w");
    if (fpB == NULL) 
    {
        printf("Cannot open file %s\n", argv[2]);
        exit(1);
    }

    printf("Threads     : %d\n", threads);
    printf("Matrix size : %d x %d\n", size, size);
    printf("Chunk size  : %d\n", chunk);

    /*
     *  Memory allocation for matrices A, B and array M
     */
    A = (int **) malloc(size * sizeof(int *));
    B = (int **) malloc(size * sizeof(int *));
    for (i = 0; i < size; i++) 
    {
        A[i] = (int *) malloc(size * sizeof(int));
        if (A[i] == NULL) 
        {
            printf("Memory allocation failed for A[%d]\n", i);
            exit(1);
        }
        B[i] = (int *) malloc(size * sizeof(int));
        if (B[i] == NULL) 
        {
            printf("Memory allocation failed for B[%d]\n", i);
            exit(1);
        }
    }
    M = (int *) malloc(threads * sizeof(int));
    if (M == NULL) 
    {
        printf("Memory allocation failed for M\n");
        exit(1);
    }

    /*
     *  Initializations:
     * 
     *  flag to check if A is strictly diagonally dominant,
     *  and the matrix A itself.
     */
    flag = 1;

    create2DArray(A, size);
    print2DArray(fpA, A, size);

    /*
     *  Initialize total execution time measurement for all parallel sub-tasks:
     *  a, b, c, d1, d2.1, d2.2
     */
    all_time_start = 0.0;
    all_time_end = 0.0;

// ========================================== [Start of Parallel Computation] ========================================== 

    /*
     *  === Task: a === 
     *  Check (in parallel) if matrix A is strictly diagonally dominant 
     */
    // --------- Start timing for sub-task a ---------  
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;   
    
    printf("=================================== [Task a.] ===================================\n");
    #pragma omp parallel shared(flag) private(i, j, loc_sum, loc_flag, loc_index)
    {
        loc_flag = 1;

        // Parallelize the check of matrix A with a for-loop schedule
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < size; i++)
        {
            loc_sum = 0;

            for (j = 0; j < size; j++)
                if (i != j)
                    loc_sum += abs(A[i][j]); 
                else     
                    loc_index = abs(A[i][i]); 

            // If the strictly diagonally dominant property does not hold,
            // set loc_flag to 0 so that the shared variable flag is updated accordingly
            if (loc_index <= loc_sum)
                loc_flag = 0;
        }

        // Critical section to update the shared variable flag
        #pragma omp atomic
        flag *= loc_flag;            
    }
    
    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- End timing for sub-task a ---------

    printf("Is A strictly diagonal dominant?\n");
    if (!flag)
    {
        printf("NO\n");
        printf("The array has been stored in file %s\n", argv[1]);

        printf("\n--------------------------------------------\n");
        printf("Task a. finished in %lf sec.\n", loc_time_end - loc_time_start);
        printf("--------------------------------------------\n");
        printf("=================================================================================\n");
        
        // Print total execution time of the parallel program
        printf("\n--------------------------------------------\n");
        printf("Parallel program finished in %lf sec.\n", all_time_end - all_time_start);
        printf("--------------------------------------------\n");
        
        exit(0);
    }

    // Matrix A is strictly diagonally dominant
    printf("YES\n");
    printf("The array has been stored in file %s\n", argv[1]);

    printf("--------------------------------------------\n");
    printf("Task a. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("=================================================================================\n");

    /*
     *  === Task: b === 
     *  Compute in parallel m = max(|Aii|) for i = 0...N-1 
     */
    m = A[0][0]; // Initialize m with the first diagonal element of A

    // --------- Start timing for sub-task b --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task b.] ===================================\n");
    #pragma omp parallel default(shared) private(i)
    {
        // Compute m using reduction clause
        #pragma omp for schedule(static, chunk) reduction(max : m)
        for (i = 0; i < size; i++)
            if (A[i][i] > m)
                m = A[i][i];
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- End timing for sub-task b ---------

    printf("m = max(|Aii|) =>\n");
    printf("m = %d\n", m);

    printf("--------------------------------------------\n");
    printf("Task b. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("=================================================================================\n");


    /*
     *  === Task: c === 
     *  Create in parallel a new matrix B with elements:
     *  Bij = m - |Aij| for i <> j, and Bij = m for i = j  
     */
    // --------- Start timing for sub-task c --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task c.] ===================================\n");
    #pragma omp parallel default(shared) private(i, j)
    {
        // The collapse(2) directive merges the nested for-loops into one, equivalent to
        // for (i = 0; i < N * N; i++) { ... } and the scheduling is done using for schedule
        #pragma omp for schedule(static, chunk) collapse(2)
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                if (i == j)
                    B[i][j] = m;
                else
                    B[i][j] = m - A[i][j];
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- End timing for sub-task c ---------

    print2DArray(fpB, B, size);
    printf("Bij = m - |Aij| for i <> j and Bij = m for i = j\n");
    printf("The array has been stored in file %s\n", argv[2]);
    
    printf("--------------------------------------------\n");
    printf("Task c. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("==================================================================================\n");

    /*
     *  === Task: d === 
     *  Compute in parallel the minimum element of matrix B: min_val = min(|Bij|)
     *      
     *      === Task: d1 ===
     *      using reduction 
     */   
    min_val = B[0][0]; // Initialize min_val with the first element of B

    // --------- Start timing for sub-task d1 --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task d1.] ===================================\n");
    #pragma omp parallel default(shared) private(i, j)
    {
        // Compute min_val using reduction clause
        #pragma omp for schedule(static, chunk) reduction(min : min_val)
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                if (B[i][j] < min_val)
                    min_val = B[i][j];
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- End timing for sub-task d1 ---------

    printf("With reduction\n");
    printf("m = min(|Bij|) =>\n");
    printf("m = %d\n", min_val);
    
    printf("--------------------------------------------\n");
    printf("Task d1. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("===================================================================================\n");

    /*
     *  === Task: d === 
     *  Compute in parallel the minimum element of matrix B: min_val = min(|Bij|)
     *      
     *      === Task: d2 ===
     *      without using reduction
     * 
     *          === Task: d2.1 ===
     *          with critical section protection  
     */  
    min_val = B[0][0]; // Reinitialize min_val with the first element of B

    // --------- Start timing for sub-task d2.1 --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task d2.1] ===================================\n");
    #pragma omp parallel shared(min_val) private(i, j)
    {
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                if (B[i][j] < min_val)
                {
                    // Critical section to update the shared variable min_val
                    #pragma omp critical (inc_min_val)
                    {
                        min_val = B[i][j];
                    }
                }
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- End timing for sub-task d2.1 ---------

    printf("With critical section\n");
    printf("m = min(|Bij|) =>\n");
    printf("m = %d\n", min_val);
    
    printf("--------------------------------------------\n");
    printf("Task d2.1 finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("===================================================================================\n");

    /*
     *  === Task: d === 
     *  Compute in parallel the minimum element of matrix B: min_val = min(|Bij|)
     *      
     *      === Task: d2 ===
     *      without using reduction
     * 
     *          === Task: d2.2 ===
     *          using the binary tree algorithm	  
     */ 
    min_val = B[0][0]; // Reinitialize min_val with the first element of B

    // --------- Start timing for sub-task d2.2 ---------
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task d2.2] ===================================\n");
    #pragma omp parallel default(shared) private(tid, i, j, incr, temp0, temp1, loc_min)
    {
        tid = omp_get_thread_num();
        loc_min = 1000000;

        // Each thread computes the local minimum of matrix B and stores it in M[tid]
        // where tid is the thread ID 
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                if (B[i][j] < loc_min)
                    loc_min = B[i][j];

        // Store the local minimum in the corresponding position of array M
        M[tid] = loc_min;

        // Synchronize threads for each phase of the binary tree algorithm
        #pragma omp barrier

        // Initialize the loop index for the binary tree algorithm
        incr = 1;
        
        // Begin phases of the binary tree algorithm
        while (incr < threads)
        {
            // Since we cannot use critical section mechanisms here,
            // we ensure that the correct thread performs the comparison for its assigned elements.
            // This is achieved using the conditions: tid % (2 * incr) == 0 and tid + incr < T
            if (tid % (2 * incr) == 0 && tid + incr < threads) 
            {
                temp0 = M[tid];
                temp1 = M[tid + incr];
                loc_min = (temp0 <= temp1) ? temp0 : temp1;
                M[tid] = loc_min;
            }

            // Synchronize threads for the next phase of the binary tree algorithm
            #pragma omp barrier 

            // Increase the loop index for the binary tree algorithm
            incr = 2 * incr;
        }
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- End timing for sub-task d2.2 ---------
    
    // The minimum element of matrix B is found using the binary tree algorithm.
    // It is stored in M[0] of array M.
    printf("Binary Tree Algorithm\n");
    printf("m = min(|Bij|) =>\n");
    printf("m = %d\n", M[0]);

    printf("--------------------------------------------\n");
    printf("Task d2.2 finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("===================================================================================\n");
    
// ========================================== [End of Parallel Computation] ==========================================
    
    // Print total execution time of the parallel program
    printf("\n--------------------------------------------\n");
    printf("Parallel program finished in %lf sec.\n", all_time_end - all_time_start);
    printf("--------------------------------------------\n");

    /*
     *  Close files and free memory
     */
    fclose(fpA);
    fclose(fpB);

    for (i = 0; i < size; i++) 
    {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    free(M);

    return 0;
}

/*
 *  === Function: create2DArray ===
 *  Parameters: Pointer to an array of dimension N containing integers, matrix size  
 *  Returns: Nothing
 * 
 *  Function that creates a random N x N matrix where, depending on the result of rand() % 2,
 *  the matrix is either strictly diagonally dominant or not.
 *  The matrix is returned by reference.
 */
void create2DArray(int **Array, int size)
{
    int i, j;
    int rowSum;

    srand(time(NULL));
    
    // Randomly decide if the matrix will be strictly diagonally dominant or not.
    // Values in the interval [0, 1]
    if (rand() % 2) 
    {
        // Strictly diagonally dominant
        for (i = 0; i < size; i++) 
        {
            rowSum = 0;
            for (j = 0; j < size; j++) 
            {
                if (i == j) 
                {
                    Array[i][i] = rand() % 21 - 10; // Diagonal elements. Values in the range [-10, 10]
                    Array[i][i] = Array[i][i] >= 0 ? Array[i][i] + 20 : Array[i][i] - 20; // Randomly adjust sign
                } 
                else 
                {
                    Array[i][j] = rand() % 21 - 10; // Off-diagonal elements. Values in the range [-10, 10]
                    rowSum += abs(Array[i][j]);
                }
            }
            // Ensure the strictly diagonally dominant property:
            // |Aii| > Σ|Aij|, j=0...N-1, j<>i
            if (rowSum >= abs(Array[i][i])) 
            {
                Array[i][i] = rowSum + rand() % 5 + 1; // Adjust the diagonal element so the property holds
                Array[i][i] *= (rand() % 2 == 0) ? 1 : -1; // Randomly choose the sign
            }
        }
    } 
    else
    {
        // Not strictly diagonally dominant
        for (i = 0; i < size; i++) 
        {
            rowSum = 0;
            for (j = 0; j < size; j++)
            {
                if (i == j) 
                {
                    Array[i][i] = rand() % 11 - 5; // Diagonal elements. Values in the range [-5, 5]
                } 
                else 
                {
                    Array[i][j] = rand() % 21 - 10; // Off-diagonal elements. Values in the range [-10, 10]
                    rowSum += abs(Array[i][j]);
                }
            }
            // Ensure the matrix is NOT strictly diagonally dominant:
            // |Aii| <= Σ|Aij|, j=0...N-1, j<>i
            if (abs(Array[i][i]) > rowSum) 
            {
                Array[i][i] = rowSum - rand() % 5 - 1; // Adjust the diagonal element so the property does NOT hold
                Array[i][i] *= (rand() % 2 == 0) ? 1 : -1; // Randomly choose the sign
            }
        }
    }
}

/*
 *  === Function: print2DArray ===
 *  Parameters: Pointer to an output file, N x N matrix of integers, matrix size
 *  Returns: Nothing
 * 
 *  Function that prints an N x N matrix to an output file.
 */
void print2DArray(FILE *fp, int **Array, int size)
{
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
            fprintf(fp, "%4d ", Array[i][j]);
        fprintf(fp, "\n");
    }
}
