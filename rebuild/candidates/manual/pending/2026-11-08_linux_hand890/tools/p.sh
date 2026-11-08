#!/bin/bash
# p.sh <addr> <src.cpp> [extra flags...]: grade one source with the agent890 header overlay; honours // Leaf: and // Flags:
# saves the grade text to work/grades/<stem>.txt, the object to work/objs/<stem>.obj and logs one line in ATTEMPTS.log
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent890
export FABLE_HDR_OVERLAY=$A/hdr
a=$1; f=$2; shift 2
stem=$(basename $f .cpp)
# preprocess check (catches #error / missing include in 0.1 s instead of a spinning cl)
if ! g++ -E -P -x c++ -D_MSC_VER=1310 -I$A/hdr -Irebuild/include $f -o /dev/null 2>$A/work/grades/$stem.pp; then
  echo "$a $stem PREPROCESS_FAIL"; head -5 $A/work/grades/$stem.pp; exit 1; fi
fl=$(grep -o "^// Flags: .*" $f | sed 's|// Flags: ||'); [ "$fl" = "none" ] && fl=""
lf=$(grep -o "^// Leaf: .*" $f | sed 's|// Leaf: ||')
timeout 240 python3 $S/tools/q.py $a $f $lf --flags $fl "$@" > $A/work/grades/$stem.txt 2>&1
st=$(head -1 $A/work/grades/$stem.txt | grep -oE "RELOCATION_MATCH|MATCH|DIFFER\([0-9v]+\)|SRC_FAIL" | head -1)
[ -z "$st" ] && st="TIMEOUT/ERR"
cp $S/wine/q/$a/$a.obj $A/work/objs/$stem.obj 2>/dev/null
echo "$(date +%H:%M:%S) $a $stem $st flags=[$fl $*]" | tee -a $A/ATTEMPTS.log
if [ "$st" = "SRC_FAIL" ]; then grep -E "error|warning C4" $A/work/grades/$stem.txt | head -15; fi
