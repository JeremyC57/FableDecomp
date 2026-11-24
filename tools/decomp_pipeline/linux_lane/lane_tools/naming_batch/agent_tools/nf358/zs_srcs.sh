#!/usr/bin/env bash
# zs_srcs.sh: phase D, D1 / NF300 VC7.1 `cl /Zs` with the row's flags for every locked source whose clang precheck FAILED.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF358
cd $A/work
awk -F'\t' '$3 ~ /^OK/ && $4=="FAIL"{print $1}' lock_pre.tsv | while read a; do
  sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  fl=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(' '.join(nf.flags_of('$a', open('$sp').read())))")
  timeout 900 python3 $A/tools/nf.py zs ZS_$a $sp src $fl
done
echo ZS_SRCS_DONE >> $A/work/pre_zs.log
