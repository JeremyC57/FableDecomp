#!/bin/bash
# recheck.sh <tag> [overlay dir]: one vland --recheck-landed run over work/<tag>_payload.json + work/<tag>_oracle.tsv (nf.py pay work/<tag> ...)
# -> work/recheck_<tag>.log; done file work/rc_<tag>.done. With an overlay dir, FABLE_HDR_OVERLAY points at it.
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF316; W=$A/work; cd $W   # NF316: cwd = my work dir, never the repo (a verify_and_land import can create a D:\ path dir relative to cwd)
[ -n "${2:-}" ] && export FABLE_HDR_OVERLAY=$2
PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools LANDVERIFY_WORK=$W/wd_$1 timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/$1_payload.json $W/$1_oracle.tsv --recheck-landed > $W/recheck_$1.log 2>&1
echo done > $W/rc_$1.done
