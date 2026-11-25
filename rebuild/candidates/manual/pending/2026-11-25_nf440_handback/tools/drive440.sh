#!/usr/bin/env bash
# drive440.sh <batch>: generate + gate one batch under batches/<batch> (rows from work/sel_<batch>.txt)
set -u
source $S/wine/env.sh; export S; unset TMPDIR TMP; export LANDVERIFY_STRICT_LEAF=1
W=$S/w22/agentNF440; b=$1; B=$W/batches/$b; cd $W
A=$(cat work/sel_$b.txt)
rm -rf $B; mkdir -p $B/{rows,tests,orig/rows,orig/tests,head,evidence,obj}
python3 tools/gen440.py ../batches/$b $A > $B/evidence/gen.txt 2>&1
python3 - "$B" $A <<'PY'
import sys, shutil, os
sys.path.insert(0, 'tools'); import nf
B = sys.argv[1]
for a in sys.argv[2:]:
    shutil.copy(nf.src_of(a), B + '/orig/rows/%s.cpp' % a); t = nf.test_of(a)
    if t and t.exists(): shutil.copy(t, B + '/orig/tests/%s_test.cpp' % a)
PY
timeout 3000 python3 tools/hn426.py $B $A > $B/evidence/hn.txt 2>&1
python3 tools/objsec440.py $B runs/pilot/obj/00403790.obj > /dev/null 2>&1
timeout 3000 bash tools/ngate428.sh ../batches/$b > /dev/null 2>&1
timeout 3000 bash tools/pk428.sh ../batches/$b > /dev/null 2>&1
timeout 3000 python3 tools/linkprep440.py $B > $B/evidence/linkprep.txt 2>&1
timeout 1800 python3 tools/link428.py $b 'CCountedPointer' work/link_$b.args > /dev/null 2>&1; cp evidence/link_$b.txt $B/evidence/link.txt
BATCH=../batches/$b timeout 5000 bash tools/runpair428.sh $b $A > work/runpair_$b.out 2>&1
cp work/vland_${b}h.log $B/evidence/vland_head.log; cp work/vland_${b}n.log $B/evidence/vland_new.log; cp work/p_${b}h/payload.json $B/head/payload.json
echo DRIVE_DONE $b
