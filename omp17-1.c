#include <omp.h>
#include <stdio.h>
#include <semaphore.h>

main ()
{

int i;

sem_t semA, semB;
sem_init(&semA, 0, 1);
sem_init(&semB, 0, 0);

#pragma omp parallel private(i)
  {
  #pragma omp sections nowait
    {
    #pragma omp section
     for (i=1; i<=10; i++)
      { 
       sem_wait(&semA);
       sleep(1);
       printf("PING\n");
       sem_post(&semB);
      }
    #pragma omp section
     for (i=1; i<=10; i++)
      { 
       sem_wait(&semB);
       sleep(1);
       printf("PONG\n");
       sem_post(&semA);
      }
    } 
  }
}

