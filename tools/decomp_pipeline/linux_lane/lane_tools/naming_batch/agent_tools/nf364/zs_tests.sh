#!/usr/bin/env bash
# zs_tests.sh <addr...>: D1 VC7.1 `cl /Zs` precheck of each row's landed test (nf.py zs; tests /Od /W3), sequential; log work/pre_zs.log
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
for a in "$@"; do
  t=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  timeout 900 python3 $A/tools/nf.py zs T_$a "$t" test
done
echo ZS_TESTS_DONE "$@" >> $A/work/pre_zs.log
