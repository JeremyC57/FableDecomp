"""Static vtable census for a 32-bit MSVC PE.
Finds RTTI complete object locators (COL) -> type descriptor names, the vtables that follow each COL,
and vtable stores in code (mov dword ptr [reg(+disp)], imm32 where imm32 is a vtable-looking table).
usage: vtables.py <pe> <out_prefix>
outputs: <prefix>_rtti_vt.tsv (vtable_va, nslots, col_offset, class mangled, base classes)
         <prefix>_norrti_vt.tsv (vtable_va, nslots, store_sites) for stored tables with no COL
"""
import sys; sys.path.append("/root/.local/lib/python3.13/site-packages")
import pefile, struct, re, collections
pe = pefile.PE(sys.argv[1], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
secs = {s.Name.rstrip(b"\0").decode(): (base + s.VirtualAddress, base + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData)) for s in pe.sections}
text = secs[".text"]
def in_(r, v): return r[0] <= v < r[1]
data_ranges = [r for n, r in secs.items() if n in (".rdata", ".data")]
def in_data(v): return any(in_(r, v) for r in data_ranges)
def d(va):
    o = va - base
    return struct.unpack_from("<I", img, o)[0] if 0 <= o <= len(img) - 4 else None
def cstr(va):
    o = va - base; e = img.index(b"\0", o); return img[o:e].decode("latin-1")

# type descriptors
tds = {}
for m in re.finditer(rb"\.\?A[VU][\x21-\x7e]+\x00", img):
    tds[base + m.start() - 8] = m.group()[:-1].decode("latin-1")
# COLs: scan rdata/data dword-aligned for (sig 0, off, cd, ptd in tds, pchd)
cols = {}
for r in data_ranges:
    lo, hi = r[0] - base, min(r[1] - base, len(img)) - 20
    for o in range(lo, hi, 4):
        sig, off, cd, ptd, pchd = struct.unpack_from("<5I", img, o)
        if sig == 0 and ptd in tds and in_data(pchd) and off < 0x10000:
            cols[base + o] = (off, ptd, pchd)
def chd_bases(pchd):
    try:
        attr, n, arr = d(pchd + 4), d(pchd + 8), d(pchd + 12)
        out = []
        for i in range(min(n, 64)):
            bcd = d(arr + 4 * i); out.append(tds.get(d(bcd), "?"))
        return out
    except Exception:
        return []
def nslots(vt):
    n = 0
    while True:
        v = d(vt + 4 * n)
        if v is None or not in_(text, v): break
        n += 1
        if n > 2000: break
    return n
# vtables after COL pointers
rtti_vt = {}
for r in data_ranges:
    lo, hi = r[0] - base, min(r[1] - base, len(img)) - 8
    for o in range(lo, hi, 4):
        v = struct.unpack_from("<I", img, o)[0]
        if v in cols:
            vt = base + o + 4
            n = nslots(vt)
            if n: rtti_vt[vt] = (n, v)
# vtable stores in code: C7 /0 with modrm mod=00/01/10 , imm32 -> candidate
stores = collections.defaultdict(int)
t = img[text[0] - base: text[1] - base]
for m in re.finditer(rb"\xc7[\x00-\x07\x40-\x47\x80-\x87]", t):
    i = m.start(); modrm = t[i + 1]; mod = modrm >> 6; rm = modrm & 7
    j = i + 2
    if rm == 4: j += 1  # SIB
    if mod == 0 and rm == 5: j += 4
    elif mod == 1: j += 1
    elif mod == 2: j += 4
    if j + 4 > len(t): continue
    imm = struct.unpack_from("<I", t, j)[0]
    if in_data(imm) and d(imm) is not None and in_(text, d(imm)):
        stores[imm] += 1
norrti = {}
for vt, c in stores.items():
    if vt in rtti_vt: continue
    prev = d(vt - 4)
    if prev in cols: continue
    n = nslots(vt)
    if n >= 2: norrti[vt] = (n, c)
pre = sys.argv[2]
with open(pre + "_rtti_vt.tsv", "w") as f:
    for vt, (n, c) in sorted(rtti_vt.items()):
        off, ptd, pchd = cols[c]
        f.write("%08x\t%d\t%d\t%s\t%s\n" % (vt, n, off, tds[ptd], ",".join(chd_bases(pchd)[1:])))
with open(pre + "_norrti_vt.tsv", "w") as f:
    for vt, (n, c) in sorted(norrti.items()):
        f.write("%08x\t%d\t%d\n" % (vt, n, c))
print("TDs", len(tds), "COLs", len(cols), "RTTI vtables", len(rtti_vt), "stored vtables without COL (>=2 slots)", len(norrti))
