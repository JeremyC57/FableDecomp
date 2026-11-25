#!/usr/bin/env python3
"""mkb438.py <plan.tsv> <rows dir> <prefix> <rowlist file> [size=50]: rebind438 plan (row, mode, old, new, target; several lines per
row allowed) -> batches <prefix>1.. of <= size ROWS: rows/, orig/rows, orig/tests (HEAD copies by address), rows.txt, renames.tsv
(address old new target). Claims each row's lock (missing / released -> NF438); rows held elsewhere are left out and printed.
Writes work/<prefix>_addr.txt (reserved_check input) and work/<prefix>_targets.txt (definers)."""
import sys, os, shutil, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); S = str(nf.S)
plan, rd, pre, lst = sys.argv[1:5]; size = int(sys.argv[5]) if len(sys.argv) > 5 else 50
want = open(lst).read().split()
P = collections.OrderedDict()
for l in open(plan):
    x = l.rstrip('\n').split('\t')
    if x[0] in want: P.setdefault(x[0], []).append(x)
def claim(a):
    d = S + '/w22/nflocks/' + a; os.makedirs(d, exist_ok=True); p = d + '/owner'
    c = open(p).read().strip() if os.path.exists(p) else 'released'
    if c == 'released' or c == '': open(p, 'w').write('NF443\n'); c = 'NF443'
    return c == 'NF443'
ok = [a for a in P if claim(a)]; skipped = [a for a in P if a not in ok]
for i in range(0, len(ok), size):
    b = os.environ.get('BNAME') or '%s%d' % (pre, i // size + 1); B = A + '/batches/' + b
    for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence', 'head'): os.makedirs(B + '/' + d, exist_ok=True)
    ch = ok[i:i + size]
    for a in ch:
        shutil.copy(nf.src_of(a), B + '/orig/rows/%s.cpp' % a); tt = nf.test_of(a)
        if tt and tt.exists(): shutil.copy(tt, B + '/orig/tests/%s_test.cpp' % a)
        shutil.copy(rd + '/%s.cpp' % a, B + '/rows/%s.cpp' % a)
    open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in ch))
    open(B + '/renames.tsv', 'w').write(''.join('\t'.join([x[0], x[2], x[3], x[4]]) + '\n' for a in ch for x in P[a]))
    print(b, len(ch))
open(A + '/work/%s_addr.txt' % pre, 'w').write('address\n' + ''.join(a + '\n' for a in ok))
open(A + '/work/%s_targets.txt' % pre, 'w').write(''.join(t + '\n' for t in sorted({x[4] for a in ok for x in P[a]})))
print('rows held elsewhere (left out):', skipped)
