#include <omp.h>
#include <stdio.h>

main ()  {

int nthreads, tid;

/* Δυνατότητα επανακαθορισμού του αριθμού νημάτων */
// omp_set_num_threads(8);
//
/* Δημιουργία των νημάτων */
#pragma omp parallel private(tid)
  {
   /* Βρες ποια είναι η ταυτότητά σου */
   tid = omp_get_thread_num();
   printf("Hello World from thread %d\n", tid);

   /* Εκτελείται μόνο από το νήμα #0 */
   if (tid == 0) 
     {
      /* Βρες πόσα νήματα έχουν δημιουργηθεί */
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
     }
  } /* Τερματισμός όλων των νημάτων */
}
//
