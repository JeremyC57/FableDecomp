#!/bin/bash
# runp.sh <addr> <kind:src>...: run pk.py <kind> <addr> <src> sequentially (one background chain, one work path); timeout 900 per compile.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF350
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
a=$1; shift
cd $A/work
for ks in "$@"; do
  k=${ks%%:*}; s=${ks#*:}
  timeout 900 python3 $A/tools/pk.py $k $a $s >> $A/work/runq.log 2>&1
done
echo "CHAIN_DONE $a $*" >> $A/work/runq.log
