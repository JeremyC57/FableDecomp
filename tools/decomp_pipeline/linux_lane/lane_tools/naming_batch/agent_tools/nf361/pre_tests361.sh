#!/usr/bin/env bash
# pre_tests361.sh <addr list>: clang precheck of each landed TEST (run BEFORE the lock); work/pre_tests.tsv: addr result
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361
for a in $(cat "$1"); do
  tp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  tr=$(python3 $A/tools/nf.py pre T_$a $tp nowchar | awk '{print $3}')
  echo -e "$a\t$tr"
done > $A/work/pre_tests.tsv
echo PRE_TESTS_DONE >> $A/work/pre_tests.tsv
