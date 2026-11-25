#!/usr/bin/env python3
"""namegate438.py <batch>: name_gate.py on the HEAD row (orig/rows) and the new row (rows/), each with its test, from the repo root;
outputs in batches/<b>/evidence/ng/<a>_{h,n}.txt and a summary evidence/namegate_summary.tsv: address, HEAD FAIL count, new FAIL count,
FAIL kinds gone, FAIL kinds new, verdict (NO_WORSE when new <= HEAD)."""
import sys, os, re, subprocess, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = str(nf.A); R = '/home/user/FableDecomp'; b = sys.argv[1]; B = W + '/batches/' + b
NG = R + '/tools/decomp_pipeline/linux_lane/lane_tools/name_gate.py'
os.makedirs(B + '/evidence/ng', exist_ok=True)
SS = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv', encoding='utf-8'):
    q = l.rstrip('\n').split('\t')
    if len(q) > 1: SS[q[0]] = q[1]
out = ['address\thead_fail\tnew_fail\tgone\tadded\tverdict']
def fails(t): return [l.strip() for l in t.splitlines() if re.match(r'^\s*FAIL\b', l)]
def key(l): return re.sub(r'0x[0-9A-Fa-f]+|\b[0-9a-f]{8}\b', 'X', l)[:160]
for a in open(B + '/rows.txt').read().split():
    m = re.search(r'^// Leaf:\s*(\S+)', open(B + '/rows/%s.cpp' % a, encoding='utf-8', errors='replace').read(), re.M); leaf = SS.get(a) or (m.group(1) if m else '')
    res = {}
    for k, src, tst in (('h', B + '/orig/rows/%s.cpp' % a, B + '/orig/tests/%s_test.cpp' % a), ('n', B + '/rows/%s.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)):
        f = B + '/evidence/ng/%s_%s.txt' % (a, k)
        if not os.path.exists(f) or 'NAME_GATE' not in open(f).read():
            cmd = ['python3', NG, a, src, leaf] + (['--test', tst] if os.path.exists(tst) else [])
            try: t = subprocess.run(cmd, capture_output=True, text=True, cwd=R, timeout=900); txt = t.stdout + t.stderr
            except subprocess.TimeoutExpired: txt = 'TIMEOUT'
            open(f, 'w').write(txt)
        res[k] = fails(open(f).read())
    gh, gn = collections.Counter(map(key, res['h'])), collections.Counter(map(key, res['n']))
    out.append('%s\t%d\t%d\t%s\t%s\t%s' % (a, len(res['h']), len(res['n']), ' | '.join((gh - gn).elements())[:400], ' | '.join((gn - gh).elements())[:400], 'NO_WORSE' if len(res['n']) <= len(res['h']) else 'WORSE'))
open(B + '/evidence/namegate_summary.tsv', 'w').write('\n'.join(out) + '\n')
print(b, collections.Counter(l.split('\t')[-1] for l in out[1:]))
