#!/bin/bash
# zs_chain.sh <list file> <src|test>: D1 cl /Zs for each address in the list, sequentially (one background chain); timeout 900 each (agent NF350).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF350
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
for a in $(cat "$1"); do
  timeout 900 bash $A/tools/zs_head.sh $a $2 >> $A/work/zs_chain.log 2>&1
done
echo "ZS_CHAIN_DONE $1 $2" >> $A/work/zs_chain.log
