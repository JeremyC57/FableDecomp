#!/bin/bash
# c2.sh <src.cpp> <tag> [c2prio options]: allocator trace -> out/c2/<tag>.trace
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent892
export FABLE_HDR_OVERLAY=${FABLE_HDR_OVERLAY:-$A/hdr} C2PRIO_RUNS=$A/wk/c2runs
src=$1; t=$2; shift 2
timeout 600 python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py $src "$@" > $A/out/c2/$t.trace 2>&1
tail -2 $A/out/c2/$t.trace
