#include <stdio.h>
#include <omp.h>
int fib(int n)
{
  int i, j, tid;
  if (n<2)
    return n;
  else
    {
       #pragma omp task shared(i) private(tid) 
        { 
         tid=omp_get_thread_num();
         printf ("Thread %d computes fib(%d)\n", tid, n-1);
         i=fib(n-1);
        }

       #pragma omp task shared(j) private(tid) 
        { 
         tid=omp_get_thread_num();
         printf ("Thread %d computes fib(%d)\n", tid, n-2);
         j=fib(n-2);
        }

       #pragma omp taskwait

       return i+j;
    }
}

int main()
{
  int n = 5;
  int tid;

  omp_set_num_threads(4);

  #pragma omp parallel shared(n) private(tid)
  {
    #pragma omp single nowait
    {
     tid=omp_get_thread_num();
     printf ("Thread %d starts the execution\n\n", tid);
     printf ("fib(%d) = %d\n", n, fib(n));
    }
  }
}
