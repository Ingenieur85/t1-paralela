#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include "chrono.h"

// Shared index for dynamic allocation of queries
atomic_int current_query_idx;   // Atomic index for query assignment
pthread_mutex_t search_mutex;   // Mutex for atomic counter

typedef struct {
    long long *arr;
    int n;
    long long *Q;
    int nQ;
    long long *Pos;
} thread_data_t;

// Binary search function to find the lower bound
long long bsearch_lower_bound(long long* arr, int n, long long value) {
    int low = 0;
    int high = n;

    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] < value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

// Thread function for dynamic query allocation
void* thread_bsearch(void* arg) {
    thread_data_t* data = (thread_data_t*) arg;

    while (1) {
        int query_idx = atomic_fetch_add(&current_query_idx, 1);
        if (query_idx >= data->nQ) break;  // Exit when all queries are processed

        // Perform binary search for current query and store result
        data->Pos[query_idx] = bsearch_lower_bound(data->arr, data->n, data->Q[query_idx]);
    }

    return NULL;
}

// Generates a sorted array with incremental values
long long* generate_sorted_array(int n) {
    long long* arr = malloc(n * sizeof(long long));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    arr[0] = rand() % 100;
    for (int i = 1; i < n; i++) {
        arr[i] = arr[i - 1] + rand() % 10;
    }

    return arr;
}

// Generates random queries
long long* generate_random_queries(int nQ, long long max_value) {
    long long* queries = malloc(nQ * sizeof(long long));
    if (!queries) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < nQ; i++) {
        queries[i] = rand() % max_value;
    }

    return queries;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_elements> <number_of_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    int nQ = 100000;  // Number of queries
    srand(time(NULL));

    // Initialize chronometer
    chronometer_t chrono;
    chrono_reset(&chrono);

    // Generate input and query arrays
    long long* Input = generate_sorted_array(n);
    long long* Q = generate_random_queries(nQ, Input[n - 1]);
    long long* Pos = malloc(nQ * sizeof(long long));

    // Initialize atomic index and create threads
    atomic_init(&current_query_idx, 0);
    pthread_t threads[n_threads];
    thread_data_t thread_data = {Input, n, Q, nQ, Pos};

    // Start timing and release threads to perform searches
    chrono_start(&chrono);

    for (int i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, thread_bsearch, &thread_data);
    }

    // Wait for all threads to complete the search
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    chrono_stop(&chrono);

    // Report time and throughput
    double total_time_in_seconds = (double) chrono_gettotal(&chrono) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf ms\n", (1000*total_time_in_seconds));

    double OPS = nQ / total_time_in_seconds;
    printf("Throughput: %lf MOP/s\n", (OPS/1000000));

    // Clean up
    free(Input);
    free(Q);
    free(Pos);
    return 0;
}
