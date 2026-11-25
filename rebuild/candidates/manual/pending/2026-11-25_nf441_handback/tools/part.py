#!/usr/bin/env python3
"""part.py: classify each retail vftable VA as coda (slot-0 deleting dtor reaches CodaFree 0x00C3C370) or not; dump callees of slot 0."""
import sys, json, struct
sys.path.insert(0, __import__("os").path.dirname(__file__))
from feat import *
rv = json.load(open(sys.argv[1]))
def calls(va, depth=0):
    out = []
    for ins in MD.disasm(rd(va, extent(va)), va):
        if ins.mnemonic in ("call", "jmp") and ins.op_str.startswith("0x"):
            out.append(int(ins.op_str, 16))
        if ins.mnemonic == "ret": break
    return out
CF = {0x00C3C370}; CM = {0x00C3C360}
for va, r in sorted(rv.items()):
    if not r["slots"]: continue
    s0 = int(r["slots"][0], 16)
    c = calls(s0)
    coda = bool(set(c) & CF)
    # one level deeper (dtor called from sdd)
    if not coda:
        for t in c:
            if is_text(t) and set(calls(t)) & CF: coda = True; break
    print("%s\t%s\t%d\t%s" % (va, "coda" if coda else "-", len(r["slots"]), " ".join("%08x" % x for x in c)))
