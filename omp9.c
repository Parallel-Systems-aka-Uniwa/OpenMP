#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE 100
#define N     1000000

main ()  
{

int i, x, chunk;

omp_lock_t lock1;

omp_init_lock(&lock1);

chunk = CHUNKSIZE;

omp_set_num_threads(4);

#pragma omp parallel private(i)
  {
  #pragma omp for schedule(static,chunk) 
  for (i=0; i < N; i++)
    {
     omp_set_lock(&lock1);       
     x=x+1;
     omp_unset_lock(&lock1);
    }

  }  /* Τερματισμός όλων των νημάτων */

printf("%d\n", x); 

}

