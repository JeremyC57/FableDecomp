#!/usr/bin/env python3
"""blk.py <c2prio.json> <lo> <hi> [--pass N] [--all]
For the blocks whose source lines intersect [lo, hi] (lines relative to the function's first line as c2prio stores them;
pass --abs <defline> to give absolute file lines), print per priority pass the block's w, K, the referenced candidates
(uid, root name, cost) and the live-through ones, plus every candidate's choose/chosen outcome (register, costs).
Candidates are tracked by object (ids are reused by c2's free list): 'new' events create objects, 'parent' links pieces."""
import json, sys, collections
args = sys.argv[1:]
J = json.load(open(args[0])); lo, hi = int(args[1]), int(args[2])
absl = int(args[args.index('--abs') + 1]) if '--abs' in args else None
if absl is not None: lo -= absl; hi -= absl
fn = J['functions'][0]; ev = fn['events']
REG = {0: '-', 1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
objs = []          # uid -> dict
cur = {}           # id -> uid
names = {}
def nm(leaf):
    k = leaf.get('kind')
    if leaf.get('name'): return leaf['name']
    if k == 13:
        n = leaf.get('node') or []
        return 'const %s' % (leaf.get('value', '?'))
    return {3: 'temp', 4: 'local', 5: 'param', 7: 'global'}.get(k, 'k%s' % k)
phase = 0
passes = []        # list of (phase, [blk events])
cb = None
chosen = {}
for i, e in enumerate(ev):
    k = e['e']
    if k == 'new':
        par = cur.get(e['parent']) if e.get('parent') is not None else None
        o = {'uid': len(objs), 'id': e['id'], 'parent': par, 'leaf': e.get('leaf', {}), 'born': i, 'reg': None, 'costs': None}
        o['name'] = nm(o['leaf']) if par is None else objs[par]['name']
        o['root'] = o['uid'] if par is None else objs[par]['root']
        objs.append(o); cur[e['id']] = o['uid']
    elif k in ('driver', 'resort', 'split'):
        if k != 'split': passes.append((i, k, []));
    elif k == 'blk':
        cb = {'blk': e['blk'], 'depth': e['depth'], 'lines': e['lines']}
    elif k == 'blkend':
        cb.update(w=e['w'], wK=e['wK'], refl=[(cur.get(r[0]), r[2]) for r in e['refl']], live=[(cur.get(r[0]), r[1], r[2]) for r in e['live']])
        if passes: passes[-1][2].append(cb)
    elif k == 'chosen':
        u = cur.get(e['id'])
        if u is not None: objs[u]['reg'] = REG.get(e['reg'], e['reg']); objs[u]['costs'] = {REG.get(r, r): c for r, c in enumerate(e['costs']) if c}
    elif k == 'choose':
        u = cur.get(e['id'])
        if u is not None: objs[u]['prio'] = e['prio']; objs[u]['allowed'] = e['allowed']
# const values from the c2prio trace table are not in the leaf; label consts by id of the root
def label(u):
    if u is None: return '?'
    o = objs[u]; r = objs[o['root']]
    s = '%s#%d' % (r['name'], r['id'])
    if o['parent'] is not None: s += '/piece u%d' % u
    return s
want = [p for p in range(len(passes))]
if '--pass' in args: want = [int(args[args.index('--pass') + 1])]
for pi in want:
    i, kind, blks = passes[pi]
    sel = [b for b in blks if any(lo <= l <= hi for l in b['lines'])]
    if not sel: continue
    print('=== pass %d (%s @ev %d): %d blocks' % (pi, kind, i, len(sel)))
    for b in sel:
        print('  blk %x depth %d w %d K %d lines %s' % (b['blk'], b['depth'], b['w'], b['wK'] // max(b['w'], 1), b['lines'][:12]))
        for u, c in b['refl']:
            o = objs[u] if u is not None else {}
            print('     ref  u%-5s %-34s cost %2d  -> %s prio %s %s' % (u, label(u), c, o.get('reg'), o.get('prio'), o.get('costs')))
        if '--all' in args:
            for u, fl, c in b['live']:
                o = objs[u] if u is not None else {}
                print('     live u%-5s %-34s fl %x cost %2d -> %s prio %s' % (u, label(u), fl, c, o.get('reg'), o.get('prio')))
