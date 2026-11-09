#!/usr/bin/env python3
"""rorder.py <pk.json> <items.tsv> [N]: our sorted slot order v retail's (our slots ordered by their members' retail
offset), densities from our weights; '*' = position differs, 'SPLIT' = members of our slot at different retail offsets,
'INV' = retail order puts a denser slot after a lighter one (an inequality on retail's weights)."""
import json, sys, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sim
ev = json.load(open(sys.argv[1]))
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[2])][1:]
N = int(sys.argv[3]) if len(sys.argv) > 3 else 60
pre = [e for e in ev if e[0] == 'presort'][0]
SL = sim.slots_from_raw(pre[3]); nparams = pre[2]
v, _, _ = sim.layout(SL, nparams)
mem = collections.defaultdict(list)
for r in rows:
    if r[9] != 'None': mem[int(r[9])].append(r)
def rinfo(s):
    c = collections.Counter(int(r[7]) for r in mem[s] if r[7] != 'None' and r[8].split('/')[0] == r[8].split('/')[1])
    return c
ours = []
for k, s in enumerate(v[nparams:]):
    c = rinfo(s['id']); our = collections.Counter(int(r[6]) for r in mem[s['id']] if r[6] != 'None').most_common(1)
    ret = c.most_common(1)[0][0] if c else None
    ours.append(dict(pos=k, id=s['id'], size=s['size'], w=s['w'], d=sim.dens(s), our=our[0][0] if our else None, ret=ret,
                     split=len(c) > 1, rets=dict(c)))
known = [o for o in ours if o['ret'] is not None]
rs = sorted(known, key=lambda o: (-o['ret'], o['pos']))
if __name__ == '__main__':
    k0 = next((k for k, (a, b) in enumerate(zip(known, rs)) if a['id'] != b['id']), None)
    print('slots', len(ours), 'with retail offset', len(known), 'first differing position', k0)
    for k in range(max(0, (k0 or 0) - 2), min(len(rs), (k0 or 0) + N)):
        a, b = known[k], rs[k]
        f = lambda o: f"{o['id']:4d} {o['size']:3d} {o['w']:4d} {o['d']:6d} {o['our']!s:>6} {o['ret']!s:>6}" + (' SPLIT' + str(o['rets']) if o['split'] else '')
        print(f"{k:4d} | {f(a):34s} | {f(b)}" + ('' if a['id'] == b['id'] else '  *') +
              ('  INV' if k and rs[k - 1]['d'] < b['d'] else ''))
