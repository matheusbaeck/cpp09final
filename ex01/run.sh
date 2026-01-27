#!/bin/bash

EXEC="./cpp"

# Format: "Arguments | Expected Output"
# We wrap the * in single quotes inside the string
TESTS=(
    "2 3 + | 5"
    "2 2 3 + - | -3"
    "2 2 3 + \"*\" | 10"
    "5 1 2 + 4 '*' + 3 - | 14"
    "10 0 / | Zero division"
    "1 + | Not enough numbers in the stack"
    "a 2 | Invalid number"
)

echo "=== RPN Calculator Tests ==="

set -f  # Disable globbing
for test in "${TESTS[@]}"; do
    # Split by the pipe delimiter
    IFS="|" read -r input expected <<< "$test"
    
    # Trim whitespace
    input=$(echo $input | xargs)
    expected=$(echo $expected | xargs)

    # Execute (using eval to handle the quoted '*' correctly)
    output=$(eval $EXEC $input 2>&1)

    if [[ "$output" == "$expected" ]]; then
        echo -e "[\033[32mPASS\033[0m] Input: $input  =>  $output"
    else
        echo -e "[\033[31mFAIL\033[0m] Input: $input"
        echo -e "       Got:      '$output'"
        echo -e "       Expected: '$expected'"
    fi
set +f  # Re-enable globbing
done