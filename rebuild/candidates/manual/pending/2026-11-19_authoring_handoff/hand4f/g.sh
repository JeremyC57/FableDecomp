#!/bin/bash
# g.sh <addr> <src> [leaf] [--flags ...]  : grade + compact diff; logs to ATTEMPTS.log (IDEA env = note)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; mkdir -p /run/user/0
export W=$S/w24/hand4f FABLE_HDR_OVERLAY=${HO:-$S/w24/hand4f/hdr}
cd /home/user/FableDecomp
timeout 400 python3 $S/tools/q.py "$@" > $W/tmp/g_$1.txt 2>&1
python3 $W/sd.py $W/tmp/g_$1.txt > $W/tmp/d_$1.txt
cat $W/tmp/d_$1.txt
gr=$(grep -oE "RELOCATION_MATCH|[^_]MATCH|DIFFER\([0-9v]+\)|SRC_FAIL|OBJDUMP_ERR" $W/tmp/d_$1.txt | head -1); nd=$(tail -1 $W/tmp/d_$1.txt)
echo -e "$(date +%H:%M)\t$1\t$(basename $2)\t$gr\t$nd\t${IDEA:-}" >> $W/ATTEMPTS.log
