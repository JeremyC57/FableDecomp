#!/bin/bash
# recheck_final.sh: one vland --recheck-landed run over work/final_payload.json (nf.py pay) -> recheck_final.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF333; W=$A/work; cd /home/user/FableDecomp
PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools LANDVERIFY_WORK=$W/wd_final timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/final_payload.json $W/final_oracle.tsv --recheck-landed > $A/recheck_final.log 2>&1
echo done > $W/rf.done
