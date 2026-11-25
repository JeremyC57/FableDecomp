#!/usr/bin/env python3
"""owners.py: for each candidate row (S0 named by s0cls.py, and the T3 S1 / S2 G2 / S3 G3 rows), the functions that store the row's
vftable (vtstores.tsv) and have a landed source (build_candidates.ps1 Source), i.e. candidate ctor-owned units. Also the row's lock owner.
Writes r2/work/owners.tsv: addr shape leaf vt n_stores owners(addr:src;...) lock"""
import sys, re, json
from collections import defaultdict
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
st = defaultdict(set)
for l in open(A + "r2/work/vtstores.tsv"):
    v, f, o, d = l.split("\t"); st[v].add(f)
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
rows = []
for l in open(A + "r2/work/s0cls.tsv"):
    q = l.rstrip("\n").split("\t")
    if q[5] != "-": rows.append((q[0], "S0", q[5], q[2]))
for l in open(A + "T3.txt"):
    q = l.rstrip("\n").split("\t")
    if len(q) < 4 or q[0] != "nf424": continue
    sh = q[2]
    if sh in ("S1 header port", "S1 local model", "S1u callee identity", "S1x identity", "S2 G2", "S3 G3", "G1-CS port"):
        a = q[1][2:].lower(); h = hist[a]
        vt = h["retail_vt"].split(";")[0].split("/")[0]
        rcls = h["retail_cls"]
        leaf = h["rtti"] or ""
        rows.append((a, sh, leaf, vt))
out = []
for a, sh, leaf, vt in rows:
    fs = sorted(st.get(vt.lower(), ()))
    own = []
    for f in fs:
        s = nf.field(f, "Source")
        if s: own.append("%s:%s" % (f, s))
    out.append("\t".join([a, sh, leaf, vt, str(len(fs)), ";".join(own), nf.lock_owner(a)]))
open(A + "r2/work/owners.tsv", "w").write("\n".join(out) + "\n")
from collections import Counter
print(Counter((o.split("\t")[1], o.split("\t")[5] != "") for o in out))
print(Counter(o.split("\t")[6] for o in out))
