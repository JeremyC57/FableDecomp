#!/usr/bin/env python3
"""fwpub.py: FableWin.exe public map. Writes fw_pubs.tsv (va, name). Import: load() -> (va2name, name2va, reader)."""
import struct, re, sys, os, pickle
EXE = "/tmp/claude-0/egod_pdb/FableWin.exe"; PUB = "/tmp/claude-0/egod_pdb/FableWin.publics.txt"
CACHE = os.path.dirname(os.path.abspath(__file__)) + "/../fw_pubs.pickle"
d = open(EXE, "rb").read()
pe = struct.unpack_from("<I", d, 0x3c)[0]; ns = struct.unpack_from("<H", d, pe + 6)[0]; op = struct.unpack_from("<H", d, pe + 20)[0]
ib = struct.unpack_from("<I", d, pe + 24 + 28)[0]
secs = [struct.unpack_from("<IIII", d, pe + 24 + op + 40 * i + 8) for i in range(ns)]
def rd(va, n=4):
    rva = va - ib
    for vs, sva, rs, ro in secs:
        if sva <= rva < sva + max(vs, rs): return d[ro + rva - sva: ro + rva - sva + n]
    return b""
def dw(va):
    b = rd(va); return struct.unpack("<I", b)[0] if len(b) == 4 else None
TEXT = (ib + secs[0][1], ib + secs[0][1] + max(secs[0][0], secs[0][2]))
def load():
    if os.path.exists(CACHE): return pickle.load(open(CACHE, "rb"))
    v2n, n2v = {}, {}
    name = None
    for l in open(PUB, encoding="utf-8", errors="replace"):
        m = re.search(r"S_PUB32 \[size = \d+\] `(.*)`", l)
        if m: name = m.group(1); continue
        m = re.search(r"addr = (\d+):(\d+)", l)
        if m and name and 1 <= int(m.group(1)) <= len(secs):
            va = ib + secs[int(m.group(1)) - 1][1] + int(m.group(2))
            v2n.setdefault(va, name); n2v[name] = va; name = None
    pickle.dump((v2n, n2v), open(CACHE, "wb")); return v2n, n2v
def follow(va):
    """follow incremental-link jmp thunks"""
    for _ in range(3):
        b = rd(va, 5)
        if len(b) == 5 and b[0] == 0xE9: va = va + 5 + struct.unpack("<i", b[1:])[0]
        else: break
    return va
if __name__ == "__main__":
    v2n, n2v = load(); print(len(v2n), len(n2v))
