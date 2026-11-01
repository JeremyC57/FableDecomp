# regchain.py <nest.tsv>: row -> vtable (slot) -> storing fn (Alloc, or ctor -> its caller Alloc) -> registration string + registering fn
import sys, re, struct, subprocess, pe
from vtowner import vts, PDB
def callers(t):
    out = []
    for nm, v, vs, ro, rs in pe.SEC:
        if nm != '.text': continue
        blob = pe.B[ro:ro + rs]; i = blob.find(b'\xe8')
        while i >= 0:
            if i + 5 <= len(blob) and v + i + 5 + struct.unpack_from('<i', blob, i + 1)[0] == t: out.append(v + i)
            i = blob.find(b'\xe8', i + 1)
    return out
def fstart(va):
    # walk back to the int3/ret padding boundary (function start heuristic)
    o = pe.owner(va)
    if o: return int(o.split()[0], 16)
    b = pe.rd(va - 0x4000, 0x4000); i = len(b) - 1
    while i > 0:
        if b[i - 1] in (0xCC, 0xC3) and b[i] not in (0xCC,) and (b[i-2] in (0xCC,0xC3,0x00) or b[i-1]==0xCC): return va - 0x4000 + i
        i -= 1
    return None
def reg(al):
    res = []
    for s, r in pe.find(al, ('.text',)):
        if pe.rd(r - 3, 3)[:2] != b'\xc7\x46' and pe.rd(r - 3, 3)[:2] != b'\xc7\x47': continue
        b = pe.rd(r - 120, 120); strs = []
        for m in re.finditer(rb'\x68(....)', b):
            v = struct.unpack('<I', m.group(1))[0]
            if 0x01200000 <= v < 0x01400000:
                c = pe.cstr(v, 60)
                if c and c.isprintable() and len(c) > 2: strs.append(c)
        res.append((r, strs[-1] if strs else '?', pe.owner(r)))
    return res
if __name__ == '__main__':
    for l in open(sys.argv[1]):
        q = l.rstrip('\n').split('\t'); a = int(q[0], 16)
        for st, k, refs in vts(a):
            for r, o in refs:
                fs = int(o.split()[0], 16) if o else fstart(r)
                chain = [o or '%08x?' % fs]
                al = fs
                rr = reg(al)
                if not rr:
                    cs = callers(fs)
                    for c in cs:
                        o2 = pe.owner(c); al = int(o2.split()[0], 16) if o2 else fstart(c)
                        chain.append('<- %08x %s' % (al, o2 or '?')); rr = reg(al)
                print(q[0], q[2], q[3], 'vt %08x s%d' % (st, k), ' '.join(chain), '| reg', ['"%s" in %s' % (x[1], x[2]) for x in rr])
