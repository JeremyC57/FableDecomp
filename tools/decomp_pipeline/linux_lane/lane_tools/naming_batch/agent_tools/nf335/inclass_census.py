#!/usr/bin/env python3
"""inclass_census.py <addresses file> <PDB-symbol source: GEN.tsv>... (NF324, 2026-11-24; repo root): the new-name census (BRIEF_NF_BATCH)
for done rows. For each address, every landed source and test other than the row's own is searched for:
  - an in-class body of the member inside a local model of the class (`class|struct <Class> ... { ... <Method>(...) [const] { ... }`),
    which would emit a pick-any COMDAT of the same name that clashes with the strong self (GOTCHAS nf80 / nf86);
  - an out-of-line `<Class>::<Method>(...) {` body (another strong copy);
  - a `// Leaf:` naming the symbol.
Prints one line per hit (`HIT <addr> <file> <kind>`) and a count; no hits = the census is clean."""
import sys, re, glob, os
sys.path.insert(0, "tools/decomp_pipeline"); import naming_status as ns
addrs = set(open(sys.argv[1]).read().split())
P = {}
for g in sys.argv[2:]:
    for l in open(g).read().splitlines()[1:]:
        q = l.split("\t")
        if q[0] in addrs: P[q[0]] = q[3]
ent = ns.landed_rows()
files = glob.glob("rebuild/src/compiled/*/*/*.cpp") + glob.glob("rebuild/tests/*/*/*.cpp")
text = {f: re.sub(r'//[^\n]*|/\*.*?\*/', ' ', open(f, encoding="utf-8", errors="replace").read(), flags=re.S) for f in files}
raw_leaf = {}
for f in files:
    m = re.search(r'^//\s*Leaf:\s*(\S+)', open(f, encoding="utf-8", errors="replace").read(), re.M)
    if m: raw_leaf[f] = m.group(1)
def block_end(t, i):
    d = 1
    while d and i < len(t):
        d += {"{": 1, "}": -1}.get(t[i], 0); i += 1
    return i
by_cls = {}
for a, p in P.items():
    m = re.match(r'\?([^@]+)@([^@]+)@@', p); by_cls.setdefault(m.group(2), []).append((a, m.group(1), p))
hits = 0
for f, t in text.items():
    for cls, lst in by_cls.items():
        if cls not in t: continue
        own = {("rebuild/src/compiled/" + ent[a]["Source"]) for a, _, _ in lst if a in ent} | \
              {("rebuild/tests/" + ent[a].get("TestSource", "")) for a, _, _ in lst if a in ent}
        for a, meth, p in lst:
            if f in {"rebuild/src/compiled/" + ent[a]["Source"], "rebuild/tests/" + ent[a].get("TestSource", "")}: continue
            for d in re.finditer(r'\b(?:class|struct)\s+' + re.escape(cls) + r'\b[^;{()]*\{', t):
                body = t[d.end():block_end(t, d.end())]
                if re.search(r'\b' + re.escape(meth) + r'\s*\([^;{}()]*\)\s*(?:const\s*)?\{', body):
                    print("HIT", a, f, "in-class body in a local model"); hits += 1
            if re.search(r'\b' + re.escape(cls) + r'::' + re.escape(meth) + r'\s*\([^;{}]*\)\s*(?:const\s*)?\{', t):
                print("HIT", a, f, "out-of-line body"); hits += 1
            if raw_leaf.get(f) == p: print("HIT", a, f, "Leaf names the symbol"); hits += 1
print("census hits", hits, "rows", len(P))
