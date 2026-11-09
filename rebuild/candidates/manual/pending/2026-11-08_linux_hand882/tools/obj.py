#!/usr/bin/env python3
"""obj.py <c2prio.json> <lo> <hi> [--abs DEFLINE] [--root NAME]
Object-level history of every candidate (root or piece) referenced in a block whose lines intersect [lo, hi]:
creation (parent), each choose (prio, allowed), chosen (register, costs), lowspill skip, drop, forward, split membership,
and the blocks it is referenced in (per pass, w*K and cost). Ids are reused by c2: objects are tracked by creation order."""
import json, sys, collections
a = sys.argv[1:]
J = json.load(open(a[0])); lo, hi = int(a[1]), int(a[2])
off = int(a[a.index('--abs') + 1]) if '--abs' in a else 0
lo -= off; hi -= off
ev = J['functions'][0]['events']
REG = {0: '-', 1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
AL = {1: 'a', 2: 'c', 3: 'd', 7: 's', 8: 'i', 4: 'b', 6: 'p'}
objs = []; cur = {}
hist = collections.defaultdict(list)
refs = collections.defaultdict(list)
npass = 0; cb = None
def name(u):
    o = objs[u]; r = objs[o['root']]
    lf = r['leaf']; k = lf.get('kind')
    n = lf.get('name') or ({13: 'const', 3: 'temp', 4: 'local', 5: 'param', 7: 'global'}.get(k, 'k%s' % k))
    if k == 13:
        nd = lf.get('node') or [0] * 8
        n = 'const %d' % (nd[6] if nd[7] == 0 else nd[6] - (1 << 32))
    return n + '#%d' % r['id']
for i, e in enumerate(ev):
    k = e['e']
    if k == 'new':
        par = cur.get(e['parent']) if e.get('parent') is not None else None
        u = len(objs)
        objs.append({'id': e['id'], 'parent': par, 'leaf': e.get('leaf', {}), 'root': u if par is None else objs[par]['root']})
        cur[e['id']] = u
        if par is not None: hist[par].append((i, 'split-> u%d' % u))
        continue
    if k in ('driver', 'resort'): npass += 1
    if k == 'blk': cb = e; continue
    if k == 'blkend':
        for r in e['live']:
            if r[1] & 0x10: refs[cur.get(r[0])].append((npass, cb['lines'], e['wK'], r[2]))
        for r in e['refl']:
            refs[cur.get(r[0])].append((npass, cb['lines'], e['wK'], r[2]))
        continue
    if k == 'split':
        for x in e['ids']:
            if x in cur: hist[cur[x]].append((i, 'in split pass'))
        continue
    if 'id' in e and e['id'] in cur:
        u = cur[e['id']]
        if k == 'choose': hist[u].append((i, 'choose prio %d allowed %s' % (e['prio'], ''.join(AL.get(x, '?') for x in e['allowed']))))
        elif k == 'chosen': hist[u].append((i, '-> %s costs %s' % (REG.get(e['reg'], e['reg']), {REG.get(r, r): c for r, c in enumerate(e['costs']) if c})))
        else: hist[u].append((i, '%s %s' % (k, {x: y for x, y in e.items() if x not in ('e', 'id')})))
sel = set()
for u, L in refs.items():
    if u is None: continue
    if any(any(lo <= l <= hi for l in ln) for (_, ln, _, _) in L): sel.add(u)
root = a[a.index('--root') + 1] if '--root' in a else None
for u in sorted(sel):
    if root and root not in name(u): continue
    o = objs[u]
    chain = []; p = o['parent']
    while p is not None: chain.append('u%d' % p); p = objs[p]['parent']
    print('u%d %s id %d  ancestry %s' % (u, name(u), o['id'], '<-'.join(chain) or 'root'))
    rl = sorted(set((ps, min(ln), max(ln), wk, c) for (ps, ln, wk, c) in refs[u]))
    print('    refs (pass, lines, wK, cost):', ' '.join('p%d:%d-%d:%d:%d' % r for r in rl[:14]), '...' if len(rl) > 14 else '')
    for i, h in hist[u]: print('    ev%-6d %s' % (i, h))
