#!/bin/bash
# recheck_final.sh: one vland --recheck-landed run over work/g4_payload.json (nf.py pay) -> work/recheck_g4.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF338; W=$A/work; cd /home/user/FableDecomp
PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools LANDVERIFY_WORK=$W/wd_g4 timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/g4_payload.json $W/g4_oracle.tsv --recheck-landed > $A/work/recheck_g4.log 2>&1
echo done > $W/rf4.done
