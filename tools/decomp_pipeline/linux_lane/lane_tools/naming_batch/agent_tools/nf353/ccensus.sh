#!/bin/bash
# ccensus.sh <list>: ccompile.py per row, sequential, each under its own timeout 900 (agent NF353). Run in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp
L=$(readlink -f "$1"); cd $A/work
for a in $(cat "$L"); do
  timeout 900 python3 $A/tools/ccompile.py $a >> $A/work/ccensus.err 2>&1
done
echo "CCENSUS_DONE $1" >> $A/work/census_grades.log
