#!/bin/bash
# prep.sh <tag> <file>...: precheck each file (plain, then NOWCHAR=1 on failure); logs PASS / PASS_NOWCHAR / FAIL to work/pre_<tag>.log
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF291; tag=$1; shift
for f in "$@"; do o=$(bash $A/tools/pre.sh $f); st=PASS
  if [ -n "$o" ]; then o2=$(NOWCHAR=1 bash $A/tools/pre.sh $f); if [ -z "$o2" ]; then st=PASS_NOWCHAR; else st=FAIL; fi; fi
  echo "$(date +%T) $st $f" | tee -a $A/work/pre_$tag.log; [ $st = FAIL ] && echo "$o" | head -2
done; true
