#!/bin/bash
# grade_lane.sh <tag> <changed list file> <control list file> <donefile>: HEAD + new grades of the changed rows, HEAD-only of the
# controls, one row per grade command, each on its own `timeout 900` line (nf.py grade / hgrade); logs work/gr_<tag>.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF336; W=$A/work; cd $W
for a in $(cat $2); do
  timeout 900 python3 $A/tools/nf.py grade $1 $a
done
for a in $(cat $3); do
  timeout 900 python3 $A/tools/nf.py hgrade $1 $a
done
echo done > $4
