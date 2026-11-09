#!/bin/bash
# p7.sh <addr> <src.cpp>: grade one source (q.py) from work/s7 (never cd into the repo); agent890 header overlay; logs ATTEMPTS.log
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent890; f=$(readlink -f $2); cd $A/work/s7
export FABLE_HDR_OVERLAY=$A/hdr
a=$1; stem=$(basename $f .cpp)
[ -f "$f" ] || { echo "no such source: $2"; exit 1; }
fl=$(grep -o "^// Flags: .*" $f | sed 's|// Flags: ||'); [ "$fl" = "none" ] && fl=""
lf=$(grep -o "^// Leaf: .*" $f | sed 's|// Leaf: ||')
timeout 240 python3 $S/tools/q.py $a $f $lf --flags $fl > $A/work/grades/$stem.txt 2>&1
st=$(head -1 $A/work/grades/$stem.txt | grep -oE "RELOCATION_MATCH|MATCH|DIFFER\([0-9v]+\)|SRC_FAIL" | head -1)
[ -z "$st" ] && st="TIMEOUT/ERR"
cp $S/wine/q/$a/$a.obj $A/work/objs/$stem.obj 2>/dev/null
echo "$(date +%H:%M:%S) $a $stem $st flags=[$fl]" | tee -a $A/ATTEMPTS.log
