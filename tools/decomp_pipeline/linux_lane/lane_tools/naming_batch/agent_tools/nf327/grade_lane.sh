#!/bin/bash
# grade_lane.sh <tag> <changed list file> <control list file> <donefile>: HEAD + new grades of the changed rows, HEAD-only of the
# controls, one row per grade command, each on its own `timeout 900` line (nf.py grade / hgrade); logs work/gr_<tag>.log
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF327; W=$A/work; cd $W
for a in $(cat $2); do
  timeout 900 python3 $A/tools/nf.py grade $1 $a
done
for a in $(cat $3); do
  timeout 900 python3 $A/tools/nf.py hgrade $1 $a
done
echo done > $4
