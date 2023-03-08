//author Zihan Xia
#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include <time.h>

#include <omp.h>



#define MAX_THREADS 100



int num_threads = 1;

int* arr;

int* arr1;

int* arr2;

clock_t t;

clock_t d;

clock_t a;



void swap(int* a, int* b) {

    int temp = *a;

    *a = *b;

    *b = temp;

}



int partition(int l, int r) {

    int pivot = arr[r];

    int i = l - 1;

    for (int j = l; j < r; j++) {

        if (arr[j] < pivot) {

            i++;

            swap(&arr[i], &arr[j]);

        }

    }

    swap(&arr[i+1], &arr[r]);

    return i + 1;

}



void quicksort(int l, int r) {

    if (l < r) {

        int pivot = partition(l, r);

        quicksort(l, pivot - 1);

        quicksort(pivot + 1, r);

    }

}



typedef struct {

    int l;

    int r;

} args_t;



void* parallel_quicksort(void* arg) {

    

    args_t* args = (args_t*)arg;

    int l = args->l;

    int r = args->r;

    if (l < r) {

        int pivot = partition(l, r);

        if (num_threads > 1) {

            pthread_t left_thread, right_thread;

            args_t left_args = {l, pivot - 1};

            args_t right_args = {pivot + 1, r};

            if (num_threads > 2) {

                num_threads--;

                pthread_create(&left_thread, NULL, parallel_quicksort, &left_args);

                pthread_create(&right_thread, NULL, parallel_quicksort, &right_args);

                pthread_join(left_thread, NULL);

                pthread_join(right_thread, NULL);

                num_threads++;

            } else {

                quicksort(l, pivot - 1);

                quicksort(pivot + 1, r);

            }

        } else {

            quicksort(l, pivot - 1);

            quicksort(pivot + 1, r);

        }

    }

    pthread_exit(NULL);

}





int partition_pure(int* arr, int l, int r) {

    int pivot = arr[r];

    int i = l - 1;

    for (int j = l; j < r; j++) {

        if (arr[j] < pivot) {

            i++;

            swap(&arr[i], &arr[j]);

        }

    }

    swap(&arr[i+1], &arr[r]);

    return i + 1;

}



void quicksort_pure(int* arr, int l, int r) {

    if (l < r) {

        int pivot = partition_pure(arr, l, r);

        quicksort_pure(arr, l, pivot - 1);

        quicksort_pure(arr, pivot + 1, r);

    }

}











int partition_open(int arr[], int low, int high) {

    int pivot = arr[high];

    int i = low - 1;



    for (int j = low; j < high; j++) {

        if (arr[j] < pivot) {

            i++;

            swap(&arr[i], &arr[j]);

        }

    }



    swap(&arr[i+1], &arr[high]);

    return i+1;

}



void quicksort_open(int arr[], int low, int high) {

    if (low < high) {

        int pivot = partition_open(arr, low, high);



        #pragma omp parallel sections

        {

            #pragma omp section

            {

                quicksort_open(arr, low, pivot-1);

            }

            #pragma omp section

            {

                quicksort_open(arr, pivot+1, high);

            }

        }

    }

}











int main(int argc, char** argv) {

    if (argc < 2) {

        printf("Usage: ./parallel_quicksort <array size> [<num threads>]\n");

        return 1;

    }

    int n = atoi(argv[1]);



    if (argc >= 3) {

        num_threads = atoi(argv[2]);

    }

    if (num_threads > MAX_THREADS) {

        num_threads = MAX_THREADS;

    }

    

    arr1 =  malloc(n * sizeof(int));

    arr = malloc(n * sizeof(int));

    arr2 = malloc(n * sizeof(int));

    srand(time(NULL));

    for (int i = 0; i < n; i++) {

        arr[i] = rand() % 100;

    }



    for (int i =0; i<n;i++){

      arr1[i] = arr[i];

      arr2[i] = arr[i];

    }

 

    /**

    printf("Before sorting:\n");

    for (int i = 0; i < n; i++) {

        printf("%d ", arr[i]);

    }

    printf("\n");

    */

    t = clock();

    pthread_t threads[MAX_THREADS];

    args_t args = {0, n - 1};

    pthread_create(&threads[0], NULL, parallel_quicksort, &args);

    pthread_join(threads[0], NULL);

    /***

    printf("After sorting:\n");

    for (int i = 0; i < n; i++) {

        printf("%d ", arr[i]);

    }

    printf("\n");

    ***/

    t = clock()-t;

    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    //printf("pthread took %f seconds to execute \n", time_taken);

    free(arr);

    //printf("standard library\n");

    d = clock();

    /***

    printf("Before sorting:\n");

    for (int i = 0; i < n; i++) {

        printf("%d ", arr1[i]);

    }

    printf("\n");

    ***/

    quicksort_pure(arr1, 0, n - 1);

    /***

    printf("After sorting:\n");

    for (int i = 0; i < n; i++) {

        printf("%d ", arr1[i]);

    }

    printf("\n");

    ***/

    d = clock()-d;

    double time_taken1 = ((double)d)/CLOCKS_PER_SEC; // in seconds

    free(arr1);



    

    // printf("Openmp library\n");

    a = clock();

    /***

    printf("Before sorting:\n");

    for (int i = 0; i < n; i++) {

        printf("%d ", arr2[i]);

    }

    printf("\n");

    ***/

    #pragma omp parallel

    {

        #pragma omp single

        {

            quicksort_open(arr2, 0, n-1);

        }

    }

    /***

    printf("After sorting:\n");

    for (int i = 0; i < n; i++) {

        printf("%d ", arr2[i]);

    }

    printf("\n");

    ***/

    a = clock()-a;

    double time_taken2 = ((double)a)/CLOCKS_PER_SEC; // in seconds



    printf("Openmp library took %f seconds to execute \n", time_taken2);

    printf("standard library took %f seconds to execute \n", time_taken1);

    printf("pthread took %f seconds to execute \n", time_taken);

    free(arr2);



    FILE *fptr;

    fptr = fopen("two.txt","a");



    if(fptr == NULL)

      {

	printf("Error!");   

	exit(1);             

      }

    fprintf(fptr,"%f, %f\n",time_taken1,time_taken2);

    fclose(fptr);

    return 0;

}

