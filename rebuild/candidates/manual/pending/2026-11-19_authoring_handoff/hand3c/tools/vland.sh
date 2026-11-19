#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; mkdir -p /run/user/0; export W=$S/w24/hand2c FABLE_HDR_OVERLAY=$S/w24/hand2c/hdr; cd /home/user/FableDecomp
python3 $W/tools/mkpayload.py "$@"
rm -rf $W/lv
LANDVERIFY_WORK=$W/lv LANDVERIFY_STRICT_HEADERS=1 timeout 3000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/payload.json $W/oracle.tsv > $W/tmp/vland.log 2>&1
rm -rf $W/lv
grep -v "^\s*$" $W/tmp/vland.log | tail -30
