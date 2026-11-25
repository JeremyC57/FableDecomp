#!/usr/bin/env python3
"""units438.py <batch...>: UNITS.tsv per batch: rows that share an old (stand-in) symbol across the given batches are one unit
(union-find), so a table delete never lands without every row it depends on. Prints multi-row and cross-batch units."""
import sys, collections
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF438/batches/'
bs = sys.argv[1:]; by = collections.defaultdict(set); rowb = {}; par = {}
for b in bs:
    for l in open(W + b + '/renames.tsv'):
        a, o, n, t = l.rstrip('\n').split('\t'); by[o].add(a); rowb[a] = b
def f(x):
    while par.get(x, x) != x: x = par[x]
    return x
for o, s in by.items():
    s = sorted(s)
    for x in s[1:]: par[f(x)] = f(s[0])
for b in bs:
    rows = open(W + b + '/rows.txt').read().split()
    open(W + b + '/UNITS.tsv', 'w').write('address\tunit\n' + ''.join('%s\t%s\n' % (a, f(a)) for a in rows))
    print(b, 'multi-row units', sum(1 for c in collections.Counter(f(a) for a in rows).values() if c > 1),
          'cross-batch', sorted({f(a) for a in rows if rowb.get(f(a)) != b}))
