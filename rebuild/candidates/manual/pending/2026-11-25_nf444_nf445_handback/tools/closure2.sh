#!/bin/bash
# closure2.sh <regex> <out> <file>...: compile each HEAD row source (q.py, empty overlay) and list its UNDEFINED symbols matching <regex>.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; cd /home/user/FableDecomp; export FABLE_HDR_OVERLAY=$W/h_overlay/inc
re=$1; out=$2; shift 2; : > $out
for f in "$@"; do a=$(basename $f .cpp); a=${a: -8}
  python3 $S/tools/q.py $a $f x > /dev/null 2>&1; o=$S/wine/q/$a/$a.obj
  [ -f $o ] || { echo -e "$a\tNO_OBJ" >> $out; continue; }
  u=$(objdump -t $o | grep "(sec  0)" | awk '{print $NF}' | grep -E "$re" | sort -u | tr '\n' ' ')
  d=$(objdump -t $o | grep -v "(sec  0)" | grep "(scl   2)" | awk '{print $NF}' | grep -E "$re" | sort -u | tr '\n' ' ')
  echo -e "$a\tUND: $u\tDEF: $d" >> $out
done; echo DONE >> $out
