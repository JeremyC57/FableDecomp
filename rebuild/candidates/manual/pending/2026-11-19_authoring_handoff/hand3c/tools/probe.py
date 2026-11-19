#!/usr/bin/env python3
"""probe.py <src.cpp> <leaf> : compile (no grade, no ATTEMPTS line) and print per-column x87 term orders (k + b/a loaded first)."""
import sys, os, re
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
W = Path(os.environ["W"]); src = Path(sys.argv[1]); leaf = sys.argv[2]
work = W/"tmp"/"probe"; work.mkdir(parents=True, exist_ok=True)
st, built, sec = V.parity_of(V.vc71(src.read_text()), "probe", leaf, b"\0", work, V.env())
if not built: print(st); sys.exit(1)
ins = [(i.mnemonic, i.op_str) for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(built, 0)]
def mem(op):
    m = re.search(r"\[(\w+)(?: ([+-]) (0x[0-9a-f]+|\d+))?\]", op)
    if not m: return None
    off = int(m.group(3), 0) if m.group(3) else 0
    return m.group(1), (-off if m.group(2) == "-" else off)
groups = []; cur = []
for i, (mn, op) in enumerate(ins):
    if mn == "fld" and mem(op):
        j = next((k for k in range(i+1, min(i+5, len(ins))) if ins[k][0] == "fmul"), None)
        if j is not None and mem(ins[j][1]): cur.append((mem(op), mem(ins[j][1])))
    if mn == "fstp" and cur: groups.append(cur); cur = []
out = []
for g in groups:
    if len(g) != 4: continue
    regs = {}
    for p in g:
        for r, o in p: regs.setdefault(r, []).append(o)
    rb = next((r for r, offs in regs.items() if len(set(offs)) == 4 and max(offs) - min(offs) == 0x30), None)
    if rb is None: out.append("?"); continue
    mn_ = min(regs[rb]); s = []
    for f, m in g:
        b = f if f[0] == rb else m
        s.append("%d%s" % ((b[1] - mn_) // 16, "b" if f[0] == rb else "a"))
    out.append(" ".join(s))
print(st if st.startswith("SRC") else "built %d" % len(built), "|", " | ".join(out))
