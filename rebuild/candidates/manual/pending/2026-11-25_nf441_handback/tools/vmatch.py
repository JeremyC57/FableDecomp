#!/usr/bin/env python3
"""vmatch.py <rvt.json> <sdk_rel.json> <lib> : score retail vftables vs SDK ??_7 by per-slot body shape similarity.
Alignment: DP over slot sequences (allows insert/delete), score = sum of sims."""
import sys, json, functools
sys.path.insert(0, __import__("os").path.dirname(__file__))
from feat import *
rv = json.load(open(sys.argv[1])); sd = json.load(open(sys.argv[2]))["vts"]; L = Lib(sys.argv[3])
cache = {}
def s(ra, nm):
    k = (ra, nm)
    if k not in cache:
        nm2 = "??_G" + nm[4:] if nm.startswith("??_E") else nm
        x = sim(retail_toks(int(ra, 16)), L.toks(nm2))
        cache[k] = 0.0 if x is None else x
    return cache[k]
def align(rs, ss):
    n, m = len(rs), len(ss)
    D = [[0.0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            D[i][j] = max(D[i-1][j], D[i][j-1], D[i-1][j-1] + s(rs[i-1], ss[j-1]))
    return D[n][m]
only = set(sys.argv[4].split(",")) if len(sys.argv) > 4 else None
for va, r in sorted(rv.items()):
    if only and va not in only: continue
    rs = r["slots"]
    res = []
    for nm, v in sd.items():
        ss = v["slots"]
        if abs(len(ss) - len(rs)) > 4 or not ss: continue
        sc = align(rs, ss)
        res.append((sc / max(len(rs), len(ss)), sc, nm, len(ss)))
    res.sort(reverse=True)
    print("%s\t%d\t%s" % (va, len(rs), " | ".join("%s %.2f(%d)" % (x[2][4:-5], x[0], x[3]) for x in res[:4])), flush=True)
