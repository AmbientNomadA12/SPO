#!/bin/bash
echo "Products with greater quantity than $N"
awk -F '\t' -v quantity="$N" 'NR>1 && $2>quantity {print $1}' inputfile
