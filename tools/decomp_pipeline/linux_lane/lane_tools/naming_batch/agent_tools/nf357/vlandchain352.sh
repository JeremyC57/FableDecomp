#!/usr/bin/env bash
# vlandchain352.sh (NF357 copy: work/treeN = full rebuild/include copy with the two new headers) <batch...>: for each batch name (work/pay_<b>_{H,N}_payload.json / _oracle.tsv from mkbatch349.py), vland --recheck-landed
# HEAD (repo headers, no overlay) then new (FABLE_HDR_OVERLAY = work/eqnew, a FULL copy of rebuild/include carrying the new
# CCountedPointer.h, so MSVC's includer-directory lookup cannot reach the repo copy), each with its own `timeout 900`, from the repo root;
# logs work/vland_<b>_<H|N>.log. Run the whole script in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
cd /home/user/FableDecomp || exit 1
for b in "$@"; do
  env FABLE_HDR_OVERLAY= LANDVERIFY_WORK=$A/work/lv_${b}_H PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/work/pay_${b}_H_payload.json $A/work/pay_${b}_H_oracle.tsv --recheck-landed > $A/work/vland_${b}_H.log 2>&1
  echo "EXIT $?" >> $A/work/vland_${b}_H.log
  env FABLE_HDR_OVERLAY=$A/work/treeN LANDVERIFY_WORK=$A/work/lv_${b}_N PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/work/pay_${b}_N_payload.json $A/work/pay_${b}_N_oracle.tsv --recheck-landed > $A/work/vland_${b}_N.log 2>&1
  echo "EXIT $?" >> $A/work/vland_${b}_N.log
done
echo CHAIN_DONE "$@" >> $A/work/vlandchain.log
