#!/bin/bash
# chain31.sh: the wave 31 hand-back chain from a composed ov_final -- the recheck (scripts/rkfinal31.sh: family rows + every landed includer
# of fable_string.h / engine/CTCBase.h / engine/CTCDoor.h, two parts; runs/rk_final.* plus the supplementary runs/rk_sup.* merged), index of
# ov_final, idx/after.jsonl (HEAD index with the rows replaced), qscan, QUEUE_W31.tsv, DEFERRED.tsv, BINDINGS.tsv +
# runs/reloc_bindings.after.tsv, the multi-binding check, RENAMES.tsv, LABELS.tsv. RK=1 reruns the recheck.
S=${S:?set S to the scratch lane root}; W=$S/naming/w31; : "${W:?}"; source $S/wine/env.sh; cd $W || exit 1
cat runs/{fs,csm,tcb,tcd,dgp,c702,door,pek,hos,d3,libpk,c703,srtf}.rows.txt | sort -u > runs/final.rows.txt
python3 scripts/includers.py fable_string.h runs/inc_fs.txt 2>/dev/null; python3 scripts/includers.py engine/CTCBase.h runs/inc_tcb.txt 2>/dev/null
python3 scripts/includers.py engine/CTCDoor.h runs/inc_door.txt 2>/dev/null
cat runs/inc_fs.txt runs/inc_tcb.txt runs/inc_door.txt | sort -u > runs/hdrdep.rows.txt
cat runs/final.rows.txt runs/hdrdep.rows.txt | sort -u > runs/final_plus.rows.txt
[ -z "$RK" ] || bash scripts/rkfinal31.sh
cat runs/rk_finala.log runs/rk_finalb.log runs/rk_sup.log 2>/dev/null > logs/recheck_final.log
cat runs/rk_finala.pairs.jsonl runs/rk_finalb.pairs.jsonl > runs/rk_final.pairs.jsonl
python3 - <<'PY'
import json, os
W = os.environ['S'] + '/naming/w31'
sup = {}
if os.path.exists(W + '/runs/rk_sup.pairs.jsonl'):
    for l in open(W + '/runs/rk_sup.pairs.jsonl'): sup[json.loads(l)['a']] = l
L = [l for l in open(W + '/runs/rk_final.pairs.jsonl') if json.loads(l)['a'] not in sup]
open(W + '/runs/rk_final.pairs.jsonl', 'w').write(''.join(L) + ''.join(sup.values()))
print('pairs', len(L) + len(sup), 'supplementary', len(sup))
PY
rm -f idx/ovfinal.jsonl; BS=1 NJ=3 python3 scripts/mkindex.py $W/idx/ovfinal.jsonl $W/ov_final $W/runs/final.rows.txt | tail -1; rm -rf $W/idx/ovfinal.jsonl.objs
python3 - <<'PY'
import json, os
W = os.environ['S'] + '/naming/w31'
new = {json.loads(l)['a']: l for l in open(W + '/idx/ovfinal.jsonl')}
assert not any('"err": ' in l for l in new.values())
with open(W + '/idx/after.jsonl', 'w') as o:
    for l in open(W + '/idx/full.jsonl'):
        a = json.loads(l)['a']; o.write(new.pop(a) if a in new else l)
print('after.jsonl; unplaced', len(new))
PY
python3 scripts/qscan31.py $W/idx/full.jsonl $W/runs/qscan_base.tsv > logs/qscan_base.log 2>&1
python3 scripts/qscan31.py $W/idx/after.jsonl $W/runs/qscan_final.tsv > logs/qscan_final.log 2>&1
python3 scripts/mkq31.py
python3 scripts/mkdef31.py $W/idx/after.jsonl | cut -c1-300
python3 scripts/mkbind31.py $W/runs/rk_final.pairs.jsonl $W/idx/after.jsonl $W/runs/final.rows.txt > logs/mkbind31.log 2>&1; grep -v ^NEWDEF logs/mkbind31.log
grep -c "^CONFLICT" BINDINGS.tsv
python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py runs/reloc_bindings.after.tsv | tee logs/bindings_multi_check.log
python3 scripts/mkren31.py $W/idx/after.jsonl | head -1; python3 scripts/fixren31.py
python3 scripts/mklabels31.py; python3 scripts/fixlab31.py
