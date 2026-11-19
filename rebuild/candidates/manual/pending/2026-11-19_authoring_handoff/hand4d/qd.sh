#!/bin/bash
# qd.sh <addr> <src> [leaf] : grade + compact diff summary (no log), overlay from HO
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export W=$S/w24/hand4d FABLE_HDR_OVERLAY=${HO:-$S/w24/hand4d/hdr}
cd /home/user/FableDecomp
timeout 400 python3 $S/tools/q.py "$@" > $W/tmp/qd_$1.txt 2>&1
python3 $W/sd.py $W/tmp/qd_$1.txt > $W/tmp/qdd_$1.txt
echo "$(grep -oE 'RELOCATION_MATCH|[^_]MATCH|DIFFER\([0-9v]+\)|SRC_FAIL|OBJDUMP_ERR' $W/tmp/qdd_$1.txt | head -1) $(tail -1 $W/tmp/qdd_$1.txt) :: $(grep -v '^-----' $W/tmp/qdd_$1.txt | sed -n 2,3p | cut -c1-60,64-110 | tr '\n' ' ')"
