#!/usr/bin/env bash
# mut425.sh <batch...>: mutation control. From the new payload (work/p_<b>n), mutate each row's body
# (Copy: `*this = *(const C*)other;` -> `*this = *(const C*)this;`; Clone: `return new C(*this);` -> `return 0;`)
# and run vland at the same LANDVERIFY_WORK path; the tests must FAIL. Log: work/vland_<b>m.log
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF425; LT=tools/decomp_pipeline/linux_lane/lane_tools
cd $W/work/rsroot
for b in "$@"; do
python3 - "$W" "$b" <<'PY'
import json, sys, os, re, shutil
W, b = sys.argv[1:3]
p = json.load(open(f'{W}/work/p_{b}n/payload.json'))
for x in p['result']['authored']:
    s = x['source_cpp']
    s2 = re.sub(r'(\*this = \*\(const \w+\*\))other;', r'\1this;', s)
    s2 = re.sub(r'return new (\w+)\(\*this\);', r'return 0;', s2)
    assert s2 != s, x['address']
    x['source_cpp'] = s2
os.makedirs(f'{W}/work/p_{b}m', exist_ok=True)
json.dump(p, open(f'{W}/work/p_{b}m/payload.json', 'w')); shutil.copy(f'{W}/work/p_{b}n/oracle.tsv', f'{W}/work/p_{b}m/oracle.tsv')
PY
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_$b >/dev/null
  timeout 2400 env LANDVERIFY_WORK=$W/work/wd_$b python3 -u $LT/vland.py $W/work/p_${b}m/payload.json $W/work/p_${b}m/oracle.tsv --recheck-landed > $W/work/vland_${b}m.log 2>&1
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $W/work/wd_${b}m >/dev/null; mv $W/work/wd_$b $W/work/wd_${b}m
  echo done > $W/work/mut_$b.done
done
