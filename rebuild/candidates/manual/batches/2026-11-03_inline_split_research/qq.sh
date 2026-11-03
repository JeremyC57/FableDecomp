#!/bin/bash
# qq.sh <addr> <src> [qq.py args]  (OVL=<overlay dir> optional); env sourced; 200 s timeout
export S=$S
source $S/wine/env.sh >/dev/null 2>&1; export S
cd /home/user/FableDecomp
FABLE_HDR_OVERLAY=${OVL:-} timeout 200 python3 $S/inlinesplit/qq.py "$@"
