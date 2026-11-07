#!/bin/bash
# B.sh <addr> <base.cpp> <spec.py>: run batch.py with the lane env (2 grades at a time)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
export FABLE_HDR_OVERLAY=$S/w22/agent859/hdr
python3 $S/w22/agent859/bin/batch.py "$@"
