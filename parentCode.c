/* 
Fabiano de Sá Filho
GRR: 20223831
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chrono.h"

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
    int search_count = 0;

/*
    printf("Generated input array:\n");
    print_array(Input, n);
    printf("\n");
    printf("Generated query array:\n");
    print_array(Q, nQ);
    printf("\n");
*/

    // Start timing, execute binary search for each query, and stop timing
    chrono_start(&chrono);
    for (int i = 0; i < nQ; i++) {
        //long long result = bsearch_lower_bound(Input, n, Q[i]);
        bsearch_lower_bound(Input, n, Q[i]);
        search_count++;
        //printf("Query %d: %lld\n", i+1, result);
    }
    chrono_stop(&chrono);

    // Report time
    //chrono_reportTime(&chrono, "Binary search for Part A");

    // calcular e imprimir a VAZAO (numero de operacoes/s)
    double total_time_in_seconds = (double) chrono_gettotal(&chrono) /
                                      ((double)1000*1000*1000);
    printf( "total_time_in_seconds: %lf s\n", total_time_in_seconds );

    double OPS = search_count/total_time_in_seconds;
    printf( "Throughput: %lf OP/s\n", OPS ); 

    // Clean up
    free(Input);
    free(Q);

    return 0;
}
