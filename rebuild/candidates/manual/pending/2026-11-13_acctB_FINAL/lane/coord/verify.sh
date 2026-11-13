#!/bin/bash
# usage: verify.sh <N>   -- coordinator re-verification of agent<N>'s hand-back (read-only; never lands)
export S=/opt/fable_lane; source $S/wine/env.sh; export S=/opt/fable_lane; cd /home/user/FableDecomp
N=$1; A=$S/w22/agent$N; O=$A/coordverify; mkdir -p $O
export FABLE_HDR_OVERLAY=$A/hdr
echo "== purity grep (src + test)"; grep -n -E '__asm|\bnaked\b|_emit|\bregister\b|\bvolatile\b|__forceinline|\bwchar_t\b' $A/src/*.cpp $A/test/*.cpp 2>/dev/null | grep -v 'FABLE_FORCEINLINE' | head -40
echo "== local padded structs"; grep -n -E 'char\s+_?pad[A-Za-z0-9_]*\[' $A/src/*.cpp 2>/dev/null | head -20
echo "== grades (q.py, RTM)"; bash $A/gall.sh 2>&1 | tee $O/grades.txt
echo "== tests g++"; bash $A/tall.sh 2>&1 | tee $O/tests_gxx.txt
echo "== tests VC7.1"; python3 $A/tall.py 2>&1 | tee $O/tests_vc.txt
echo "== finish + vland (read-only)"; python3 $A/finish.py
rm -rf $A/lv_coord; LANDVERIFY_WORK=$A/lv_coord timeout 3000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $A/payload.json $A/oracle.tsv > $O/vland.log 2>&1; tail -n 40 $O/vland.log; rm -rf $A/lv_coord
echo "== portable spellings"; python3 tools/decomp_pipeline/check_portable_spellings.py $A/src $A/hdr 2>&1 | tail -5
