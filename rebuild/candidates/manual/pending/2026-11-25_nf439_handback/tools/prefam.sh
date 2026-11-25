#!/usr/bin/env bash
# prefam.sh <batch>: clang precheck of every HEAD row, HEAD test and new row of a batch (nf.py pre, NOWCHAR) -> work/pre.log
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439; B=$W/batches/$1
for f in $B/rows/*.cpp; do a=$(basename $f .cpp)
  python3 $W/tools/nf.py pre ${1}H_$a $B/orig/rows/$a.cpp nw >/dev/null
  [ -f $B/orig/tests/${a}_test.cpp ] && python3 $W/tools/nf.py pre ${1}T_$a $B/orig/tests/${a}_test.cpp nw >/dev/null
  python3 $W/tools/nf.py pre ${1}N_$a $f nw >/dev/null
done; echo PREFAM_DONE $1 >> $W/work/pre.log
