#!/bin/bash
first=0
second=1
sum=0
echo $first
for ((i=0;i<$1;++i))
do
echo "$second * $2" | bc
sum=$(($first + $second))
first=$second
second=$sum
done
