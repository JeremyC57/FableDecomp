#!/usr/bin/env python3
"""vt.py <vtable-va> [nslots]: dump retail vtable slots with RTTI (vt[-1] COL) and best-known labels.
Labels: the catalog (auto-re-candidates) then ghidra_out/labels_*.tsv in name order (later wins)."""
import sys, os, struct, glob, csv
S = os.environ["S"]; R = os.environ.get("FABLE_ROOT", S + "/mig8/repo")
exe = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]; ns = struct.unpack_from("<H", exe, pe + 6)[0]
opt = pe + 24; osz = struct.unpack_from("<H", exe, pe + 20)[0]; IB = 0x400000
secs = []
for i in range(ns):
    o = opt + osz + 40 * i
    name = exe[o:o+8].rstrip(b"\0").decode(); vs, va, rs, ra = struct.unpack_from("<IIII", exe, o + 8)
    secs.append((name, IB + va, max(vs, rs), ra, rs))
def rd(va, n=4):
    for name, sva, sz, ra, rs in secs:
        if sva <= va < sva + sz:
            off = va - sva
            return exe[ra + off: ra + off + n] if off + n <= rs else b"\0" * n
    return None
def u32(va):
    b = rd(va); return struct.unpack("<I", b)[0] if b else None
def sec_of(va):
    for name, sva, sz, ra, rs in secs:
        if sva <= va < sva + sz: return name
    return None
lab = {}
for r in csv.reader(open(R + "/rebuild/oracles/auto-re-candidates.tsv"), delimiter="\t"):
    if r and r[0] != "address": lab[r[0].lower()] = r[1]
for f in sorted(glob.glob(R + "/ghidra_out/labels_*.tsv")):
    for r in csv.reader(open(f, encoding="utf-8", errors="replace"), delimiter="\t"):
        if len(r) >= 2 and len(r[0]) == 8 and r[0] != "address":
            try: int(r[0], 16)
            except ValueError: continue
            lab[r[0].lower()] = r[1]
def cstr(va):
    s = b""
    while True:
        b = rd(va + len(s), 1)
        if not b or b == b"\0": return s.decode("latin1")
        s += b
def rtti(vt):
    col = u32(vt - 4)
    if not col or sec_of(col) != ".rdata": return None
    td = u32(col + 12)
    return cstr(td + 8) if td else None
if __name__ == "__main__":
    vt = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 40
    print(f"vtable {vt:08x} RTTI {rtti(vt)}")
    for i in range(n):
        p = u32(vt + 4 * i)
        if p is None or sec_of(p) != ".text":
            print(f"  [{i}] {p and hex(p)} (end)"); break
        if i > 0 and sec_of(u32(vt + 4*i - 4 + 4) or 0) != ".text": break
        print(f"  [{i:3}] {p:08x} {lab.get('%08x' % p, '')}")
