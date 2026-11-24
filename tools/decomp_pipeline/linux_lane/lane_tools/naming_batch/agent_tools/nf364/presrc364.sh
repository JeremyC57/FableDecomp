#!/usr/bin/env bash
# presrc364.sh <addr...>: clang precheck of each row's HEAD source (NF_TREE = pristine copy work/inc_old) and rows/<addr>.cpp (NF_TREE = work/inc_new);
# a plain run first, NOWCHAR when it fails. Log work/pre.log (labels HS_<a> / NS_<a>).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
cd $A/work
for a in "$@"; do
  s=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  NF_TREE=$A/work/inc_old python3 $A/tools/nf.py pre HS_$a "$s" | grep -q PASS || NF_TREE=$A/work/inc_old python3 $A/tools/nf.py pre HS_$a "$s" nowchar
  NF_TREE=$A/work/inc_new python3 $A/tools/nf.py pre NS_$a "$A/rows/$a.cpp" | grep -q PASS || NF_TREE=$A/work/inc_new python3 $A/tools/nf.py pre NS_$a "$A/rows/$a.cpp" nowchar
done
echo PRESRC_DONE "$@" >> $A/work/pre.log
