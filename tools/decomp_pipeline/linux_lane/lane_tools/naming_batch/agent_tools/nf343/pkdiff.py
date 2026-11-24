#!/usr/bin/env python3
"""pkdiff.py <evidence/pk_X.txt> <evidence/pk_Y.txt>: per copy symbol, HEAD v new pk_all verdict / size / address source / leafref / reach,
flagging the NF340 gate: NEW leaf-called NEQ (a/a2/o source or leaf=y/reach=y) or EQ->NEQ = HOLD; already-NEQ = debt; NEQ->EQ = fixed."""
import sys, re
def load(p):
    d = {}
    for l in open(p):
        if not l.startswith('PK '): continue
        f = l.split(None, 7)
        sym = f[7].rsplit(' ', 1)[-1].strip() if False else l.strip().split()[-1]
        d[sym] = dict(verdict=f[1], addr=f[3], src=f[4], size=f[6], leaf='leaf=y' in l, reach='reach=y' in l)
    return d
H, N = load(sys.argv[1]), load(sys.argv[2])
for s in sorted(set(H) | set(N)):
    h, n = H.get(s), N.get(s)
    hv = h['verdict'] if h else 'ABSENT'; nv = n['verdict'] if n else 'ABSENT'
    lc = n and (n['leaf'] or n['reach'] or any(x in n['src'].split(',') for x in ('a', 'a2', 'o')))
    same = h and n and hv == nv and h['size'] == n['size'] and h['src'] == n['src'] and h['leaf'] == n['leaf'] and h['reach'] == n['reach']
    if same: continue
    hlc = h and (h['leaf'] or h['reach'] or any(x in h['src'].split(',') for x in ('a', 'a2', 'o')))
    tag = ''
    if n and nv.startswith('NEQ') and lc and (not h or not hv.startswith('NEQ')): tag = 'HOLD(new/turned leaf-called NEQ)'
    elif n and nv.startswith('NEQ') and lc and h and hv.startswith('NEQ') and not hlc: tag = 'HOLD(NEQ newly leaf-called)'
    elif n and nv.startswith('NEQ') and h and hv.startswith('NEQ'): tag = 'debt(already NEQ)'
    elif h and hv.startswith('NEQ') and nv.startswith('EQ'): tag = 'FIXED'
    print('%-8s %-8s size %s->%s addr %s src %s leaf/reach %s %s  %s' % (hv, nv, (h or {}).get('size', '-'), (n or {}).get('size', '-'),
          (n or h)['addr'], (n or h)['src'], 'y' if n and n['leaf'] else 'n', 'y' if n and n['reach'] else 'n', tag), s[:110])
