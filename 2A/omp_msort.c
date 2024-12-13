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

#define T 100
#define N 10000000
#define LIMIT 1000

void multisort(int *start, int *space, int size);
void quicksort(int *start, int *end);
int* pivotPartition(int *start, int *end);
void swap(int *a, int *b);
void merge(int *startA, int *endA, int *startB, int *endB, int *space);

int main(int argc, char *argv[]) 
{
    int *A, *Space;                 // Ο πίνακας Α που θα ταξινομηθεί και ο πίνακας Space που θα χρησιμοποιηθεί για την ταξινόμηση
    FILE *fpA_unsort, *fpA_sort;    // Αρχεία εξόδου για την αποθήκευση των πινάκων Α πριν και μετά την ταξινόμηση
    int threads, size;              // Ο αριθμός των threads και το μέγεθος του πίνακα
    int i;                          // Δείκτης επανάληψης
    double start_time, end_time;    // Χρόνος εκτέλεσης του παράλληλου προγράμματος

/*
 *  Ορισμός του αριθμού των threads και άνοιγμα των αρχείων εξόδου 
 */
    // Επιβεβαιώνουμε ότι οι παράμετροι είναι ακέραιοι
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

    printf("Threads             : %d\n", threads);
    printf("Matrix size         : %d\n", size);
    printf("Limit for quicksort : %d\n", LIMIT);
    printf("-----------------------------------\n");

/*
 *  Δέσμευση μνήμης για τους πίνακες Α και Space
 */
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

/*
 *  Αρχικοποίηση του πίνακα Α με τυχαίες τιμές
 */
    srand(time(NULL));

    for (i = 0; i < size; i++)
    {
        A[i] = rand() % 199 - 99;                  // Τιμές στο διάστημα [-99, 99]
        A[i] = A[i] >= 0 ? A[i] + 10 : A[i] - 10;  // Τυχαία επιλογή προσήμου
    }
    
    printf("Before sorting\n");
    printf("-----------------------------------\n");
    
    for (i = 0; i < size; i++)
        fprintf(fpA_unsort, "%d ", A[i]);
    printf("\n");

// ================================= Έναρξη παράλληλου υπολογισμού =================================
    // ---- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος ----
    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        multisort(A, Space, size);
    }

    end_time = omp_get_wtime();
    // ---- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος ----
// ================================= Λήξη παράλληλου υπολογισμού =================================

    printf("-----------------------------------\n");
    printf("After sorting\n");
    printf("-----------------------------------\n");
    
    for (i = 0; i < size; i++)
        fprintf(fpA_sort, "%d ", A[i]);
    printf("\n");

    printf("-----------------------------------\n");
    printf("Multisort finished in %lf sec.\n", end_time - start_time);
    printf("-----------------------------------\n");

    fclose(fpA_unsort);
    fclose(fpA_sort);

    free(A);
    free(Space);
    
    return 0;
}

/*
 * === Συνάρτηση multisort ===
 * Παράμετροι: 
 *    - int *start: Δείκτης στην αρχή του πίνακα που πρέπει να ταξινομηθεί.
 *    - int *space: Δείκτης σε έναν βοηθητικό πίνακα ίδιου μεγέθους με τον αρχικό,
 *                  που χρησιμοποιείται κατά τη συγχώνευση των τμημάτων.
 *    - int size: Το μέγεθος του πίνακα που θα ταξινομηθεί.
 * Επιστρέφει: 
 *      Τίποτα (void).
 *
 * Η συνάρτηση multisort υλοποιεί έναν παράλληλο αλγόριθμο ταξινόμησης με χρήση του OpenMP.
 * Ο αλγόριθμος χωρίζει τον πίνακα σε τέσσερα τμήματα (quarters), ταξινομεί κάθε τμήμα 
 * αναδρομικά και στη συνέχεια συγχωνεύει τα ταξινομημένα τμήματα. Όταν το μέγεθος του
 * τμήματος είναι μικρότερο από το όριο LIMIT, χρησιμοποιείται ο ακολουθιακός αλγόριθμος quicksort
 * για την τοπική ταξινόμηση.
 *
 */
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

/*
 * === Συνάρτηση quicksort ===
 * Παράμετροι: 
 *    - int *start: Δείκτης στην αρχή του τμήματος του πίνακα που πρέπει να ταξινομηθεί.
 *    - int *end: Δείκτης στο τέλος του τμήματος του πίνακα που πρέπει να ταξινομηθεί.
 * Επιστρέφει: 
 *      Τίποτα (void).
 *
 * Η συνάρτηση quicksort υλοποιεί τον αναδρομικό αλγόριθμο γρήγορης ταξινόμησης (QuickSort).
 * 1. Επιλέγει ένα στοιχείο οδηγός (pivot) από το τμήμα του πίνακα που ταξινομείται.
 * 2. Τοποθετεί όλα τα στοιχεία μικρότερα ή ίσα με τον οδηγό στα αριστερά του
 *    και όλα τα μεγαλύτερα στα δεξιά του, διαχωρίζοντας τον πίνακα σε δύο υποπίνακες.
 * 3. Επαναλαμβάνει αναδρομικά τη διαδικασία για τον αριστερό και δεξιό υποπίνακα
 *    μέχρι να επιτευχθεί η πλήρης ταξινόμηση.
 *
 */
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

/*
 * === Συνάρτηση pivotPartition ===
 * Παράμετροι:
 *    - int *start: Δείκτης στην αρχή του τμήματος του πίνακα που πρέπει να διαχωριστεί.
 *    - int *end: Δείκτης στο τέλος του τμήματος του πίνακα που πρέπει να διαχωριστεί.
 * Επιστρέφει: 
 *    - int*: Δείκτης στο νέο σημείο όπου βρίσκεται ο οδηγός (pivot) μετά τον διαχωρισμό.
 *
 * Η συνάρτηση pivotPartition εκτελεί τον διαχωρισμό του πίνακα γύρω από ένα στοιχείο οδηγός (pivot).
 * 1. Το pivot ορίζεται αρχικά ως το τελευταίο στοιχείο του τμήματος του πίνακα.
 * 2. Μετακινεί όλα τα στοιχεία μικρότερα ή ίσα με το pivot στην αριστερή πλευρά του πίνακα,
 *    ενώ τα μεγαλύτερα παραμένουν στη δεξιά πλευρά.
 * 3. Τέλος, το pivot τοποθετείται στη σωστή του θέση, δηλαδή μεταξύ των μικρότερων και των μεγαλύτερων στοιχείων.
 *
 */
int* pivotPartition(int *start, int *end)
{
    int *pvt;
    int *i, *j;

    pvt = end;  
    i = start - 1;  

    for (j = start; j < end; j++)  
    {
        if (*j <= *pvt)  
        {
            i++;
            swap(i, j); 
        }
    }

    swap(i + 1, pvt);  

    return i + 1; 
}

/*
 * === Συνάρτηση swap ===
 * Παράμετροι:
 *    - int *a: Δείκτης στο πρώτο στοιχείο που θα ανταλλαγεί.
 *    - int *b: Δείκτης στο δεύτερο στοιχείο που θα ανταλλαγεί.
 * Επιστρέφει: 
 *      Τίποτα (void).
 *
 * Η συνάρτηση swap πραγματοποιεί την ανταλλαγή (swap) των τιμών δύο μεταβλητών 
 * που δείχνονται από τους δείκτες a και b. 
 *
 */
void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}


/*
 * === Συνάρτηση merge ===
 * Παράμετροι:
 *    - int *startA: Δείκτης στο πρώτο στοιχείο του πρώτου υποπίνακα (A).
 *    - int *endA: Δείκτης στο τελευταίο στοιχείο του πρώτου υποπίνακα (A).
 *    - int *startB: Δείκτης στο πρώτο στοιχείο του δεύτερου υποπίνακα (B).
 *    - int *endB: Δείκτης στο τελευταίο στοιχείο του δεύτερου υποπίνακα (B).
 *    - int *space: Δείκτης σε προσωρινό χώρο αποθήκευσης.
 * Επιστρέφει: 
 *      Τίποτα (void).
 *
 * Η συνάρτηση merge συγχωνεύει δύο ταξινομημένα υποτμήματα (A και B) σε έναν
 * ενιαίο ταξινομημένο πίνακα. Ο συγχωνευμένος πίνακας αποθηκεύεται αρχικά
 * στον προσωρινό πίνακα space και στη συνέχεια επιστρέφεται στον αρχικό πίνακα.
 *
 */
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

    for (i = space; i < k; i++)
    {
        *startA = *i;
        startA++;
    }
}