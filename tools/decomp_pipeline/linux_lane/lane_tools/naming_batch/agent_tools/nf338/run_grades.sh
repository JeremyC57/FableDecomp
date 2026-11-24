#!/bin/bash
# run_grades.sh <job>: one lane of the grade run (each grade command on its own `timeout 900` line); writes work/job_<job>.done
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF338; W=$A/work; cd $W
ct=($(cat $W/ctl.txt)); n=${#ct[@]}; q=$(( (n+3)/4 ))
case $1 in
  1) timeout 900 python3 $A/tools/nf.py grade c1 $(sed -n 1,2p $W/ch.txt) ;;
  2) timeout 900 python3 $A/tools/nf.py grade c2 $(sed -n 3p $W/ch.txt) ;;
  3) timeout 900 python3 $A/tools/nf.py grade c3 $(sed -n 4p $W/ch.txt) ;;
esac
k=$(( $1 - 1 ))
timeout 900 python3 $A/tools/nf.py hgrade k$1 ${ct[@]:$((k*q)):$q}
echo done > $W/job_$1.done
