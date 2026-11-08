#!/bin/bash
# c2.sh <src.cpp> <tag> [c2prio options]: allocator trace -> work/c2/<tag>.trace
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
export FABLE_HDR_OVERLAY=$S/w22/agent890/hdr C2PRIO_RUNS=$S/w22/agent890/work/c2runs
src=$1; t=$2; shift 2
timeout 600 python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py $src "$@" > $S/w22/agent890/work/c2/$t.trace 2>&1
tail -1 $S/w22/agent890/work/c2/$t.trace
