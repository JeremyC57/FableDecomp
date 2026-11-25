#!/bin/bash
# closure.sh: compile the HEAD object of every landed row whose source names CVtable_012beda4 (any case) and list the relocation
# symbols of each object that name the old class (??0 / ??1 / ??_7 / any member). Output: $W/evidence/closure_head.txt
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; cd /home/user/FableDecomp; export FABLE_HDR_OVERLAY=$W/empty_overlay
out=$W/evidence/closure_head.txt; : > $out
for f in $(grep -rli "CVtable_012beda4" rebuild/src | sort); do
  a=$(basename $f .cpp); a=${a: -8}
  python3 $S/tools/q.py $a $f x > /dev/null 2>&1
  o=$S/wine/q/$a/$a.obj
  if [ -f $o ]; then
    refs=$(objdump -r $o | awk '{print $3}' | grep -i "CVtable_012beda4" | sort -u | tr '\n' ' ')
    defs=$(objdump -t $o | grep -i "CVtable_012beda4" | awk '{print $NF}' | sort -u | tr '\n' ' ')
    echo -e "$a\trefs: $refs\tdefs: $defs" >> $out
  else echo -e "$a\tNO_OBJ" >> $out; fi
done
echo DONE >> $out
