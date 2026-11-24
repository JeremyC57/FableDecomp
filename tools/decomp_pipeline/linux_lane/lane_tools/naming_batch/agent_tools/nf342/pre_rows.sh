#!/bin/bash
# pre_rows.sh <addr list> <donefile>: clang precheck (nf.py pre) of each row's landed HEAD source (H_<a>) and landed test (T_<a>) -> work/pre.log
# (NF339's pre_rows.sh retargeted to agentNF342; one row per call, so several lists run in parallel)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export PYTHONDONTWRITEBYTECODE=1
A=$S/w22/agentNF342
for a in $(cat $1); do
  s=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import common as C; print(C.src_path('$a') or '')")
  t=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import common as C; print(C.test_path('$a') or '')")
  [ -n "$s" ] && timeout 120 python3 $A/tools/nf.py pre H_$a "$s" > /dev/null
  [ -n "$t" ] && timeout 120 python3 $A/tools/nf.py pre T_$a "$t" > /dev/null
done
echo done > $2
