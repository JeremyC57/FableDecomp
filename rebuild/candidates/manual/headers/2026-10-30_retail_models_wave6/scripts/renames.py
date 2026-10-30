#!/usr/bin/env python3
"""renames.py <mcheck.log>...: for every RELOC_RENAMED pair old->new, the retail address each spelling is bound to in
rebuild/oracles/reloc_bindings.tsv. Prints: addr old new oldaddr newaddr verdict (SAMEADDR | NEWUNBOUND | CONFLICT | LABEL)."""
import sys, os, re, collections
R = os.environ["FABLE_ROOT"]
bind = collections.defaultdict(set)
for l in open(R + "/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) >= 2: bind[q[0]].add(q[1].lower())
pairs = collections.Counter(); rows = collections.defaultdict(set)
for f in sys.argv[1:]:
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) < 3 or q[1] != "RELOC_RENAMED": continue
        for m in re.finditer(r"[0-9a-f]+:(\S+)->(\S+)", q[2]):
            pairs[(m.group(1), m.group(2))] += 1; rows[(m.group(1), m.group(2))].add(q[0])
        if "count" in q[2] or "leafsym" in q[2]: print(q[0], "OTHER", q[2][:200])
for (o, n), c in sorted(pairs.items()):
    oa, na = bind.get(o, set()), bind.get(n, set())
    if o.startswith("$L") or "?A0x" in o: v = "LABEL"
    elif not oa: v = "OLDUNBOUND"
    elif na and na == oa: v = "SAMEADDR"
    elif not na: v = "NEWUNBOUND"
    else: v = "CONFLICT"
    print(f"{v}\t{c}\t{o}\t{n}\t{','.join(sorted(oa))}\t{','.join(sorted(na))}\t{','.join(sorted(rows[(o,n)]))[:120]}")
