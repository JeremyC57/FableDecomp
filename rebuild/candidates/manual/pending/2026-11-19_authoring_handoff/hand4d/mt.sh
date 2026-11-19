#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export W=$S/w24/hand4d FABLE_HDR_OVERLAY=$S/w24/hand4d/hdr; cd /home/user/FableDecomp
timeout 600 python3 $W/mt.py "$@"
