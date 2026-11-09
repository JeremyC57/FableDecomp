#!/usr/bin/env python3
"""bd.py <c2prio8.json with bdump> <K> <id>[,id..]: blocks where candidate id is live (+0x50) at the dump before choice K,
the registers used there (+0x54), and for each used callee-saved register the coloured candidates live in that block
holding it (else 'fixed' = a pre-coloured / non-candidate use)."""
import sys, json, struct
js, KK, IDS = sys.argv[1], int(sys.argv[2]), [int(x) for x in sys.argv[3].split(',')]
BASE = 233
RN = {1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
t = json.load(open(js) if not js.endswith('.gz') else __import__('gzip').open(js, 'rt'))
fn = [f for f in t['functions'] if 'RunCutsceneMacro' in f['name']][0]
leaf = {}; reg = {}; K = 0; pend = None; dump = None; desc = {}
def nm(cid):
    lf = leaf.get(cid) or {}
    if lf.get('kind') == 13:
        v = struct.unpack('<i', struct.pack('<I', lf['node'][6]))[0]; return 'c%s' % (v if -100 < v < 100 else hex(v & 0xffffffff))
    return (lf.get('name') or {3: 'temp', 4: 'local', 5: 'param', 7: 'global'}.get(lf.get('kind'), '?')).lstrip('_')
kof = {}
for e in fn['events']:
    k = e['e']
    if k == 'new': leaf[e['id']] = e.get('leaf'); reg.pop(e['id'], None)
    elif k == 'chosen':
        K += 1; reg[e['id']] = e['reg']; kof[e['id']] = K
    elif k == 'bdump' and e['K'] == KK: dump = e; break
regs_now = dict(reg)
for cid in IDS:
    tot = set(); rows = []
    for b, lines, live, used, _ in dump['blocks']:
        if cid not in live: continue
        tot |= set(used)
        lab = []
        for r in used:
            if r not in (4, 7, 8): continue
            holders = [h for h in live if regs_now.get(h) == r]
            lab.append('%s:%s' % (RN[r], ','.join('#%d %s K%d' % (h, nm(h), kof[h]) for h in holders) or 'FIXED'))
        ls = '%s..%s' % (BASE - 1 + min(lines), BASE - 1 + max(lines)) if lines else '-'
        rows.append((ls, lab))
    print('#%d %s: %d blocks, used union %s' % (cid, nm(cid), len(rows), sorted(str(RN.get(r, r)) for r in tot)))
    for ls, lab in rows:
        if lab: print('   %-12s %s' % (ls, ' | '.join(lab)))
