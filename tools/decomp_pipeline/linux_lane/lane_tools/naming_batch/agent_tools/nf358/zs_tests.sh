#!/usr/bin/env bash
# zs_tests.sh: phase B, D1 VC7.1 `cl /Zs` (/Od /W3) of each landed test whose clang precheck FAILED (no lock; syntax-only). Log work/pre_zs.log.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF358
cd $A/work
awk -F'\t' '$2=="FAIL"{print $1"\t"$3}' pre_tests.tsv | while IFS=$'\t' read a tp; do
  timeout 900 python3 $A/tools/nf.py zs ZT_$a $tp test
done
echo ZS_TESTS_DONE >> $A/work/pre_zs.log
