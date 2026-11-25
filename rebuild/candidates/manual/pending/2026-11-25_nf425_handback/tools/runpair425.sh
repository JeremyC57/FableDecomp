#!/usr/bin/env bash
# runpair425.sh <batch>: vland --recheck-landed HEAD payload (recheck.py) and new payload (batches/<b>/rows, tests) under ONE
# LANDVERIFY_WORK path (work/wd_<b>), moved aside to wd_<b>h / wd_<b>n. Runs from work/rsroot (symlinks into the repo).
set -u
W=$S/w22/agentNF425; b=$1; B=$W/batches/$b
cd $W/work/rsroot
LT=tools/decomp_pipeline/linux_lane/lane_tools
mkdir -p $W/work/p_${b}h; python3 $LT/recheck.py $W/work/p_${b}h $(cat $B/rows.txt) > $W/work/p_${b}h.log 2>&1
python3 - "$B" "$W" "$b" <<'PY'
import json, sys, os, shutil, re
B, W, tag = sys.argv[1:4]
p = json.load(open(f'{W}/work/p_{tag}h/payload.json'))
for x in p['result']['authored']:
    f = f"{B}/rows/{x['address']}.cpp"
    x['source_cpp'] = open(f, encoding='utf-8').read()
    m = re.search(r'^// Leaf:\s*(\S+)', x['source_cpp'], re.M)
    if m: x['leaf'] = m.group(1)
    x['test_cpp'] = open(f"{B}/tests/{x['address']}_test.cpp", encoding='utf-8').read()
os.makedirs(f'{W}/work/p_{tag}n', exist_ok=True)
json.dump(p, open(f'{W}/work/p_{tag}n/payload.json', 'w'))
shutil.copy(f'{W}/work/p_{tag}h/oracle.tsv', f'{W}/work/p_{tag}n/oracle.tsv')
os.makedirs(f'{B}/head', exist_ok=True); shutil.copy(f'{W}/work/p_{tag}h/payload.json', f'{B}/head/payload.json')
PY
for k in h n; do
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_$b >/dev/null
  timeout 2400 env LANDVERIFY_WORK=$W/work/wd_$b python3 -u $LT/vland.py $W/work/p_${b}${k}/payload.json $W/work/p_${b}${k}/oracle.tsv --recheck-landed > $W/work/vland_${b}${k}.log 2>&1
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_${b}${k} >/dev/null; mv $W/work/wd_$b $W/work/wd_${b}${k}
done
echo RUNPAIR_DONE > $W/work/runpair_${b}.done
