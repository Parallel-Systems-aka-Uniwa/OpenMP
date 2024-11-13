#include <stdio.h>
#include <omp.h>

#define N 10

void printMat(int mat[N][N]);

int main(int argc, char *argv[]) 
{
    int mat[N][N];
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            mat[i][j] = (i * i + j * j) % (N + 1);
    
    printMat(mat);
    

    return 0;
}

void printMat(int mat[N][N])
{
    int i, j;

    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}