#!/bin/bash
first=0
second=1
sum=0
for ((i=0;i<$1;++i))
do
echo "$second * $2" | bc
sum=$(($first + $second))
first=$second
second=$sum
done
