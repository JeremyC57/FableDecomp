#!/usr/bin/env bash
# pre_srcs361.sh <label> <file>...: clang precheck of each source (after its lock), result lines in work/pre.log; work/pre_srcs_<label>.tsv
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF368
lab=$1; shift
for f in "$@"; do
  a=$(basename $f .cpp | grep -o '[0-9a-f]\{8\}$')
  r=$(python3 $A/tools/nf.py pre S${lab}_$a $f nowchar | awk '{print $3}')
  echo -e "$a\t$r\t$f"
done > $A/work/pre_srcs_$lab.tsv
echo PRE_DONE >> $A/work/pre_srcs_$lab.tsv
