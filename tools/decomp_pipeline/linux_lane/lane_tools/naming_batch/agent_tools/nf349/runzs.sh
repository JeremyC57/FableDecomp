#!/usr/bin/env bash
# runzs.sh <addr<TAB>path file> <src|test>: one `timeout 900` VC7.1 cl /Zs (nf.py zs) per file, sequential; run in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF349
while IFS=$'\t' read -r a p; do [ -n "$a" ] && timeout 900 python3 $A/tools/nf.py zs zs_$2_$a "$p" "$2"; done < "$1"
echo RUNZS_DONE >> $A/work/pre_zs.log
