#!/bin/bash
# probe every listed row source with --trace --rotation, two at a time
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent864
run(){ timeout 900 $A/bin/P.sh $A/src/$1.cpp tr --trace --rotation > /dev/null 2>&1; echo "$1 done $(date +%T)" >> $A/tmp/probeall.log; }
rows=("$@")
for ((i=0;i<${#rows[@]};i+=2)); do run ${rows[i]} & [ -n "${rows[i+1]}" ] && run ${rows[i+1]}; wait; done
echo ALLDONE >> $A/tmp/probeall.log
