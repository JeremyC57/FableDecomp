#!/usr/bin/env python3
"""bind_ren.py [--apply] <mcheck log>...: for each RELOC_RENAMED relocation pair old->new of a row (not leafsym), the
binding line `old<TAB>target<TAB>row` becomes `new<TAB>target<TAB>row` (same retail target: the bytes and the
relocation offset are unchanged, only the spelling the row emits). A pair whose old spelling has no line for that row
is reported. Records each change in $S/mig9/bind_add.tsv (symbol, address, row, from)."""
import sys, os, re
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
P = R + "/rebuild/oracles/reloc_bindings.tsv"
lines = open(P).read().split("\n")
idx = {}
for i, l in enumerate(lines[1:], 1):
    q = l.split("\t")
    if len(q) >= 3: idx[(q[0], q[2].lower())] = i
ch = []; miss = []
latest = {}
for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and len(q[0]) == 8: latest[q[0]] = q
for a, q in latest.items():
    if q[1] != "RELOC_RENAMED" or q[2].startswith("leafsym"): continue
    for m in re.finditer(r"[0-9a-f]+:(\S+)->(\S+)", q[2]):
        o, n = m.group(1), m.group(2)
        k = (o, a)
        if k not in idx:
            if (n, a) in idx: continue
            miss.append((a, o, n)); continue
        i = idx.pop(k); t = lines[i].split("\t")
        lines[i] = "\t".join([n] + t[1:]); idx[(n, a)] = i
        ch.append((n, t[1], a, o))
for c in ch: print("bind", *c, sep="\t")
for m in miss: print("MISS", *m, sep="\t")
if "--apply" in sys.argv:
    open(P, "w").write("\n".join(lines))
    with open(S + "/mig9/bind_add.tsv", "a") as fh:
        for c in ch: fh.write("\t".join(c) + "\n")
