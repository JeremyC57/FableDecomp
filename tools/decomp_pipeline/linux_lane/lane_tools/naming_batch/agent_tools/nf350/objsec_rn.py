#!/usr/bin/env python3
"""objsec_rn.py <head.obj> <new.obj> [--expect=FILE] : objsec HEAD v new UNDER RENAMES (agent NF350, 2026-11-24).
Sections are paired by index (non-debug only). For each pair: name, characteristics, raw bytes and the relocation list (offset, type)
must be equal; the relocation SYMBOLS are paired by position and every (old, new) pair is recorded. The COMDAT selection of each
section symbol (aux record) is compared too. The recorded pairs must form a bijection (one old name -> one new name and back), and the
external DEF / UND sets must be equal under it. Prints the rename map (old -> new, only where they differ), then
ALL_EQ / NOT_EQ <reasons>. --expect=FILE (old<TAB>new lines): every differing pair must be listed there (else NOT_EQ: unexpected rename)."""
import sys, struct, collections

def coff(path):
    d = open(path, 'rb').read()
    ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8); oh = struct.unpack_from('<H', d, 16)[0]
    strt = d[psym + 18 * nsym:]
    def sname(raw):
        if raw[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', raw, 4)[0]; return strt[off:strt.index(b'\0', off)].decode('latin1')
        return raw.rstrip(b'\0').decode('latin1')
    syms = {}; sl = []; secsel = {}; i = 0
    while i < nsym:
        o = psym + 18 * i; raw = d[o:o + 8]; val, sec, typ, scl, naux = struct.unpack_from('<IhHBB', d, o + 8)
        n = sname(raw); syms[i] = n; sl.append((n, val, sec, typ, scl))
        if scl == 3 and naux == 1 and sec > 0 and val == 0:      # section definition symbol: aux = length, nreloc, nline, checksum, number, selection
            ln, nr, nl, ck, num, sel = struct.unpack_from('<IHHIHB', d, o + 18)
            secsel.setdefault(sec, (sel, num))
        i += 1 + naux
    secs = []
    for k in range(ns):
        o = 20 + oh + 40 * k; raw = d[o:o + 8]
        nm = raw.rstrip(b'\0').decode('latin1')
        if nm.startswith('/'): off = int(nm[1:]); nm = strt[off:strt.index(b'\0', off)].decode('latin1')
        vs, va, rs, rp, rr, rl, nr, nl, ch = struct.unpack_from('<IIIIIIHHI', d, o + 8)
        data = d[rp:rp + rs] if rp else b''
        rel = [struct.unpack_from('<IIH', d, rr + 10 * j) for j in range(nr)]
        secs.append(dict(idx=k + 1, name=nm, ch=ch, data=data, rel=[(v, syms[si], t) for v, si, t in rel], sel=secsel.get(k + 1)))
    # the external / static symbols that name each section (COMDAT label)
    lab = collections.defaultdict(list)
    for n, v, sec, typ, scl in sl:
        if sec > 0 and scl in (2, 3) and not n.startswith('.') and not n.startswith('$'): lab[sec].append(n)
    ext = {}
    for n, v, sec, typ, scl in sl:
        if scl == 2: ext[n] = 'DEF' if sec > 0 else 'UND'
    return secs, lab, ext

args = [x for x in sys.argv[1:] if not x.startswith('--')]
opt = {x.split('=', 1)[0]: x.split('=', 1)[1] for x in sys.argv[1:] if x.startswith('--') and '=' in x}
H, hl, he = coff(args[0]); N, nl, ne = coff(args[1])
H = [s for s in H if not s['name'].startswith('.debug')]; N = [s for s in N if not s['name'].startswith('.debug')]
bad = []; pairs = collections.OrderedDict()
def pair(o, n):
    pairs.setdefault((o, n), 0); pairs[(o, n)] += 1
if len(H) != len(N): bad.append('section count %d v %d' % (len(H), len(N)))
for x, y in zip(H, N):
    t = '%s#%d' % (x['name'], x['idx'])
    if x['name'] != y['name']: bad.append('%s name %s' % (t, y['name']))
    if x['ch'] != y['ch']: bad.append('%s characteristics %08x v %08x' % (t, x['ch'], y['ch']))
    if x['data'] != y['data']: bad.append('%s bytes (%d v %d)' % (t, len(x['data']), len(y['data'])))
    if (x['sel'] or (None,))[0] != (y['sel'] or (None,))[0]: bad.append('%s COMDAT selection %s v %s' % (t, x['sel'], y['sel']))
    if [(v, ty) for v, s, ty in x['rel']] != [(v, ty) for v, s, ty in y['rel']]: bad.append('%s relocation offsets/types' % t)
    for (v1, s1, t1), (v2, s2, t2) in zip(x['rel'], y['rel']): pair(s1, s2)
    a, b = sorted(hl.get(x['idx'], [])), sorted(nl.get(y['idx'], []))
    if len(a) != len(b): bad.append('%s label count %s v %s' % (t, a, b))
    for s1, s2 in zip(a, b): pair(s1, s2)
fw = collections.defaultdict(set); bw = collections.defaultdict(set)
for (o, n) in pairs: fw[o].add(n); bw[n].add(o)
for o, ns_ in fw.items():
    if len(ns_) > 1: bad.append('old %s -> several new %s' % (o, sorted(ns_)))
for n, os_ in bw.items():
    if len(os_) > 1: bad.append('new %s <- several old %s' % (n, sorted(os_)))
mp = {o: next(iter(ns_)) for o, ns_ in fw.items()}
# external symbols not seen in any relocation / label (pure UND or DEF without section labels) pair by exact name only
mapped = {mp.get(s, s): k for s, k in he.items()}
if mapped != ne:
    only_h = sorted(set(mapped.items()) - set(ne.items())); only_n = sorted(set(ne.items()) - set(mapped.items()))
    bad.append('external DEF/UND sets differ under the map: head-only %s new-only %s' % (only_h[:6], only_n[:6]))
ren = [(o, n) for o, n in mp.items() if o != n]
if opt.get('--expect'):
    exp = {tuple(l.split('\t')[:2]) for l in open(opt['--expect']).read().splitlines() if '\t' in l}
    for p in ren:
        if p not in exp: bad.append('unexpected rename %s -> %s' % p)
print('# renames (%d):' % len(ren))
for o, n in ren: print('RENAME\t%s\t%s\t%s' % (he.get(o, 'local'), o, n))
print('# sections %d v %d; external symbols %d v %d' % (len(H), len(N), len(he), len(ne)))
print('ALL_EQ' if not bad else 'NOT_EQ ' + ' | '.join(bad[:12]))
