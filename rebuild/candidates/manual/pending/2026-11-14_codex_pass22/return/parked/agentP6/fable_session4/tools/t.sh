#!/bin/bash
# t.sh <src> "<description>" : grade, log to ATTEMPTS.log
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; D=$S/w22/agentP6G; T=$S/w22/agentP6F/tools
r=$(bash $T/g.sh 00f3e120 $1 "" -v)
h=$(echo "$r" | head -1 | sed 's/00f3e120 CTriangleStripifier2::FindBestStartVertex //; s/flags=.*\] //')
echo "$(basename $1 .cpp) | $2 | $h" >> $D/ATTEMPTS.log
echo "$(basename $1 .cpp): $h"; echo "$r" | tail -n +2 | head -${3:-8}
