#!/usr/bin/env python3
"""xrcheck423.py <batch...>: for every row of each batch, resolve the new object's leaf relocations against the retail bytes (nf.py xr
logic) and assert each renamed (new) symbol resolves to the renames.tsv target, and the old symbol no longer appears.
Writes batches/<b>/evidence/xrcheck.txt."""
import sys, os, glob, csv, subprocess
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439'
tot = bad = 0
for b in sys.argv[1:]:
    B = W + '/batches/' + b; out = []
    for a, old, new, t in [l.rstrip('\n').split('\t') for l in open(B + '/renames.tsv') if l.strip()]:
        o = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a))[0]
        x = subprocess.run([sys.executable, W + '/tools/nf.py', 'xr', a, o], capture_output=True, text=True).stdout
        pairs = [l.split('\t')[:2] for l in x.splitlines() if '\t' in l]
        tg = sorted({p[0] for p in pairs if p[1] == new}); ok = tg == [t] and not any(p[1] == old for p in pairs)
        tot += 1; bad += not ok
        out.append('%s\t%s\t%s -> %s\t%s' % (a, 'OK' if ok else 'BAD', new, ','.join(tg) or 'none', t))
    open(B + '/evidence/xrcheck.txt', 'w').write('\n'.join(out) + '\n')
print('renames checked', tot, 'bad', bad)
