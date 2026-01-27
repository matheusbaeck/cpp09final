#!/bin/bash

# Check if correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <quantity> <max_int>"
    exit 1
fi

QUANTITY=$1
MAX=$2

# Validate that quantity does not exceed the range of available unique numbers
if [ "$QUANTITY" -gt "$MAX" ]; then
    echo "Error: Quantity ($QUANTITY) cannot be greater than Max Int ($MAX) for unique numbers."
    exit 1
fi

# Generate unique random positive integers using shuf
-i 1-$MAX: specifies the range from 1 to max_int
-n $QUANTITY: specifies how many numbers to pick
tr '\n' ' ': replaces newlines with spaces to create a single line list
shuf -i 1-"$MAX" -n "$QUANTITY" | tr '\n' ' '
echo "" # Add a final newline for clean output