#!/bin/bash
# c2p.sh <src.cpp> <tag> [extra c2prio args]: c2prio --trace --blocks (json kept) -> t8/c2p/<tag>.{trace,json}
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886
src=$(readlink -f $1); t=$2; shift 2
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
export C2PRIO_RUNS=$A/t8/runs FABLE_HDR_OVERLAY=$A/hdr
timeout 1800 python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py $src --func 'Action@CManager@NUISystem' --flags "/GS" --trace --blocks --json $A/t8/c2p/$t.json "$@" > $A/t8/c2p/$t.trace 2>&1
echo "rc $?"
