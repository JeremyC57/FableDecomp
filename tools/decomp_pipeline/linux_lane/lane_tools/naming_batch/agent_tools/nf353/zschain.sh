#!/bin/bash
# zschain.sh <tag-prefix> <list> <src|test|new>: D1 VC7.1 cl /Zs (nf.py zs; row flags for sources, /Od /W3 for tests) per row, sequential,
# each under its own timeout 900 (agent NF353). Run in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp; unset FABLE_HDR_OVERLAY
cd $A
for a in $(cat "$2"); do
  if [ "$3" = test ]; then f=$(python3 -c "import sys; sys.path.insert(0,'tools'); import nf; print(nf.test_of('$a'))"); fl="";
  else
    if [ "$3" = new ]; then f=$A/rows/$a.cpp; else f=$(python3 -c "import sys; sys.path.insert(0,'tools'); import nf; print(nf.src_of('$a'))"); fi
    fl=$(python3 -c "import sys; sys.path.insert(0,'tools'); import nf; print(' '.join(nf.flags_of('$a', open('$f').read())))"); fi
  k=$3; [ "$k" = new ] && k=src
  timeout 900 python3 tools/nf.py zs "$1_$a" "$f" $k $fl >/dev/null 2>&1
done
echo "ZS_DONE $1" >> work/pre_zs.log
