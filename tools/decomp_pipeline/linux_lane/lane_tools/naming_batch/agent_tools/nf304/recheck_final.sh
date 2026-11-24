#!/bin/bash
# recheck_final.sh: one vland --recheck-landed run over work/final_payload.json (nf.py pay) -> recheck_final.log
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF304; W=$A/work; cd /home/user/FableDecomp
PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools LANDVERIFY_WORK=$W/wd_final timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/final_payload.json $W/final_oracle.tsv --recheck-landed > $A/recheck_final.log 2>&1
echo done > $W/rf.done
