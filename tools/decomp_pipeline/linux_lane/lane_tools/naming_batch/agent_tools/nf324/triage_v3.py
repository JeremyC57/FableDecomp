#!/usr/bin/env python3
"""triage_v3.py <ENTRIES.tsv> <flat4_RESULTS.tsv> <v4 GEN.tsv> <out TRIAGE.tsv>  (NF324, 2026-11-24; run from the repo root)
Why the v3 flatten_pilot.py stops on each entry (its candidate filter, replayed without compiling, then its flat4 verdict) next to the v4
tool's verdict / route / reason / bucket. Columns: address, v3_stage, v3_reason, v4_verdict, v4_route, v4_bucket, v4_reason."""
import re, sys, collections
E = [l.split("\t") for l in open(sys.argv[1]).read().splitlines()[1:]]
F = {l.split("\t")[0]: l.split("\t") for l in open(sys.argv[2]).read().splitlines()[1:]}
G = {l.split("\t")[0]: l.split("\t") for l in open(sys.argv[3]).read().splitlines()[1:]}
pdb = set()
for fn in ("egor", "fablewin"):
    for l in open(f"ghidra_out/{fn}_pdb_names.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) > 1: pdb.add(q[1])
bym = collections.defaultdict(list)
for p in pdb:
    m = re.match(r'\?([^@?]+)@([^@]+)@@', p)
    if m: bym[(m.group(1), m.group(2))].append(p)
out = []
for t, s, p, *_ in E:
    meth, cls = re.match(r'\?([^@]+)@([^@]+)@@', p).groups()
    m = re.match(r'\?([A-Za-z0-9]+)_([A-Za-z0-9]+)@@YI', s)
    conv = re.match(r'\?\w+@@Y(.)', s).group(1)
    if conv != "I": st, why = "not-candidate", {"G": "self is __stdcall", "A": "self is __cdecl"}.get(conv, "conv " + conv)
    elif not m: st, why = "not-candidate", "class or method name contains `_` (v3 regex)"
    elif len(bym[(meth, cls)]) != 1: st, why = "not-candidate", f"{len(bym[(meth, cls)])} PDB symbols for {cls}::{meth}"
    elif not bym[(meth, cls)][0].endswith("XZ"): st, why = "not-candidate", "PDB member takes parameters"
    elif t in F: st, why = "flat4", F[t][1] + ": " + F[t][4]
    else: st, why = "candidate-not-in-flat4", "-"
    g = G.get(t, ["", "?", "", "", "?", "?", "?"])
    out.append((t, st, why, g[1], g[4], g[6], g[5]))
with open(sys.argv[4], "w") as o:
    o.write("address\tv3_stage\tv3_reason\tv4_verdict\tv4_route\tv4_bucket\tv4_reason\n" + "".join("\t".join(r) + "\n" for r in out))
c = collections.Counter(re.sub(r'\d+ PDB', 'N PDB', r[2]) if r[1] == "not-candidate" else r[1] + ": " + re.sub(r'[A-Z]\w+::\w+|engine/\w+\.h|\b[CN][A-Z]\w+|[0-9a-f]{8}', 'X', r[2])[:80] for r in out)
for k, v in c.most_common(): print(v, k)
