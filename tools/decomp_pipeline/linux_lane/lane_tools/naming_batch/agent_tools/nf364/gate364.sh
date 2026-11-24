#!/usr/bin/env bash
# gate364.sh <tag> <owner:yes|no> <addr...>: one-path VC7.1 compiles against ONE include tree path (work/tree, a full copy of rebuild/include):
#   phase H: tree header = pristine (work/inc_old) -> [owner 00429c6f HEAD] + each row's HEAD text  (pass.py H -> work/obj/H_<a>.obj)
#   phase N: tree header = NF364 (work/inc_new)    -> each rows/<a>.cpp                              (pass.py N -> work/obj/<a>.obj)
# then both checkers (pkrun352.py) on H and N into evidence/pk_gate/ (H_<a> / N_<a>). The header hash is logged per phase. Sequential;
# launch in the background. Every compile has `timeout 900` on its own line.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp FABLE_HDR_OVERLAY=$A/work/tree
tag=$1; own=$2; shift 2
cd $A/work
H=engine/CCountedPointer.h
cp $A/work/inc_old/$H $A/work/tree/$H; echo "PHASE H $tag $(sha256sum $A/work/tree/$H)" >> $A/work/gate_phase.log
if [ "$own" = yes ]; then
  timeout 900 python3 $A/tools/pass.py $tag H "00429c6f=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('00429c6f'))")"
fi
for a in "$@"; do
  sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  timeout 900 python3 $A/tools/pass.py $tag H "$a=$sp"
done
cp $A/work/inc_new/$H $A/work/tree/$H; echo "PHASE N $tag $(sha256sum $A/work/tree/$H)" >> $A/work/gate_phase.log
for a in "$@"; do
  timeout 900 python3 $A/tools/pass.py $tag N "$a=$A/rows/$a.cpp"
done
for a in "$@"; do
  timeout 900 python3 $A/tools/pkrun352.py $A/evidence/pk_gate $a H=$A/work/obj/H_$a.obj N=$A/work/obj/$a.obj
done
echo "GATE_DONE $tag $*" >> $A/work/run_done.log
