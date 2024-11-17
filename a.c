#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CZ 2
#define N 8
// σσ2. Τον αριθμό των threads τον δίνει ο χρήστης
#define T 4

int main()
{
    int A[N];
    int i;
    int chunk;

    chunk = CZ;
    omp_set_num_threads(T);

    #pragma omp parallel
    {
        #pragma omp for schedule (static, chunk) private(i)
        for (i = 0; i < N; i++)
        {
            A[i] = i;
            printf("Thread %d : A[%d] = %d\n", omp_get_thread_num(), i, A[i]);
        } 
    }
}