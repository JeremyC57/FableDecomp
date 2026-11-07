#!/bin/bash
# usage: gr.sh <addr> <srcfile> <tag>   -> grades with Leaf/Flags lines, saves tmp/<tag>.out, copies probe/<tag>.cpp, prints status + md
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent855
export FABLE_HDR_OVERLAY=$A/hdr
a=$1; f=$2; t=$3
[ "$f" != "$A/probe/$t.cpp" ] && cp $f $A/probe/$t.cpp
fl=$(grep -m1 -o "^// Flags: .*" $f | sed 's|// Flags: ||'); lf=$(grep -m1 -o "^// Leaf: .*" $f | sed 's|// Leaf: ||' | awk '{print $1}')
timeout ${TO:-300} python3 $S/tools/q.py $a $f $lf ${fl:+--flags $fl} > $A/tmp/$t.out 2>&1
st=$(grep -m1 -E "^$a " $A/tmp/$t.out)
if grep -q -- "--- retail" $A/tmp/$t.out; then
  python3 $A/bin/rmd.py $a "$lf" x > $A/tmp/$t.diff 2>&1
  md=$(tail -1 $A/tmp/$t.diff | awk '{print $2}')
  rl=$(grep -o -- "--- retail ([0-9]*" $A/tmp/$t.out | grep -o "[0-9]*$"); bl=$(grep -o -- "--- built ([0-9]*" $A/tmp/$t.out | grep -o "[0-9]*$")
  r="$t: $(echo $st | awk '{print $3}') ${bl}v${rl} md=$md"; echo "$r"; echo "$(date +%H:%M) $a $r" >> $A/ATTEMPTS.log
else
  r="$t: ${st:-NO_STATUS} $(grep -m3 -E 'error C|WARN' $A/tmp/$t.out | cut -c1-200)"; echo "$r"; echo "$(date +%H:%M) $a $r" >> $A/ATTEMPTS.log
fi
