#!/usr/bin/env python3
"""ngate439.py <batch>: name_gate.py on HEAD (orig/rows) and new (rows) of every row, with the HEAD test; verdict OK when the new run's
FAIL lines are a subset of HEAD's (no worse). Writes batches/<b>/evidence/name_gate/{H,N}_<a>.txt and evidence/name_gate_summary.tsv."""
import sys, os, subprocess, csv, re
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439'; R = '/home/user/FableDecomp'
LT = R + '/tools/decomp_pipeline/linux_lane/lane_tools'
b = sys.argv[1]; B = W + '/batches/' + b; E = B + '/evidence/name_gate'; os.makedirs(E, exist_ok=True)
SELF = {r[0]: r for r in csv.reader(open(R + '/rebuild/oracles/self_symbols.tsv'), delimiter='\t') if r}
out = []; bad = 0
for a in open(B + '/rows.txt').read().split():
    r = SELF.get(a, [a, '', '', '']); leaf = r[1] if r[1].startswith('?') or r[1].startswith('_') else ''
    if not leaf:
        m = re.search(r'^// Leaf:\s*(\S+)', open(B + '/rows/%s.cpp' % a).read(), re.M); leaf = m.group(1) if m else r[3]
    t = B + '/orig/tests/%s_test.cpp' % a; res = {}
    for k, src in (('H', B + '/orig/rows/%s.cpp' % a), ('N', B + '/rows/%s.cpp' % a)):
        cmd = [sys.executable, LT + '/name_gate.py', a, src, leaf] + (['--test', t] if os.path.exists(t) else [])
        p = subprocess.run(cmd, capture_output=True, text=True, cwd=R, timeout=600)
        open('%s/%s_%s.txt' % (E, k, a), 'w').write(p.stdout + p.stderr[-2000:])
        res[k] = (set(l.strip() for l in p.stdout.split('\n') if l.startswith('FAIL')), p.stdout.strip().split('\n')[-1] if p.stdout.strip() else 'NO OUTPUT')
    new = res['N'][0] - res['H'][0]
    v = 'OK' if not new and 'NAME_GATE' in res['N'][1] else 'WORSE'
    bad += v != 'OK'
    out.append('%s\t%s\tH: %s | N: %s\t%s' % (a, v, res['H'][1], res['N'][1], ' ; '.join(sorted(new))[:300]))
open(B + '/evidence/name_gate_summary.tsv', 'w').write('\n'.join(out) + '\n'); print('\n'.join(out)); print('WORSE', bad)
