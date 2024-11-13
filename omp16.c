#include <omp.h>

main ()
{

int i, n, p, np;
int tid, result;
result = 1;

printf("Give an integer: ");
scanf("%d", &n);

#pragma omp parallel shared(n) private(i)
  {
   #pragma omp for schedule(static) reduction(*:result)
     for (i=1; i <= n; i++)
       result*=i;
  }  

printf("Final Result: %d\n", result);
}

