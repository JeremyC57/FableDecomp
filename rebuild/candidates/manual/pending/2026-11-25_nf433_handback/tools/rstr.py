#!/usr/bin/env python3
import sys, os, struct
d = open(os.environ['FABLE_EXE'], 'rb').read(); pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]
op = struct.unpack_from('<H', d, pe + 20)[0]; ib = struct.unpack_from('<I', d, pe + 24 + 28)[0]
for a in sys.argv[1:]:
    va = int(a, 16)
    for i in range(ns):
        vs, vaddr, rs, rp = struct.unpack_from('<IIII', d, pe + 24 + op + 40 * i + 8)
        if ib + vaddr <= va < ib + vaddr + max(vs, rs):
            off = rp + va - ib - vaddr; print(a, repr(d[off:d.index(b'\0', off)].decode('latin-1')))
