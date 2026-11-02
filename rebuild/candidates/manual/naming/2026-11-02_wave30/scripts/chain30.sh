#!/bin/bash
# chain30.sh: the wave 30 hand-back chain from a composed ov_final -- recheck of the family rows (no shared header is edited this wave:
# runs/hdrdep.rows.txt is empty), index of ov_final, idx/after.jsonl (HEAD index with the rows replaced), qscan, QUEUE_W30.tsv, DEFERRED.tsv,
# BINDINGS.tsv + runs/reloc_bindings.after.tsv, the multi-binding check, RENAMES.tsv, LABELS.tsv. SKIP_RK=1 reuses runs/rk_final.*.
S=${S:?set S to the scratch lane root}; W=$S/naming/w30; : "${W:?}"; source $S/wine/env.sh; cd $W || exit 1
cat runs/{cst,csm,c699,stl,lhn}.rows.txt | sort -u > runs/final.rows.txt
: > runs/hdrdep.rows.txt
cat runs/final.rows.txt runs/hdrdep.rows.txt | sort -u > runs/final_plus.rows.txt
[ -n "$SKIP_RK" ] || bash scripts/recheck.sh final ov_final $W/runs/final_plus.rows.txt 2 | tail -4
cp runs/rk_final.log logs/recheck_final.log
grep "WARN leaf" runs/rk_final.log
rm -f idx/ovfinal.jsonl; BS=1 NJ=4 python3 scripts/mkindex.py $W/idx/ovfinal.jsonl $W/ov_final $W/runs/final.rows.txt | tail -1; rm -rf $W/idx/ovfinal.jsonl.objs
python3 - <<'PY'
import json, os
W = os.environ['S'] + '/naming/w30'
new = {json.loads(l)['a']: l for l in open(W + '/idx/ovfinal.jsonl')}
assert not any('"err": ' in l for l in new.values())
with open(W + '/idx/after.jsonl', 'w') as o:
    for l in open(W + '/idx/full.jsonl'):
        a = json.loads(l)['a']; o.write(new.pop(a) if a in new else l)
print('after.jsonl; unplaced', len(new))
PY
python3 scripts/qscan30.py $W/idx/full.jsonl $W/runs/qscan_base.tsv > logs/qscan_base.log 2>&1
python3 scripts/qscan30.py $W/idx/after.jsonl $W/runs/qscan_final.tsv > logs/qscan_final.log 2>&1
python3 scripts/mkq30.py
python3 scripts/mkdef30.py $W/idx/after.jsonl | cut -c1-300
python3 scripts/mkbind30.py $W/runs/rk_final.pairs.jsonl $W/idx/after.jsonl $W/runs/final.rows.txt > logs/mkbind30.log 2>&1; grep -v ^NEWDEF logs/mkbind30.log
grep -c "^CONFLICT" BINDINGS.tsv
python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py runs/reloc_bindings.after.tsv | tee logs/bindings_multi_check.log
python3 scripts/mkren30.py $W/idx/after.jsonl | head -1; python3 scripts/fixren30.py
python3 scripts/mklabels30.py; python3 scripts/fixlab30.py
