#!/usr/bin/env bash
# compile_h.sh <listfile>: phase E, one-path HEAD compile + grade of each row (pass.py H; refuses rows not locked by NF361), then both
# checkers (pkrun352.py: pk_all --leaf --row-length --tsv, pickany_vs_retail --leaf) into work/pkc/, then clean.py of work/one/<addr>.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361
CL=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py
export FABLE_ROOT=/home/user/FableDecomp
cd $A/work
while read a; do
  sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  timeout 900 python3 $A/tools/pass.py H_$(basename $1) H "$a=$sp"
  [ -f obj/H_$a.obj ] && timeout 900 python3 $A/tools/pkrun352.py $A/work/pkc $a H=$A/work/obj/H_$a.obj >> pkrun.log 2>&1
  python3 $CL $A $A/work/one/$a > /dev/null
done < "$1"
echo "COMPILE_DONE $1" >> $A/work/compile_done.log
