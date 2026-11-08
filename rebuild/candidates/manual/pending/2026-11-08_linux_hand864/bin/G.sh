#!/bin/bash
# G.sh <addr> <src.cpp> [tag] [-q] [--note TEXT] [--extra FLAGS]: grade with the agent864 overlay (gr.py)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
export FABLE_HDR_OVERLAY=$S/w22/agent864/hdr
timeout 240 python3 $S/w22/agent864/bin/gr.py "$@"
