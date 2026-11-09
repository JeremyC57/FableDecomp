#!/usr/bin/env python3
"""rslots.py <pk.json> <items.tsv>: our slots (from the packer events) v retail's (items grouped by their retail offset);
retail's slots in offset order with densities computed from OUR item weights; flags density inversions."""
import json, sys, collections
sys.path.insert(0, __import__('os').path.dirname(__file__))
import sim
ev = json.load(open(sys.argv[1]))
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[2])][1:]
by = {int(r[0]): r for r in rows}
i0 = [k for k, e in enumerate(ev) if e[0] == 'list'][0]
slot_of = {}; cur = None
for e in ev[i0 + 1:]:
    if e[0] == 'pipe': break
    if e[0] == 'first': slot_of[e[1]] = e[2]
    elif e[0] == 'place': cur = e[1]
    elif e[0] in ('join2', 'new'): slot_of[cur] = e[1]
pre = [e for e in ev if e[0] == 'presort'][0]
S = sim.slots_from_raw(pre[3]); nparams = pre[2]
v, _, _ = sim.layout(S, nparams)
mem = collections.defaultdict(list)
for idx, s in slot_of.items(): mem[s].append(idx)
# our slot offset = most common our offset of members
def soff(s, col):
    c = collections.Counter(by[i][col] for i in mem[s] if by[i][col] != 'None')
    return int(c.most_common(1)[0][0]) if c else None
ours = [(s['id'], s['size'], s['w'], sim.dens(s), soff(s['id'], 6), soff(s['id'], 7)) for s in v[nparams:]]
if __name__ == '__main__':
    mode = sys.argv[3] if len(sys.argv) > 3 else 'cmp'
    if mode == 'cmp':
        # retail sorted order = our slots ordered by their retail offset (desc); list where it differs from ours
        rs = sorted([o for o in ours if o[5] is not None], key=lambda o: -o[5])
        os_ = [o for o in ours if o[5] is not None]
        print('pos | ours: slot size w dens our_off ret_off | retail-order: slot size w dens our_off ret_off')
        k0 = next((k for k, (a, b) in enumerate(zip(os_, rs)) if a[0] != b[0]), None)
        print('first differing position', k0)
        for k in range(max(0, (k0 or 0) - 3), min(len(rs), (k0 or 0) + int(sys.argv[4] if len(sys.argv) > 4 else 40))):
            a, b = os_[k], rs[k]
            print(f"{k:4d} | {a[0]:4d} {a[1]:3d} {a[2]:4d} {a[3]:6d} {a[4]!s:>6} {a[5]!s:>6} | {b[0]:4d} {b[1]:3d} {b[2]:4d} {b[3]:6d} {b[4]!s:>6} {b[5]!s:>6}"
                  + ('' if a[0] == b[0] else '  *') + ('  INV' if k and rs[k - 1][3] < b[3] else ''))
