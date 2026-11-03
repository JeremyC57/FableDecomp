#!/bin/bash
# regrade one row read-only against the slim overlay (only the row's changed + new headers over our current tree)
a=$1; C=$S/codex12
cd /home/user/FableDecomp
rm -rf $C/work3/$a
FABLE_HDR_OVERLAY=$C/slim/$a LANDVERIFY_WORK=$C/work3/$a timeout 600 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $C/payloads/$a.json $C/oracle.tsv > $C/logs3/$a.log 2>&1
echo "$a rc=$? $(grep -E "^$a +[A-Z_]+ +(PASS|FAIL|-)" $C/logs3/$a.log | tail -1)"
