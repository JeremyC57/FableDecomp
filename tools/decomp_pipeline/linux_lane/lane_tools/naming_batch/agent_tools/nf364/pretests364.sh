#!/usr/bin/env bash
# pretests364.sh <addr...>: clang precheck (nf.py pre, repo include tree, then NOWCHAR if the plain run fails) of each row's landed test.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
cd $A/work
for a in "$@"; do
  t=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  python3 $A/tools/nf.py pre T_$a "$t" | grep -q PASS || python3 $A/tools/nf.py pre T_$a "$t" nowchar
done
echo PRETESTS_DONE >> $A/work/pre.log
