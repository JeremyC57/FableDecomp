#!/usr/bin/env python3
"""cntmatch.py: per retail CODA VA, per SDK class: number of retail slots whose instruction count (and mnemonic seq ratio>=.8) equals an SDK slot body of that class (any position)."""
import sys, json
sys.path.insert(0, __import__("os").path.dirname(__file__))
from feat import *
S = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"
L = Lib(S + "/sdk/gamecoda/tree/CODA/lib/Win32/CODAApi.lib")
rv = json.load(open("work/rvt.json")); sd = json.load(open("work/sdk_rel.json"))["vts"]
vas = [l.split()[1] for l in open("work/coda_struct.txt") if l.startswith("==")]
def mn(t): return tuple(x.split()[0] for x in t)
for va in vas:
    rs = rv[va]["slots"]; res = []
    for k, v in sd.items():
        ss = [("??_G" + s[4:] if s.startswith("??_E") else s) for s in v["slots"]]
        st = [L.toks(s) for s in ss]
        hit = 0; tot = 0
        for i, r in enumerate(rs):
            if r == "00bfea08": continue
            rt = retail_toks(int(r, 16)); tot += 1
            if len(rt) < 3: continue
            for t in st:
                if t and len(t) == len(rt) and sim(mn(t), mn(rt)) >= 0.8: hit += 1; break
        if hit: res.append((hit, -abs(len(ss) - len(rs)), k[4:-5], len(ss)))
    res.sort(reverse=True)
    print(va, len(rs), " | ".join("%s %d(%d)" % (x[2], x[0], x[3]) for x in res[:4]))
