#!/usr/bin/env bash
# runtb427.sh <t>: Task B (test-only) vland pair. HEAD payload work/p_<t>h (recheck.py, repo copies); the new payload is that
# payload with ONLY test_cpp replaced by r2/<t>/tests/<a>_test.cpp (source_cpp, leaf, flags, pass_pattern untouched; asserted).
# Then the mutation control (mut425.sh form) on the new payload. Logs work/vland_<t>{h,n,m}.log
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF427; t=$1; LT=tools/decomp_pipeline/linux_lane/lane_tools
cd $W/work/rsroot; export FABLE_HDR_OVERLAY=$W/hdr
python3 - "$W" "$t" <<'PY'
import json, sys, os, shutil
W, t = sys.argv[1:3]
h = json.load(open(f'{W}/work/p_{t}h/payload.json')); n = json.loads(json.dumps(h))
for x in n['result']['authored']:
    nt = open(f"{W}/r2/{t}/tests/{x['address']}_test.cpp", encoding='utf-8').read()
    assert x['pass_pattern'] in nt, x['address']
    x['test_cpp'] = nt
for a, b in zip(h['result']['authored'], n['result']['authored']):
    assert {k: v for k, v in a.items() if k != 'test_cpp'} == {k: v for k, v in b.items() if k != 'test_cpp'}
os.makedirs(f'{W}/work/p_{t}n', exist_ok=True)
json.dump(n, open(f'{W}/work/p_{t}n/payload.json', 'w')); shutil.copy(f'{W}/work/p_{t}h/oracle.tsv', f'{W}/work/p_{t}n/oracle.tsv')
print('payload ok', len(n['result']['authored']))
PY
for k in h n; do
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_$t >/dev/null
  timeout 3600 env LANDVERIFY_WORK=$W/work/wd_$t python3 -u $LT/vland.py $W/work/p_${t}${k}/payload.json $W/work/p_${t}${k}/oracle.tsv --recheck-landed > $W/work/vland_${t}${k}.log 2>&1
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_${t}${k} >/dev/null; rm -rf $W/work/wd_${t}${k}; mv $W/work/wd_$t $W/work/wd_${t}${k}
done
bash $W/tools425/mut425.sh $t
echo RUNTB_DONE > $W/work/runtb_$t.done
