#!/bin/bash
# run_grades.sh <job>: one lane of the grade run (each grade command on its own `timeout 900` line); writes work/job_<job>.done
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF310; W=$A/work; cd $W
ct=($(cat $W/ctl.txt)); n=${#ct[@]}; q=$(( (n+3)/4 ))
case $1 in
  1) timeout 900 python3 $A/tools/nf.py grade c1 $(sed -n 1,2p $W/ch.txt) ;;
  2) timeout 900 python3 $A/tools/nf.py grade c2 $(sed -n 3p $W/ch.txt) ;;
  3) timeout 900 python3 $A/tools/nf.py grade c3 $(sed -n 4p $W/ch.txt) ;;
esac
k=$(( $1 - 1 ))
timeout 900 python3 $A/tools/nf.py hgrade k$1 ${ct[@]:$((k*q)):$q}
echo done > $W/job_$1.done
