#!/usr/bin/env bash
# presrc.sh <addr file>: clang precheck of HEAD (repo include only), R (repo include), W (hdrW + repo), N (hdr + repo) per row -> work/pre.log
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF349
R=/home/user/FableDecomp/rebuild/include
while read -r a; do
  [ -n "$a" ] || continue
  src=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  NF_TREE=$R python3 $A/tools/nf.py pre head_$a "$src"
  NF_TREE=$R python3 $A/tools/nf.py pre R_$a "$A/work/var/${a}_R.cpp"
  NF_TREE=$A/work/hdrW:$R python3 $A/tools/nf.py pre W_$a "$A/work/var/${a}_W.cpp"
  NF_TREE=$A/hdr:$R python3 $A/tools/nf.py pre new_$a "$A/rows/$a.cpp"
done < "$1"
echo PRESRC_DONE >> $A/work/pre.log
