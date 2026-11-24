#!/usr/bin/env bash
# part2_364.sh: Part 2 probe on 0x00603000 (diagnosis only). Both compiles against the probe tree work/ptree (a full copy of rebuild/include
# with the NF364_PROBE_RESET_SIZE_INLINE block appended; inert unless the row defines it): HEAD text -> work/obj/H_00603000.obj, probe text
# rows/probe/00603000_p2.cpp -> work/obj/00603000_p2.obj; then both checkers into evidence/part2/.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp FABLE_HDR_OVERLAY=$A/work/ptree
cd $A/work
sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('00603000'))")
timeout 900 python3 $A/tools/pass.py part2 H "00603000=$sp"
timeout 900 python3 $A/tools/pass.py part2 N "00603000@p2=$A/rows/probe/00603000_p2.cpp"
timeout 900 python3 $A/tools/pkrun352.py $A/evidence/part2 00603000 H=$A/work/obj/H_00603000.obj P2=$A/work/obj/00603000_p2.obj
echo "PART2_DONE" >> $A/work/run_done.log
