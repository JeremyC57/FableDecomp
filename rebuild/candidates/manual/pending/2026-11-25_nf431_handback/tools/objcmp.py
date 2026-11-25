#!/usr/bin/env python3
"""objcmp.py <W> <tag> <addr...>: object section compare, HEAD (work/wd_<tag>h) v new (work/wd_<tag>n) object of each row.
Per row: the self function's section (the one defining the graded self symbol): bytes equal, and relocations equal by offset/type with the
symbol pairs listed (old -> new spelling); every other non-debug section: count and bytes; defined external sets (minus the self
rename) and undefined sets diffed; any new ??_7 / ??_R symbol. Output: one TSV line per row -> stdout."""
import struct, sys, glob, os
W, tag = sys.argv[1:3]
def coff(p):
    d = open(p, 'rb').read()
    mach, ns, ts, symp, nsym, ohs, ch = struct.unpack_from('<HHIIIHH', d, 0)
    strt = symp + nsym * 18
    def nm(raw):
        if raw[:4] == b'\0\0\0\0':
            o = struct.unpack_from('<I', raw, 4)[0]; e = d.index(b'\0', strt + o); return d[strt + o:e].decode('latin-1')
        return raw.rstrip(b'\0').decode('latin-1')
    syms = {}; i = 0; sl = []
    while i < nsym:
        raw = d[symp + i * 18: symp + i * 18 + 18]
        n = nm(raw[:8]); val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', raw, 8)
        syms[i] = (n, sec, cls, typ); sl.append((n, sec, cls, typ, val)); i += 1 + aux
    secs = []
    for s in range(ns):
        o = 20 + ohs + s * 40
        name = d[o:o + 8].rstrip(b'\0').decode('latin-1'); size, ptr, prel, _, nrel = struct.unpack_from('<IIIIH', d, o + 16)
        ch_ = struct.unpack_from('<I', d, o + 36)[0]
        raw = d[ptr:ptr + size] if ptr else b''
        rel = [struct.unpack_from('<IIH', d, prel + k * 10) for k in range(nrel)]
        secs.append((name, raw, [(r[0], syms[r[1]][0], r[2]) for r in rel], ch_))
    defined = {n for n, sec, cls, typ, v in sl if sec > 0 and cls == 2}
    undef = {n for n, sec, cls, typ, v in sl if sec == 0 and cls == 2}
    fsec = {n: sec for n, sec, cls, typ, v in sl if sec > 0 and cls == 2}
    return secs, defined, undef, fsec
def selfsym(log, a):
    return None
for a in sys.argv[3:]:
    ph = glob.glob(f'{W}/work/wd_{tag}h/*/*/{a}/{a}.obj'); pn = glob.glob(f'{W}/work/wd_{tag}n/*/*/{a}/{a}.obj')
    if not ph or not pn: print(f'{a}\tNO_OBJ'); continue
    sh, dh, uh, fh = coff(ph[0]); sn, dn, un, fn = coff(pn[0])
    src = open(glob.glob(f'{W}/work/wd_{tag}n/*/*/{a}/{a}.cpp')[0]).read()
    import re
    new = re.search(r'^// Leaf: (\S+)', src, re.M).group(1)
    # HEAD self: the defined symbol whose section's bytes equal the new self's section bytes
    nsec = sn[fn[new] - 1] if new in fn else None
    cands = [s for s in dh if sh[fh[s] - 1][1] == (nsec[1] if nsec else None)]
    old = cands[0] if len(cands) == 1 else (sorted(cands, key=len)[0] if cands else '?')
    notes = []
    if nsec is None: notes.append('new self not defined')
    elif old == '?': notes.append('no HEAD section with equal bytes')
    else:
        osec = sh[fh[old] - 1]
        if [(o, t) for o, s, t in osec[2]] != [(o, t) for o, s, t in nsec[2]]: notes.append('self relocation offsets/types differ')
        pairs = sorted({(s1, s2) for (o1, s1, t1), (o2, s2, t2) in zip(osec[2], nsec[2]) if s1 != s2})
        if pairs: notes.append('callee respell: ' + '; '.join(f'{x} -> {y}' for x, y in pairs))
    dd = sorted((dn - {new}) - (dh - {old})); dm = sorted((dh - {old}) - (dn - {new}))
    if dd: notes.append('defined +' + ','.join(dd))
    if dm: notes.append('defined -' + ','.join(dm))
    vt = [x for x in dd if x.startswith(('??_7', '??_R'))]
    if vt: notes.append('NEW_VFTABLE ' + ','.join(vt))
    textn = sum(1 for s in sn if s[0].startswith('.text')); texth = sum(1 for s in sh if s[0].startswith('.text'))
    if textn != texth: notes.append(f'.text sections {texth} -> {textn}')
    verdict = 'SELF_EQ' if nsec is not None and old != '?' and 'self relocation offsets/types differ' not in notes else 'CHECK'
    print(f'{a}\t{verdict}\t{old}\t{new}\t' + ' | '.join(notes))
