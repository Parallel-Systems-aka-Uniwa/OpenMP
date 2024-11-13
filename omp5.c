#include <omp.h>
#include <stdio.h>
#define N     10

main ()
{

int i,t;
int a[N],b[N],c[N],d[N],e[N],f[N];

/* Αρχικοποιήσεις διανυσμάτων */
for (i=0; i < N; i++) {
  a[i] = i * 2;
  b[i] = i * 3;
  e[i] = i * 4;
  f[i] = i * 5;

  }

#pragma omp parallel shared(a,b,c,d,e,f) private(i)
  {

  #pragma omp sections 
    {

    #pragma omp section
    for (i=0; i < N; i++)
      c[i] = a[i] + b[i];

    #pragma omp section
    for (i=0; i < N; i++)
      d[i] = e[i] + f[i];

    }  /* τέλος της διαταγής sections */

  }  /* τέλος της παράλληλης περιοχής */

    printf("Final Result Vector C:\n");
    for (t=0; t < N; t++) 
      printf("%d ", c[t]);
    printf("\n");
    printf("Final Result Vector D:\n");
    for (t=0; t < N; t++) 
      printf("%d ", d[t]); 
    printf("\n");
}

