#!/bin/bash
# pre_tests.sh <files tsv (addr owner src test)> <donefile>: clang precheck of every landed TEST (label T_<addr>) -> work/pre.log; run BEFORE the locks
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF312
while IFS=$'\t' read -r a o s t; do
  timeout 120 python3 $A/tools/nf.py pre T_$a "$t" > /dev/null
done < $1
echo done > $2
