#!/bin/bash
# pretests.sh <label-prefix> <list> <src|test>: clang -fsyntax-only precheck (nf.py pre) of each row's landed source or test (agent NF353).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
cd $A
for a in $(cat "$2"); do
  if [ "$3" = test ]; then f=$(python3 -c "import sys; sys.path.insert(0,'tools'); import nf; print(nf.test_of('$a'))");
  elif [ "$3" = new ]; then f=$A/rows/$a.cpp;
  else f=$(python3 -c "import sys; sys.path.insert(0,'tools'); import nf; print(nf.src_of('$a'))"); fi
  python3 tools/nf.py pre "$1_$a" "$f" >/dev/null
done
echo "PRE_DONE $1" >> work/pre.log
