#!/usr/bin/env bash
# runpass.sh <tag> <H|N> <addr<TAB>src list file>: one `timeout 900` grade per row (pass.py), sequential; run the whole script in the
# background. FABLE_HDR_OVERLAY must be set by the caller (same tree path for the H and N passes).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF341
while IFS=$'\t' read -r a p; do
  [ -n "$a" ] || continue
  timeout 900 python3 $A/tools/pass.py "$1" "$2" "$a=$p"
done < "$3"
echo RUNPASS_DONE >> $A/work/pass_$1.log
