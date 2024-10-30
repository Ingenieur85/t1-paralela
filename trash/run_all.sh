#!/bin/bash

# Define the sets of sizes and thread counts you want to test
array_sizes=(1000 5000 10000)       # Example array sizes
thread_counts=(1 2 4 8)             # Example thread counts
repetitions=10                      # Number of times each test should be run

# Run the tests and save results to separate CSV files for each array size
for size in "${array_sizes[@]}"; do
    # Create a separate CSV file for each array size
    output_file="test_results_${size}.csv"
    
    # Write the CSV header for this array size
    echo "Array Size,Thread Count,Execution Times,Throughputs" > "$output_file"

    for threads in "${thread_counts[@]}"; do
        # Arrays to store results for each metric
        times=()
        throughputs=()
        
        # Run each combination 'repetitions' times
        for ((i=1; i<=repetitions; i++)); do
            # Run the executable and capture the output
            output=$(./parteA "$size" "$threads")
            
            # Extract the time and throughput metrics
            execution_time=$(echo "$output" | grep -oP '(?<=total_time_in_seconds: )[^ ]*')
            throughput=$(echo "$output" | grep -oP '(?<=Throughput: )[^ ]*')
            
            # Append the metrics to their respective arrays
            times+=("$execution_time")
            throughputs+=("$throughput")
        done
        
        # Write the results for this array size and thread count to the CSV file
        echo "$size,$threads,${times[*]},${throughputs[*]}" >> "$output_file"
    done
done
