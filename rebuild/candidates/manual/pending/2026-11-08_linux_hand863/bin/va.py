#!/usr/bin/env python3
# va.py <hexva> [ndwords] : dump dwords of the retail exe at a VA
import sys, os, struct
d = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", d, 0x3c)[0]
ns = struct.unpack_from("<H", d, pe + 6)[0]; osz = struct.unpack_from("<H", d, pe + 20)[0]
secs = []
for i in range(ns):
    o = pe + 24 + osz + 40 * i
    vs, va, rs, ro = struct.unpack_from("<IIII", d, o + 8)
    secs.append((va + 0x400000, max(vs, rs), ro))
def off(v):
    for va, sz, ro in secs:
        if va <= v < va + sz: return ro + v - va
v = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 8
for i in range(n):
    print(f"{v+4*i:08x}: {struct.unpack_from('<I', d, off(v+4*i))[0]:08x}")
