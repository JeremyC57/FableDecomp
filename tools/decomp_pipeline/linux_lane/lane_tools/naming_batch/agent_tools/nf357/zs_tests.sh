#!/usr/bin/env bash
# zs_tests.sh <pre.tsv>: D1 VC7.1 /Zs of every test whose clang precheck failed (column 2 FAIL); run BEFORE the row's lock.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
cd $A/work
for a in $(awk -F'\t' '$2=="FAIL"{print $1}' "$1"); do
  tp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  FABLE_HDR_OVERLAY=$A/work/tree timeout 900 python3 $A/tools/nf.py zs ZT_$a $tp test
done
echo ZS_TESTS_DONE "$1" >> $A/work/pre_zs.log
