#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CZ 2
#define N 8
// σσ2. Τον αριθμό των threads τον δίνει ο χρήστης
#define T 4

void printA(int A[N][N]);

int main(int argc, char *argv[]) 
{
    int A[N][N];
    int i, j;
    int chunk, flag;
    int loc_sum, loc_flag, loc_index;

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

    printA(A);

    // a. Να ελέγχει (παράλληλα) αν ο πίνακας Α είναι αυστηρά διαγώνια δεσπόζων    
    #pragma omp parallel shared(flag) private(i, j, loc_sum, loc_flag)
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

    // b. m = max(|Aii|), i = 0...N-1
    

    return 0;
}

void printA(int A[N][N])
{
    int i, j;

    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}