#!/usr/bin/env python3
"""lsb_T.py: map each NScript level script to its PDB master-data type T. Sources: (1) Ego_r.pdb names of nested entity
classes `@<Entity>@<Script>@NScript@@` + the entity layout's `PMasterData` type (struct_layouts_egor.tsv; an entity name
that occurs with two types is ignored); (2) the landed template rows `class X : public CLevelScriptBase<T>`.
Writes $S/mig9/work/lsb_T.tsv: script<TAB>T_pdb<TAB>n_entities<TAB>T_rows<TAB>agree."""
import os, re, glob, collections
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
pm = collections.defaultdict(set); cur = None
for l in open(R + "/ghidra_out/struct_layouts_egor.tsv"):
    q = l.rstrip("\n").split("\t")
    if q[0] == "@STRUCT": continue
    if len(q) >= 5 and q[4] == "PMasterData": pm[q[0]].add(q[3].replace(" *", "").strip())
nest = collections.defaultdict(set)
for f in ("egor_pdb_names.tsv", "fablewin_pdb_names.tsv"):
    for l in open(R + "/ghidra_out/" + f):
        for m in re.finditer(r"@(\w+)@(C\w+)@NScript@@", l):
            nest[m.group(2)].add(m.group(1))
rows = collections.defaultdict(set)
for f in glob.glob(R + "/rebuild/src/compiled/**/*.cpp", recursive=True):
    if f.endswith(".pre.cpp"): continue
    for m in re.finditer(r"(?:class|struct)\s+(\w+)\s*:\s*public\s+CLevelScriptBase<\s*(?:NScript::)?(\w+)\s*>", open(f, errors="replace").read()):
        rows[m.group(1)].add(m.group(2))
out = []
for s in sorted(set(nest) | set(rows)):
    ts = collections.Counter()
    for e in nest.get(s, ()):
        if len(pm.get(e, ())) == 1: ts[next(iter(pm[e]))] += 1
    tp = ",".join(sorted(ts)); tr = ",".join(sorted(rows.get(s, ())))
    if not tp and not tr: continue
    agree = "yes" if (tp and tr and tp == tr) else ("pdb" if tp and not tr else ("rows" if tr and not tp else "CONFLICT"))
    out.append(f"{s}\t{tp}\t{sum(ts.values())}\t{tr}\t{agree}")
open(S + "/mig9/work/lsb_T.tsv", "w").write("\n".join(out) + "\n")
print(collections.Counter(x.split("\t")[4] for x in out))
# --- source 3: retail RegisterScripts master-data alloc per script (work/regmap.tsv from regscripts.py + regmap.py)
DATA = {"ebp:cdbd20": "CQ_SunnyvaleMasterData", "cd4a60": "CalextestData", "cd4b80": "CDanyalTestScriptsData",
        "cd4b20": "Cjoss_test_scriptsData", "cd4ac0": "CEmptyData", "ebx:?": "(none)"}
reg = {}
for l in open(S + "/mig9/work/regmap.tsv"):
    q = l.rstrip("\n").split("\t")
    for c in q[3].split(","):
        if c: reg[c] = DATA.get(q[2], "?" + q[2])
def nz(s): return s.replace("_", "")
final = []
allc = set(nest) | set(rows) | set(x for x in [] )
names = set(l.split("\t")[0] for l in out)
for s in sorted(names | set(nest) | set(rows)):
    tp = ""; ts = collections.Counter()
    for e in nest.get(s, ()):
        if len(pm.get(e, ())) == 1: ts[next(iter(pm[e]))] += 1
    tp = ",".join(sorted(ts)); tr = ",".join(sorted(rows.get(s, ()))); tg = reg.get(nz(s), "")
    srcs = [x for x in (tp, tg) if x]
    final.append(f"{s}\t{tp}\t{tg}\t{tr}")
open(S + "/mig9/work/lsb_T3.tsv", "w").write("\n".join(final) + "\n")
