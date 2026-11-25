#!/usr/bin/env python3
"""mkbatch428.py <gen dir> <prefix> <per batch>: cut PLAN.tsv into batches/<prefix>N: rows/, tests/, orig/rows/<a>.cpp + orig/tests/<a>_test.cpp
(HEAD copies by address), rows.txt, PLAN.tsv. Rows must be locked NF428."""
import sys, os, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
G, pre, k = sys.argv[1], sys.argv[2], int(sys.argv[3])
P = [l.rstrip('\n').split('\t') for l in open(G + '/PLAN.tsv') if l.strip()]
for p in P: assert nf.lock_owner(p[0]) == 'NF428', p[0]
for j in range(0, len(P), k):
    b = '%s%d' % (pre, j // k + 1); B = str(nf.A) + '/batches/' + b
    for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'head', 'evidence', 'obj'): os.makedirs(B + '/' + d, exist_ok=True)
    ch = P[j:j + k]
    for p in ch:
        a = p[0]
        shutil.copy(nf.src_of(a), B + '/orig/rows/%s.cpp' % a); t = nf.test_of(a)
        if t and t.exists(): shutil.copy(t, B + '/orig/tests/%s_test.cpp' % a)
        shutil.copy(G + '/rows/%s.cpp' % a, B + '/rows/%s.cpp' % a); shutil.copy(G + '/tests/%s_test.cpp' % a, B + '/tests/%s_test.cpp' % a)
    open(B + '/rows.txt', 'w').write(''.join(p[0] + '\n' for p in ch))
    open(B + '/PLAN.tsv', 'w').write(''.join('\t'.join(p) + '\n' for p in ch))
    print(b, len(ch))
