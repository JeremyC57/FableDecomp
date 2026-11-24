#!/usr/bin/env python3
"""scan_t.py: scan retail .text for the VC7.1 inline-float-getter return-home shape:
   mov A,[B+d] ; mov C,A ; push C ; (<=3 insns) ; mov dword ptr [esp+x],A    (dead or live $T home)."""
import os, struct, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
exe = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]
nsec = struct.unpack_from("<H", exe, pe+6)[0]; opt = struct.unpack_from("<H", exe, pe+20)[0]
sec = pe+24+opt
for i in range(nsec):
    name, vs, va, rs, ro = struct.unpack_from("<8sIIII", exe, sec+40*i)
    if name.startswith(b".text"): text = exe[ro:ro+rs]; base = 0x400000+va
md = Cs(CS_ARCH_X86, CS_MODE_32)
ins = []
off = 0
while off < len(text):
    got = False
    for x in md.disasm_lite(text[off:], base+off):
        ins.append(x); got = True; last = x
    off = (last[0]-base+last[1]) if got else off
    off += 1 if not got or True else 0
    ins.append((0,0,"bad",""))
hits = []
for k in range(len(ins)-6):
    a = ins[k]; b = ins[k+1]; c = ins[k+2]
    A = None
    if b[2] == "mov":
        mm = re.match(r"(e\w\w), (e\w\w)$", b[3])
        if mm: A = mm.group(2)
    if not A: continue
    if not (b[2] == "mov" and re.match(r"e\w\w, " + A + "$", b[3])): continue
    C = b[3].split(",")[0]
    if not (c[2] == "push" and c[3] == C): continue
    for j in range(k+3, k+7):
        if ins[j][2] == "mov" and re.match(r"dword ptr \[esp \+ 0x\w+\], " + A + "$", ins[j][3]):
            hits.append((a[0], a[3])); break
print(len(hits))
for h in hits[:60]: print("%08x %s" % h)
