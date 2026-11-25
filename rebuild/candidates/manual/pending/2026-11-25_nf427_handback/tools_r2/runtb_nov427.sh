#!/usr/bin/env bash
# runtb_nov427.sh: the Task B rows whose new test includes a header that hdr/ shadows (work/tb/shadow_tests.txt), re-graded from the
# new payload with FABLE_HDR_OVERLAY UNSET (repo headers only). Log work/vland_tbnov.log
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP; unset FABLE_HDR_OVERLAY
W=$S/w22/agentNF427; LT=tools/decomp_pipeline/linux_lane/lane_tools; cd $W/work/rsroot
python3 - "$W" <<'PY'
import json, sys, os, re, shutil
W = sys.argv[1]; want = {}
for l in open(f'{W}/work/tb/shadow_tests.txt'):
    m = re.search(r'r2/(t\d)/tests/([0-9a-f]{8})_test', l); want.setdefault(m.group(1), set()).add(m.group(2))
out = None; orc = []
for t, s in want.items():
    p = json.load(open(f'{W}/work/p_{t}n/payload.json'))
    keep = [x for x in p['result']['authored'] if x['address'] in s]
    if out is None: out = p; out['result']['authored'] = []
    out['result']['authored'] += keep
    lines = open(f'{W}/work/p_{t}n/oracle.tsv').read().splitlines()
    if not orc: orc.append(lines[0])
    orc += [l for l in lines[1:] if l.split('\t')[0].lower() in s]
os.makedirs(f'{W}/work/p_tbnov', exist_ok=True)
json.dump(out, open(f'{W}/work/p_tbnov/payload.json', 'w')); open(f'{W}/work/p_tbnov/oracle.tsv', 'w').write('\n'.join(orc) + '\n')
print(len(out['result']['authored']), len(orc) - 1)
PY
python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_tbnov >/dev/null
timeout 2400 env LANDVERIFY_WORK=$W/work/wd_tbnov python3 -u $LT/vland.py $W/work/p_tbnov/payload.json $W/work/p_tbnov/oracle.tsv --recheck-landed > $W/work/vland_tbnov.log 2>&1
echo done > $W/work/tbnov.done
