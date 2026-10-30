#!/usr/bin/env python3
"""ren_kind.py <mcheck log>...: classify every RELOC_RENAMED pair of the latest status per row against reloc_bindings.tsv:
same-target (old and new bound to one address), new-unbound (old bound once: bind_new adds it), conflict (new bound
elsewhere: revert), old-unbound, $L. Prints per-kind pattern counts and the rows with a conflict / old-unbound pair."""
import sys, os, re, collections
R = os.environ["FABLE_ROOT"]
bind = collections.defaultdict(set)
for l in open(R + "/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) >= 2: bind[q[0]].add(q[1].lower())
latest = {}
for f in sys.argv[1:]:
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and len(q[0]) == 8: latest[q[0]] = q
kinds = collections.Counter(); pat = collections.Counter(); bad = collections.defaultdict(list)
def shape(s): return re.sub(r"\?\$CCountedPointer@.*?@@@@", "?$CCountedPointer@<T>@@", re.sub(r"_[0-9a-f]{8}", "_<a>", s))[:90]
for a, q in sorted(latest.items()):
    if q[1] != "RELOC_RENAMED": continue
    if q[2].startswith("leafsym"): kinds["leafsym"] += 1; pat["leafsym"] += 1; continue
    rk = set()
    for m in re.finditer(r"[0-9a-f]+:(\S+)->(\S+)", q[2]):
        o, n = m.group(1), m.group(2)
        if o.startswith("$L") and n.startswith("$L"): k = "$L"
        elif len(bind.get(o, ())) != 1: k = "old-unbound"
        elif n in bind: k = "same-target" if bind[n] == bind[o] else "conflict"
        else: k = "new-unbound"
        rk.add(k); pat[(k, shape(o) + " -> " + shape(n))] += 1
        if k in ("conflict", "old-unbound"): bad[a].append(f"{k} {o}->{n}")
    if "count" in q[2]: rk.add("count-change"); bad[a].append("count change")
    for k in rk: kinds[k] += 1
print(dict(kinds))
for p, c in pat.most_common(40): print(c, p)
for a, l in sorted(bad.items()): print("BAD", a, " | ".join(l)[:300])
