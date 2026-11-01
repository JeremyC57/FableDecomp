#!/bin/bash
# chain26.sh: the wave 26 hand-back chain from a composed ov_final -- full recheck (4 parts), index of ov_final, idx/after.jsonl
# (HEAD index with the 48 rows replaced), qscan, QUEUE_W26.tsv, DEFERRED.tsv, BINDINGS.tsv + runs/reloc_bindings.after.tsv,
# the multi-binding check, RENAMES.tsv, LABELS.tsv.
S=${S:?set S to the scratch lane root}; W=$S/naming/w27; : "${W:?}"; source $S/wine/env.sh; cd $W || exit 1
cat runs/{nul,tbvm,wcol,pred,gossip,rbt,vlong,vws,gst,c693,fdos,c694,cti}.rows.txt > runs/final.rows.txt
bash scripts/recheck.sh final ov_final $W/runs/final.rows.txt 4 | tail -4
grep "WARN leaf" runs/rk_final.log
rm -f idx/ovfinal.jsonl; python3 scripts/mkindex.py $W/idx/ovfinal.jsonl $W/ov_final $W/runs/final.rows.txt | tail -1; rm -rf $W/idx/ovfinal.jsonl.objs
python3 - <<'PY'
import json, os
W = os.environ['S'] + '/naming/w27'
new = {json.loads(l)['a']: l for l in open(W + '/idx/ovfinal.jsonl')}
assert not any('"err"' in l for l in new.values())
with open(W + '/idx/after.jsonl', 'w') as o:
    for l in open(W + '/idx/full.jsonl'):
        a = json.loads(l)['a']; o.write(new.pop(a) if a in new else l)
print('after.jsonl; unplaced', len(new))
PY
python3 scripts/qscan26.py $W/idx/after.jsonl $W/runs/qscan_final.tsv > logs/qscan_final.log 2>&1
python3 scripts/mkq26.py
python3 scripts/mkdef26.py $W/idx/after.jsonl
python3 scripts/mkbind26.py $W/runs/rk_final.pairs.jsonl $W/idx/after.jsonl $W/runs/final.rows.txt > logs/mkbind26.log 2>&1; grep -v ^NEWDEF logs/mkbind26.log
grep -c "^CONFLICT" BINDINGS.tsv
python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py runs/reloc_bindings.after.tsv | tee logs/bindings_multi_check.log
python3 scripts/mkren26.py $W/idx/after.jsonl | head -1; python3 scripts/fixren26.py
python3 scripts/mklabels26.py; python3 scripts/fixlab26.py
