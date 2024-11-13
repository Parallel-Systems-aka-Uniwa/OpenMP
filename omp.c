#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CHUNKSIZE 1
#define N 4

void printA(int A[N][N]);

int main(int argc, char *argv[]) 
{
    int A[N][N];
    int i, j;
    int loc_sum, loc_index;
    int chunk;
    int flag, loc_flag;

    omp_set_num_threads(2);

    chunk = CHUNKSIZE;
    flag = 1;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] = i + j;//(i * i + j * j) % (N + 1);
    
    printA(A);
    
    #pragma omp parallel shared(flag) private(i, j, loc_sum, loc_flag)
    {
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < N; i++)
        {
            loc_sum = 0;

            for (j = 0; j < N; j++)
                if (i != j)
                   { 
                        loc_sum += abs(A[i][j]); 
                        printf("Thread %d : loc_sum = %d\n", omp_get_thread_num(), loc_sum); 
                    }
                else
                    { 
                         
                        loc_index = abs(A[i][j]); 
                        printf("Thread %d : loc_index = %d\n", omp_get_thread_num(), loc_index);
                        }
            if (loc_index > loc_sum)
                loc_flag = 1;
            else
                loc_flag = 0;
        }

        #pragma omp atomic
        flag *= loc_flag;            
    }
    
    printf("Is flag true? %d\n", flag);

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