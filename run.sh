#!/bin/bash

make clean
make

# Define the set of parts (executables)
parts=("parteA")

# Name of the main script that will be called
main_script="./gen_data.sh" 

# Check if the main script exists and is executable
if [ ! -x "$main_script" ]; then
    echo "Error: $main_script not found or not executable."
    exit 1
fi

# Run the main script for each part
for part in "${parts[@]}"; do
    echo "Running tests for $part..."
    $main_script "$part"
    echo "Completed tests for $part."
done
