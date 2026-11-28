#!/usr/bin/env python3
"""vtchk.py <obj> [--answers NAME_ANSWERS.tsv]: read-only vftable + vftable-reached copy check (AUTH30).
For every ??_7 COMDAT the object defines: slot k relocation symbol -> retail address set (NAME_ANSWERS use_this_spelling /
same_spelling_at, reloc_bindings.tsv, self_symbols.tsv) v the retail vftable dword k (class from the mangled ??_7 name, VA from
rebuild/manifest/rtti-vtables.tsv; slot count = run of .text pointers). For each slot symbol the object DEFINES (a code COMDAT),
compares its bytes with retail at the retail slot address (relocation fields masked both sides) and each relocation target.
Prints one line per slot and a summary per vftable. Reads only."""
import sys, os, struct, collections
R = '/home/user/FableDecomp'
S = os.environ['S']
EXE = S + '/dl/game/Fable The Lost Chapters/Fable.exe'
ex = open(EXE, 'rb').read()
pe = struct.unpack_from('<I', ex, 0x3c)[0]; ns_ = struct.unpack_from('<H', ex, pe + 6)[0]; op = struct.unpack_from('<H', ex, pe + 20)[0]
XS = [struct.unpack_from('<IIII', ex, pe + 24 + op + 40 * i + 8) for i in range(ns_)]
def rd(va, n):
    r = va - 0x400000
    for vs, sva, rs, ro in XS:
        if sva <= r < sva + max(vs, rs): return ex[ro + r - sva:ro + r - sva + n]
    return None
def dw(va): return struct.unpack('<I', rd(va, 4))[0]
def coff(p):
    d = open(p, 'rb').read(); _, nsec, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0); strt = symp + nsym * 18
    def nm(o):
        r = d[o:o + 8]
        if r[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', r, 4)[0]; e = d.index(b'\0', strt + off); return d[strt + off:e].decode('latin1')
        return r.rstrip(b'\0').decode('latin1')
    secs = []
    for k in range(nsec):
        s = struct.unpack_from('<8sIIIIIIHHI', d, 20 + opt + k * 40)
        secs.append(dict(name=s[0].rstrip(b'\0').decode('latin1'), size=s[3], ptr=s[4], rel=s[5], nrel=s[7], flags=s[9]))
    syms = []; idx = {}; i = 0
    while i < nsym:
        o = symp + i * 18; val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', d, o + 8); n = nm(o)
        syms.append((n, val, sec, typ, cls)); idx[i] = n; i += 1 + aux
    def raw(si): s = secs[si - 1]; return bytearray(d[s['ptr']:s['ptr'] + s['size']])
    def rel(si):
        s = secs[si - 1]; out = []
        for k in range(s['nrel']):
            off, sx, ty = struct.unpack_from('<IIH', d, s['rel'] + k * 10); out.append((off, idx.get(sx, '?'), ty))
        return sorted(out)
    return syms, secs, raw, rel
s2a = collections.defaultdict(set)
ans = sys.argv[sys.argv.index('--answers') + 1] if '--answers' in sys.argv else S + '/codex30/NAME_ANSWERS.tsv'
for l in open(ans, encoding='utf-8', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) > 2 and f[2]:
        s2a[f[2]].add(int(f[0], 16))
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    try: s2a[f[0]].add(int(f[1], 16))
    except (IndexError, ValueError): pass
for l in open(R + '/rebuild/oracles/self_symbols.tsv', errors='replace').read().splitlines()[1:]:
    f = l.split('\t')
    try:
        if f[1]: s2a[f[1]].add(int(f[0], 16))
    except (IndexError, ValueError): pass
for _a in sys.argv:
    if _a.startswith('--self='):
        _s, _v = _a[7:].rsplit('=', 1); s2a[_s].add(int(_v, 16))
VAOVR = {}
for _a in sys.argv:
    if _a.startswith('--va='):
        _s, _v = _a[5:].rsplit('=', 1); VAOVR[_s] = int(_v, 16)
# .rdata scan helper: every dword location whose value is in a set (to place RTTI-less vftables by a known slot target)
def rdata_hits(vals, lo=0x01200000, hi=0x01400000):
    out = []
    for vs, sva, rs, ro in XS:
        base = 0x400000 + sva
        if base + rs < lo or base > hi: continue
        blob = ex[ro:ro + rs]
        for o in range(0, len(blob) - 3, 4):
            if struct.unpack_from('<I', blob, o)[0] in vals: out.append(base + o)
    return out
VT = collections.defaultdict(set)
for l in open(R + '/rebuild/manifest/rtti-vtables.tsv').read().splitlines()[1:]:
    f = l.split('\t'); VT[f[0]].add(int(f[1], 16))
def cls_of(m):
    # ??_7<names>@@6B<...>  -> C++ class with namespaces (reverse of @ parts)
    body = m[4:m.index('@@6')]
    parts = body.split('@')
    return '::'.join(reversed(parts))
VTVAS = set()
for _s, _as in list(s2a.items()):
    if _s.startswith('??_7'): VTVAS |= {x for x in _as if x >= 0x01200000}
for _c, _vs in VT.items(): VTVAS |= _vs
VTVAS = sorted(VTVAS)
import bisect
def retlen(va):
    # run of .text pointers, cut at the next known vftable start (RTTI-less NScript tables sit back to back with no COL)
    i = bisect.bisect_right(VTVAS, va); lim = (VTVAS[i] - va) // 4 if i < len(VTVAS) else 400
    n = 0
    while n < min(400, lim):
        x = dw(va + 4 * n)
        if not (0x401000 <= x < 0x01200000): break
        n += 1
    return n
obj = sys.argv[1]
syms, secs, raw, rel = coff(obj)
defd = {n: (v, s) for n, v, s, t, c in syms if s > 0}
tot = collections.Counter()
for n, v, s, t, c in syms:
    if not n.startswith('??_7') or s <= 0: continue
    C = cls_of(n)
    rs = [(o, sy) for o, sy, ty in rel(s) if o >= v]
    slots = [sy for o, sy in rs]
    vas = sorted(a for a in s2a.get(n, ()) if a >= 0x01200000)     # the ??_7 symbol's own bound / answered VA first
    src = 'bound'
    if n in VAOVR: vas = [VAOVR[n]]; src = 'given'
    if not vas: vas = sorted(VT.get(C, ())); src = 'rtti-class'
    if not vas:
        # place it by a slot whose target is known: every .rdata location holding that target, minus 4*slot
        cand = collections.Counter()
        for k, sy in enumerate(slots):
            sy2 = ('??_G' + sy[4:]) if sy.startswith('??_E') else sy
            A = s2a.get(sy2, set())
            own = n[4:n.index('@@6')] + '@@'
            if A and len(A) <= 3 and own in sy2:      # only slots the class itself defines place its table
                for h in rdata_hits(A): cand[h - 4 * k] += 1
        if cand:
            best_n = max(cand.values()); vas = sorted(v for v, c in cand.items() if c == best_n); src = 'placed(%d slots)' % best_n
    print('VFTABLE', n, 'class', C, 'obj_slots', len(slots), 'retail_vtables', [hex(x) for x in vas], src)
    if not vas: tot['NO_RETAIL(unchecked)'] += 1; continue
    best = None
    for va in vas:
        nr = retlen(va); ret = [dw(va + 4 * k) for k in range(nr)]
        ag = sum(1 for k, sy in enumerate(slots) if k < nr and ret[k] in s2a.get(('??_G' + sy[4:]) if sy.startswith('??_E') else sy, ()))
        if best is None or ag > best[0]: best = (ag, va, nr, ret)
    ag, va, nr, ret = best
    print('  retail', hex(va), 'slots', nr, 'agree', ag, '/', len(slots), 'COUNT', 'OK' if nr == len(slots) else 'DIFF')
    for k, sy in enumerate(slots):
        if sy.startswith('??_E'): sy = '??_G' + sy[4:]   # VC7.1 weak external ??_E -> ??_G (GOTCHAS nf425)
        r = ret[k] if k < nr else None
        a = s2a.get(sy, set())
        st = 'agree' if r in a else ('unknown(unchecked)' if not a else 'WRONG')
        body = ''
        if sy in defd and r is not None:
            vv, ss = defd[sy]
            if secs[ss - 1]['flags'] & 0x20:
                b = raw(ss); rr = bytearray(rd(r, len(b))); bad = []
                for o, rsy, ty in rel(ss):
                    x = struct.unpack_from('<I', rr, o)[0]
                    tg = (r + o + 4 + struct.unpack('<i', struct.pack('<I', x))[0]) & 0xffffffff if ty == 0x14 else x
                    if tg not in s2a.get(rsy, set()): bad.append('%s@+%x->%x' % (rsy[:40], o, tg))
                    b[o:o + 4] = b'\0' * 4; rr[o:o + 4] = b'\0' * 4
                body = ' body%d %s relocs %s' % (len(b), 'EQ' if b == rr else 'NEQ', 'EQ' if not bad else bad)
        print('   %2d %-8s %s ret=%s%s' % (k, st, sy[:90], hex(r) if r else None, body))
        tot[st] += 1
        if 'NEQ' in body: tot['body_NEQ'] += 1
print('SUMMARY', dict(tot))
