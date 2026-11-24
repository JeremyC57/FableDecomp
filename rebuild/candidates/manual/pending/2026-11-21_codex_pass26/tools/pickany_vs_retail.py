#!/usr/bin/env python3
"""pickany_vs_retail.py <Fable.exe> <reloc_bindings.tsv> <row.obj> <row-address>

Lists every function COMDAT (external symbol in a code section) the row's object defines that reloc_bindings.tsv maps to a retail
address other than the row's own, and compares the copy with Fable.exe at that address over the copy's length. Relocation fields of
the copy are masked on both sides. EQ = byte-identical; NEQ = the copy differs, so the final link could pick it and break the owner
(docs/NAMING_AND_HEADERS.md section 6). For each NEQ it also says whether the row's own leaf (the largest code section, or the one
named by --leaf) relocates to the symbol: a leaf-referenced NEQ holds the row."""
import struct, sys, collections

def coff(path):
    d = open(path, 'rb').read()
    _, nsec, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0)
    strt = symp + nsym * 18
    def nm(o):
        r = d[o:o + 8]
        if r[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', r, 4)[0]; e = d.index(b'\0', strt + off); return d[strt + off:e].decode('latin1')
        return r.rstrip(b'\0').decode('latin1')
    secs = []
    for k in range(nsec):
        s = struct.unpack_from('<8sIIIIIIHHI', d, 20 + opt + k * 40)
        secs.append(dict(size=s[3], ptr=s[4], rel=s[5], nrel=s[7], flags=s[9]))
    syms = []; i = 0
    while i < nsym:
        o = symp + i * 18; val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', d, o + 8)
        syms.append((i, nm(o), val, sec, typ, cls)); i += 1 + aux
    byidx = {s[0]: s[1] for s in syms}
    def body(secno):
        s = secs[secno - 1]; raw = bytearray(d[s['ptr']:s['ptr'] + s['size']]); rel = []
        for k in range(s['nrel']):
            off, si, _ = struct.unpack_from('<IIH', d, s['rel'] + k * 10); raw[off:off + 4] = b'\0\0\0\0'; rel.append((off, byidx.get(si, '?')))
        return raw, rel
    return syms, secs, body

def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--leaf=')]
    leafname = next((a.split('=', 1)[1] for a in sys.argv[1:] if a.startswith('--leaf=')), None)
    exe_p, tab_p, obj_p, row = args; row = row.lower().replace('0x', '')
    exe = open(exe_p, 'rb').read()
    pe = struct.unpack_from('<I', exe, 0x3c)[0]; nsec = struct.unpack_from('<H', exe, pe + 6)[0]; opt = struct.unpack_from('<H', exe, pe + 20)[0]
    base = struct.unpack_from('<I', exe, pe + 24 + 28)[0]; sh = pe + 24 + opt; xs = []
    for k in range(nsec):
        o = sh + 40 * k; vs, va, rs, rp = struct.unpack_from('<IIII', exe, o + 8); xs.append((va, max(vs, rs), rp))
    def rd(va, n):
        rva = va - base
        for s, sz, rp in xs:
            if s <= rva < s + sz: return exe[rp + rva - s:rp + rva - s + n]
        return None
    tab = collections.defaultdict(set)
    for l in open(tab_p, encoding='utf-8', errors='replace').read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) >= 2: tab[f[0]].add(f[1].lower())
    syms, secs, body = coff(obj_p)
    funcs = [s for s in syms if s[3] > 0 and s[5] == 2 and s[4] == 0x20]
    leaf = next((s for s in funcs if s[1] == leafname), None) if leafname else max(funcs, key=lambda s: secs[s[3] - 1]['size'])
    leafrel = {n for _, n in body(leaf[3])[1]} if leaf else set()
    for _, name, val, sec, _, _ in funcs:
        for t in sorted(tab.get(name, set()) - {row}):
            raw, rel = body(sec); r = rd(int(t, 16), len(raw))
            if r is None: print('NOREAD', t, name); continue
            rr = bytearray(r)
            for off, _ in rel: rr[off:off + 4] = b'\0\0\0\0'
            ok = bytes(rr) == bytes(raw)
            print(('EQ ' if ok else 'NEQ'), t, len(raw), name, '' if ok else ('LEAF-REFERENCED' if name in leafrel else 'not referenced by the leaf'))

if __name__ == '__main__':
    main()
