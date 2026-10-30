#!/usr/bin/env python3
"""bind_add.py [--apply] <mcheck.log>...: for each RELOC_RENAMED pair whose new spelling is unbound and whose old spelling
is bound to exactly one address, append `new<TAB>addr<TAB>row` to rebuild/oracles/reloc_bindings.tsv (the retail spelling
of that address). Records every addition in $S/mig6/work/bind_add.tsv."""
import sys, os, re, collections
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
P = R + "/rebuild/oracles/reloc_bindings.tsv"
lines = open(P).read().splitlines()
bind = collections.defaultdict(set)
for l in lines[1:]:
    q = l.split("\t")
    if len(q) >= 2: bind[q[0]].add(q[1].lower())
add = {}
for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) < 3 or q[1] != "RELOC_RENAMED": continue
        for m in re.finditer(r"[0-9a-f]+:(\S+)->(\S+)", q[2]):
            o, n = m.group(1), m.group(2)
            if n in bind or n in add: continue
            if len(bind.get(o, ())) == 1: add[n] = (list(bind[o])[0], q[0], o)
for n, (a, row, o) in sorted(add.items()): print(f"{n}\t{a}\t{row}\t(from {o})")
if "--apply" in sys.argv and add:
    with open(P, "a") as fh:
        for n, (a, row, o) in sorted(add.items()): fh.write(f"{n}\t{a}\t{row}\n")
    with open(S + "/mig6/work/bind_add.tsv", "a") as fh:
        for n, (a, row, o) in sorted(add.items()): fh.write(f"{n}\t{a}\t{row}\t{o}\n")
