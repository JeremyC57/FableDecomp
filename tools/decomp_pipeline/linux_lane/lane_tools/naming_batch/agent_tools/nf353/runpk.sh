#!/bin/bash
# runpk.sh <H|N> <list file>: pk.py per row, sequential, each under its own timeout 900 (agent NF353). Run in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp
L=$(readlink -f "$2"); cd $A/work
for a in $(cat "$L"); do
  timeout 900 python3 $A/tools/pk.py $1 $a >> $A/work/runpk.err 2>&1
done
echo "RUNPK_DONE $1 $2" >> $A/work/grades_$1.log
