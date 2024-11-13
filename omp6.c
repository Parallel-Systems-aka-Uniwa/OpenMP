#include <omp.h>
#include <stdio.h>

#define CHUNKSIZE 100
#define N     1000000

main ()  
{

int i, j, chunk;
int x=0;

chunk = CHUNKSIZE;

omp_set_num_threads(4);

#pragma omp parallel private(i,j)
  {
  #pragma omp for schedule(static,chunk) 
  for (i=0; i < N; i++)
    {
       x=x+1;
    }

  }  /* Τερματισμός όλων των νημάτων */

printf("%d\n", x); 

}

