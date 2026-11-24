#!/bin/bash
# runone.sh <tag> <payload dir>: one vland --recheck-landed of <dir>/payload.json under work/wd_<tag> -> work/recheck_<tag>.log, work/r1_<tag>.done
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF323; W=$A/work; cd $W
P=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools
PYTHONPATH=$P LANDVERIFY_WORK=$W/wd_$1 timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $2/payload.json $2/oracle.tsv --recheck-landed > $W/recheck_$1.log 2>&1
echo done > $W/r1_$1.done
