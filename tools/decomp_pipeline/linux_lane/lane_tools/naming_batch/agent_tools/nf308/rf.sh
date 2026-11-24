#!/bin/bash
# rf.sh <payload-dir> <log>: one vland --recheck-landed run over the payload (overlay hdr/), LANDVERIFY_WORK inside work/
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF308; cd /home/user/FableDecomp
LANDVERIFY_WORK=$A/work/lvw FABLE_HDR_OVERLAY=$A/hdr timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $1/payload.json $1/oracle.tsv --recheck-landed > $2 2>&1
echo RF_DONE >> $2
