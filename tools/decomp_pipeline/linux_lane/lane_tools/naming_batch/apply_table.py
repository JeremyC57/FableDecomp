#!/usr/bin/env python3
"""Coordinator: apply a hand-back's TABLE_CHANGES.tsv to rebuild/oracles/reloc_bindings.tsv.
   apply_table.py <TABLE_CHANGES.tsv> [--dry]
Columns: action symbol target witness note. Actions: delete (symbol,target), add, replace (`old -> new` in the symbol column; the line
keeps its target and takes the given witness), rewitness. Every miss is reported and nothing is written if any line misses.
Reports symbols with two or more targets before and after (a new one is a RELOC_CONFLICT to hold)."""
import os, re, sys
from collections import defaultdict
R = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp'); p = R + '/rebuild/oracles/reloc_bindings.tsv'
t = open(p).read(); assert t.endswith('\n')
L = t.split('\n')[:-1]; hdr = L[0]; rows = [l.split('\t') for l in L[1:]]
def multi(rs):
    m = defaultdict(set)
    for r in rs: m[r[0]].add(r[1])
    return {k for k, v in m.items() if len(v) > 1}
before = multi(rows)
idx = defaultdict(list)
for i, r in enumerate(rows): idx[(r[0], r[1])].append(i)
c = defaultdict(int); miss = []; dele = set()
for l in open(sys.argv[1]).read().splitlines()[1:]:
    if not l.strip(): continue
    f = l.split('\t'); act, sym, tg = f[0], f[1], f[2].lower().replace('0x', ''); wit = f[3].lower().replace('0x', '') if len(f) > 3 else ''
    if '->' in wit: wit = wit.split('->')[-1].strip()   # a rewitness written `old -> new` (landing nf312 wrote it into the table once)
    if wit and not re.fullmatch(r'[0-9a-f]{8}', wit): miss.append('bad witness ' + l); continue
    if act == 'delete':
        if (sym, tg) in idx: dele.update(idx[(sym, tg)]); c['delete'] += 1
        else: miss.append(l)
    elif act == 'add':
        if (sym, tg) in idx: miss.append('dup ' + l); continue
        rows.append([sym, tg, wit]); idx[(sym, tg)].append(len(rows) - 1); c['add'] += 1
    elif act == 'replace':
        old, new = [s.strip() for s in sym.split(' -> ')]
        if (old, tg) not in idx: miss.append(l); continue
        for i in idx[(old, tg)]: rows[i] = [new, tg, wit or rows[i][2]]
        c['replace'] += 1
    elif act == 'rewitness':
        if (sym, tg) not in idx: miss.append(l); continue
        for i in idx[(sym, tg)]: rows[i][2] = wit
        c['rewitness'] += 1
    else: miss.append('unknown action ' + l)
out = [r for i, r in enumerate(rows) if i not in dele]
after = multi(out)
print(dict(c), 'misses', len(miss)); [print('  MISS', m[:160]) for m in miss]
print('multi-target before', len(before), 'after', len(after), 'new', sorted(after - before)[:10])
if miss or '--dry' in sys.argv: sys.exit(1 if miss else 0)
open(p, 'w').write('\n'.join([hdr] + ['\t'.join(r) for r in out]) + '\n')
