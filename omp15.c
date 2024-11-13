#include <omp.h>
#include <stdio.h>
main(int argc, char **argv)
 {
  omp_set_nested(1);
  int tid;
  #pragma omp parallel private(tid)
   {
    tid = omp_get_thread_num();
    printf("Hello 1 !!! from thread %d\n", tid);
    #pragma omp parallel private(tid)
     {
      tid = omp_get_thread_num();
      printf("Hello 2 !!! from thread %d\n", tid);
      #pragma omp parallel private(tid)
       {
        tid = omp_get_thread_num();
        printf("Hello 3 !!! from thread %d\n", tid);
       }
     }
   }
 }

