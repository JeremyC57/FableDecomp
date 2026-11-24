#!/usr/bin/env bash
# vland361.sh <batch...>: vland --recheck-landed of work/pay_<b>_H (landed HEAD text) then work/pay_<b>_N (new text), repo headers (no
# header change in NF361, so no overlay), each with its own `timeout 900`, from the repo root; logs work/vland_<b>_<H|N>.log. Background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
cd /home/user/FableDecomp || exit 1
for b in "$@"; do for k in H N; do
  env FABLE_HDR_OVERLAY= LANDVERIFY_WORK=$A/work/lv_${b}_$k PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/work/pay_${b}_${k}_payload.json $A/work/pay_${b}_${k}_oracle.tsv --recheck-landed > $A/work/vland_${b}_$k.log 2>&1
  echo "EXIT $?" >> $A/work/vland_${b}_$k.log
done; done
echo CHAIN_DONE "$@" >> $A/work/vlandchain.log
