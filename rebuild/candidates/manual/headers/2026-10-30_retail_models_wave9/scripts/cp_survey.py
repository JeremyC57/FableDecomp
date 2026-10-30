#!/usr/bin/env python3
"""cp_survey.py: every row-local CCountedPointer / CCPPointerInfo block, normalized and clustered. Writes work/cp_survey.json."""
import sys, re, os, glob, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
files = sorted(f for f in glob.glob(R + "/rebuild/src/compiled/**/*.cpp", recursive=True) if not f.endswith(".pre.cpp"))
out = {}
for f in files:
    t = open(f, encoding="utf-8", errors="replace").read()
    if "CCountedPointer" not in t: continue
    blks = []; pos = 0
    while True:
        b = find_block(t, "CCountedPointer", pos)
        if not b: break
        blks.append(norm(t[b[0]:b[1]])); pos = b[1]
    pi = find_block(t, "CCPPointerInfo")
    out[os.path.relpath(f, R)] = {"cp": blks, "pi": norm(t[pi[0]:pi[1]]) if pi else None}
json.dump(out, open(S + "/mig9/work/cp_survey.json", "w"))
c = collections.Counter(); cpi = collections.Counter()
n = 0
for f, d in out.items():
    if d["cp"]: n += 1
    c[len(d["cp"])] += 1
    for b in d["cp"][:1]: cpi[b] += 1
print("files mentioning", len(out), "with local blocks", n, "blockcount dist", dict(c))
print("distinct first blocks", len(cpi))
for b, k in cpi.most_common(40): print(k, "|", b[:400])
