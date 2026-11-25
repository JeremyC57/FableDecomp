#!/usr/bin/env python3
"""mkbatch425.py <batch> <gen.tsv> <gendir> <addr...|file>: lay out batches/<b>/: rows/<a>.cpp + tests/<a>_test.cpp (generated),
orig/rows/<a>.cpp + orig/tests/<a>_test.cpp (HEAD copies BY ADDRESS), rows.txt, selfmap.tsv (address, HEAD self, new self),
expect.tsv (address, *, expected callee name) from the generator table (column 7 = target name, when present)."""
import sys, os, shutil, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
b, gtsv, gdir = sys.argv[1:4]; rows = sys.argv[4:]
if len(rows) == 1 and os.path.isfile(rows[0]): rows = open(rows[0]).read().split()
B = L.A + '/batches/' + b
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence', 'logs'): os.makedirs(B + '/' + d, exist_ok=True)
g = {r[0]: r for r in csv.reader(open(gtsv), delimiter='\t')}
sm = open(B + '/selfmap.tsv', 'w'); ex = open(B + '/expect.tsv', 'w')
for a in rows:
    if L.nf.lock_owner(a) != 'NF425': sys.exit('NOLOCK ' + a)
    shutil.copy('%s/%s.cpp' % (gdir, a), B + '/rows/%s.cpp' % a)
    shutil.copy('%s/%s_test.cpp' % (gdir, a), B + '/tests/%s_test.cpp' % a)
    shutil.copy(str(L.nf.src_of(a)), B + '/orig/rows/%s.cpp' % a)
    t = L.nf.test_of(a)
    if t and t.exists(): shutil.copy(str(t), B + '/orig/tests/%s_test.cpp' % a)
    sm.write('%s\t%s\t%s\n' % (a, L.WL[a]['self_leaf'], L.WL[a]['rtti']))
    for nm in (g[a][6].split(',') if len(g[a]) > 6 and g[a][6] not in ('', '-') else []): ex.write('%s\t*\t%s\n' % (a, nm))
open(B + '/rows.txt', 'w').write('\n'.join(rows) + '\n')
print(b, len(rows))
