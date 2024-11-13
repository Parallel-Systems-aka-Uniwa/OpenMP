#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE 10
#define N     100

main ()  {

int   i, chunk;
float a[N], b[N], result;

/* Αρχικοποιήσεις */
chunk = CHUNKSIZE;
result = 0.0;
for (i=0; i < N; i++)
  {
  a[i] = 1.0;
  b[i] = 2.0;
  }

#pragma omp parallel default(shared) private(i)    
{   
#pragma omp for schedule(static,chunk) reduction(+:result)  
  for (i=0; i < N; i++)
    {
       result = result + (a[i] * b[i]); 
    }
}
printf("Final result= %f\n", result);
}

