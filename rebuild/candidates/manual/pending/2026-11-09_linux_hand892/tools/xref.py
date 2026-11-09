#!/usr/bin/env python3
"""xref.py <target-va-hex>... : list E8 call sites in .text that target the given addresses."""
import os, struct, sys
exe = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]; ns = struct.unpack_from("<H", exe, pe+6)[0]; opt = struct.unpack_from("<H", exe, pe+20)[0]
o = pe+24+opt; vs, sva, rs, ro = struct.unpack_from("<IIII", exe, o+8)
text = exe[ro:ro+rs]; base = 0x400000 + sva
targets = set(int(a, 16) for a in sys.argv[1:])
for i in range(len(text) - 5):
    if text[i] == 0xE8:
        t = base + i + 5 + struct.unpack_from("<i", text, i+1)[0]
        if t in targets: print(hex(base+i), "->", hex(t))
