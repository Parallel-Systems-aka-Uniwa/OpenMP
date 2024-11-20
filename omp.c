/*
 *  === Αρχείο: omp.c ===
 *
 *  Ονοματεπώνυμο: Αθανασίου Βασίλειος Ευάγγελος
 *  Αριθμός Μητρώου: 19390005
 *  Πρόγραμμα Σπουδών: ΠΑΔΑ
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define CZ 2
#define N 10
// σσ2. Τον αριθμό των threads τον δίνει ο χρήστης
#define T 6

void print2DArray(FILE *fp, int Array[N][N]);

int main(int argc, char *argv[]) 
{
    int A[N][N];                                // Ο πίνακας Α που θα εξετάσουμε αν είναι αυστηρά διαγώνια δεσπόζων
    int B[N][N];                                // Bij = m – |Aij| για i<>j και Bij = m για i=j 
    int M[T];                                   // Ο πίνακας Μ που θα χρησιμοποιήσουμε για τον αλγόριθμο δυαδικού δένδρου
    FILE *fpA, *fpB;                            // Αρχεία εξόδου για την αποθήκευση των πινάκων Α και Β
    int i, j;                                   // Δείκτες επανάληψης
    int rowSum;                                 // Άθροισμα των στοιχείων μιας γραμμής του πίνακα Α
    int chunk, flag, tid;                       // Αριθμός επαναλήψεων ανά thread, Είναι ή δεν είναι ο Α αυστηρά διαγώνια δεσπόζων, Αναγνωριστικό thread
    int loc_sum, loc_flag, loc_index, loc_min;  // Τοπικές μεταβλητές για κάθε thread
    int incr, temp0, temp1;                     // Μεταβλητές για τον αλγόριθμο δυαδικού δένδρου
    int m;                                      // Μέγιστη τιμή της διαγωνίου του πίνακα Α
    int min_val;                                // Ελάχιστη τιμή του πίνακα Β
    double all_time_start, all_time_end;        // Χρόνος εκτέλεσης του παράλληλου προγράμματος
    double loc_time_start, loc_time_end;        // Χρόνος εκτέλεσης της κάθε παράλληλης εργασίας

/*
 *  Ορισμός του αριθμού των threads και άνοιγμα των αρχείων εξόδου 
 */
    omp_set_num_threads(T);

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

/*
 *  Αρχικοποίησεις 
 *  του αριθμού επαναλήψεων ανά thread, 
 *  του flag που ελέγχει αν ο Α είναι αυστηρά διαγώνια δεσπόζων,
 *  του πίνακα Α
 */
    srand(time(NULL));

    chunk = CZ;
    flag = 1;
    if (rand() % 2) {
        // Strictly Diagonal Dominant
        for (i = 0; i < N; i++) {
            rowSum = 0;
            for (j = 0; j < N; j++) {
                if (i == j) {
                    A[i][j] = rand() % 21 - 10; // Diagonal element (-10 to 10)
                    A[i][j] = A[i][j] >= 0 ? A[i][j] + 20 : A[i][j] - 20; // Ensure larger magnitude
                } else {
                    A[i][j] = rand() % 21 - 10; // Off-diagonal element (-10 to 10)
                    rowSum += abs(A[i][j]);
                }
            }
            // Ensure diagonal element is greater than sum of other row elements
            if (abs(A[i][i]) <= rowSum) {
                A[i][i] = rowSum + rand() % 5 + 1; // Adjust to maintain strict diagonal dominance
                A[i][i] *= (rand() % 2 == 0) ? 1 : -1; // Randomly keep it positive or negative
            }
        }
    } else {
        // NOT Strictly Diagonal Dominant
        for (i = 0; i < N; i++) {
            rowSum = 0;
            for (j = 0; j < N; j++) {
                if (i == j) {
                    A[i][j] = rand() % 11 - 5; // Diagonal element (-5 to 5)
                } else {
                    A[i][j] = rand() % 21 - 10; // Off-diagonal element (-10 to 10)
                    rowSum += abs(A[i][j]);
                }
            }
            // Ensure diagonal element is smaller than or equal to sum of other row elements
            if (abs(A[i][i]) > rowSum) {
                A[i][i] = rowSum - rand() % 5 - 1; // Adjust to violate strict diagonal dominance
                A[i][i] *= (rand() % 2 == 0) ? 1 : -1; // Randomly keep it positive or negative
            }
        }
    }

    print2DArray(fpA, A);

/*
 *  Έναρξη μέτρησης χρόνου εκτέλεσης του παράλληλου προγράμματος
 */
    all_time_start = omp_get_wtime();
    
/*
 *  a. Να ελέγχει (παράλληλα) αν ο πίνακας Α είναι αυστηρά διαγώνια δεσπόζων
 */  
    loc_time_start = omp_get_wtime();   
    
    #pragma omp parallel shared(flag) private(i, j, loc_sum, loc_flag, loc_index)
    {
        loc_flag = 1;

        #pragma omp for schedule(static, chunk)
        for (i = 0; i < N; i++)
        {
            loc_sum = 0;

            for (j = 0; j < N; j++)
                if (i != j)
                    loc_sum += abs(A[i][j]); 
                else     
                    loc_index = abs(A[i][j]); 
                       
            if (loc_index <= loc_sum)
                loc_flag = 0;
        }

        #pragma omp atomic
        flag *= loc_flag;            
    }
    
    loc_time_end = omp_get_wtime();
    // --------- Parallel Finish ---------

    // σσ1. δεν ισχύει το a.
    if (!flag)
    {
        printf("Not Hooray :(\n");
        printf("Task a. finished in %lf \n", loc_time_end - loc_time_start);
        exit(0);
    }

    printf("Hooray :D\n");
    printf("Task a. finished in %lf \n", loc_time_end - loc_time_start);

    // b. m = max(|Aii|), i = 0...N-1
    m = A[0][0];

    loc_time_start = omp_get_wtime();

    #pragma omp parallel default(shared) private(i)
    {
        #pragma omp for schedule(static, chunk) reduction(max : m)
        for (i = 0; i < N; i++)
            if (A[i][i] > m)
                m = A[i][i];
    }

    loc_time_end = omp_get_wtime();
    // --------- Parallel Finish ---------

    printf("max = %d\n", m);
    printf("Task b. finished in %lf \n", loc_time_end - loc_time_start);

    // c. Bij = m - |Aij| για i <> j και Bij = m για i = j
    loc_time_start = omp_get_wtime();

    #pragma omp parallel default(shared) private(i, j)
    {
        #pragma omp for schedule(static, chunk) collapse(1)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (i == j)
                    B[i][j] = m;
                else
                    B[i][j] = m - A[i][j];
    }

    loc_time_end = omp_get_wtime();
    // --------- Parallel Finish ---------

    print2DArray(fpB, B);
    printf("Task c. finished in %lf \n", loc_time_end - loc_time_start);
    
    min_val = B[0][0];

    // d. min_val = min(|Bij|)
    // d1. με reduction
    loc_time_start = omp_get_wtime();

    #pragma omp parallel default(shared) private(i, j)
    {
        #pragma omp for schedule(static, chunk) reduction(min : min_val)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < min_val)
                    min_val = B[i][j];
    }

    loc_time_end = omp_get_wtime();
    // --------- Parallel Finish ---------

    printf("With reduction --> min = %d\n", min_val);
    printf("Task d1. finished in %lf \n", loc_time_end - loc_time_start);

    min_val = B[0][0];

    // d2. χωρίς reduction
    // d2.1 αμοιβαίος αποκλεισμός
    loc_time_start = omp_get_wtime();

    #pragma omp parallel shared(min_val) private(i, j)
    {
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < min_val)
                {
                    #pragma omp critical (inc_min_val)
                    {
                        min_val = B[i][j];
                    }
                }
    }

    loc_time_end = omp_get_wtime();
    // --------- Parallel Finish ---------

    printf("With critical --> min = %d\n", min_val);
    printf("Task d2.1 finished in %lf \n", loc_time_end - loc_time_start);

    min_val = B[0][0];
    printf("\n\n\n--------------------\n\n\n");

    // d2.2 αλγόριθμος δυαδικού δένδρου
    loc_time_start = omp_get_wtime();
    #pragma omp parallel default(shared) private(tid, i, j, incr, loc_min)
    {
        tid = omp_get_thread_num();
        M[tid] = B[tid*chunk][tid*chunk];
        
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < M[tid])
                    M[tid] = B[i][j];
        
        M[T+tid] = 1000000; // Initialize memory
        incr = 1; // Initialize increment
        while (incr < T)
        {
            // Print the values before the operations in the current loop iteration
            printf("Before operation: tid=%d, incr=%d, M[%d]=%d, M[%d]=%d\n", 
                tid, incr, tid, M[tid], tid+incr, M[tid+incr]);

            temp0 = M[tid];
            temp1 = M[tid + incr];

            // Print the temporary values fetched from memory
            printf("temp0=%d, temp1=%d\n", temp0, temp1);

            loc_min = (temp0 <= temp1) ? temp0 : temp1;

            // Print the calculated local minimum
            printf("loc_min=%d\n", loc_min);

            M[tid] = loc_min;

            // Print the updated memory value
            printf("After operation: M[%d]=%d\n", tid, M[tid]);

            incr = 2 * incr;

            // Print the new increment value
            printf("Updated incr=%d\n", incr);
            
            printf("\n");
        }

    }
    
    loc_time_end = omp_get_wtime();
    // --------- Parallel Finish ---------
    for(i = 0; i < T; i++)
        printf("M[%d] = %d\n", i, M[i]);

    printf("With binary tree algo --> min = %d\n", M[0]);
    printf("Task d2.2 finished in %lf \n", loc_time_end - loc_time_start);

    all_time_end = omp_get_wtime();
    printf("Parallel program finished in %lf \n", all_time_end - all_time_start);

    fclose(fpA);
    fclose(fpB);

    return 0;
}

void print2DArray(FILE *fp, int Array[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            fprintf(fp, "%4d ", Array[i][j]);
        fprintf(fp, "\n");
    }
}

