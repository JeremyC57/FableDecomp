#!/bin/bash
src=$(readlink -f $1)
# c2p.sh <src.cpp> <tag> [extra c2prio args]: c2prio --trace --blocks --json -> tools/c2p/<tag>.{trace,json}
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=${A:-$S/w22/agent882}
t=$2; shift 2
export C2PRIO_RUNS=$A/tools/c2p/runs FABLE_HDR_OVERLAY=${HDR:-$A/hdr}
timeout 1800 python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py $src --func "${FUNC:-ConstructQuestListPC@CTCInventoryQuests}" --trace --blocks --json $A/tools/c2p/$t.json "$@" > $A/tools/c2p/$t.trace 2>&1
echo "rc $?"
