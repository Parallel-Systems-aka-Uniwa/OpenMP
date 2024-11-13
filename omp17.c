#include <omp.h>
#include <stdio.h>

main ()
{

omp_lock_t lock1;
omp_lock_t lock2;
int i;

omp_init_lock(&lock1);
omp_unset_lock(&lock1);

omp_init_lock(&lock2);
omp_set_lock(&lock2);

#pragma omp parallel private(i)
  {
  #pragma omp sections nowait
    {
    #pragma omp section
     for (i=1; i<=20; i++)
      { 
       omp_set_lock(&lock1);
       printf("PING\n");
       omp_unset_lock(&lock2);
      }
    #pragma omp section
     for (i=1; i<=20; i++)
      { 
       omp_set_lock(&lock2); 
       printf("PONG\n");
       omp_unset_lock(&lock1);
      }
    } 
  }
}

