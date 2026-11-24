#!/bin/bash
# vlandchain.sh <name...>: vland --recheck-landed on work/pay_<name>_{H,N}_payload.json (+ _oracle.tsv), HEAD then new, each under its own
# timeout 900, from the repo root, no header overlay; logs work/vland_<name>_{H,N}.log (agent NF353). Run in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp; unset FABLE_HDR_OVERLAY
cd /home/user/FableDecomp || exit 1
for b in "$@"; do
  for k in H N; do
    LANDVERIFY_WORK=$A/work/lv_${b} PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/work/pay_${b}_${k}_payload.json $A/work/pay_${b}_${k}_oracle.tsv --recheck-landed > $A/work/vland_${b}_${k}.log 2>&1
    echo "EXIT $?" >> $A/work/vland_${b}_${k}.log
  done
done
echo CHAIN_DONE "$@" >> $A/work/vlandchain.log
