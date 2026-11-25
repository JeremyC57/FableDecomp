#!/usr/bin/env bash
# runpair.sh <tag> <addr...>: vland --recheck-landed HEAD payload (recheck.py) and new payload (batches/b1/rows/<a>.cpp, tests/<a>_test.cpp)
# under ONE LANDVERIFY_WORK path (work/wd_<tag>), moved aside to wd_<tag>h / wd_<tag>n. Runs from work/rsroot (symlinks into the repo).
set -u
W=$S/w22/agentNF428; B=$W/runs/${BATCH:?}; tag=$1; shift
cd $W/work/rsroot
LT=tools/decomp_pipeline/linux_lane/lane_tools
mkdir -p $W/work/p_${tag}h; python3 $LT/recheck.py $W/work/p_${tag}h "$@" > $W/work/p_${tag}h.log 2>&1
python3 - "$B" "$W" "$tag" <<'PY'
import json, sys, os, shutil, re
B, W, tag = sys.argv[1:4]
p = json.load(open(f'{W}/work/p_{tag}h/payload.json'))
for x in p['result']['authored']:
    f = f"{B}/rows/{x['address']}.cpp"
    if os.path.exists(f):
        x['source_cpp'] = open(f, encoding='utf-8').read()
        m = re.search(r'^// Leaf:\s*(\S+)', x['source_cpp'], re.M)
        if m: x['leaf'] = m.group(1)
    t = f"{B}/tests/{x['address']}_test.cpp"
    if os.path.exists(t): x['test_cpp'] = open(t, encoding='utf-8').read()
os.makedirs(f'{W}/work/p_{tag}n', exist_ok=True)
json.dump(p, open(f'{W}/work/p_{tag}n/payload.json', 'w'))
shutil.copy(f'{W}/work/p_{tag}h/oracle.tsv', f'{W}/work/p_{tag}n/oracle.tsv')
PY
for k in h n; do
  rm -rf $W/work/wd_$tag
  timeout 1800 env LANDVERIFY_WORK=$W/work/wd_$tag python3 -u $LT/vland.py $W/work/p_${tag}${k}/payload.json $W/work/p_${tag}${k}/oracle.tsv --recheck-landed > $W/work/vland_${tag}${k}.log 2>&1
  rm -rf $W/work/wd_${tag}${k}; mv $W/work/wd_$tag $W/work/wd_${tag}${k}
done
echo RUNPAIR_DONE > $W/work/runpair_${tag}.done
