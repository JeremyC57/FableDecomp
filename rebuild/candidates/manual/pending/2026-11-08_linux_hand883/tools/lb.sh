#!/bin/bash
# lb.sh <name>...: grade probe/<name>.cpp one at a time with the row's // Flags:, print one leaderboard line each (+ LEADERBOARD.tsv)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent883
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump
L='?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z'
for n in "$@"; do
  fl=$(grep -o "^// Flags: .*" $A/probe/$n.cpp | sed 's|// Flags: ||')
  r=$(bash $A/gp.sh $n ${fl:+--flags $fl} | head -1)
  if echo "$r" | grep -q "SRC_FAIL"; then echo "$n SRC_FAIL"; grep -i "error" $A/tmp/g_$n.txt | head -3; continue; fi
  cp $S/wine/q/006096e0/006096e0.obj $A/tmp/$n.obj
  m=$(python3 $A/tools/cmpx.py $A/tmp/$n.obj 006096e0 "$L")
  s=$(python3 $A/tools/cmpx.py $A/tmp/$n.obj 006096e0 "$L" --segs --mask frame | grep "^seg " | awk '{print $2}' | tr '\n' ',')
  line="$n	$(echo $r | grep -o 'DIFFER([0-9v]*)\|MATCH\|RELOCATION_MATCH')	$(echo $m | sed 's/.*frame esp, \(0x[0-9a-f]*\) v.*exact \([0-9.]*%\).*regs+frame \([0-9.]*%\)/fr \1 ex \2 rf \3/')	segs $(echo $s | tr ',' '\n' | grep -c .) [$s]"
  echo "$line"; echo "$(date +%H:%M) $line" >> $A/LEADERBOARD.tsv
  rm -f $A/tmp/g_$n.txt
done
