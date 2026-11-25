#!/usr/bin/env python3
"""vtscan.py: every vftable VA in .rdata 0x012B1000..0x012C0000 stored by a mov [..], imm32 in retail .text; slots; store sites."""
import sys, struct, json
sys.path.insert(0, __import__("os").path.dirname(__file__))
from common import *
va0, sz, ro, rs = SECS[0]
text = D[ro:ro + rs]; tb = BASE + va0
stores = {}
for i in range(2, len(text) - 4):
    v = struct.unpack_from("<I", text, i)[0]
    if 0x012B1000 <= v < 0x012C0000 and v % 4 == 0:
        ok = False
        if text[i-2] == 0xC7 and (text[i-1] & 0xC0) == 0 and (text[i-1] & 7) not in (4, 5): ok = True       # mov [reg], imm
        elif i >= 3 and text[i-3] == 0xC7 and (text[i-2] & 0xC0) == 0x40 and (text[i-2] & 7) != 4: ok = True  # mov [reg+d8], imm
        elif i >= 3 and text[i-3] == 0xC7 and text[i-2] == 0x04: ok = True   # mov [sib], imm
        elif i >= 4 and text[i-4] == 0xC7 and (text[i-3] & 0xC0) == 0x40 and (text[i-3] & 7) == 4: ok = True
        elif i >= 6 and text[i-6] == 0xC7 and (text[i-5] & 0xC0) == 0x80: ok = True
        if ok and is_text(dw(v)):
            stores.setdefault(v, []).append(tb + i)
vas = sorted(stores)
out = {}
for k, v in enumerate(vas):
    nxt = vas[k + 1] if k + 1 < len(vas) else v + 4 * 200
    slots = []; p = v
    while p < nxt:
        t = dw(p)
        if not is_text(t): break
        slots.append(t); p += 4
    out["%08x" % v] = {"slots": ["%08x" % s for s in slots], "stores": ["%08x" % s for s in stores[v]]}
json.dump(out, open(sys.argv[1], "w"), indent=0)
print(len(out))
