#!/bin/bash
# pkcli.sh <addr>: the pk_all CLI (--leaf from the row's own `// Leaf:` line, --row-length from the catalog) on work/obj/H_<addr>.obj and
# N_<addr>.obj -> evidence/pkcli_{H,N}_<addr>.txt, then a diff. FABLE_EXE / FABLE_ROOT exported first (agent NF354). Reads objects only.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF354
source $S/wine/env.sh >/dev/null 2>&1; export S FABLE_EXE FABLE_ROOT=/home/user/FableDecomp; unset TMPDIR TMP
a=$1; N=${2:-N}
src=$A/rows/$a.cpp; [ -f $src ] || src=$(python3 -c "import sys;sys.path.insert(0,'$A/tools');import nf;print(nf.src_of('$a'))")
leaf=$(grep -m1 '^// Leaf:' $src | awk '{print $3}')
len=$(python3 -c "import sys;sys.path.insert(0,'$A/tools');import nf;print(nf.catalog()['$a']['length'])")
for k in H $N; do
  python3 -I -B /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $A/work/obj/${k}_$a.obj $a --leaf="$leaf" --row-length=$len > $A/evidence/pkcli_${k}_$a.txt 2>&1
done
echo "# pk_all CLI --leaf=$leaf --row-length=$len : H v $N" > $A/evidence/pkcli_diff_H_v_${N}_$a.txt
diff $A/evidence/pkcli_H_$a.txt $A/evidence/pkcli_${N}_$a.txt >> $A/evidence/pkcli_diff_H_v_${N}_$a.txt && echo "IDENTICAL" >> $A/evidence/pkcli_diff_H_v_${N}_$a.txt
cat $A/evidence/pkcli_diff_H_v_${N}_$a.txt
