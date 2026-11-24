#!/bin/bash
# pre_tests.sh <files tsv (addr owner src test)> <donefile>: clang precheck of every landed TEST (label T_<addr>) -> work/pre.log; run BEFORE the locks
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF336
while IFS=$'\t' read -r a o s t; do
  timeout 120 python3 $A/tools/nf.py pre T_$a "$t" > /dev/null
done < $1
echo done > $2
