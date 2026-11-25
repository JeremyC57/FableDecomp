#!/usr/bin/env bash
# runpair432.sh <batch> <newroot-name>: vland --recheck-landed of the HEAD payload (repo root rsroot) and of the batch rows / tests
# (root work/<newroot-name>: changed table + header overlay), both under ONE LANDVERIFY_WORK path (work/wd_<b>), then moved aside to
# wd_<b>h / wd_<b>n. Also writes orig/ HEAD copies and head/payload.json.
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF436; b=$1; NR=$W/work/$2; B=$W/batches/$b
source $S/wine/env.sh; export S; unset TMPDIR TMP
LT=tools/decomp_pipeline/linux_lane/lane_tools
cd $W/work/rsroot; mkdir -p $W/work/p_${b}h; python3 $LT/recheck.py $W/work/p_${b}h $(cat $B/rows.txt) > $W/work/p_${b}h.log 2>&1
python3 - "$B" "$W" "$b" <<'PY'
import json, sys, os, shutil, re
B, W, tag = sys.argv[1:4]
p = json.load(open(f'{W}/work/p_{tag}h/payload.json'))
os.makedirs(f'{B}/orig/rows', exist_ok=True); os.makedirs(f'{B}/orig/tests', exist_ok=True); os.makedirs(f'{B}/head', exist_ok=True)
for x in p['result']['authored']:
    a = x['address']
    open(f'{B}/orig/rows/{a}.cpp', 'w', encoding='utf-8').write(x['source_cpp'])
    open(f'{B}/orig/tests/{a}_test.cpp', 'w', encoding='utf-8').write(x['test_cpp'])
    x['source_cpp'] = open(f"{B}/rows/{a}.cpp", encoding='utf-8').read()
    m = re.search(r'^// Leaf:\s*(\S+)', x['source_cpp'], re.M)
    if m: x['leaf'] = m.group(1)
    x['test_cpp'] = open(f"{B}/tests/{a}_test.cpp", encoding='utf-8').read()
    if os.path.exists(f'{B}/RENAMES.tsv'):   # the catalog rename the lander applies first (module / name)
        for l in open(f'{B}/RENAMES.tsv').read().splitlines()[1:]:
            f = l.split('\t')
            if f[0] == a: x['module'], x['name'] = f[1], f[2]
os.makedirs(f'{W}/work/p_{tag}n', exist_ok=True)
json.dump(p, open(f'{W}/work/p_{tag}n/payload.json', 'w'))
ren = {}
if os.path.exists(f'{B}/RENAMES.tsv'):   # apply_renames rewrites the oracle name too (module::name)
    for l in open(f'{B}/RENAMES.tsv').read().splitlines()[1:]:
        f = l.split('\t'); ren[f[0]] = f[1] + '::' + f[2]
ol = open(f'{W}/work/p_{tag}h/oracle.tsv', encoding='utf-8').read().split('\n')
for i, l in enumerate(ol):
    f = l.split('\t')
    if f and f[0] in ren: f[1] = ren[f[0]]; ol[i] = '\t'.join(f)
open(f'{W}/work/p_{tag}n/oracle.tsv', 'w', encoding='utf-8').write('\n'.join(ol))
shutil.copy(f'{W}/work/p_{tag}h/payload.json', f'{B}/head/payload.json')
PY
for k in h n; do
  if [ $k = h ]; then R=$W/work/rsroot; else R=$NR; fi
  cd $R; rm -rf $W/work/wd_$b
  timeout 3600 env FABLE_ROOT=$R LANDVERIFY_WORK=$W/work/wd_$b python3 -u $LT/vland.py $W/work/p_${b}${k}/payload.json $W/work/p_${b}${k}/oracle.tsv --recheck-landed > $W/work/vland_${b}${k}.log 2>&1
  rm -rf $W/work/wd_${b}${k}; mv $W/work/wd_$b $W/work/wd_${b}${k}
done
echo RUNPAIR_DONE $b
