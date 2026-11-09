#!/usr/bin/env python3
"""slots.py <pk.json> <items.tsv>: reconstruct the packer's slot per item (first-fit join / second-path join / new slot)
and print slots in creation order with our offset and the retail offsets of their members."""
import json, sys, collections
ev = json.load(open(sys.argv[1]))
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[2])][1:]
by = {int(r[0]): r for r in rows}
i0 = [k for k, e in enumerate(ev) if e[0] == 'list'][0]
slot_of = {}; created = []; cur = None
for e in ev[i0 + 1:]:
    if e[0] == 'pipe': break
    if e[0] == 'first': slot_of[e[1]] = e[2]
    elif e[0] == 'place': cur = e[1]
    elif e[0] == 'join2': slot_of[cur] = e[1]
    elif e[0] == 'new': slot_of[cur] = e[1]; created.append((e[1], cur))
mem = collections.defaultdict(list)
for idx, s in slot_of.items(): mem[s].append(idx)
if __name__ == '__main__':
    for s, first in created:
        ms = sorted(mem[s])
        ours = sorted(set(by[i][6] for i in ms)); rets = collections.Counter(by[i][7] for i in ms)
        print(f"slot {s:4d} size {by[first][1]:>3} ours {','.join(ours):>8} retail {dict(rets)} items " +
              ' '.join(f"{i}:{by[i][5]}" for i in ms[:8]) + (' ...' if len(ms) > 8 else ''))
