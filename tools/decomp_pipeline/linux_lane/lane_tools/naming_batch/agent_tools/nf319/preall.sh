#!/bin/bash
# preall.sh <label> <idx..>: clang precheck of rows/<addr>.cpp on work/tree for each ENTRIES index; prints idx addr PASS/FAIL own=<n> hdr=<n>
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319; cd $A
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
L=$1; shift
for i in "$@"; do a=$(sed -n "$((i+1))p" ENTRIES.tsv | cut -f1); [ -f rows/$a.cpp ] || { echo "$i $a NOROW"; continue; }
  r=$(NF_TREE=$A/work/tree timeout 100 python3 tools/nf.py pre ${L}_$a $A/rows/$a.cpp)
  if echo "$r" | grep -q ' PASS '; then echo "$i $a PASS"; else
    own=$(grep -c "rows/$a.cpp" work/pre_err_${L}_$a.txt); tot=$(grep -c . work/pre_err_${L}_$a.txt); echo "$i $a FAIL own=$own hdr=$((tot-own))"; fi
done
