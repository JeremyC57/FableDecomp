#!/bin/bash
# chain32.sh: the wave 32 hand-back chain from a composed ov_final -- the recheck (RK=1: family rows + every landed includer of
# engine/CAIStateGroupBase.h + the seven 0x00CA6370 funclets, three parallel parts; runs/rk_final.*), index of ov_final, idx/after.jsonl
# (HEAD index with the rows replaced), qscan, QUEUE_W32.tsv, DEFERRED.tsv, BINDINGS.tsv + runs/reloc_bindings.after.tsv, the
# multi-binding check, RENAMES.tsv, LABELS.tsv.
S=${S:?set S to the scratch lane root}; W=$S/naming/w32; : "${W:?}"; source $S/wine/env.sh; cd $W || exit 1
cat runs/{aisg,aisd,dshow}.rows.txt | sort -u > runs/final.rows.txt
python3 scripts/includers.py engine/CAIStateGroupBase.h runs/inc_aisg.txt 2>/dev/null; cp runs/inc_aisg.txt runs/hdrdep.rows.txt
printf '%s\n' 012169f0 012169f8 01216a03 01216a0e 01216a19 01216a24 01216a32 > runs/dshow_funclets.rows.txt
cat runs/final.rows.txt runs/hdrdep.rows.txt runs/dshow_funclets.rows.txt | sort -u > runs/final_plus.rows.txt
[ -z "$RK" ] || bash scripts/recheck.sh final ov_final $W/runs/final_plus.rows.txt 3 > logs/recheck_final_driver.log 2>&1
cp runs/rk_final.log logs/recheck_final.log
rm -f idx/ovfinal.jsonl; BS=40 NJ=3 python3 scripts/mkindex.py $W/idx/ovfinal.jsonl $W/ov_final $W/runs/final.rows.txt | tail -1; rm -rf $W/idx/ovfinal.jsonl.objs
python3 - <<'PY'
import json, os
W = os.environ['S'] + '/naming/w32'
new = {json.loads(l)['a']: l for l in open(W + '/idx/ovfinal.jsonl')}
assert not any('"err": ' in l for l in new.values())
with open(W + '/idx/after.jsonl', 'w') as o:
    for l in open(W + '/idx/full.jsonl'):
        a = json.loads(l)['a']; o.write(new.pop(a) if a in new else l)
print('after.jsonl; unplaced', len(new))
PY
python3 scripts/qscan32.py $W/idx/full.jsonl $W/runs/qscan_base.tsv > logs/qscan_base.log 2>&1
python3 scripts/qscan32.py $W/idx/after.jsonl $W/runs/qscan_final.tsv > logs/qscan_final.log 2>&1
python3 scripts/mkq32.py
python3 scripts/mkdef32.py $W/idx/after.jsonl | cut -c1-300
python3 scripts/mkbind32.py $W/runs/rk_final.pairs.jsonl $W/idx/after.jsonl $W/runs/final.rows.txt > logs/mkbind32.log 2>&1; grep -v ^NEWDEF logs/mkbind32.log
grep -c "^CONFLICT" BINDINGS.tsv
python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py runs/reloc_bindings.after.tsv | tee logs/bindings_multi_check.log
python3 scripts/mkren32.py $W/idx/after.jsonl | head -1; python3 scripts/fixren32.py
python3 scripts/mklabels32.py; python3 scripts/fixlab32.py
