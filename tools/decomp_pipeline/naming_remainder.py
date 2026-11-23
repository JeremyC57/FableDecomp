#!/usr/bin/env python3
# Run from the repo root: python3 tools/decomp_pipeline/naming_remainder.py
"""Naming remainder analysis (coordinator, 2026-11-23). Re-derives the tracker's per-address status, then measures the unmeasured rows
from the committed compile gate's object_symbol column, the STLport-only split, and BSim precision on agree_pdb addresses."""
import sys, os, re, collections
sys.path.insert(0, "tools/decomp_pipeline"); sys.argv = ["x"]
import naming_status as ns
pdb = ns.pdb_names(); ent = ns.landed_rows()
by = ns.table_spellings(open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read())
leaf = {ad: ns.leaf_of(e["Source"]) for ad, e in ent.items()}
status = {}; spell = {}
for t in sorted(set(by) | set(ent)):
    sp = {}
    for s, c in by.get(t, {}).items(): sp.setdefault(ns.norm(s), set()).update(c)
    sl, how = ns.resolve_leaf(leaf.get(t, ""), sp)
    if sl: sp.setdefault(sl, set()).add("self")
    spell[t] = sp
    if not sp: status[t] = "unmeasured"; continue
    names = sorted(sp); pd = [s for s in names if s in pdb]
    if len(names) == 1:
        s = names[0]; status[t] = "agree_pdb" if s in pdb else ("standin" if ns.STANDIN.search(s) else ("agree_std" if ns.is_std(s) else "agree_unverified"))
    else: status[t] = "split_fixable" if len(pd) == 1 else ("split_pdb_multi" if len(pd) > 1 else "split_unknown")
print("tracker statuses", collections.Counter(status.values()))
# 1. compile-gate self symbols for unmeasured rows
gate = {}
for l in open("rebuild/compile-gate/retail-parity.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    c = l.split("\t")
    if len(c) > 5 and c[5]: gate[c[0].lower()] = c[5]
um = [t for t, s in status.items() if s == "unmeasured"]
hit = [t for t in um if t in gate]
cls = collections.Counter()
for t in hit:
    s = gate[t]
    cls["pdb" if s in pdb else ("standin" if ns.STANDIN.search(s) else ("std" if ns.is_std(s) else ("unmangled" if not s.startswith("?") else "unverified")))] += 1
print("unmeasured", len(um), "with a gate object_symbol", len(hit), cls)
# 2. STLport-only spellings among open
openst = ("standin", "split_unknown", "split_fixable")
stl = [t for t, s in status.items() if s in openst and spell[t] and all("@std@@" in x for x in spell[t])]
print("open addresses whose every spelling is an STLport std:: form", len(stl), collections.Counter(status[t] for t in stl))
# 3. BSim precision on agree_pdb
bs = {}
for f in ("ghidra_out/bsim_port_audit_final.tsv", "ghidra_out/bsim_port_audit_05final.tsv"):
    for l in open(f, errors="replace"):
        if l.startswith("#"): continue
        c = l.rstrip("\n").split("\t")
        if len(c) < 5: continue
        try: sim, sig = float(c[3]), float(c[4])
        except ValueError: continue
        a = c[0].lower().zfill(8)
        if a not in bs or sig > bs[a][2]: bs[a] = (c[2], sim, sig)
for thr in (5, 10, 20):
    ok = bad = 0
    for t, s in status.items():
        if s != "agree_pdb" or t not in bs: continue
        n, sim, sig = bs[t]
        if sim < 0.95 or sig < thr or "@std@@" in n or n not in pdb: continue
        if n in spell[t]: ok += 1
        else: bad += 1
    nopen = sum(1 for t, s in status.items() if s in ("standin", "split_unknown") and t in bs and bs[t][1] >= 0.95 and bs[t][2] >= thr
                and "@std@@" not in bs[t][0] and bs[t][0] in pdb)
    print("BSim sim>=0.95 sig>=%d (no std): precision on agree_pdb %d/%d = %.1f%%; open standin/split_unknown it names: %d" % (thr, ok, ok + bad, 100.0 * ok / max(1, ok + bad), nopen))
# 4. what the unverified gate symbols look like: same name+class as a PDB name (signature differs) or a name absent from the PDB
def nc(s):
    m = re.match(r'\?([^@]+)@([^@]+)@', s); return (m.group(1), m.group(2)) if m else None
pnc = collections.Counter(nc(p) for p in pdb if p.startswith("?"))
kinds = collections.Counter(); ex = collections.defaultdict(list)
for t in hit:
    s = gate[t]
    if s in pdb or ns.STANDIN.search(s) or ns.is_std(s) or not s.startswith("?"): continue
    k = nc(s)
    kind = "name+class in PDB (signature / access differs)" if k and pnc[k] else ("free function / ctor-dtor form" if not k else "name+class absent from PDB")
    kinds[kind] += 1; ex[kind].append((t, s))
print(kinds)
for k, v in ex.items(): print(" ", k, [x[1][:70] for x in v[:4]])
# 5. flattened `Class_Method` free functions: is the member target derivable from the name alone?
bymeth = collections.defaultdict(list)
for p in pdb:
    k = nc(p)
    if k: bymeth[k].append(p)
classes = set(k[1] for k in bymeth)
res = collections.Counter(); exs = collections.defaultdict(list)
for t in hit:
    s = gate[t]
    m = re.match(r'\?([A-Za-z0-9]+)_([A-Za-z0-9_]+)@@Y', s)
    if not m: continue
    c, meth = m.group(1), m.group(2)
    if c not in classes: res["class not in PDB"] += 1; continue
    cands = bymeth.get((meth, c), [])
    r = "unique PDB member" if len(cands) == 1 else ("several overloads" if cands else "no such member")
    res[r] += 1; exs[r].append((t, s[:60], cands[:2]))
print("flattened Class_Method free functions:", sum(res.values()), res)
for k in ("unique PDB member", "several overloads", "no such member"):
    for e in exs[k][:3]: print("  ", k, e[0], e[1], [x[:60] for x in e[2]])
# how many of these flattened rows have no table binder at all (only the self)?
nob = sum(1 for t in hit if re.match(r'\?[A-Za-z0-9]+_[A-Za-z0-9_]+@@Y', gate[t]) and not by.get(t))
print("flattened rows with no table binder:", nob)
