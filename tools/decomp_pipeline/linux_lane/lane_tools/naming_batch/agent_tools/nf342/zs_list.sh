#!/bin/bash
# zs_list.sh <label list> <donefile>: D1 VC7.1 `cl /Zs` prechecks (nf.py zs) of H_<a> (HEAD source, row flags) / T_<a> (HEAD test, /Od /W3)
# plus a clang NOWCHAR rerun of each -> work/pre_zs.log, work/pre.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export PYTHONDONTWRITEBYTECODE=1
A=$S/w22/agentNF342; cd $A/work
for lab in $(cat $1); do
  a=${lab:2}
  if [ "${lab:0:1}" = H ]; then
    f=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import common as C; print(C.src_path('$a'))")
    fl=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(' '.join(nf.flags_of('$a', open('$f').read())))")
    timeout 120 python3 $A/tools/nf.py pre $lab "$f" nowchar > /dev/null
    timeout 300 python3 $A/tools/nf.py zs $lab "$f" src $fl > /dev/null
  else
    f=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import common as C; print(C.test_path('$a'))")
    timeout 120 python3 $A/tools/nf.py pre $lab "$f" nowchar > /dev/null
    timeout 300 python3 $A/tools/nf.py zs $lab "$f" test > /dev/null
  fi
done
echo done > $2
