#!/bin/bash

# Define the sets of sizes and thread counts you want to test
array_sizes=(1000000 2000000 4000000 8000000 16000000)
thread_counts=(1 2 3 4 5 6 7 8)
repetitions=10

# Output file (CSV format)
output_file="partA_data.csv"

# Write the CSV header
echo "Array Size,Thread Count,Execution Times,Throughputs" > "$output_file"

# Run the tests for part A
for size in "${array_sizes[@]}"; do
    for threads in "${thread_counts[@]}"; do
        # Arrays to store results for each metric
        times=()
        throughputs=()
        
        # Run each combination 'repetitions' times
        for ((i=1; i<=repetitions; i++)); do
            # Run the executable and capture the output
            output=$(./parteA "$size" "$threads")
            
            # Extract the time and throughput metrics using grep
            execution_time=$(echo "$output" | grep -oP '(?<=total_time_in_seconds: )[^ ]*')
            throughput=$(echo "$output" | grep -oP '(?<=Throughput: )[^ ]*')
            
            # Append the metrics to their respective arrays
            times+=("$execution_time")
            throughputs+=("$throughput")
        done
        
        # Write the results to the CSV file in the specified format
        echo "$size,$threads,${times[*]},${throughputs[*]}" >> "$output_file"
    done
done
