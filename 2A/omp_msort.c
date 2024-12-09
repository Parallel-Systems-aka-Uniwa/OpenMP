/*
 *  === Αρχείο: omp_msort.c ===
 *
 *  Ονοματεπώνυμο: Αθανασίου Βασίλειος Ευάγγελος
 *  Αριθμός Μητρώου: 19390005
 *  Πρόγραμμα Σπουδών: ΠΑΔΑ
 *  
 *  Μεταγλώττιση: gcc -o omp_msort omp_msort.c -fopenmp
 *  Εκτέλεση: ./omp_msort A_unsort.txt A_sort.txt
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


#define T 1
#define N 10000
#define CZ 2


void create1DArray(int *Array, int size);
void print1DArray(FILE *fp, int *Array, int size);

int main(int argc, char *argv[]) 
{
    int *A;
    FILE *fpA_unsort, *fpA_sort;
    int threads, size, chunk;

    threads = T;
    size = N;
    chunk = CZ;

    omp_set_num_threads(threads);

    if (argc != 3) 
    {
        printf("Usage: %s A_unsort.txt A_sort.txt\n", argv[0]);
        exit(1);
    }

    fpA_unsort = fopen(argv[1], "w");
    if (fpA_unsort == NULL) 
    {
        printf("Cannot open file %s\n", argv[1]);
        exit(1);
    }

    fpA_sort = fopen(argv[2], "w");
    if (fpA_sort == NULL)
    {
        printf("Cannot open file %s\n", argv[2]);
        exit(1);
    }

    printf("Threads     : %d\n", threads);
    printf("Matrix size : %d\n", size);
    printf("Chunk size  : %d\n", chunk);

    A = (int *) malloc(size * sizeof(int));
    if (A == NULL)
    {
        printf("Memory allocation failed for A\n");
        exit(1);
    }

    create1DArray(A, size);
    print1DArray(fpA_unsort, A, size);

    fclose(fpA_unsort);
    fclose(fpA_sort);

    free(A);
    
    return 0;
}

void create1DArray(int *Array, int size)
{
    int i;

    srand(time(NULL));

    for (i = 0; i < size; i++)
    {
        Array[i] = rand() % 199 - 99;
        Array[i] = Array[i] >= 0 ? Array[i] + 10 : Array[i] - 10; 
    }
}

void print1DArray(FILE *fp, int *Array, int size)
{
    int i;

    for (i = 0; i < size; i++)
        fprintf(fp, "%d ", Array[i]);
}
