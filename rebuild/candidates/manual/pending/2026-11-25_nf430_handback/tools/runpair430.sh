#!/usr/bin/env bash
# runpair430.sh <batch> [newroot]: vland --recheck-landed of HEAD payload from rsroot and of batch rows from <newroot> (default rsnew),
# both under ONE LANDVERIFY_WORK path (work/wd_<b>), moved aside to wd_<b>h / wd_<b>n.
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF430; b=$1; NR=${2:-$A/work/rsnew}; B=$A/batches/$b
source $S/wine/env.sh; export S; unset TMPDIR TMP
LT=tools/decomp_pipeline/linux_lane/lane_tools
cd $A/work/rsroot; mkdir -p $A/work/p_${b}h; python3 $LT/recheck.py $A/work/p_${b}h $(cat $B/rows.txt) > $A/work/p_${b}h.log 2>&1
python3 - "$B" "$A" "$b" <<'PY'
import json, sys, os, shutil, re
B, W, tag = sys.argv[1:4]
p = json.load(open(f'{W}/work/p_{tag}h/payload.json'))
for x in p['result']['authored']:
    x['source_cpp'] = open(f"{B}/rows/{x['address']}.cpp", encoding='utf-8').read()
    m = re.search(r'^// Leaf:\s*(\S+)', x['source_cpp'], re.M)
    if m: x['leaf'] = m.group(1)
    x['test_cpp'] = open(f"{B}/tests/{x['address']}_test.cpp", encoding='utf-8').read()
os.makedirs(f'{W}/work/p_{tag}n', exist_ok=True)
json.dump(p, open(f'{W}/work/p_{tag}n/payload.json', 'w'))
shutil.copy(f'{W}/work/p_{tag}h/oracle.tsv', f'{W}/work/p_{tag}n/oracle.tsv')
shutil.copy(f'{W}/work/p_{tag}h/payload.json', f'{B}/head/payload.json')
PY
for k in h n; do
  if [ $k = h ]; then cd $A/work/rsroot; else cd $NR; fi
  rm -rf $A/work/wd_$b
  timeout 3600 env LANDVERIFY_WORK=$A/work/wd_$b python3 -u $LT/vland.py $A/work/p_${b}${k}/payload.json $A/work/p_${b}${k}/oracle.tsv --recheck-landed > $A/work/vland_${b}${k}.log 2>&1
  rm -rf $A/work/wd_${b}${k}; mv $A/work/wd_$b $A/work/wd_${b}${k}
done
echo RUNPAIR_DONE > $A/work/runpair_${b}.done
