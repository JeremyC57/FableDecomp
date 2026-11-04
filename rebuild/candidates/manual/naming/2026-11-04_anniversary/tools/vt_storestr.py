"""Strings referenced near each code site that stores a vtable (mov dword [r+d], imm32=vtable).
usage: vt_storestr.py <pe> <vt.tsv>[,<vt2.tsv>] <out.tsv> [window]
out: vtable_va \t nsites \t strings joined by \x1f
"""
import sys; sys.path.append("/root/.local/lib/python3.13/site-packages")
import pefile, struct, re, collections
pe = pefile.PE(sys.argv[1], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
secs = {s.Name.rstrip(b"\0").decode(): (base + s.VirtualAddress, base + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData)) for s in pe.sections}
text = secs[".text"]; rd = [secs[".rdata"], secs[".data"]]
WIN = int(sys.argv[4], 0) if len(sys.argv) > 4 else 0x100
want = set()
for p in sys.argv[2].split(","):
    for l in open(p): want.add(int(l.split("\t")[0], 16))
pr = re.compile(rb"[\x20-\x7e]{4,200}\x00")
def lit(va):
    if not any(r[0] <= va < r[1] for r in rd): return None
    m = pr.match(img, va - base)
    return m.group()[:-1].decode() if m else None
t0 = text[0] - base; t = img[t0: text[1] - base]
sites = collections.defaultdict(list)
for m in re.finditer(rb"\xc7[\x00-\x07\x40-\x47\x80-\x87]", t):
    i = m.start(); modrm = t[i + 1]; mod = modrm >> 6; rm = modrm & 7
    j = i + 2
    if rm == 4: j += 1
    if mod == 0 and rm == 5: j += 4
    elif mod == 1: j += 1
    elif mod == 2: j += 4
    if j + 4 > len(t): continue
    imm = struct.unpack_from("<I", t, j)[0]
    if imm in want: sites[imm].append(i)
with open(sys.argv[3], "w") as fo:
    for vt in sorted(want):
        ss = set()
        for i in sites.get(vt, ()):
            a, b = max(0, i - WIN), min(len(t) - 4, i + WIN)
            for k in range(a, b):
                v = struct.unpack_from("<I", t, k)[0]
                if (v & 0xff000000) == (base & 0xff000000) or True:
                    s = lit(v)
                    if s: ss.add(s)
        fo.write("%08x\t%d\t%s\n" % (vt, len(sites.get(vt, ())), "\x1f".join(sorted(ss))))
print("vtables", len(want), "with sites", len(sites))
