#!/bin/bash
# P.sh <src.cpp> <tag> [c2prio options...]: run the c2 allocator probe with the agent864 overlay; output -> probe/<base>.<tag>.out
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
export FABLE_HDR_OVERLAY=$S/w22/agent864/hdr
export C2PRIO_RUNS=$S/w22/agent864/tmp/c2runs
src=$1; tag=$2; shift 2
b=$(basename $src .cpp)
timeout 300 python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py $src "$@" > $S/w22/agent864/probe/$b.$tag.out 2>&1
echo "probe $b.$tag rc=$? lines=$(wc -l < $S/w22/agent864/probe/$b.$tag.out)"
# run dirs are deleted by the tool itself (no --keep)
