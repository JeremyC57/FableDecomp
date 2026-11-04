"""For each vtable listed in <vt.tsv> (col1 = vtable va hex, col2 = nslots), disassemble each slot function
(heuristic extent) and collect referenced C-string literals. Emits one line per vtable:
va \t nslots \t slot_va,... \t string1\x1fstring2...
usage: vt_strings.py <pe> <vt.tsv>[,<vt2.tsv>] <out.tsv>
"""
import sys; sys.path.append("/root/.local/lib/python3.13/site-packages")
import pefile, struct, re
import capstone
pe = pefile.PE(sys.argv[1], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
secs = {s.Name.rstrip(b"\0").decode(): (base + s.VirtualAddress, base + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData)) for s in pe.sections}
text = secs[".text"]; rd = [secs[".rdata"], secs[".data"]]
def in_(r, v): return r[0] <= v < r[1]
def d(va):
    o = va - base
    return struct.unpack_from("<I", img, o)[0] if 0 <= o <= len(img) - 4 else None
pr = re.compile(rb"[\x20-\x7e]{4,200}\x00")
def lit(va):
    if not any(in_(r, va) for r in rd): return None
    o = va - base
    m = pr.match(img, o)
    if m: return m.group()[:-1].decode()
    return None
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
md.skipdata = False
cache = {}
def func_strings(fva):
    if fva in cache: return cache[fva]
    out = []; o = fva - base; maxt = fva; n = 0
    code = img[o:o + 0x4000]
    for ins in md.disasm(code, fva):
        n += 1
        if ins.mnemonic == "call" and ins.op_str.startswith("0x"):
            out.append(ins.op_str[2:].rjust(8, "0"))
        if ins.mnemonic.startswith("j") and ins.op_str.startswith("0x"):
            try:
                t = int(ins.op_str, 16)
                if t > maxt and t < fva + 0x4000: maxt = t
            except ValueError: pass
        if ins.mnemonic in ("ret", "retn") and ins.address >= maxt: break
        if ins.mnemonic == "jmp" and ins.address >= maxt and not ins.op_str.startswith("0x"): break
        if ins.mnemonic == "int3" or n > 3000: break
    cache[fva] = out
    return out
rows = []
for path in sys.argv[2].split(","):
    for line in open(path):
        f = line.rstrip("\n").split("\t"); vt = int(f[0], 16); ns = int(f[1])
        slots = [d(vt + 4 * i) for i in range(ns)]
        rows.append((vt, ns, slots))
with open(sys.argv[3], "w") as fo:
    for vt, ns, slots in rows:
        ss = set()
        per = []
        for s in slots:
            per.append(s)
        fo.write("%08x\t%d\t%s\n" % (vt, ns, ",".join("%08x" % s for s in slots)))
# second file: function -> strings
with open(sys.argv[3] + ".fcall", "w") as fo:
    seen = set()
    for vt, ns, slots in rows:
        for s in slots:
            if s in seen: continue
            seen.add(s)
            st = func_strings(s)
            if st: fo.write("%08x\t%s\n" % (s, ",".join(st)))
print("vtables", len(rows), "functions", len(cache))
