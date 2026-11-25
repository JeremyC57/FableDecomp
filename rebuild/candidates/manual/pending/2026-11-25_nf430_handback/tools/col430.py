#!/usr/bin/env python3
"""col430.py: for each vftable VA, read retail [va-4] and decode an RTTI COL if present; count slots."""
import sys, struct
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools")
from pe import read_va, load
d, base, secs = load(__import__("pe").EXES["retail"])
TEXT = (0x401000, 0x401000 + 0x1000000)
# find text section bounds
for sva, sz, ro, rs in secs:
    pass
text = secs[0]
tlo, thi = base + text[0], base + text[0] + text[1]
def dw(va):
    b = read_va("retail", va, 4)
    return struct.unpack("<I", b)[0] if len(b) == 4 else None
def col(va):
    p = dw(va - 4)
    if p is None: return ""
    sig, off, cd, td = [dw(p + 4 * i) for i in range(4)]
    if sig != 0 or td is None: return ""
    name = read_va("retail", td + 8, 200).split(b"\0")[0]
    if not name.startswith(b".?A"): return ""
    return "%s off=%x" % (name.decode(), off)
def slots(va):
    n = 0
    while True:
        p = dw(va + 4 * n)
        if p is None or not (tlo <= p < thi): break
        n += 1
        if n > 400: break
    return n
for line in open(sys.argv[1]):
    va = int(line.strip().split("_")[-1], 16)
    print("%08x\t%d\t%s\t%08x" % (va, slots(va), col(va), dw(va - 4) or 0))
