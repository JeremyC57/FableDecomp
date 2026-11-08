#!/bin/bash
# batch.sh <listfile>: lines "addr<TAB>src<TAB>tag<TAB>note<TAB>[extra flags]" graded two at a time
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent864
one(){ IFS=$'\t' read -r addr src tag note extra <<< "$1"; if [ -n "$extra" ]; then $A/bin/G.sh $addr $src $tag -q --note "$note" --extra "$extra" >/dev/null 2>&1; else $A/bin/G.sh $addr $src $tag -q --note "$note" >/dev/null 2>&1; fi; }
mapfile -t L < "$1"
for ((i=0;i<${#L[@]};i+=2)); do one "${L[i]}" & [ -n "${L[i+1]}" ] && one "${L[i+1]}"; wait; done
echo "BATCH DONE $1" >> $A/tmp/batch.log
