#!/usr/bin/env python3
"""jev.py <c2prio7.json> <base> [--upto K] [--names a,b] [--all]: list chosen events (decision index K, id, candidate,
prio, allowed, register, first/last block lines) -- by default only constants and named watch list."""
import sys, json, struct
js, base = sys.argv[1], int(sys.argv[2])
UPTO = int(sys.argv[sys.argv.index('--upto') + 1]) if '--upto' in sys.argv else 10**9
NAMES = sys.argv[sys.argv.index('--names') + 1].split(',') if '--names' in sys.argv else []
ALL = '--all' in sys.argv
RN = {1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
LET = {1: 'a', 2: 'c', 3: 'd', 7: 's', 8: 'i', 4: 'b', 6: 'p'}
t = json.load(open(js))
fn = [f for f in t['functions'] if 'RunCutsceneMacro' in f['name']][0]
cur = {}
class C: pass
def desc(c):
    lf = c.leaf or {}
    k = lf.get('kind'); nm = lf.get('name')
    if k == 13:
        node = lf.get('node') or []
        v = struct.unpack('<i', struct.pack('<I', node[6]))[0] if node and len(node) >= 7 else None
        s = 'const %s' % (v if v is not None and -10 < v < 10 else (hex(v & 0xffffffff) if v is not None else '?'))
    elif nm: s = nm[1:] if nm.startswith('_') else nm
    else: s = {3: 'temp', 4: 'local', 5: 'param', 7: 'global'}.get(k, 'k%s' % k)
    return s if c.root is c else s + '<' + str(c.root.id)
K = 0; pend = {}; nsplit = 0
for e in fn['events']:
    k = e['e']
    if k == 'new':
        c = C(); c.id = e['id']; c.leaf = e.get('leaf'); c.refs = set()
        par = cur.get(e['parent']) if e.get('parent') is not None else None
        c.root = par.root if par else c
        cur[e['id']] = c
    elif k == 'ref':
        if e['id'] in cur and e['line']: cur[e['id']].refs.add(base - 1 + e['line'])
    elif k == 'choose': pend[e['id']] = e
    elif k == 'split': nsplit += 1
    elif k == 'chosen':
        K += 1
        if K > UPTO: break
        c = cur.get(e['id']); ch = pend.get(e['id'], {})
        if c is None: continue
        d = desc(c)
        if not (ALL or d.startswith('const') or any(n == d.split('<')[0] for n in NAMES)): continue
        l1 = ch.get('l1') or []; l2 = ch.get('l2') or []
        ext = '%s..%s' % (base - 1 + min(l1) if l1 else '?', base - 1 + max(l2) if l2 else '?')
        al = ''.join(LET[r] if r in (ch.get('allowed') or []) else '.' for r in (1, 2, 3, 7, 8, 4, 6))
        costs = {RN.get(i, i): v for i, v in enumerate(e['costs']) if v}
        print(f'K{K:<5d} sp{nsplit:<4d} #{c.id:<5d} {d[:28]:28s} p{ch.get("prio", 0):<6d} {al} -> {RN.get(e["reg"], e["reg"])} ext {ext:12s} {costs}')
