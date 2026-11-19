#!/bin/bash
# qq.sh <addr> <src> [flags...] : grade only (no log), overlay from HO (default hdr)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; mkdir -p /run/user/0
export FABLE_HDR_OVERLAY=${HO:-$S/w24/hand4d/hdr}
cd /home/user/FableDecomp
a=$1; f=$2; shift 2
timeout 300 python3 $S/tools/q.py $a $f "$@" 2>&1 | grep -oE "RELOCATION_MATCH|[^_]MATCH|DIFFER\([0-9v]+\)|SRC_FAIL|OBJDUMP_ERR" | head -1
