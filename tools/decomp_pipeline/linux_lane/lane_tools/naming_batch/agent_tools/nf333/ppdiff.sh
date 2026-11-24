#!/bin/bash
# ppdiff.sh <addr...>: gcc -E -P (VC7.1 macros) of the HEAD source v rows/<addr>.cpp, whitespace-normalised diff (NF300 Q1 evidence)
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF333; R=/home/user/FableDecomp
for a in "$@"; do
  h=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  for k in h n; do f=$h; [ $k = n ] && f=$A/rows/$a.cpp
    timeout 60 g++ -E -P -x c++ -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -I$R/rebuild/include -isystem $S/wine/vc71/include "$f" 2>/dev/null | grep -v '^\s*$' | sed 's/[[:space:]]\+/ /g' > $A/work/pp_${k}_$a.txt; done
  echo "## $a"; diff $A/work/pp_h_$a.txt $A/work/pp_n_$a.txt
done
