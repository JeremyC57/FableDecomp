#!/bin/bash
# regrade one row read-only in its own materialized header context (second run, after the 04:03 bindings change)
a=$1; C=$S/codex12
cd /home/user/FableDecomp
rm -rf $C/work2/$a
FABLE_HDR_OVERLAY=$C/hdr/$a LANDVERIFY_WORK=$C/work2/$a timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $C/payloads/$a.json $C/oracle.tsv > $C/logs2/$a.log 2>&1
echo "$a rc=$? $(grep -E "^$a +[A-Z_]+ +(PASS|FAIL|-)" $C/logs2/$a.log | tail -1)"
