# objcensus.py (NF326): object side of the CScriptThing unit census.
#   syms <symfile> <obj...>    per object and watched symbol: DEF/sel=<n> (COMDAT selection: 1 = no-duplicates, i.e. a strong definition;
#                              2 = pick-any) or DEF/plain (no COMDAT), UND, or '-' (absent); plus whether the object DEFs ??_7CScriptThing@@6B@
#                              and the leaf-call column: 'C' when some OTHER function section of the object relocates to the symbol (a call
#                              site / vtable slot), so the reader can tell an emitted-and-called copy from an emitted-only one.
#   retail <sym> <addr> <obj...>  the COMDAT that defines <sym> in each object v Fable.exe's bytes at <addr> (length = the COMDAT's), with
#                              every relocation field zeroed on both sides; each relocation's retail target (rel32 / dir32 decoded from
#                              retail) printed with the symbol the object names, so a copy that matches bytes but binds another symbol
#                              shows up (EXTRA_RULES 2026-11-20 "relocations aside").
# Output is tab-separated on stdout. No retail bytes are printed (only EQUAL / DIFF and offsets).
import sys, struct
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf

def comdat_info(path):
    d = open(path, 'rb').read()
    ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8)
    sel = {}   # section number -> selection (from the section-definition symbol's aux record)
    i = 0
    while i < nsym:
        o = psym + 18 * i; val, sec, typ, scl, naux = struct.unpack_from('<IhHBB', d, o + 8)
        if scl == 3 and naux == 1 and val == 0 and sec > 0:
            a = psym + 18 * (i + 1)
            s = d[a + 14]
            if sec not in sel: sel[sec] = s
        i += 1 + naux
    return sel

def syms(symfile, objs):
    W = [l.strip() for l in open(symfile) if l.strip()]
    print('obj\t' + '\t'.join(W) + '\tVT')
    for o in objs:
        secs, sl = nf.coff(o); sel = comdat_info(o)
        defs = {}; und = set()
        for i, n, v, sec, typ, scl in sl:
            if scl != 2: continue
            if sec > 0: defs[n] = sec
            elif sec == 0: und.add(n)
        cols = []
        for w in W:
            called = any(s['name'].startswith('.text') and (k + 1) != defs.get(w) and any(r[1] == w for r in s['rel']) for k, s in enumerate(secs)) \
                     or any(not s['name'].startswith('.text') and any(r[1] == w for r in s['rel']) for s in secs)
            if w in defs:
                c = 'DEF/sel=%d' % sel[defs[w]] if sel.get(defs[w]) else 'DEF/plain'
            elif w in und: c = 'UND'
            else: c = '-'
            cols.append(c + ('+C' if called else ''))
        vt = 'VT' if '??_7CScriptThing@@6B@' in defs else ''
        print(o.rsplit('/', 1)[-1] + '\t' + '\t'.join(cols) + '\t' + vt)

def retail(sym, addr, objs):
    exe = nf.exe(); va0 = int(addr, 16)
    def rbytes(va, n):
        for nm, base, data, ch in exe:
            if base <= va < base + len(data): return data[va - base: va - base + n]
    for o in objs:
        secs, sl = nf.coff(o); s = None
        for i, n, v, sec, typ, scl in sl:
            if n == sym and sec > 0 and scl == 2: s = secs[sec - 1]; break
        if s is None: print('%s\tNODEF' % o.rsplit('/', 1)[-1]); continue
        ours = bytearray(s['data']); r = bytearray(rbytes(va0, len(ours)) or b'')
        tg = []
        for off, nm, t in s['rel']:
            x = struct.unpack_from('<I', r, off)[0] if off + 4 <= len(r) else 0
            tgt = (va0 + off + 4 + x) & 0xffffffff if t == 0x14 else x
            tg.append('%x:%08x=%s' % (off, tgt, nm))
            for k in range(4):
                if off + k < len(ours): ours[off + k] = 0
                if off + k < len(r): r[off + k] = 0
        print('%s\t%s\tlen=%d\t%s' % (o.rsplit('/', 1)[-1], 'BYTES_EQUAL' if bytes(ours) == bytes(r) else 'BYTES_DIFF', len(ours), ' '.join(tg)))

if __name__ == '__main__':
    if sys.argv[1] == 'syms': syms(sys.argv[2], sys.argv[3:])
    elif sys.argv[1] == 'retail': retail(sys.argv[2], sys.argv[3], sys.argv[4:])
