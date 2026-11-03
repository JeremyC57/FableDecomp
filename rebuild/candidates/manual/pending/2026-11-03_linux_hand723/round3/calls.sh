#!/bin/bash
f=$1
echo "R: $(sed -n 2,/^---\ built/p $f | grep -c call) O: $(sed -n '/^--- built/,$p' $f | grep -c call)  R-size: $(sed -n 2,/^---\ built/p $f | wc -l) O-size: $(sed -n '/^--- built/,$p' $f | wc -l)"
paste <(sed -n 2,/^---\ built/p $f | grep -B1 "call" | grep -v "^--" | awk '{$1="";$2="";print}' | paste -d'|' - - ) <(sed -n '/^--- built/,$p' $f | grep -B1 "call" | grep -v "^--" | awk '{$1="";$2="";print}' | paste -d'|' - -) | cut -c1-200
