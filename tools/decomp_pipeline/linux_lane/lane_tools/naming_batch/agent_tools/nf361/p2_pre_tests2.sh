#!/usr/bin/env bash
# pre_tests2.sh <rows> <out>: phase A for the closure rows (same as pre_tests.sh, separate output).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361
while read a; do
  tp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  if [ ! -f "$tp" ]; then echo -e "$a\tNOTEST\t$tp"; continue; fi
  r=$(python3 $A/tools/nf.py pre CT_$a $tp nowchar | awk '{print $3}')
  echo -e "$a\t$r\t$tp"
done < "$1" > "$2"
