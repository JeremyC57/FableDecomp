#!/bin/bash
a=$(basename $1 .cpp); f=$1
leaf=$(grep -oP '^// Leaf:\s*\K\S+' $f | head -1)
if [ -z "$leaf" ]; then n=$(awk -F'\t' -v a=$a 'tolower($1)==a{print $2; exit}' $FABLE_ROOT/rebuild/oracles/auto-re-candidates.tsv); c=${n%::*}; c=${c##*::}; m=${n##*::}; case $m in operator=) leaf="??4$c@";; *) leaf=$m;; esac; fi
r=$(timeout 300 python3 $S/mig3/scripts/q.py $a $f "$leaf" 2>&1 | head -1)
echo "$r"
