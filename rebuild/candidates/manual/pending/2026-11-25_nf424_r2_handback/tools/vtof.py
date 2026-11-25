#!/usr/bin/env python3
"""vtof.py: for every S0 row with data references, find the vftable that holds it (the reference site), the slot index (walk back
while the previous dword is a .text pointer), and the COL / RTTI type descriptor at vftable-4 when present. Writes r2/work/s0_vt.tsv."""
import os, sys, struct, re
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/tools"); import nf
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
secs = nf.exe(); text = [s for s in secs if s[0] == ".text"][0]
def rd32(va):
    for n, b, d, ch in secs:
        if b <= va < b + len(d) - 3: return struct.unpack_from("<I", d, va - b)[0]
def rdstr(va, n=200):
    for nm, b, d, ch in secs:
        if b <= va < b + len(d):
            o = va - b; e = d.index(b"\0", o); return d[o:e].decode("latin1")
def intext(x): return x is not None and text[1] <= x < text[1] + len(text[2])
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
refs = {l.split("\t")[0]: l.rstrip("\n").split("\t")[1:] for l in open(A + "work/refs.tsv").read().splitlines()[1:]}
out = []
for a, r in sorted(hist.items()):
    if r["group"] not in ("g_lbp", "g_tsw"): continue
    sites = [x.split("@") for x in refs[a][4].split(";") if x]
    res = []
    for kind, s in sites:
        if kind != "absd": res.append("%s@%s" % (kind, s)); continue
        va = int(s, 16); k = 0
        while intext(rd32(va - 4 * (k + 1))): k += 1
        vt = va - 4 * k
        col = rd32(vt - 4); td = None
        if col and not intext(col):
            tdp = rd32(col + 12)
            if tdp:
                nm = rdstr(tdp + 8)
                if nm and nm.startswith(".?A"): td = nm
        n = 0
        while intext(rd32(vt + 4 * n)): n += 1
        res.append("vt=%08x slot=%d nslots=%d rtti=%s" % (vt, k, n, td or "-"))
    out.append("%s\t%s\t%s\t%s" % (a, r["group"], r["sig"], " | ".join(res) or "noref"))
open(A + "r2/work/s0_vt.tsv", "w").write("\n".join(out) + "\n")
from collections import Counter
print(Counter(("rtti" if "rtti=.?A" in l else ("vt" if "vt=" in l else l.split("\t")[3][:5])) for l in out))
