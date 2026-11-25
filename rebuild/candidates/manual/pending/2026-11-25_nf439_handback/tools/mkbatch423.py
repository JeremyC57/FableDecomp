#!/usr/bin/env python3
"""mkbatch423.py <plan.tsv> <rows dir> <batch prefix> [size=50]: cut a gen423 plan into batches <prefix>1.. of <= size rows:
rows/, orig/rows, orig/tests (HEAD copies named by address), rows.txt, renames.tsv (address old new target). Claims the lock of each
row (free or released -> NF439; a row held by anyone else is left out and listed) and of each target (for the definer compile;
release them with release423.sh). Writes work/<prefix>_addr.txt (reserved_check input) and work/<prefix>_targets.txt."""
import sys, os, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); S = str(nf.S)
plan, rd, pre = sys.argv[1:4]; size = int(sys.argv[4]) if len(sys.argv) > 4 else 50
def claim(a):
    d = S + '/w22/nflocks/' + a; os.makedirs(d, exist_ok=True); p = d + '/owner'
    c = open(p).read().strip() if os.path.exists(p) else 'released'
    if c == 'released': open(p, 'w').write('NF439\n'); c = 'NF439'
    return c == 'NF439'
P = [l.rstrip('\n').split('\t') for l in open(plan) if l.strip()]
ok = [x for x in P if claim(x[0])]; skipped = [x[0] for x in P if x not in ok]
tg = sorted({x[4] for x in ok}); tg_ok = tg   # NF439: targets are compiled from HEAD only, never locked
rows_u = []
for x in ok:
    if x[0] not in rows_u: rows_u.append(x[0])
for i in range(0, len(rows_u), size):
    b = '%s%d' % (pre, i // size + 1); B = A + '/batches/' + b
    for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence', 'head'): os.makedirs(B + '/' + d, exist_ok=True)
    chr_ = set(rows_u[i:i + size]); ch = [x for x in ok if x[0] in chr_]
    for a, k, s, P_, t in ch:
        shutil.copy(nf.src_of(a), B + '/orig/rows/%s.cpp' % a); tt = nf.test_of(a)
        if tt and tt.exists(): shutil.copy(tt, B + '/orig/tests/%s_test.cpp' % a)
        shutil.copy(rd + '/%s.cpp' % a, B + '/rows/%s.cpp' % a)
    open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in rows_u[i:i + size]))
    open(B + '/renames.tsv', 'w').write(''.join('\t'.join([a, s, P_, t]) + '\n' for a, k, s, P_, t in ch))
    print(b, len(ch))
open(A + '/work/%s_addr.txt' % pre, 'w').write('address\n' + ''.join(a + '\n' for a in rows_u))
open(A + '/work/%s_targets.txt' % pre, 'w').write(''.join(t + '\n' for t in tg_ok))
print('rows locked elsewhere (left out):', skipped, '| targets not lockable:', sorted(set(tg) - set(tg_ok)))
