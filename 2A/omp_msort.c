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


#define T 4
#define N 20
#define LIMIT 4

void multisort(int *start, int *space, int size);
void quicksort(int *start, int *end);
int* pivotPartition(int *start, int *end);
void swap(int *a, int *b);
void merge(int *startA, int *endA, int *startB, int *endB, int *space);

int main(int argc, char *argv[]) 
{
    int *A, *Space;
    FILE *fpA_unsort, *fpA_sort;
    int threads, size;
    int i;

    srand(time(NULL));

    threads = T;
    size = N;

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

    printf("Threads          : %d\n", threads);
    printf("Matrix size      : %d\n", size);

    A = (int *) malloc(size * sizeof(int));
    if (A == NULL)
    {
        printf("Memory allocation failed for A\n");
        exit(1);
    }
    
    Space = (int *) malloc(size * sizeof(int));
    if (Space == NULL)
    {
        printf("Memory allocation failed for Space\n");
        exit(1);
    }

    for (i = 0; i < size; i++)
    {
        A[i] = rand() % 199 - 99;
        A[i] = A[i] >= 0 ? A[i] + 10 : A[i] - 10; 
    }
    for (i = 0; i < size; i++)
        fprintf(fpA_unsort, "%d ", A[i]);

    #pragma omp parallel
    {
        #pragma omp single
        multisort(A, Space, size);
    }

    for (i = 0; i < size; i++)
        fprintf(fpA_sort, "%d ", A[i]);

    fclose(fpA_unsort);
    fclose(fpA_sort);

    free(A);
    free(Space);
    
    return 0;
}

void multisort(int *start, int *space, int size)
{
    int quarter;
    int *startA, *startB, *startC, *startD;
    int *spaceA, *spaceB, *spaceC, *spaceD;

    if (size < LIMIT)
    {
        quicksort(start, start + size - 1);
        return;
    }

    quarter = size / 4;
    startA = start; spaceA = space;
    startB = startA + quarter; spaceB = spaceA + quarter;
    startC = startB + quarter; spaceC = spaceB + quarter;
    startD = startC + quarter; spaceD = spaceC + quarter;

    #pragma omp task firstprivate(start, space, size)
    multisort(startA, spaceA, quarter);

    #pragma omp task firstprivate(start, space, size)
    multisort(startB, spaceB, quarter);

    #pragma omp task firstprivate(start, space, size)
    multisort(startC, spaceC, quarter);

    #pragma omp task firstprivate(start, space, size)
    multisort(startD, spaceD, size - 3 * quarter);

    #pragma omp taskwait

    #pragma omp task firstprivate(start, space, size)
    merge(startA, startA + quarter - 1, startB, startB + quarter - 1, spaceA);

    #pragma omp task firstprivate(start, space, size)
    merge(startC, startC + quarter - 1, startD, start + size - 1, spaceC);

    #pragma omp taskwait

    merge(spaceA, spaceC - 1, spaceC, spaceA + size - 1, startA);
}

void quicksort(int *start, int *end)
{
    int *pvt;

    if (start < end)
    {
        pvt = pivotPartition(start, end);
        quicksort(start, pvt - 1);
        quicksort(pvt + 1, end);
    }
}

int* pivotPartition(int *start, int *end)
{
    int *pvt;
    int *i, *j;

    pvt = end;  // Pivot is the last element
    i = start - 1;  // Initialize i to one element before the start

    for (j = start; j < end; j++)  // Iterate over the elements
    {
        if (*j <= *pvt)  // Compare values
        {
            i++;
            swap(i, j);  // Swap values
        }
    }

    swap(i + 1, pvt);  // Put pivot in its correct position

    return i + 1;  // Return the partition index
}


void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void merge(int *startA, int *endA, int *startB, int *endB, int *space)
{
    int *i = startA, *j = startB, *k = space;

    while (i <= endA && j <= endB)
    {
        if (*i <= *j)
        {
            *k = *i;
            i++;
        }
        else
        {
            *k = *j;
            j++;
        }
        k++;
    }

    while (i <= endA)
    {
        *k = *i;
        i++;
        k++;
    }

    while (j <= endB)
    {
        *k = *j;
        j++;
        k++;
    }

    // Copy the merged result back to the original array
    for (i = space; i < k; i++)
    {
        *startA = *i;
        startA++;
    }
}


/*
void quicksort(int *Array, int start, int end)
{
    int pvt;

    if (start < end)
    {
        pvt = pivotPartition(Array, start, end, end);
        quicksort(Array, start, pvt - 1);
        quicksort(Array, pvt + 1, end);
    }
}

int pivotPartition(int *A, int start, int end, int pvt)
{
    int i, j;

    swap(&A[pvt], &A[end]);
    i = start;
    j = end;

    while (i < j)
        while(i < j && A[i] <= A[end])
            i++;
        while(i < j && A[j] >= A[end])
            j--;
        swap(&A[i], &A[j]);
    swap(A[i], A[end]);

    return i;
}

void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void merge(int *A, int startA, int endA, int *B, int startB, int endB, int *C, int start, int end)
{
    int i, j, k;

    i = startA;
    j = startB;
    k = start - 1;
    A[endA + 1] = B[endB] + 1;
    B[endB + 1] = A[endA] + 1;

    while (i <= endA || j <= endB)
    {
        k++;
        if (A[i] <= B[j])
        {
            C[k] = A[i];
            i++;
        }
        else
        {
            C[k] = B[j];
            j++;
        }
    }
}
*/