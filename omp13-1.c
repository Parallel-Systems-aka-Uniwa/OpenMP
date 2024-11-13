#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE 10
#define N     100

main ()  {

int   i, chunk, tid, num;
float a[N], b[N], loc_result, result;

/* Αρχικοποιήσεις */
chunk = CHUNKSIZE;
result = 0.0;
for (i=0; i < N; i++)
  {
  a[i] = 1.0;
  b[i] = 2.0;
  }

#pragma omp parallel shared(a,b,result) private(tid,i,loc_result)    
  {  
    tid=omp_get_thread_num();
    num=omp_get_num_threads();

    loc_result = 0.0;

    for (i=(tid*(N/num)); i < ((tid+1)*(N/num)); i++)
         {
            loc_result = loc_result + (a[i] * b[i]); 
         }
    #pragma omp atomic
      result += loc_result;
  }

printf("Final result= %f\n", result);
}

