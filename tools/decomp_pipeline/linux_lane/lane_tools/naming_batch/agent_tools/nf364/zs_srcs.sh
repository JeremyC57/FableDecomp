#!/usr/bin/env bash
# zs_srcs.sh <addr>: D1 / NF305 Q2 VC7.1 `cl /Zs` of the row's HEAD source and rows/<addr>.cpp with the row's catalog flags (work/pre_zs.log)
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
a=$1
sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
fl=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(' '.join(nf.flags_of('$a', open('$sp').read())))")
timeout 900 python3 $A/tools/nf.py zs H_$a "$sp" src $fl
timeout 900 python3 $A/tools/nf.py zs N_$a "$A/rows/$a.cpp" src $fl
echo ZS_SRCS_DONE $a >> $A/work/pre_zs.log
