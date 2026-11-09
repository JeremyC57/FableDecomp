#!/bin/bash
# gr.sh <addr> <src> [tag] [...] -- one grade, timeout 240, header overlay = agent892/hdr
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
export FABLE_HDR_OVERLAY=${FABLE_HDR_OVERLAY:-$S/w22/agent892/hdr}
timeout 240 python3 $S/w22/agent892/tools/gq.py "$@" || echo "TIMEOUT/ERR $1 $2"
