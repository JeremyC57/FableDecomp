#!/usr/bin/env python3
"""Retail base-class records from Fable.exe RTTI (ClassHierarchyDescriptor / BaseClassArray).
usage: rtti_bases.py <Fable.exe> <out.tsv>
out: class_typedesc_name  demangled  nbases  direct_bases(name@mdisp[,v])  all_bases(name@mdisp)"""
import struct, sys, re
IB = 0x400000
data = open(sys.argv[1], 'rb').read()
e = struct.unpack_from('<I', data, 0x3C)[0]; coff = e + 4
nsec = struct.unpack_from('<H', data, coff + 2)[0]; optsz = struct.unpack_from('<H', data, coff + 16)[0]
sb = coff + 20 + optsz; secs = []
for i in range(nsec):
    o = sb + i * 40
    name = data[o:o + 8].rstrip(b'\0').decode('latin1')
    vs, va, rs, rp = struct.unpack_from('<IIII', data, o + 8)
    secs.append((name, va, max(vs, rs), rp, rs))
def off(va):
    rva = va - IB
    for n, sva, vsz, rp, rs in secs:
        if sva <= rva < sva + vsz and rva - sva < rs: return rp + rva - sva
    return None
def u32(va):
    o = off(va); return None if o is None else struct.unpack_from('<I', data, o)[0]
def cstr(va):
    o = off(va); end = data.find(b'\0', o, o + 1024); return data[o:end].decode('latin1')
td = {}
for n, sva, vsz, rp, rs in secs:
    if n not in ('.data', '.rdata'): continue
    blob = data[rp:rp + rs]; p = 0
    while True:
        p = blob.find(b'.?A', p)
        if p < 0: break
        tva = IB + sva + p - 8
        nm = cstr(IB + sva + p)
        if nm.startswith('.?A'): td[tva] = nm
        p += 3
sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
from msvc_demangle import demangle  # templates too, in the Ego_r donor spelling
chd = {}
for n, sva, vsz, rp, rs in secs:
    if n not in ('.data', '.rdata'): continue
    blob = data[rp:rp + rs]; o = 0
    while o < len(blob) - 20:
        sig, offs, cdo, ptd, pcd = struct.unpack_from('<IIIII', blob, o)
        if sig == 0 and ptd in td and off(pcd) is not None and offs == 0:
            chd.setdefault(ptd, pcd)
        o += 4
rows = []
for ptd, pcd in chd.items():
    sig, attr, nb, pbca = (u32(pcd + 4 * i) for i in range(4))
    if nb is None or nb > 200 or off(pbca) is None: continue
    bcds = []
    for i in range(nb):
        b = u32(pbca + 4 * i)
        btd, ncont, mdisp, pdisp, vdisp, battr = (u32(b + 4 * k) for k in range(6))
        if mdisp >= 0x80000000: mdisp -= 1 << 32
        if pdisp >= 0x80000000: pdisp -= 1 << 32
        bcds.append((td.get(btd, '?'), ncont, mdisp, pdisp, vdisp))
    if not bcds or bcds[0][0] != td[ptd]: continue
    direct = []; i = 1
    while i < len(bcds):
        nm, nc, md, pd, vd = bcds[i]
        direct.append(f"{demangle(nm)}@{md}" + (f",v{pd}:{vd}" if pd != -1 else ''))
        i += 1 + nc
    allb = [f"{demangle(b[0])}@{b[2]}" + (f",v{b[3]}:{b[4]}" if b[3] != -1 else '') for b in bcds[1:]]
    rows.append((td[ptd], demangle(td[ptd]), str(nb - 1), ';'.join(direct), ';'.join(allb)))
rows.sort(key=lambda r: r[1])
with open(sys.argv[2], 'w') as f:
    f.write('typedesc\tclass\tnbases\tdirect\tall\n')
    for r in rows: f.write('\t'.join(r) + '\n')
print(len(td), 'typedescs', len(chd), 'chd', len(rows), 'rows')
