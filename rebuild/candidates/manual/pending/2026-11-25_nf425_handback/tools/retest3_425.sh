#!/usr/bin/env bash
# retest3_425.sh <batch...>: after a comment-only row change, rebuild work/p_<b>n for the batch's CURRENT rows (rows.txt) with the batch's
# row and test text, re-run vland at the same LANDVERIFY_WORK path -> work/vland_<b>n3.log, objects in work/wd_<b>n3. No mutation rerun
# (code unchanged; vland_<b>m.log stands).
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF425; LT=tools/decomp_pipeline/linux_lane/lane_tools; CL=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py
cd $W/work/rsroot
for b in "$@"; do
python3 - "$W" "$b" <<'PY'
import json, sys
W, b = sys.argv[1:3]
rows = open(f'{W}/batches/{b}/rows.txt').read().split()
p = json.load(open(f'{W}/work/p_{b}n/payload.json'))
p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] in rows]
for x in p['result']['authored']:
    x['source_cpp'] = open(f"{W}/batches/{b}/rows/{x['address']}.cpp", encoding='utf-8').read()
    x['test_cpp'] = open(f"{W}/batches/{b}/tests/{x['address']}_test.cpp", encoding='utf-8').read()
assert len(p['result']['authored']) == len(rows)
json.dump(p, open(f'{W}/work/p_{b}n/payload.json', 'w'))
PY
  python3 $CL $W $W/work/wd_$b >/dev/null
  timeout 2400 env LANDVERIFY_WORK=$W/work/wd_$b python3 -u $LT/vland.py $W/work/p_${b}n/payload.json $W/work/p_${b}n/oracle.tsv --recheck-landed > $W/work/vland_${b}n3.log 2>&1
  python3 $CL $W $W/work/wd_${b}n3 >/dev/null; mv $W/work/wd_$b $W/work/wd_${b}n3
  echo done > $W/work/retest3_$b.done
done
