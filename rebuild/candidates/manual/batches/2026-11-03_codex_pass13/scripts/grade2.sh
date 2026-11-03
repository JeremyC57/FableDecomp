#!/bin/bash
# second regrade: the row's staged overlay only (verified_pending), over the current rebuild/include
a=$1; source $S/wine/env.sh; export S; C=$S/codex13
cd /home/user/FableDecomp
V=rebuild/candidates/manual/pending/2026-11-02_codex_pass13/verified_pending
rm -rf $C/ov/$a $C/work2/$a; mkdir -p $C/ov/$a $C/logs2
python3 $V/restore_overlay.py $a $C/ov/$a
FABLE_HDR_OVERLAY=$C/ov/$a LANDVERIFY_WORK=$C/work2/$a timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $C/payloads/$a.json $C/oracle.tsv > $C/logs2/$a.log 2>&1
echo "$a rc=$? $(grep -E "^$a +[A-Z_]+ +(PASS|FAIL|-)" $C/logs2/$a.log | tail -1)"
