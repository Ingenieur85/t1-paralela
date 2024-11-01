#!/bin/bash

# Fabiano A. de Sá Filho
# GRR 20223831

if [ -z "$1" ]; then
    echo "Usage: $0 <executable_name>"
    exit 1
fi
executable="$1"

#parameters
array_sizes=(1000000 2000000 4000000 8000000 16000000)
thread_counts=(1 2 3 4 5 6 7 8)
repetitions=10

output_file="${executable}_data.csv"
echo "Fazendo testes para $executable" > "$output_file"
echo "Tamanho do Vetor","Threads","Tempo (ms)","MegaOPs" >> "$output_file"


# Run the tests and save results to separate CSV files for each array size
for size in "${array_sizes[@]}"; do
    # Create a separate CSV file for each array size
    #output_file="${executable}_data_${size}.csv"
    
    # Write the CSV header for this array size
    #echo >> "$output_file"
    #echo "Fazendo testes para tamanho $size" >> "$output_file"
    
    for threads in "${thread_counts[@]}"; do
        #echo >> "$output_file"
        # Arrays to store results for each metric
        times=()
        throughputs=()
        
        # Run each combination 'repetitions' times
        for ((i=1; i<=repetitions; i++)); do
            # Run the executable and capture the output
            output=$("./$executable" "$size" "$threads")
            
            # Extract the time and throughput metrics
            execution_time=$(echo "$output" | grep -oP '(?<=total_time_in_seconds: )[^ ]*')
            throughput=$(echo "$output" | grep -oP '(?<=Throughput: )[^ ]*')
            
            # Append the metrics to their respective arrays
            times+=("$execution_time")
            throughputs+=("$throughput")
        done
        
        # Write the results for this array size and thread count to the CSV file
        #echo "Threads: $threads" >> "$output_file"
        #echo "Tempo (s)","OPs/s" >> "$output_file"
        for ((i=0; i<repetitions; i++)); do
            echo "$size","$threads",${times[i]},${throughputs[i]} >> "$output_file"
        done
    done
done
