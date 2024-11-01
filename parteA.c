#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include "chrono.h"

#define MAX_THREADS 64

// Structure to pass arguments to thread function
typedef struct {
    long long* arr;
    int n;
    long long value;
    int low;
    int high;
    int* result;
    atomic_int* search_count;
} thread_data_t;

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

// Threaded binary search function to find the lower bound
void* threaded_bsearch(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int low = data->low;
    int high = data->high;

    while (low < high) {
        int mid = low + (high - low) / 2;
        atomic_fetch_add(data->search_count, 1); // Increment search count atomically

        if (data->arr[mid] < data->value) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    *(data->result) = low;
    return NULL;
}

// Modified binary search with parallelization
long long parallel_bsearch_lower_bound(long long* arr, int n, long long value, int n_threads, atomic_int* search_count) {
    pthread_t threads[MAX_THREADS];
    thread_data_t thread_data[MAX_THREADS];
    int segment = n / n_threads;
    int result = -1;

    for (int i = 0; i < n_threads; i++) {
        int low = i * segment;
        int high = (i == n_threads - 1) ? n : (i + 1) * segment;

        thread_data[i] = (thread_data_t) {
            .arr = arr,
            .n = n,
            .value = value,
            .low = low,
            .high = high,
            .result = &result,
            .search_count = search_count
        };

        pthread_create(&threads[i], NULL, threaded_bsearch, &thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}

// Main function with updated parallelization
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_elements> <number_of_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    if (n_threads > MAX_THREADS) n_threads = MAX_THREADS;

    int nQ = 100000;  // Number of queries
    srand(time(NULL));

    // Initialize chronometer
    chronometer_t chrono;
    chrono_reset(&chrono);

    // Generate input and query arrays
    long long* Input = generate_sorted_array(n);
    long long* Q = generate_random_queries(nQ, Input[n - 1]);
    atomic_int search_count = 0;

    // Start timing
    chrono_start(&chrono);
    for (int i = 0; i < nQ; i++) {
        parallel_bsearch_lower_bound(Input, n, Q[i], n_threads, &search_count);
    }
    chrono_stop(&chrono);

    // Report time
    double total_time_in_seconds = (double) chrono_gettotal(&chrono) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    double OPS = search_count / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);

    // Clean up
    free(Input);
    free(Q);

    return 0;
}
