#!/bin/bash
# read-only regrade of one pass-13 row in its own restored header context
a=$1; source $S/wine/env.sh; export S; C=$S/codex13
cd /home/user/FableDecomp
rm -rf $C/work/$a
FABLE_HDR_OVERLAY=$C/hdr/$a LANDVERIFY_WORK=$C/work/$a timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $C/payloads/$a.json $C/oracle.tsv > $C/logs/$a.log 2>&1
echo "$a rc=$? $(grep -E "^$a +[A-Z_]+ +(PASS|FAIL|-)" $C/logs/$a.log | tail -1)"
