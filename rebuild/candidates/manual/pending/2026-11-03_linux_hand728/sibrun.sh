#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp; D=$S/w22/chunk728; T=tools/decomp_pipeline/linux_lane/lane_tools
b=$1
FABLE_HDR_OVERLAY=$D/hdr LANDVERIFY_WORK=$D/lv timeout 115 python3 -u $T/vland.py $D/sib/$b.json $D/sib/$b.tsv --recheck-landed > $D/sib/$b.out 2>&1
echo "exit $?"; grep -E "^\s*(0x)?[0-9a-fA-F]{8}|MATCH|DIFFER|FAIL|PASS" $D/sib/$b.out | tail -12
