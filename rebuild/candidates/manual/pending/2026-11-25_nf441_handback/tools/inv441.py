#!/usr/bin/env python3
"""inv441.py: retail vftable inventory for the libpk/CODA placeholder VAs: slots, slot targets' current names."""
import sys, struct
sys.path.insert(0, __import__("os").path.dirname(__file__))
from common import *
rows = t3_libpk()
vas = set(v for a, l in rows for v in l)
sym2a, a2sym, selfs = tables()
# all vftable-ish VAs in range from table ??_7 bindings too
allv = set(vas)
for s, A in sym2a.items():
    if s.startswith("??_7"):
        for a in A:
            if 0x012B1000 <= a < 0x012C0000: allv.add(a)
allv = sorted(allv)
for i, v in enumerate(allv):
    nxt = allv[i + 1] if i + 1 < len(allv) else v + 4 * 400
    slots = []
    p = v
    while p < nxt:
        t = dw(p)
        if not is_text(t): break
        slots.append(t); p += 4
    names = sorted(x for x in a2sym.get(v, ()) if x.startswith("??_7"))
    held = sum(1 for a, l in rows if v in l)
    print("%08x\t%d\t%d\t%s\t%s" % (v, len(slots), held, ",".join(names), " ".join("%08x" % t for t in slots)))
