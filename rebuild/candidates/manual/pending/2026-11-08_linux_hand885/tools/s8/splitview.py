#!/usr/bin/env python3
"""splitview.py <json> <K> [value...]: for the split after choice K (rdump at the re-sort), list the pieces created from
const -1/0/1 parents: piece id, parent, #blocks, first/last block index and lines, used-register union (callee-saved), and
gaps (runs of the parent's blocks that went to no piece)."""
import sys, json, struct
js, KK = sys.argv[1], int(sys.argv[2])
t = json.load(open(js)); fn = t['functions'][0]
leaf = {}; par = {}; K = 0; newin = []; dumpb = None; dumpa = None; lastb = None
RN = {4: 'b', 7: 's', 8: 'i'}
for e in fn['events']:
    k = e['e']
    if k == 'new':
        leaf[e['id']] = e.get('leaf'); par[e['id']] = e.get('parent')
        if K == KK: newin.append((e['id'], e.get('parent'), e.get('leaf')))
    elif k == 'chosen':
        K += 1
        if K > KK and dumpa is not None: break
    elif k == 'rdump' and e['K'] == KK: dumpa = e['blocks']
    elif k in ('rdump', 'bdump') and e['K'] < KK: lastb = e
def cv(lf):
    lf = lf or {}
    if lf.get('kind') == 13:
        return struct.unpack('<i', struct.pack('<I', lf['node'][6]))[0] if lf.get('node') else None
    return lf.get('name')
B = dumpa
for pid, pp, lf in newin:
    v = cv(lf)
    if v not in (-1, 0, 1): continue
    idx = [i for i, b in enumerate(B) if pid in b[2]]
    if not idx: print('piece #%d of #%s (c%s): no live blocks' % (pid, pp, v)); continue
    used = set()
    for i in idx: used |= set(r for r in B[i][3] if r in RN)
    ln = lambda i: ('%d..%d' % (B[i][1][0] + 232, B[i][1][-1] + 232)) if B[i][1] else '-'
    print('piece #%-5d of #%-5s c%-2s blocks %4d  idx %4d (%s) .. %4d (%s)  used %s' % (pid, pp, v, len(idx), idx[0], ln(idx[0]), idx[-1], ln(idx[-1]), ''.join(sorted(RN[r] for r in used))))
