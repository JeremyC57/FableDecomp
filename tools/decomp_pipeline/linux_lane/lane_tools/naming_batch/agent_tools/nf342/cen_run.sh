#!/bin/bash
# cen_run.sh <tag>: vland --recheck-landed over work/cen_<tag>/payload.json (cwd = work dir), objects kept in work/cen_<tag>/wd (NF339's, retargeted)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export PYTHONDONTWRITEBYTECODE=1
A=$S/w22/agentNF342; W=$A/work; D=$W/cen_$1; cd $W
PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools LANDVERIFY_WORK=$D/wd timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $D/payload.json $D/oracle.tsv --recheck-landed > $D/vland.log 2>&1
echo done > $D/done
