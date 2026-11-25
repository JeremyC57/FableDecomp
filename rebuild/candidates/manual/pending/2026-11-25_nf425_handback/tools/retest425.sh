#!/usr/bin/env bash
# retest425.sh <batch...>: after a test-only change (batches/<b>/tests), refresh work/p_<b>n test text, re-run vland on the new payload
# (same LANDVERIFY_WORK path; objects are unchanged by a test edit) -> work/vland_<b>n2.log, then the mutation control (mut425.sh).
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF425; LT=tools/decomp_pipeline/linux_lane/lane_tools; CL=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py
cd $W/work/rsroot
for b in "$@"; do
python3 - "$W" "$b" <<'PY'
import json, sys
W, b = sys.argv[1:3]
p = json.load(open(f'{W}/work/p_{b}n/payload.json'))
for x in p['result']['authored']: x['test_cpp'] = open(f"{W}/batches/{b}/tests/{x['address']}_test.cpp").read()
json.dump(p, open(f'{W}/work/p_{b}n/payload.json', 'w'))
PY
  python3 $CL $W $W/work/wd_$b >/dev/null
  timeout 2400 env LANDVERIFY_WORK=$W/work/wd_$b python3 -u $LT/vland.py $W/work/p_${b}n/payload.json $W/work/p_${b}n/oracle.tsv --recheck-landed > $W/work/vland_${b}n2.log 2>&1
  python3 $CL $W $W/work/wd_${b}n2 >/dev/null; mv $W/work/wd_$b $W/work/wd_${b}n2
  bash $W/tools/mut425.sh $b
done
