#!/bin/bash

for i in 6 7 8 9 10 11 12 13
do
    echo $i
    time (./a.out $i $i &> /dev/null)
done
