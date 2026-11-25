#!/usr/bin/env python3
"""diag.py: per thin-row compile, status + whether the leaf is defined in the object + the ??_G symbols the owner TU defines.
Writes r2/work/diag.tsv: addr shape status leaf_present n_G Gsyms"""
import sys, re, glob, os
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
gt = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/gen_thin.tsv")}
res = {}
for f in sorted(glob.glob(A + "r2/work/cc2_*.log")):
    tag = f.split("cc2_")[1][:-4]
    for l in open(f):
        q = l.split()
        if len(q) > 2 and re.match(r"[0-9a-f]{8}$", q[0]): res[q[0]] = (tag, q[1])
out = []
for a, (tag, st) in sorted(res.items()):
    o = A + "r2/obj/%s/%s.obj" % (tag, a)
    leaf = gt[a][4]
    if os.path.exists(o):
        s = nf.ext_syms(o); g = sorted(x[4:] for x in s if x.startswith("DEF ??_G"))
        lp = ("DEF " + leaf) in s
    else: g = []; lp = False
    out.append("\t".join([a, gt[a][1], st, "LEAF" if lp else "NOLEAF", str(len(g)), " ".join(g)[:400]]))
open(A + "r2/work/diag.tsv", "w").write("\n".join(out) + "\n")
from collections import Counter
print(Counter((o.split("\t")[1], o.split("\t")[2].split("(")[0], o.split("\t")[3]) for o in out))
