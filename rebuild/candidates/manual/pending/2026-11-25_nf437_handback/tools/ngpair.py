#!/usr/bin/env python3
# ngpair.py <batch>: name_gate HEAD v new for every row (+ its test) of a batch; FAIL-line sets compared (new must be a subset of HEAD).
import sys, os, subprocess, csv, re
from concurrent.futures import ThreadPoolExecutor
R = '/home/user/FableDecomp'; W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); B = W + '/batches/' + sys.argv[1]
rows = open(B + '/rows.txt').read().split()
ss = {}
for r in csv.reader(open(R + '/rebuild/oracles/self_symbols.tsv'), delimiter='\t'):
    if r: ss[r[0].lower().replace('0x', '')] = r[1]
for l in open(B + '/RESULTS.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    if len(q) > 3 and q[3] not in ('', '?'): ss[q[0]] = q[3]
ONLY = set(os.environ.get('ONLY', '').split())
if ONLY: rows = [r for r in rows if r in ONLY]
NG = R + '/tools/decomp_pipeline/linux_lane/lane_tools/name_gate.py'
E = B + '/evidence/ng'; os.makedirs(E, exist_ok=True)
def run(a, side):
    src = B + ('/orig/rows/%s.cpp' if side == 'H' else '/rows/%s.cpp') % a
    t = B + ('/orig/tests/%s_test.cpp' if side == 'H' else '/tests/%s_test.cpp') % a
    cmd = ['python3', NG, a, src, ss.get(a, '?')] + (['--test', t] if os.path.exists(t) else [])
    p = subprocess.run(cmd, capture_output=True, text=True, cwd=R, timeout=900)
    open('%s/%s_%s.log' % (E, side, a), 'w').write(p.stdout + p.stderr[-2000:])
    return [re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', re.sub(r'\s+', ' ', l.strip())) for l in p.stdout.splitlines() if 'FAIL' in l and 'NAME_GATE' not in l]
def one(a):
    h = run(a, 'H'); n = run(a, 'N')
    new = [x for x in n if x not in h]; gone = [x for x in h if x not in n]
    return '%s\tHEAD_FAIL=%d\tNEW_FAIL=%d\t%s\tnew: %s\tgone: %d' % (a, len(h), len(n), 'NO_WORSE' if not new else 'WORSE', ' | '.join(new)[:600], len(gone))
with ThreadPoolExecutor(4) as ex: res = list(ex.map(one, rows))
open(B + '/evidence/name_gate_pair%s.tsv' % ('_rerun' if ONLY else ''), 'w').write('\n'.join(res) + '\n'); print('\n'.join(res))
