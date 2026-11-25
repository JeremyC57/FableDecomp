#!/usr/bin/env python3
"""s0join.py: for each S0 row, the vftable start = nearest vptr-store target <= the reference site, the functions that store it,
their names (addr2names), the row's own names and callees. Writes r2/work/s0join.tsv."""
import json, bisect, re
from collections import defaultdict
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
a2n = json.load(open(A + "work/addr2names.json"))
st = defaultdict(set)
for l in open(A + "r2/work/vtstores.tsv"):
    v, f, o, d = l.split("\t"); st[int(v, 16)].add(f)
keys = sorted(st)
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
out = []
for l in open(A + "r2/work/s0_vt.tsv"):
    a, g, sig, refs = l.rstrip("\n").split("\t")
    r = hist[a]
    ent = []
    for part in refs.split(" | "):
        m = re.match(r"vt=(\w+) slot=(\d+)", part)
        if not m: ent.append(part); continue
        site = int(m.group(1), 16) + 4 * int(m.group(2))
        i = bisect.bisect_right(keys, site) - 1
        if i < 0 or site - keys[i] > 4 * 120: ent.append("site=%08x nostore" % site); continue
        vt = keys[i]; fs = sorted(st[vt])
        ent.append("site=%08x vt=%08x slot=%d stores=%s" % (site, vt, (site - vt) // 4,
                   ",".join("%s[%s]" % (f, "/".join(a2n.get(f, ["?"])[:2])) for f in fs[:4]) + ("+%d" % (len(fs) - 4) if len(fs) > 4 else "")))
    out.append("\t".join([a, g, r["callees"], "/".join(a2n.get(a, [])[:3]), " | ".join(ent)]))
open(A + "r2/work/s0join.tsv", "w").write("\n".join(out) + "\n")
