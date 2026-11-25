#!/usr/bin/env python3
"""vtslots.py: for each S0 vt row, the names (addr2names) of every slot of the containing vftable (vt start from s0join, run to the
next vptr-store target or a non-.text dword). Writes r2/work/s0slots.tsv: addr vt slot nslots then one column per slot 'addr=name'."""
import sys, json, re, struct, bisect
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
secs = nf.exe(); text = [s for s in secs if s[0] == ".text"][0]
def rd32(va):
    for n, b, d, ch in secs:
        if b <= va < b + len(d) - 3: return struct.unpack_from("<I", d, va - b)[0]
def intext(x): return x is not None and text[1] <= x < text[1] + len(text[2])
a2n = json.load(open(A + "work/addr2names.json"))
keys = sorted({int(l.split("\t")[0], 16) for l in open(A + "r2/work/vtstores.tsv")})
out = []
for l in open(A + "r2/work/s0join.tsv"):
    q = l.rstrip("\n").split("\t")
    for m in re.finditer(r"vt=(\w+) slot=(\d+)", q[4]):
        vt = int(m.group(1), 16); i = bisect.bisect_right(keys, vt)
        nxt = keys[i] if i < len(keys) else 1 << 40
        cols = []; k = 0
        while intext(rd32(vt + 4 * k)) and vt + 4 * k < nxt and k < 200:
            f = "%08x" % rd32(vt + 4 * k); cols.append("%d:%s=%s" % (k, f, (a2n.get(f) or ["?"])[0])); k += 1
        out.append("\t".join([q[0], m.group(1), m.group(2), str(k)] + cols))
open(A + "r2/work/s0slots.tsv", "w").write("\n".join(out) + "\n")
print(len(out))
