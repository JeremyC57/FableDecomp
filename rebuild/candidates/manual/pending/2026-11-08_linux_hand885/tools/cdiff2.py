#!/usr/bin/env python3
"""cdiff2.py <m.obj> <m.asm (/FAs)> <c2prio7.json> <base line> [--n N] [--all] [--from K] [--id ID]:
c2's colouring decisions IN ALLOCATION ORDER, each compared with retail. Root candidates use their reference lines
(DEF/USE events); split pieces use the extent between their first and last referencing blocks (+0x28/+0x2c, recorded
by c2prio7 at choose time). Votes: every aligned instruction on those lines whose ours operand is the chosen register
(or its byte alias) votes retail's operand at that position (register, M = frame memory, I = immediate, ? = unaligned).
Prints K (decision index), id, candidate, prio, ours register, retail votes, verdict."""
import sys, os, re, json
args = sys.argv[1:]
obj, lst, js, base = args[0], args[1], args[2], int(args[3])
NN = int(args[args.index('--n') + 1]) if '--n' in args else 40
FROM = int(args[args.index('--from') + 1]) if '--from' in args else 0
ONLY = int(args[args.index('--id') + 1]) if '--id' in args else None
ALL = '--all' in args
# reuse cdiff.py's alignment (it reads sys.argv[1:3])
sys.argv = [sys.argv[0], obj, lst, '/dev/null']
src = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'cdiff.py')).read()
exec(src[:src.index('T = open(trace)')])
RN = {1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
t = json.load(open(js))
fn = [f for f in t['functions'] if 'RunCutsceneMacro' in f['name']][0]
cur = {}
class C: pass
def desc(c):
    lf = c.leaf or {}
    k = lf.get('kind'); nm = lf.get('name')
    if k == 13:
        node = lf.get('node') or []
        v = None
        if node and len(node) >= 7:
            import struct; v = struct.unpack('<i', struct.pack('<I', node[6]))[0]
        s = 'const %s' % (v if v is not None and -10 < v < 10 else (hex(v & 0xffffffff) if v is not None else '?'))
    elif nm: s = nm[1:] if nm.startswith('_') else nm
    else: s = {3: 'temp', 4: 'local', 5: 'param', 7: 'global'}.get(k, 'k%s' % k)
    return s if c.root is c else s + '<' + str(c.root.id)
B8 = {'eax': 'al', 'ecx': 'cl', 'edx': 'dl', 'ebx': 'bl'}
def votes2(reg, lns, byte):
    v = {}
    want = B8.get(reg) if byte else reg
    if want is None: return v
    for ln in lns:
        for k in byline.get(ln, []):
            oo = ops(O[k])
            for p, x in enumerate(oo):
                if x == want or (not byte and re.search(r'\b%s\b' % reg, x) and x.startswith('[')):
                    if k not in pair: v['?'] = v.get('?', 0) + 1; continue
                    rr = ops(R[pair[k]])
                    c2 = cls(rr[p]) if p < len(rr) else None
                    v[c2 or '?'] = v.get(c2 or '?', 0) + 1
    return v
K = 0; shown = 0; pend = {}
for e in fn['events']:
    k = e['e']
    if k == 'new':
        c = C(); c.id = e['id']; c.leaf = e.get('leaf'); c.refs = set()
        par = cur.get(e['parent']) if e.get('parent') is not None else None
        c.root = par.root if par else c
        cur[e['id']] = c
    elif k == 'ref':
        if e['id'] in cur and e['line']: cur[e['id']].refs.add(base - 1 + e['line'])
    elif k == 'choose':
        pend[e['id']] = e
    elif k == 'chosen':
        K += 1
        c = cur.get(e['id']); ch = pend.get(e['id'], {})
        reg = RN.get(e['reg'])
        if c is None or reg is None or K < FROM: continue
        if ONLY is not None and c.id != ONLY: continue
        if c.root is c and c.refs: lns = c.refs; how = 'refs'
        else:
            l1 = ch.get('l1') or []; l2 = ch.get('l2') or []
            if not l1 or not l2: continue
            lo, hi = base - 1 + min(l1 + l2), base - 1 + max(l1 + l2)
            lns = set(range(lo, hi + 1)); how = '%d..%d' % (lo, hi)
        ty = ((c.root.leaf or {}).get('type') or 0) & 0xfff
        v = votes2(reg, lns, ty == 1)
        if not v: continue
        tot = sum(x for kk, x in v.items() if kk != '?'); agree = v.get(reg, 0)
        if tot == 0: continue
        verdict = '=' if agree * 2 > tot else 'X'
        if verdict == 'X' or ALL or ONLY is not None:
            lr = how if how != 'refs' else ('%d..%d' % (min(lns), max(lns)))
            print(f'{K:5d} #{c.id:<5d} {desc(c)[:26]:26s} p{ch.get("prio", 0):<6d} {reg} {lr:12s} retail {dict(sorted(v.items(), key=lambda x: -x[1]))} {verdict}')
            shown += 1
            if shown >= NN: break
