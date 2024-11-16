#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CZ 2
#define N 8
// σσ2. Τον αριθμό των threads τον δίνει ο χρήστης
#define T 4

void printArray(int Array[N][N]);

int main(int argc, char *argv[]) 
{
    int A[N][N];
    int B[N][N];
    int i, j;
    int chunk, flag;
    int loc_sum, loc_flag, loc_index;
    int m;
    int min_val;

    omp_set_num_threads(T);

    chunk = CZ;
    flag = 1;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i == j) 
                A[i][j] = N + i + 1;
            else
                A[i][j] = (i + j) % 3;
        }
    }

    m = A[0][0];

    printArray(A);

    // a. Να ελέγχει (παράλληλα) αν ο πίνακας Α είναι αυστηρά διαγώνια δεσπόζων    
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
    
    // σσ1. δεν ισχύει το a.
    if (!flag)
    {
        printf("The array A is not strictly diagonal dominant.\n");
        exit(0);
    }

    printf("Hooray!\n");

    // b. m = max(|Aii|), i = 0...N-1
    #pragma omp parallel default(shared) private(i)
    {
        #pragma omp for schedule(static, chunk) reduction(max : m)
        for (i = 0; i < N; i++)
            if (A[i][i] > m)
                m = A[i][i];
    }

    printf("max = %d\n", m);

    // c. Bij = m - |Aij| για i <> j και Bij = m για i = j
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

    printArray(B);
    
    min_val = B[0][0];

    // d. min_val = min(|Bij|)
    // d1. με reduction
    #pragma omp parallel default(shared) private(i, j)
    {
        #pragma omp for schedule(static, chunk) reduction(min : min_val)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < min_val)
                    min_val = B[i][j];
    }

    printf("min = %d\n", min_val);

    // d2. χωρίς reduction
    // d2.1 αμοιβαίος αποκλεισμός
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

    printf("min = %d\n", min_val);

    // d2.2 αλγόριθμος δυαδικού δένδρου


    return 0;
}

void printArray(int Array[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            printf("%d ", Array[i][j]);
        printf("\n");
    }
}