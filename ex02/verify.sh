#!/bin/bash

# Read input from stdin
INPUT=$(cat)

# Print the original output as requested
echo "$INPUT"

# Extract the numbers after the "After:" label
SORTED_LINE=$(echo "$INPUT" | grep "After:" | sed 's/After: *//')

# Check if sorted
IS_SORTED=$(echo "$SORTED_LINE" | tr ' ' '\n' | grep -v '^$' | awk '
BEGIN { prev = -1; fail = 0 }
{
    if ($1 < prev) { fail = 1; exit }
    prev = $1
}
END { if (fail == 1) print "KO"; else print "OK" }')

# Print result in color
if [ "$IS_SORTED" == "OK" ]; then
    echo -e "\033[0;32mOK\033[0m" # Green OK
else
    echo -e "\033[0;31mKO\033[0m" # Red KO
    exit 1
fi