#!/usr/bin/env python3
"""mkbatch438.py <plan.tsv> <rows dir> <batch prefix> [size=50]: cut a gen423 plan into batches <prefix>1.. of <= size rows:
rows/, orig/rows, orig/tests (HEAD copies named by address), rows.txt, renames.tsv (address old new target). Claims the lock of each
row (free or released -> NF438; a row held by anyone else is left out and listed) and of each target (for the definer compile;
release them with release423.sh). Writes work/<prefix>_addr.txt (reserved_check input) and work/<prefix>_targets.txt."""
import sys, os, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); S = str(nf.S)
plan, rd, pre = sys.argv[1:4]; size = int(sys.argv[4]) if len(sys.argv) > 4 else 50
def claim(a):
    d = S + '/w22/nflocks/' + a; os.makedirs(d, exist_ok=True); p = d + '/owner'
    c = open(p).read().strip() if os.path.exists(p) else 'released'
    if c == 'released': open(p, 'w').write('NF443\n'); c = 'NF443'
    return c == 'NF443'
P = [l.rstrip('\n').split('\t') for l in open(plan) if l.strip()]
ok = [x for x in P if claim(x[0])]; skipped = [x[0] for x in P if x not in ok]
tg = sorted({x[4] for x in ok}); tg_ok = [t for t in tg if claim(t)]
for i in range(0, len(ok), size):
    b = '%s%d' % (pre, i // size + 1); B = A + '/batches/' + b
    for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence', 'head'): os.makedirs(B + '/' + d, exist_ok=True)
    ch = ok[i:i + size]
    for a, k, s, P_, t in ch:
        shutil.copy(nf.src_of(a), B + '/orig/rows/%s.cpp' % a); tt = nf.test_of(a)
        if tt and tt.exists(): shutil.copy(tt, B + '/orig/tests/%s_test.cpp' % a)
        shutil.copy(rd + '/%s.cpp' % a, B + '/rows/%s.cpp' % a)
    open(B + '/rows.txt', 'w').write(''.join(x[0] + '\n' for x in ch))
    open(B + '/renames.tsv', 'w').write(''.join('\t'.join([a, s, P_, t]) + '\n' for a, k, s, P_, t in ch))
    print(b, len(ch))
open(A + '/work/%s_addr.txt' % pre, 'w').write('address\n' + ''.join(x[0] + '\n' for x in ok))
open(A + '/work/%s_targets.txt' % pre, 'w').write(''.join(t + '\n' for t in tg_ok))
print('rows locked elsewhere (left out):', skipped, '| targets not lockable:', sorted(set(tg) - set(tg_ok)))
