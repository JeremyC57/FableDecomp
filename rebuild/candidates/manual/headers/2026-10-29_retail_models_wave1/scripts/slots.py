#!/usr/bin/env python3
"""slots.py <vtable-va> <nslots> [ClassMangledScope e.g. CScriptThing or CTimer@NScript]: per slot the retail address, the
best label, the matching mangled symbols in Ego_r.pdb / FableWin.pdb / Ego_d.pdb name lists (by exact mangled label, else by
`?<method>@<scope>@@` search), and the undname spelling. Output TSV on stdout."""
import sys, os, re, subprocess, collections
sys.argv, args = [sys.argv[0]], sys.argv[1:]
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vt
R = os.environ["FABLE_ROOT"]; S = os.environ["S"]
vtab = int(args[0], 16); n = int(args[1]); scope = args[2] if len(args) > 2 else None
names = {}
for k in ("egor", "fablewin", "egod"):
    s = collections.defaultdict(set)
    for l in open(f"{R}/ghidra_out/{k}_pdb_names.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2: s[q[1]].add(q[0])
    names[k] = s
allsyms = set().union(*[set(v) for v in names.values()])
def undname(ms):
    if not ms: return []
    out = subprocess.run(["wine", os.environ["FABLE_VC71"] + "/bin/undname.exe"] + ms, capture_output=True, text=True, timeout=120).stdout
    return re.findall(r'is :- "(.*)"', out)
rows = []
for i in range(n):
    p = vt.u32(vtab + 4 * i)
    if p is None or vt.sec_of(p) != ".text": break
    lab = vt.lab.get("%08x" % p, "")
    cands = []
    if lab.startswith("?") and lab in allsyms: cands = [lab]
    elif scope:
        meth = lab.split("::")[-1].split("(")[0] if "::" in lab else None
        if meth and not meth.startswith(("`", "sub_")):
            cands = sorted(x for x in allsyms if x.startswith(f"?{meth}@{scope}@@"))
    rows.append((i, p, lab, cands))
flat = [c for r in rows for c in r[3]]
un = dict(zip(flat, undname(flat))) if flat else {}
for i, p, lab, cands in rows:
    if not cands:
        print(f"{i}\t{p:08x}\t{lab}\t-\t-\t-\t-"); continue
    for c in cands:
        print(f"{i}\t{p:08x}\t{lab}\t{c}\t{'egor' if c in names['egor'] else ''}\t{'fablewin' if c in names['fablewin'] else ''}\t{un.get(c,'')}")
