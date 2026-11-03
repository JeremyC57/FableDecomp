#!/bin/bash
# p.sh <agentdir> <addr> <src> <leaf> <out>  -- ungraded probe
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S; cd /home/user/FableDecomp
FABLE_HDR_OVERLAY=$1/hdr timeout 200 python3 $S/tools/q.py $2 $3 "$4" > $5 2>&1; head -1 $5; grep "^--- " $5
