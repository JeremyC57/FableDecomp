# vtdump.py <Class> [<Class>...]: find the retail vftable(s) of a class through its RTTI (type descriptor `.?AV<Class>@@`,
# complete object locator, vftable[-1] == COL) in Fable.exe and print each slot's target address and RTTI-port label.
# Screen / evidence tool: prints addresses and names only, never bytes.
# Slots run until the next dword is not inside .text (or is another vftable's COL).
import sys, struct, os
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; secs = []
for i in range(ns):
    name = d[so + i * 40: so + i * 40 + 8].rstrip(b'\0').decode('latin1')
    vs, va, rs, raw = struct.unpack_from('<IIII', d, so + i * 40 + 8)
    secs.append((name, base + va, max(vs, rs), raw, rs))
def sec_of(va):
    for nm, s, n, r, rs in secs:
        if s <= va < s + n: return nm
def rd(va):
    for nm, s, n, r, rs in secs:
        if s <= va < s + rs: return struct.unpack_from('<I', d, r + va - s)[0]
def off2va(o):
    for nm, s, n, r, rs in secs:
        if r <= o < r + rs: return s + o - r
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open('/home/user/FableDecomp/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
# index: all dwords in .rdata (COL candidates) -> positions
rdata = [s for s in secs if s[0] in ('.rdata', '.data')]
def find_all(pat):
    i = d.find(pat)
    while i >= 0:
        yield i
        i = d.find(pat, i + 1)
for cls in sys.argv[1:]:
    td = b'.?AV' + cls.encode() + b'@@\0'
    hits = [off2va(o) - 8 for o in find_all(td)]
    if not hits:
        print(cls, 'NO_TYPEDESC'); continue
    for tdva in hits:
        # COL: sig(0) offset cdOffset pTypeDescriptor pClassHierarchy
        for o in find_all(struct.pack('<I', tdva)):
            colva = off2va(o) - 12
            if colva is None: continue
            sig, off, cdo = rd(colva), rd(colva + 4), rd(colva + 8)
            if sig != 0: continue
            for o2 in find_all(struct.pack('<I', colva)):
                vt = off2va(o2) + 4
                if sec_of(vt) not in ('.rdata', '.data'): continue
                print('%s vftable %08X (this-offset %d, COL %08X)' % (cls, vt, off, colva))
                k = 0
                while True:
                    t = rd(vt + 4 * k)
                    if t is None or sec_of(t) != '.text': break
                    if k > 0:
                        # stop if this dword is itself referenced as a COL slot of a following vftable: heuristically check -1 rule
                        pass
                    print('  slot %3d +0x%03X %08X %s' % (k, 4 * k, t, L.get('%08x' % t, '-')))
                    k += 1
