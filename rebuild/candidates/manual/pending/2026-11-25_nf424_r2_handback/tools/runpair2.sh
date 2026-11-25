#!/usr/bin/env bash
# runpair.sh <tag> <addr...>: vland --recheck-landed of the HEAD payload (repo recheck.py) and of the new payload (same entries, source_cpp
# from rows/<addr>.cpp where present) under ONE LANDVERIFY_WORK path (work/wd), moved aside to work/wd_<tag>h / wd_<tag>n. Runs from
# work/rsroot (symlinks into the repo) so nothing lands in the repo cwd. Launch in the background.
set -u
W=$S/w22/agentNF424; R2=$W/r2; tag=$1; shift
cd $W/work/rsroot
LT=tools/decomp_pipeline/linux_lane/lane_tools
mkdir -p $R2/work/p_${tag}h; python3 $LT/recheck.py $R2/work/p_${tag}h "$@" > $R2/work/p_${tag}h.log 2>&1
python3 - "$W" "$tag" <<'PY'
import json, sys, os, shutil
W, tag = sys.argv[1], sys.argv[2]
p = json.load(open(f'{W}/r2/work/p_{tag}h/payload.json'))
p['result']['authored'] = [x for x in p['result']['authored'] if os.path.exists(f"{W}/r2/final/{x['address']}.cpp")]   # NF374: new payload = changed rows only
for x in p['result']['authored']:
    f = f"{W}/r2/final/{x['address']}.cpp"
    if os.path.exists(f):
        x['source_cpp'] = open(f, encoding='utf-8').read()
        import re as _re
        m = _re.search(r'^// Leaf:\s*(\S+)', x['source_cpp'], _re.M)
        if m: x['leaf'] = m.group(1)   # NF382: the payload leaf follows the new row's // Leaf line
    t = f"{W}/r2/final/{x['address']}_test.cpp"
    if os.path.exists(t): x['test_cpp'] = open(t, encoding='utf-8').read()
os.makedirs(f'{W}/r2/work/p_{tag}n', exist_ok=True)
json.dump(p, open(f'{W}/r2/work/p_{tag}n/payload.json', 'w'))
shutil.copy(f'{W}/r2/work/p_{tag}h/oracle.tsv', f'{W}/r2/work/p_{tag}n/oracle.tsv')
PY
for k in h n; do
  OV=""; [ $k = n ] && OV=""   # NF416: new side grades with the header / owner overlay, HEAD with the repo tree
  timeout 1800 env $OV LANDVERIFY_WORK=$R2/work/wd_$tag python3 -u $LT/vland.py $R2/work/p_${tag}${k}/payload.json $R2/work/p_${tag}${k}/oracle.tsv --recheck-landed > $R2/work/vland_${tag}${k}.log 2>&1
  mv $R2/work/wd_$tag $R2/work/wd_${tag}${k}
done
echo RUNPAIR_DONE > $R2/work/runpair_${tag}.done
