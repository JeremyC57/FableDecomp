# symscan360.py <symbols file> @<obj list>: per object, DEF (with COMDAT selection) / UND entries of each named external symbol.
# Output lines: <kind>\t<sel>\t<symbol>\t<object path>; last line: objects scanned / unreadable. Reads the COFF symbol table only. (NF360)
import sys, struct
syms = set(l.rstrip('\n') for l in open(sys.argv[1]) if l.strip())
objs = [l.strip() for l in open(sys.argv[2][1:]) if l.strip()]
bad = 0
for p in objs:
    try:
        d = open(p, 'rb').read()
        if len(d) < 20 or d[:2] != b'\x4c\x01': bad += 1; continue
        psym, nsym = struct.unpack_from('<II', d, 8); strt = d[psym + 18 * nsym:]
        sel = {}; hits = []; i = 0
        while i < nsym:
            o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8); raw = d[o:o + 8]
            if scl == 3 and v == 0 and na == 1 and sec > 0: sel.setdefault(sec, d[o + 18 + 14])
            if scl == 2:
                if raw[:4] == b'\0\0\0\0':
                    off = struct.unpack_from('<I', raw, 4)[0]; n = strt[off:strt.index(b'\0', off)].decode('latin1')
                else: n = raw.rstrip(b'\0').decode('latin1')
                if n in syms: hits.append((n, sec))
            i += 1 + na
        for n, sec in hits:
            print('%s\t%s\t%s\t%s' % ('DEF' if sec > 0 else 'UND', sel.get(sec, '-') if sec > 0 else '-', n, p))
    except Exception:
        bad += 1
print('# objects %d unreadable %d' % (len(objs), bad))
