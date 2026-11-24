#!/usr/bin/env bash
# vlandchain364.sh <batch>: vland --recheck-landed of work/pay_<b>_H (HEAD texts, repo headers, no overlay) then work/pay_<b>_N (new texts,
# FABLE_HDR_OVERLAY = work/inc_new, a FULL copy of rebuild/include carrying the NF364 block), each with its own `timeout 900`, from the repo root;
# logs work/vland_<b>_<H|N>.log. Run the whole script in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
cd /home/user/FableDecomp || exit 1
b=$1
env FABLE_HDR_OVERLAY= LANDVERIFY_WORK=$A/work/lv_${b}_H PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/work/pay_${b}_H_payload.json $A/work/pay_${b}_H_oracle.tsv --recheck-landed > $A/work/vland_${b}_H.log 2>&1
echo "EXIT $?" >> $A/work/vland_${b}_H.log
env FABLE_HDR_OVERLAY=$A/work/inc_new LANDVERIFY_WORK=$A/work/lv_${b}_N PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/work/pay_${b}_N_payload.json $A/work/pay_${b}_N_oracle.tsv --recheck-landed > $A/work/vland_${b}_N.log 2>&1
echo "EXIT $?" >> $A/work/vland_${b}_N.log
echo CHAIN_DONE $b >> $A/work/vlandchain.log
