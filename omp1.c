#include <omp.h>
#include <stdio.h>

int main(int argc, char **argv)
 {
  #pragma omp parallel
   {
    printf("Hello OpenMP!\n");
   }
 }
