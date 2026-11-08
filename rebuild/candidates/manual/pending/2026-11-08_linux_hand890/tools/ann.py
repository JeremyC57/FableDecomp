#!/usr/bin/env python3
"""ann.py <dis file>: annotate a retail disassembly with callee labels (manifest + reloc_bindings symbol) and literals."""
import sys, re, os, struct, csv
csv.field_size_limit(10**8)
S = os.environ['S']; data = open(S + '/dl/game/Fable The Lost Chapters/Fable.exe', 'rb').read()
e = struct.unpack_from('<I', data, 0x3C)[0]; coff = e + 4; nsec = struct.unpack_from('<H', data, coff + 2)[0]
opt = struct.unpack_from('<H', data, coff + 16)[0]; secs = []
for i in range(nsec):
    o = coff + 20 + opt + i * 40; vs, va, rs, rp = struct.unpack_from('<IIII', data, o + 8); nm = data[o:o + 8].rstrip(b'\0').decode()
    secs.append((va, max(vs, rs), rp, nm))
def off(va):
    rva = va - 0x400000
    for va0, sz, rp, nm in secs:
        if va0 <= rva < va0 + sz: return rp + rva - va0, nm
    return None, None
def lit(va):
    o, nm = off(va)
    if o is None or nm == '.text': return ''
    end = data.find(b'\0', o, o + 120)
    if end > o:
        s = data[o:end]
        if len(s) >= 2 and all(32 <= c < 127 for c in s): return '"%s"' % s.decode()
    w = data[o:o + 160].decode('utf-16le', 'replace'); w = w[:w.index('\0')] if '\0' in w else ''
    if len(w) >= 2 and all(32 <= ord(c) < 127 for c in w): return 'L"%s"' % w
    if nm == '.rdata':
        f, = struct.unpack_from('<f', data, o); d, = struct.unpack_from('<I', data, o)
        return 'float %r / 0x%08x' % (f, d)
    return nm
man = {}
for line in open('/home/user/FableDecomp/rebuild/manifest/functions.tsv', encoding='utf-8', errors='replace'):
    p = line.split('\t')
    if len(p) > 1: man[p[0]] = p[1]
sym = {}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'), delimiter='\t'):
    sym.setdefault(r['target'].lower().replace('0x', '').zfill(8), r['symbol'])
for line in open(sys.argv[1]):
    line = line.rstrip('\n'); notes = []
    m = re.search(r'call (0x[0-9a-f]+)', line)
    if m:
        a = '%08x' % int(m.group(1), 16); notes.append(man.get(a, '?'))
    else:
        for h in re.findall(r'0x[0-9a-f]{6,8}', line):
            v = int(h, 16)
            if v >= 0x400000:
                a = '%08x' % v; l = lit(v); n = sym.get(a, '')
                notes.append((l + ' ' + n[:80]).strip())
    print(line + ('    ; ' + ' | '.join(x for x in notes if x) if any(notes) else ''))
