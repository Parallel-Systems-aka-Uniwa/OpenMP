#include <omp.h>
#include <stdio.h>
#define N 	4                 

int main (int argc, char *argv[]) 
{
int	tid, nth, i, j, k, chunk;
double a[N][N],b[N][N],c[N][N];           
chunk = 2;                    

#pragma omp parallel default(shared) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  
  /* Αρχικοποιήσεις των πινάκων */

  #pragma omp for schedule (static, chunk) 
   for (i=0; i<N; i++)
     for (j=0; j<N; j++)
       a[i][j]= i+j;
  
  #pragma omp for schedule (static, chunk)
   for (i=0; i<N; i++)
     for (j=0; j<N; j++)
       b[i][j]= i*j;
  
  #pragma omp for schedule (static, chunk)
   for (i=0; i<N; i++)
     for (j=0; j<N; j++)
       c[i][j]= 0;
  
  /* Εκτέλεση του πολλαπλασιασμού */

  #pragma omp for schedule (static, chunk) collapse (2)
  for (i=0; i<N; i++)    
   for(j=0; j<N; j++)
    { 
     printf("Thread %d computes iter %d,%d\n",tid,i,j);      
     for (k=0; k<N; k++)
       c[i][j] += a[i][k] * b[k][j];
    }
  }   

printf("***************************\n");
printf("Final Matrix:\n");
for (i=0; i<N; i++)
  {
  for (j=0; j<N; j++) 
    printf("%5.2f  ", c[i][j]);
  printf("\n"); 
  }
printf("***************************\n");
}

