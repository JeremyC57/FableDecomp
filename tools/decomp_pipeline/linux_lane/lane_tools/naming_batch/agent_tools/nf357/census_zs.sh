#!/usr/bin/env bash
# census_zs.sh: D1 VC7.1 /Zs for the census tests that failed clang (toolchain / _assert only) and the sources that failed on MSVC-only syntax.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
cd $A/work
for a in $(awk -F'\t' '$2=="FAIL"{print $1}' census_pre.tsv); do
  tp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  FABLE_HDR_OVERLAY=$A/work/tree timeout 900 python3 $A/tools/nf.py zs ZT_$a $tp test
done
for a in $(awk -F'\t' '$4=="FAIL"{print $1}' census_pre.tsv); do
  sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  fl=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(' '.join(nf.flags_of('$a', open('$sp').read())))")
  FABLE_HDR_OVERLAY=$A/work/tree timeout 900 python3 $A/tools/nf.py zs ZS_$a $sp src $fl
done
echo CENSUS_ZS_DONE >> $A/work/pre_zs.log
