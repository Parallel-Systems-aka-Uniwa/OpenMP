/*
 *  === Αρχείο: omp.c ===
 *
 *  Ονοματεπώνυμο: Αθανασίου Βασίλειος Ευάγγελος
 *  Αριθμός Μητρώου: 19390005
 *  Πρόγραμμα Σπουδών: ΠΑΔΑ
 *  
 *  Μεταγλώττιση: gcc -o omp omp.c -fopenmp
 *  Εκτέλεση: ./omp A.txt B.txt
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// σσ2. Τον αριθμό των threads τον δίνει ο χρήστης
#define T 4
#define N 16
#define CZ 6


void create2DArray(int (*Array)[N]);
void print2DArray(FILE *fp, int Array[N][N]);

int main(int argc, char *argv[]) 
{
    int A[N][N];                                // Ο πίνακας Α που θα εξετάσουμε αν είναι αυστηρά διαγώνια δεσπόζων
    int B[N][N];                                // Bij = m – |Aij| για i<>j και Bij = m για i=j 
    int M[T];                                   // Ο πίνακας Μ που θα χρησιμοποιήσουμε για τον αλγόριθμο δυαδικού δένδρου
    FILE *fpA, *fpB;                            // Αρχεία εξόδου για την αποθήκευση των πινάκων Α και Β
    int i, j, k;                                // Δείκτες επανάληψης
    int rowSum;                                 // Άθροισμα των στοιχείων μιας γραμμής του πίνακα Α
    int chunk, flag, tid;                       // Αριθμός επαναλήψεων ανά thread, Είναι ή δεν είναι ο Α αυστηρά διαγώνια δεσπόζων, Αναγνωριστικό thread
    int loc_sum, loc_flag, loc_index, loc_min;  // Τοπικές μεταβλητές για κάθε thread
    int incr, temp0, temp1;                     // Μεταβλητές για τον αλγόριθμο δυαδικού δένδρου
    int m;                                      // Μέγιστη τιμή της διαγωνίου του πίνακα Α
    int min_val;                                // Ελάχιστη τιμή του πίνακα Β
    double all_time_start, all_time_end;        // Χρόνος εκτέλεσης του παράλληλου προγράμματος
    double loc_time_start, loc_time_end;        // Χρόνος εκτέλεσης της κάθε παράλληλης εργασίας

/*
 *  Ορισμός του αριθμού των threads και άνοιγμα των αρχείων εξόδου 
 */
    omp_set_num_threads(T);

    if (argc != 3) 
    {
        printf("Usage: %s A.txt B.txt\n", argv[0]);
        exit(1);
    }

    fpA = fopen(argv[1], "w");
    if (fpA == NULL) 
    {
        printf("Cannot open file %s\n", argv[1]);
        exit(1);
    }

    fpB = fopen(argv[2], "w");
    if (fpB == NULL) 
    {
        printf("Cannot open file %s\n", argv[2]);
        exit(1);
    }

/*
 *  Αρχικοποίησεις:
 * 
 *  του αριθμού επαναλήψεων ανά thread, 
 *  του flag που ελέγχει αν ο Α είναι αυστηρά διαγώνια δεσπόζων,
 *  του πίνακα Α
 */
    chunk = CZ;
    flag = 1;

    create2DArray(A);
    print2DArray(fpA, A);

/*
 *  Αρχικοποιήσεις μέτρησης συνολικού χρόνου εκτέλεσης του παράλληλου προγράμματος για όλες τις υποεργασίες a, b, c, d1, d2.1, d2.2
 */
    all_time_start = 0.0;
    all_time_end = 0.0;

// ========================================== [Έναρξη Παράλληλου Υπολογισμού] ========================================== 

/*
 *  === Task: a === 
 *  Να ελέγχει (παράλληλα) αν ο πίνακας Α είναι αυστηρά διαγώνια δεσπόζων 
 */
    // --------- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία a ---------  
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;   
    
    printf("=================================== [Task a.] ===================================\n");
    #pragma omp parallel shared(flag) private(i, j, loc_sum, loc_flag, loc_index)
    {
        loc_flag = 1;

        // Παραλληλοποίηση του ελέγχου του πίνακα Α με for-schedule 
        #pragma omp for schedule(static, chunk)
        //#pragma omp for schedule(dynamic, chunk)
        for (i = 0; i < N; i++)
        {
            loc_sum = 0;

            for (j = 0; j < N; j++)
                if (i != j)
                    loc_sum += abs(A[i][j]); 
                else     
                    loc_index = abs(A[i][i]); 

            // Αν δεν ισχύει η ιδιότητα του αυστηρά διαγώνια δεσπόζων πίνακα
            // τότε η ιδιωτική μεταβλητή loc_flag παίρνει την τιμή 0
            // ώστε να πολλαπλασιαστεί με την κοινή μεταβλητή flag 
            // και να κρίνει ότι ο πίνακας Α δεν είναι αυστηρά διαγώνια δεσπόζων           
            if (loc_index <= loc_sum)
                loc_flag = 0;
        }

        // Κρίσιμη περιοχή για την ενημέρωση της κοινής μεταβλητής flag
        #pragma omp atomic
        flag *= loc_flag;            
    }
    
    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία a ---------

    printf("Is A strictly diagonal dominant?\n");
    // σσ1. δεν ισχύει το a.
    if (!flag)
    {
        printf("NO\n");
        printf("The array has been stored in file %s\n", argv[1]);

        printf("\n--------------------------------------------\n");
        printf("Task a. finished in %lf sec.\n", loc_time_end - loc_time_start);
        printf("--------------------------------------------\n");
        printf("=================================================================================\n");
        
        // Εκτύπωση συνολικού χρόνου εκτέλεσης του παράλληλου προγράμματος
        printf("\n--------------------------------------------\n");
        printf("Parallel program finished in %lf sec.\n", all_time_end - all_time_start);
        printf("--------------------------------------------\n");
        
        exit(0);
    }

    // Ο πίνακας Α είναι αυστηρά διαγώνια δεσπόζων
    printf("YES\n");
    printf("The array has been stored in file %s\n", argv[1]);

    printf("--------------------------------------------\n");
    printf("Task a. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("=================================================================================\n");

/*
 *  === Task: b === 
 *  Να υπολογιστεί παράλληλα το m = max(|Aii|), i = 0...N-1 
 */
    m = A[0][0]; // Αρχικοποίηση του m με το πρώτο στοιχείο της διαγωνίου του πίνακα Α

    // --------- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία b --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task b.] ===================================\n");
    #pragma omp parallel default(shared) private(i)
    {
        // Υπολογισμός του m με reduction clause
        #pragma omp for schedule(static, chunk) reduction(max : m)
        // #pragma omp for schedule(dynamic, chunk) reduction(max : m)
        for (i = 0; i < N; i++)
            if (A[i][i] > m)
                m = A[i][i];
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία b ---------

    printf("m = max(|Aii|) =>\n");
    printf("m = %d\n", m);

    printf("--------------------------------------------\n");
    printf("Task b. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("=================================================================================\n");


/*
 *  === Task: c === 
 *  Να φτιαχτεί παράλληλα ένας νέος πίνακας B με στοιχεία Bij = m - |Aij| για i <> j και Bij = m για i = j  
 */
    // --------- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία c --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task c.] ===================================\n");
    #pragma omp parallel default(shared) private(i, j)
    {
        // Με την οδηγία collapse(2) η nested for-loop συγχωνεύεται ως μία και εκτελείται ισοδύναμα 
        // for (i = 0; i < N * N; i++) { ... } και ο διαμοιρασμός στα threads γίνεται με την for schedule
        #pragma omp for schedule(static, chunk) collapse(2)
        // #pragma omp for schedule(dynamic, chunk) collapse(2) 
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (i == j)
                    B[i][j] = m;
                else
                    B[i][j] = m - A[i][j];
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία c ---------

    print2DArray(fpB, B);
    printf("Bij = m - |Aij| for i <> j and Bij = m for i = j\n");
    printf("The array has been stored in file %s\n", argv[2]);
    
    printf("--------------------------------------------\n");
    printf("Task c. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("==================================================================================\n");

/*
 *  === Task: d === 
 *  Να υπολογιστεί παράλληλα το ελάχιστο στοιχείο του πίνακα Β min_val = min(|Bij|)
 *      
 *      === Task: d1 ===
 *      με χρήση reduction 
 */   
    min_val = B[0][0]; // Αρχικοποίηση του min_val με το πρώτο στοιχείο του πίνακα Β

    // --------- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία d1 --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task d1.] ===================================\n");
    #pragma omp parallel default(shared) private(i, j)
    {
        // Υπολογισμός του min_val με reduction clause
        #pragma omp for schedule(static, chunk) reduction(min : min_val)
        // #pragma omp for schedule(dynamic, chunk) reduction(min : min_val)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < min_val)
                    min_val = B[i][j];
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία d1 ---------

    printf("With reduction\n");
    printf("m = min(|Bij|) =>\n");
    printf("m = %d\n", min_val);
    
    printf("--------------------------------------------\n");
    printf("Task d1. finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("===================================================================================\n");

/*
 *  === Task: d === 
 *  Να υπολογιστεί παράλληλα το ελάχιστο στοιχείο του πίνακα Β min_val = min(|Bij|)
 *      
 *      === Task: d2 ===
 *      χωρίς τη χρήση reduction
 * 
 *          === Task: d2.1 ===
 *          με μηχανισμούς προστασίας κρίσιμης περιοχής  
 */  
    min_val = B[0][0]; // Αρχικοποίηση του min_val με το πρώτο στοιχείο του πίνακα Β

    // --------- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία d2.1 --------- 
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task d2.1] ===================================\n");
    #pragma omp parallel shared(min_val) private(i, j)
    {

        #pragma omp for schedule(static, chunk)
        // #pragma omp for schedule(dynamic, chunk)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < min_val)
                {
                    // Κρίσιμη περιοχή για την ενημέρωση της κοινής μεταβλητής min_val
                    #pragma omp critical (inc_min_val)
                    {
                        min_val = B[i][j];
                    }
                }
    }

    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία d2.1 ---------

    printf("With critical section\n");
    printf("m = min(|Bij|) =>\n");
    printf("m = %d\n", min_val);
    
    printf("--------------------------------------------\n");
    printf("Task d2.1 finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("===================================================================================\n");

/*
 *  === Task: d === 
 *  Να υπολογιστεί παράλληλα το ελάχιστο στοιχείο του πίνακα Β min_val = min(|Bij|)
 *      
 *      === Task: d2 ===
 *      χωρίς τη χρήση reduction
 * 
 *          === Task: d2.2 ===
 *          με αλγόριθμο δυαδικού δένδρου	  
 */ 
    min_val = B[0][0]; // Αρχικοποίηση του min_val με το πρώτο στοιχείο του πίνακα Β

    // --------- Έναρξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία d2.2 ---------
    loc_time_start = omp_get_wtime();
    all_time_start += loc_time_start;

    printf("=================================== [Task d2.2] ===================================\n");
    #pragma omp parallel default(shared) private(tid, i, j, incr, temp0, temp1, loc_min)
    {
        tid = omp_get_thread_num();
        loc_min = 1000000;

        // Κάθε thread υπολογίζει το τοπικό ελάχιστο στοιχείο του πίνακα Β και τον αποθηκεύει στην θέση του πίνακα M[tid]
        // όπου tid είναι το αναγνωριστικό του thread 
        #pragma omp for schedule(static, chunk)
        // #pragma omp for schedule(dynamic, chunk)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (B[i][j] < loc_min)
                    loc_min = B[i][j];

        // Αρχικοποίηση των θέσεων του πίνακα Μ που είναι εκτός ορίων του πίνακα Μ
        // ώστε οι συγκρίσεις που θα γίνονται σε κάθε Φάση του αλγορίθμου δυαδικού δένδρου
        // να αυξάνονται κατά 2 θέσεις M[i+1], M[i+2], M[i+4], ... , κ.ο.κ.
        M[tid] = loc_min;

        // Συγχρονισμός των threads για κάθε φάση του αλγορίθμου δυαδικού δένδρου
        #pragma omp barrier

        // Αρχικοποίηση του δείκτη επανάληψης του αλγορίθμου δυαδικού δένδρου
        incr = 1;
        
        // Έναρξη Φάσεων του αλγορίθμου δυαδικού δένδρου
        while (incr < T)
        {
            // Από τη στιγμή που δεν μπορούμε να χρησιμοποιήσουμε μηχανισμούς προστασίας κρίσιμης περιοχής,
            // θα πρέπει να επιβεβαιώσουμε ότι το σωστό thread κάνει την σύγκριση για τα στοιχεία που του ανήκουν.
            // Αυτό επιτυγχάνεται με την χρήση των συνθηκών tid % (2 * incr) == 0 και tid + incr < T
            // Συνθήκη 1: tid % (2 * incr) == 0, όσο περνάνε οι φάσεις του αλγορίθμου, τα threads που εργάζονται μειώνονται
            // Συνθήκη 2: tid + incr < T, για να μην ξεφύγουμε από τα όρια του πίνακα Μ
            if (tid % (2 * incr) == 0 && tid + incr < T) 
            {
                temp0 = M[tid];
                temp1 = M[tid + incr];
                loc_min = (temp0 <= temp1) ? temp0 : temp1;
                M[tid] = loc_min;
            }

            // Συγχρονισμός των threads για την επόμενη φάση του αλγορίθμου δυαδικού δένδρου
            #pragma omp barrier 

            // Αύξηση του δείκτη επανάληψης του αλγορίθμου δυαδικού δένδρου
            incr = 2 * incr;
        }
    }

    
    loc_time_end = omp_get_wtime();
    all_time_end += loc_time_end;
    // --------- Λήξη χρόνου μέτρησης του παράλληλου προγράμματος για την υποεργασία d2.2 ---------
    
    // Εύρεση του ελάχιστου στοιχείου του πίνακα Β με την χρήση του αλγορίθμου δυαδικού δένδρου
    // Το ελάχιστο στοιχείο αποθηκεύεται στην θέση M[0] του πίνακα Μ
    printf("Binary Tree Algorithm\n");
    printf("m = min(|Bij|) =>\n");
    printf("m = %d\n", M[0]);

    printf("--------------------------------------------\n");
    printf("Task d2.2 finished in %lf sec.\n", loc_time_end - loc_time_start);
    printf("--------------------------------------------\n");
    printf("===================================================================================\n");
    
// ========================================== [Λήξη Παράλληλου Υπολογισμού] ==========================================
    
    // Εκτύπωση συνολικού χρόνου εκτέλεσης του παράλληλου προγράμματος
    printf("\n--------------------------------------------\n");
    printf("Parallel program finished in %lf sec.\n", all_time_end - all_time_start);
    printf("--------------------------------------------\n");

    fclose(fpA);
    fclose(fpB);

    return 0;
}

/*
 *  === Συνάρτηση create2DArray ===
 *  Παράμετροι: Δείκτης σε πίνακα διάστασης N που περιέχει ακέραιες τιμές  
 *  Επιστρέφει: Τίποτα
 * 
 *  Συνάρτηση που δημιουργεί έναν τυχαίο πίνακα Ν x Ν όπου ανάλογα με το αποτέλεσμα της rand() % 2
 *  ο πίνακας είναι αυστηρά διαγώνια δεσπόζων ή μη αυστηρά διαγώνια δεσπόζων. 
 *  Ο πίνακας επιστρέφεται στο σημείο κλήσης μέσω αναφοράς (by reference).
 */
void create2DArray(int (*Array)[N])
{
    int i, j;
    int rowSum;

    srand(time(NULL));
    
    // Τυχαία επιλογή αν ο πίνακας θα είναι αυστηρά διαγώνια δεσπόζων ή μη αυστηρά διαγώνια δεσπόζων. Τιμές στο διάστημα [0, 1]
    if (rand() % 2) 
    {
        // Αυστηρά διαγώνια δεσπόζων
        for (i = 0; i < N; i++) 
        {
            rowSum = 0;
            for (j = 0; j < N; j++) 
            {
                if (i == j) 
                {
                    Array[i][j] = rand() % 21 - 10; // Στοιχεία κύριας διαγωνίου. Τιμές στο διάστημα [-10, 10]
                    Array[i][j] = Array[i][j] >= 0 ? Array[i][j] + 20 : Array[i][j] - 20; // Τυχαία επιλογή προσήμου
                } 
                else 
                {
                    Array[i][j] = rand() % 21 - 10; // Στοιχεία που δεν ανήκουν στην κύρια διαγώνιο. Τιμές στο διάστημα [-10, 10]
                    rowSum += abs(Array[i][j]);
                }
            }
            // Επιβεβαιώνουμε την ιδιότητα του αυστηρά διαγώνια δεσπόζων πίνακα
            // |Αii| > Σ|Aij|, j=0...N-1, j<>i
            if (rowSum >= abs(Array[i][i])) 
            {
                Array[i][i] = rowSum + rand() % 5 + 1; // Προσαρμόζουμε τα στοιχεία της κύριας διαγωνίου ώστε να ισχύει η ιδιότητα
                Array[i][i] *= (rand() % 2 == 0) ? 1 : -1; // Τυχαία επιλέγουμε πρόσημο στοιχείου της κύριας διαγωνίου
            }
        }
    } 
    else
    {
        // Μη αυστηρά διαγώνια δεσπόζων
        for (i = 0; i < N; i++) 
        {
            rowSum = 0;
            for (j = 0; j < N; j++)
            {
                if (i == j) 
                {
                    Array[i][j] = rand() % 11 - 5; // Στοιχεία κύριας διαγωνίου. Τιμές στο διάστημα [-5, 5]
                } 
                else 
                {
                    Array[i][j] = rand() % 21 - 10; // Στοιχεία που δεν ανήκουν στην κύρια διαγώνιο. Τιμές στο διάστημα [-5, 5]
                    rowSum += abs(Array[i][j]);
                }
            }
            // Επιβεβαιώνουμε την ΜΗ διατήρηση της ιδιότητας του αυστηρά διαγώνια δεσπόζων πίνακα
            // |Αii| <= Σ|Aij|, j=0...N-1, j<>i
            if (abs(Array[i][i]) > rowSum) 
            {
                Array[i][i] = rowSum - rand() % 5 - 1; // Προσαρμόζουμε τα στοιχεία της κύριας διαγωνίου ώστε να ΜΗΝ ισχύει η ιδιότητα
                Array[i][i] *= (rand() % 2 == 0) ? 1 : -1; // Τυχαία επιλέγουμε πρόσημο στοιχείου της κύριας διαγωνίου
            }
        }
    }
}


/*
 *  === Συνάρτηση print2DArray ===
 *  Παράμετροι: Δείκτης σε αρχείο εξόδου, Πίνακας διάστασης N x N που περιέχει ακέραιες τιμές
 *  Επιστρέφει: Τίποτα
 * 
 *  Συνάρτηση που εκτυπώνει έναν πίνακα διάστασης N x N σε ένα αρχείο εξόδου
 */
void print2DArray(FILE *fp, int Array[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            fprintf(fp, "%4d ", Array[i][j]);
        fprintf(fp, "\n");
    }
}

