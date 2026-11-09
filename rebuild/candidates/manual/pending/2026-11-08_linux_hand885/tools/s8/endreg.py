#!/usr/bin/env python3
"""endreg.py <json> K L1 L2: at the bdump before choice K, which const -1/0/1 (and named) candidates are live in blocks whose
lines fall in [L1, L2], with block index, lines and used registers."""
import sys, json, struct
js, KK, L1, L2 = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
BASE = 233
RN = {1: 'a', 2: 'c', 3: 'd', 4: 'b', 6: 'p', 7: 's', 8: 'i'}
t = json.load(open(js)); fn = [f for f in t['functions'] if 'RunCutsceneMacro' in f['name']][0]
leaf = {}; K = 0; dump = None
def nm(cid):
    lf = leaf.get(cid) or {}
    if lf.get('kind') == 13:
        v = struct.unpack('<i', struct.pack('<I', lf['node'][6]))[0]; return 'c%s' % (v if -100 < v < 100 else hex(v & 0xffffffff))
    return (lf.get('name') or {3: 'temp', 4: 'local', 5: 'param', 7: 'global'}.get(lf.get('kind'), '?')).lstrip('_')
for e in fn['events']:
    k = e['e']
    if k == 'new': leaf[e['id']] = e.get('leaf')
    elif k == 'chosen': K += 1
    elif k == 'bdump' and e['K'] == KK: dump = e; break
for i, (b, lines, live, used, _) in enumerate(dump['blocks']):
    if not lines: continue
    lo, hi = BASE - 1 + min(lines), BASE - 1 + max(lines)
    if hi < L1 or lo > L2: continue
    cs = [f'#{c}{nm(c)}' for c in live if nm(c) in ('c-1', 'c0', 'c1')]
    print(i, f'{lo}..{hi}', ''.join(RN.get(r, '?') for r in used), ' '.join(cs))
