#!/usr/bin/env bash
# pipeline423.sh <prefix>: the NF423 whole-T1 caller-respell pipeline, end to end (run each stage in the background; it waits on its own jobs).
#   0. python3 tools/decomp_pipeline/naming_status.py --out $W/tracker          (fresh OPEN.tsv; run from the repo)
#   1. gen423.py work/<prefix>            -> work/<prefix>_PLAN.tsv, _rows/, _RESIDUE.tsv   (skip list: work/skip_rows.txt)
#   2. mkbatch423.py plan rows <prefix>   -> batches/<prefix>N (locks rows + targets)
#   3. reserved_check (RESERVED_REFS="") on work/<prefix>_addr.txt; screen423.py after finalize
#   4. per batch: prefam.sh (clang prechecks) -> runpair.sh (vland HEAD/new, one path) -> gates_bg.sh (gate423, pkgate423, linkgate423)
#      definers first: predef.sh work/<prefix>_targets.txt (precheck + defobj.py HEAD objects of the target self rows)
#   5. finalize423.py <b> <b> single <all batches>; split423.py <b>; finalize423.py again with the ACCEPT-only set
#   6. units423.py, payfilter423.py, xrcheck423.py, screen423.py, apply_table --dry per batch and all together
#   7. residue423.py work/<prefix>_RESIDUE.tsv RESIDUE.tsv <hold batches>; release423.sh <ACCEPT batches>
set -eu
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439; p=$1; cd $W
source $S/wine/env.sh; export S; unset TMPDIR TMP
python3 tools/gen423.py work/$p
python3 tools/mkbatch423.py work/${p}_PLAN.tsv work/${p}_rows $p
(cd /home/user/FableDecomp && RESERVED_REFS="" python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/reserved_check.py $W/work/${p}_addr.txt | head -2)
bash tools/predef.sh work/${p}_targets.txt > work/defobj_$p.out 2>&1
B=$(ls -d batches/${p}[0-9]* | xargs -n1 basename)
for b in $B; do bash tools/prefam.sh $b; BATCH=$b bash tools/runpair.sh $b $(cat batches/$b/rows.txt); bash tools/gates_bg.sh $b; done
for b in $B; do python3 tools/finalize423.py $b $b single $B; python3 tools/split423.py $b; done
for b in $B; do python3 tools/finalize423.py $b $b single $B; done
python3 tools/units423.py $B; python3 tools/payfilter423.py $B; python3 tools/xrcheck423.py $B; python3 tools/screen423.py $B
echo "next: apply_table --dry per batch, residue423.py, release423.sh"
