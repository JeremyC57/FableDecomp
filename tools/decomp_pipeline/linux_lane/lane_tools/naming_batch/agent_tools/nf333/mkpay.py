# mkpay.py <tag> <addr...>: HEAD payload from the repo's recheck.py (catalog flags, Leaf, test) -> work/<tag>h_payload.json + oracle;
# the new payload is the SAME entries with rows/<a>.cpp (and rows/<a>_test.cpp when present) and the new Leaf -> work/<tag>n_payload.json.
# Both sides then share flags / pass pattern exactly (NF323, 2026-11-23).
import sys, json, re, subprocess, shutil
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333'); W = A / 'work'
t = sys.argv[1]; addrs = sys.argv[2:]
d = W / ('pay_' + t); d.mkdir(exist_ok=True)
r = subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/recheck.py', str(d)] + addrs, cwd='/home/user/FableDecomp', capture_output=True, text=True)
print(r.stdout.strip(), r.stderr.strip()[-300:])
P = json.load(open(d / 'payload.json')); orc = (d / 'oracle.tsv').read_text()
json.dump(P, open(W / (t + 'h_payload.json'), 'w')); (W / (t + 'h_oracle.tsv')).write_text(orc)
for e in P['result']['authored']:
    a = e['address']; s = (A / 'rows' / (a + '.cpp')).read_text(encoding='utf-8'); e['source_cpp'] = s
    tp = A / 'rows' / (a + '_test.cpp')
    if tp.exists(): e['test_cpp'] = tp.read_text(encoding='utf-8')
    m = re.search(r'^//\s*Leaf:\s*(\S+)', s, re.M); e['leaf'] = m.group(1)
json.dump(P, open(W / (t + 'n_payload.json'), 'w')); (W / (t + 'n_oracle.tsv')).write_text(orc)
print(t, len(P['result']['authored']), 'rows')
