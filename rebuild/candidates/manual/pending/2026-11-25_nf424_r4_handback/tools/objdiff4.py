#!/usr/bin/env python3
"""objdiff4.py <head.obj> <new.obj>: function-level diff of two COFF objects (our compiles only): every function symbol with its
section bytes (relocation fields masked) and its relocation target names. Prints gone / new / changed symbol lists."""
import struct, sys
def funcs(p):
    d = open(p, 'rb').read()
    _, ns, _, sp, nsy, osz, _ = struct.unpack_from('<HHIIIHH', d, 0)
    secs = [struct.unpack_from('<8sIIIIIIHHI', d, 20 + osz + 40 * i) for i in range(ns)]
    st = sp + 18 * nsy
    syms = {}; i = 0
    while i < nsy:
        n, v, sn, ty, cl, na = struct.unpack_from('<8sIhHBB', d, sp + 18 * i)
        name = d[st + struct.unpack_from('<I', n, 4)[0]:].split(b'\0')[0] if n[:4] == b'\0\0\0\0' else n.rstrip(b'\0')
        syms[i] = (name.decode('latin1'), v, sn, ty, cl); i += 1 + na
    out = {}
    for idx, (name, v, sn, ty, cl) in syms.items():
        if sn > 0 and ty == 0x20 and cl in (2, 3):
            s = secs[sn - 1]; raw = bytearray(d[s[4]:s[4] + s[3]]); rel = []
            for r in range(s[7]):
                va, si, t = struct.unpack_from('<IIH', d, s[5] + 10 * r)
                raw[va:va + 4] = b'\0\0\0\0'; rel.append((va, syms.get(si, ('?',))[0]))
            out[name] = (bytes(raw[v:]), tuple(rel))
    return out
h, n = funcs(sys.argv[1]), funcs(sys.argv[2])
print("gone:" + ",".join(sorted(set(h) - set(n))))
print("new:" + ",".join(sorted(set(n) - set(h))))
print("changed:" + ",".join(sorted(k for k in set(h) & set(n) if h[k] != n[k])))
