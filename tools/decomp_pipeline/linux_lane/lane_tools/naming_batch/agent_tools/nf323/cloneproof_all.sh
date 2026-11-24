#!/bin/bash
# cloneproof_all.sh <tag> <addr:Class:vtva>...: cloneproof.py on work/wd_<tag>h v work/wd_<tag>n objects -> evidence/cloneproof_<tag>.txt
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
A=$S/w22/agentNF323; t=$1; shift; out=$A/evidence/cloneproof_$t.txt; : > $out
for x in "$@"; do
  IFS=: read a C v <<< "$x"; p=${a:0:2}/${a:2:2}/$a/$a.obj
  old=$(grep -o "^// Leaf:.*\|was ?Clone@[^ )]*" $A/orig/$a.cpp | head -1)
  oldsym=$(grep -o "was ?Clone@[^ )]*" $A/rows/$a.cpp | sed 's/^was //')
  echo "== $a $C vt=$v old=$oldsym" >> $out
  python3 $A/tools/cloneproof.py $A/work/wd_${t}h/$p $A/work/wd_${t}n/$p $C $v "$oldsym" "?Clone@$C@@UBEPAVCCreatureActionBase@@XZ" >> $out 2>&1
done
grep -c "^OK" $out; grep "^== \|FAIL\|NOT_OK" $out | cut -c1-400
