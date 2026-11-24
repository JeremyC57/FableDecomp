#!/bin/bash
# runq.sh <H|N> <addr...>: run pk.py for each address sequentially (one background chain); each pk.py under its own timeout 900.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF343
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
k=$1; shift
cd $A/work
for a in "$@"; do
  timeout 900 python3 $A/tools/pk.py $k $a >> $A/work/runq.log 2>&1
done
echo "CHAIN_DONE $k $*" >> $A/work/runq.log
