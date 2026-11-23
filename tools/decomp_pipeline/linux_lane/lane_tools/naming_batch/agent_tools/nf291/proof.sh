#!/bin/bash
# proof.sh <tag> <addr>...: objsec of work/obj/H_<a>.obj v N_<a>.obj under the renames read off the DEF/UND symbol diff
# (one old, one new: paired; several: paired in sorted order, so check the printed map) -> evidence/proof_<tag>.txt
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF291; tag=$1; shift; o=$A/evidence/proof_$tag.txt; : > $o
for a in "$@"; do
  h=$A/work/obj/H_$a.obj; n=$A/work/obj/N_$a.obj
  objdump -t $h | awk '/\(scl   2\)/{print $NF}' | sort -u > $A/work/ps_h.txt; objdump -t $n | awk '/\(scl   2\)/{print $NF}' | sort -u > $A/work/ps_n.txt
  old=$(comm -23 $A/work/ps_h.txt $A/work/ps_n.txt); new=$(comm -13 $A/work/ps_h.txt $A/work/ps_n.txt)
  map=$(paste -d= <(echo "$old") <(echo "$new") | grep -v '^=$')
  echo "##### $a renames: $(echo $map)" >> $o
  python3 $A/tools/objsec.py $h $n $map 2>&1 | grep -E '^(sections|DIFF|ALL_EQ|bad|  )' >> $o
  echo "tables H=$(grep -c '^??_7' $A/work/ps_h.txt) N=$(grep -c '^??_7' $A/work/ps_n.txt)" >> $o
done
cat $o
