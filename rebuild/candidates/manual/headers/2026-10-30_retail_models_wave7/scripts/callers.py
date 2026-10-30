#!/usr/bin/env python3
"""callers.py <target_va>...: landed rows whose retail bytes call (e8 rel32) any target; prints addr, flags, source."""
import sys, os, re, struct
R = os.environ["FABLE_ROOT"]
tg = {int(a, 16) for a in sys.argv[1:]}
ps = open(R + "/rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower().replace("0x", "")] = kv
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); a = q[0].lower()
    if a not in ent: continue
    b = bytes.fromhex(q[3]); va = int(a, 16)
    hit = [t for i in range(len(b) - 4) if b[i] == 0xE8 for t in [(va + i + 5 + struct.unpack_from("<i", b, i + 1)[0]) & 0xFFFFFFFF] if t in tg]
    if hit: print(a, ",".join("%08x" % h for h in sorted(set(hit))), ent[a].get("CompilerFlags", ""), ent[a]["Source"], sep="\t")
