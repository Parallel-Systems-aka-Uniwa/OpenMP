#include <omp.h>
#include <stdio.h>

main ()
{

int i, num_steps;
double x, step, sum, pi;
double ctime1, ctime2;

omp_set_num_threads(4);

num_steps = 1000000000;
x=0;
sum = 0.0;
step = 1.0/(double) num_steps;

ctime1 = omp_get_wtime();
#pragma omp parallel private(i,x) 
   {
#pragma omp for reduction(+:sum) schedule(static)
      for (i=0; i<num_steps; i=i+1){
         x=(i+0.5)*step;
         sum = sum + 4.0/(1.0+x*x);
      }
   }
   pi=step*sum;
ctime2 = omp_get_wtime();

printf("\nPi = %f  Time = %f \n", pi, ctime2-ctime1);

}
