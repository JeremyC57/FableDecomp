#!/usr/bin/env python3
"""bind_new.py [--apply] <mcheck log>...: bindings are keyed by symbol (symbol -> retail target; col 3 is provenance).
For each RELOC_RENAMED relocation pair old->new of a row (latest status per row; leafsym renames ignored): the new
spelling must bind the old one's target. Already bound to that target: nothing to do. Bound elsewhere: CONFLICT (the row
must be reverted). Unbound: append `new<TAB>target<TAB>row` (old lines stay: unmigrated rows may still emit them).
Records each addition in $S/mig9/bind_add.tsv (symbol, address, row, from)."""
import sys, os, re, collections
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
P = R + "/rebuild/oracles/reloc_bindings.tsv"
bind = collections.defaultdict(set)
for l in open(P).read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) >= 2: bind[q[0]].add(q[1].lower())
latest = {}
for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and len(q[0]) == 8: latest[q[0]] = q
add = {}; bad = 0
for a, q in sorted(latest.items()):
    if q[1] != "RELOC_RENAMED" or q[2].startswith("leafsym"): continue
    for m in re.finditer(r"[0-9a-f]+:(\S+)->(\S+)", q[2]):
        o, n = m.group(1), m.group(2)
        if o.startswith("$L") and n.startswith("$L"): continue
        if len(bind.get(o, ())) != 1: print("UNBOUND-OLD", a, o); bad += 1; continue
        t = list(bind[o])[0]
        if n in bind:
            if bind[n] != {t}: print("CONFLICT", a, n, sorted(bind[n]), t); bad += 1
            continue
        if n in add and add[n][0] != t: print("CONFLICT", a, n, add[n][0], t); bad += 1; continue
        add.setdefault(n, (t, a, o))
for n, (t, a, o) in sorted(add.items()): print("add", n, t, a, o, sep="\t")
print("additions", len(add), "problems", bad)
if "--apply" in sys.argv and add:
    txt = open(P).read()
    if not txt.endswith("\n"): txt += "\n"
    txt += "".join(f"{n}\t{t}\t{a}\n" for n, (t, a, o) in sorted(add.items()))
    open(P, "w").write(txt)
    with open(S + "/mig9/bind_add.tsv", "a") as fh:
        for n, (t, a, o) in sorted(add.items()): fh.write(f"{n}\t{t}\t{a}\t{o}\n")
