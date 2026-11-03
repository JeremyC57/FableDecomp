#!/bin/bash
# gradeall.sh variant -> addr grade for every row having rows/<a>/<variant>.cpp (4 parallel)
I=${S}/pragma3
v=$1
for d in $I/rows/*/; do a=$(basename $d); [ -f $d/$v.cpp ] && echo "$a $d/$v.cpp"; done | xargs -P4 -n2 sh -c 'r=$('$I'/qq.sh $0 $1 --calls 2>&1 | grep -v "^  =" | tr "\n" " " | sed "s/leaf=[^ ]* //"); echo "$r"'
