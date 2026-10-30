#!/usr/bin/env python3
"""leafaudit.py: every kept row whose latest mcheck line is a `leafsym OLD -> NEW` rename: is NEW a PDB spelling
(ghidra_out/egor_pdb_names.tsv or _fablewin_api.tsv)? If not, the PDB symbol with the same method and innermost class
(the nesting the row lacks) is printed as a NAME_FIXES candidate."""
import os, re, glob, subprocess
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; L = S + "/mig8/logs"
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
rows = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1) for f in files if f.startswith("rebuild/src/compiled/")}
last = {}
for f in sorted(glob.glob(L + "/mc_*.txt"), key=os.path.getmtime):
    for l in open(f):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 3 and q[0] in rows and q[1] in ("SAME", "RELOC_RENAMED"): last[q[0]] = q[2]
pdb = set()
for p in ("ghidra_out/egor_pdb_names.tsv", "ghidra_out/_fablewin_api.tsv"):
    for l in open(os.path.join(R, p), errors="replace"):
        for tok in l.rstrip("\n").split("\t"):
            if tok.startswith("?"): pdb.add(tok)
by = {}
for s in pdb:
    m = re.match(r"\?(\w+)@(\w+)@", s)
    if m: by.setdefault((m.group(1), m.group(2)), []).append(s)
for a, d in sorted(last.items()):
    m = re.search(r"leafsym (\S+) -> (\S+)", d)
    if not m: continue
    new = m.group(2); ok = new in pdb
    mm = re.match(r"\?(\w+)@(\w+)@", new)
    cand = by.get((mm.group(1), mm.group(2)), []) if mm else []
    print(a, "PDB" if ok else "NOTPDB", m.group(1), new, ";".join(sorted(cand)[:3]) if not ok else "", sep="\t")
