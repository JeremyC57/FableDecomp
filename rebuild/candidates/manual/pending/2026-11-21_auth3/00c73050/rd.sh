#!/bin/bash
# rd.sh <v.cpp> [extra flags]: hook readout (queue/sound prio + regs + temps), no grade
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; W=$S/w22/auth3/00c73050
cd /home/user/FableDecomp; source $S/wine/env.sh >/dev/null 2>&1
f=$1; b=$(basename $f .cpp); shift
FL="/d2inlT150"; [ -n "$NOFL" ] && FL=""
timeout 300 python3 $S/w22/auth2/c2x/c2prio_x.py $f --func UpdateStream --flags "$FL $*" --blocks queue > $W/rd_$b.txt 2>&1
echo "== $b rc=$(grep -o 'cl rc [0-9]*' $W/rd_$b.txt)"
awk '/colouring order/{p=1} /not in the list/{p=0} p && /^ +[0-9]+ +[0-9]+ /' $W/rd_$b.txt | awk '$3=="queue"||$3=="sound"||$3=="this"||$3=="temp"' 
echo "$(date +%T) $b" >> $W/readouts.log
