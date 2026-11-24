#!/usr/bin/env bash
# gate362.sh <addr...>: per row (sequential), one-path compiles of the HEAD text (pass.py H -> work/obj/H_<a>.obj) and rows/<a>.cpp
# (pass.py N -> work/obj/<a>.obj), then both checkers labelled H / N into evidence/pk_gate/ (H_<a> / N_<a> .pkall.tsv / .pvr.txt, the
# names reachflip.py and pkcmp346.py pair). Log work/pass_gate.log. Launch in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF362
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp
cd $A/work
for a in "$@"; do
  sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  timeout 900 python3 $A/tools/pass.py gate H "$a=$sp"
  timeout 900 python3 $A/tools/pass.py gate N "$a=$A/rows/$a.cpp"
  timeout 900 python3 $A/tools/pkrun352.py $A/evidence/pk_gate $a H=$A/work/obj/H_$a.obj N=$A/work/obj/$a.obj
  echo "GATE_DONE $a" >> $A/work/run_done.log
done
