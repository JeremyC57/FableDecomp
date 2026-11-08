#!/bin/bash
# varl.sh <addr> <src> <line> <prefix> <replacement-lines-file> : one variant per line of the file (line <line> replaced), graded in turn
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent863
addr=$1; src=$2; ln=$3; pre=$4; i=0
while IFS= read -r r; do i=$((i+1)); python3 -c "
import sys; L=open(sys.argv[1]).read().split('\n'); L[int(sys.argv[2])-1]=sys.argv[3]; open(sys.argv[4],'w').write('\n'.join(L))" "$src" "$ln" "$r" $A/tmp/${pre}$i.cpp
$A/bin/g $addr $A/tmp/${pre}$i.cpp ${pre}$i --note "$(echo "$r" | sed 's/^ *//' | cut -c1-110)" >/dev/null 2>&1; tail -1 $A/ATTEMPTS.log | cut -f3-5; rm -rf $A/w/${addr}_${pre}$i; done < $5
