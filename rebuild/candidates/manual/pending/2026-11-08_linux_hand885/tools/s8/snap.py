#!/usr/bin/env python3
"""snap.py <c2prio8.json> [--ids a,b] [--from K] [--to K]: const (-1/0/1) and snapname pieces: allowed set, prio, spill,
extent (first/last referencing block lines, absolute = base 233) at each csnap (before decision K), with the decision."""
import sys, json, struct
js = sys.argv[1]; args = sys.argv[2:]
IDS = set(int(x) for x in args[args.index('--ids') + 1].split(',')) if '--ids' in args else None
FR = int(args[args.index('--from') + 1]) if '--from' in args else 1
TO = int(args[args.index('--to') + 1]) if '--to' in args else 10**9
BASE = 233
RN = {1: 'a', 2: 'c', 3: 'd', 4: 'b', 6: 'p', 7: 's', 8: 'i'}
t = json.load(__import__('gzip').open(js, 'rt') if js.endswith('.gz') else open(js)); fn = [f for f in t['functions'] if 'RunCutsceneMacro' in f['name']][0]
bl = {}; leaf = {}; par = {}; K = 0; pend = None
def nm(cid):
    lf = leaf.get(cid) or {}
    if lf.get('kind') == 13:
        v = struct.unpack('<i', struct.pack('<I', lf['node'][6]))[0]; return 'c%d' % v
    return (lf.get('name') or '?').lstrip('_')
def ext(b1, b2):
    a = bl.get(b1); b = bl.get(b2)
    return '%s..%s' % (BASE - 1 + min(a) if a else '?', BASE - 1 + max(b) if b else '?')
prev = {}
for e in fn['events']:
    k = e['e']
    if k == 'new': leaf[e['id']] = e.get('leaf'); par[e['id']] = e.get('parent')
    elif k == 'bl': bl[e['blk']] = e['lines']
    elif k == 'choose': pend = e
    elif k == 'csnap':
        K = e['K']
        if not (FR <= K <= TO): continue
        ch = pend or {}
        print('K%d choose #%s prio %s allowed %s ext %s' % (K, ch.get('id'), ch.get('prio'), ''.join(RN[r] for r in ch.get('allowed') or []), ext(ch.get('b1'), ch.get('b2'))))
        for cid, reg, al, prio, spill, b1, b2, f5 in e['before']:
            if reg: continue
            if IDS and cid not in IDS: continue
            a = ''.join(RN.get(r, str(r)) for r in (al or []))
            key = (cid, a)
            mark = '' if prev.get(cid) == a else '  *'
            prev[cid] = a
            if IDS or mark or len(a) <= 1:
                print('   #%-5d %-6s par %-5s al %-4s prio %-6d spill %-5d ext %s f5 %x%s' % (cid, nm(cid), par.get(cid), a or '-', prio, spill, ext(b1, b2), f5, mark))
    elif k == 'chosen' and pend and e['id'] == pend['id'] and FR <= K <= TO:
        print('   -> %s' % RN.get(e['reg']))
