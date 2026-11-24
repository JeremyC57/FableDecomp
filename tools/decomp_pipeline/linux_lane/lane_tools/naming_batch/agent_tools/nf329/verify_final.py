#!/usr/bin/env python3
"""verify_final.py <agent folder> <final work/run>... (NF324, 2026-11-24): mechanical hand-back checks, read-only.
Each <final work/run> is a graded run dir such as work/fin12/runF (its work dir is the parent).
  - every `done` STATUS address is ACCEPT in exactly the LAST final run GRADE.tsv that has it;
  - rows/ file set == rows.txt, and rows/<a>.cpp is byte-equal to <work>/gen/<a>.cpp of that run (the graded source);
  - no done address is in work/holds.tsv;
  - the NF324 locks (S/w22/nflocks/*/owner) equal rows.txt exactly;
  - every done row's landed source still equals the HEAD copy the grade compiled (<work>/pre/<a>/head.cpp).
Prints one line per problem and a summary; exit 1 on any problem. Needs S exported. Run from the repo root."""
import os, sys, glob, filecmp
sys.path.insert(0, "tools/decomp_pipeline"); import naming_status as ns
A, runs = sys.argv[1], sys.argv[2:]
S = os.environ["S"]; bad = 0
def problem(*x):
    global bad; bad += 1; print("PROBLEM", *x)
last = {}
for r in runs:
    for l in open(f"{r}/GRADE.tsv").read().splitlines()[1:]:
        q = l.split("\t"); last[q[0]] = (q[1], os.path.dirname(r.rstrip("/")))
st = {l.split("\t")[0]: l.split("\t") for l in open(f"{A}/STATUS.tsv").read().splitlines()[1:]}
done = {a for a, q in st.items() if q[1] == "done"}
rows_txt = set(open(f"{A}/rows.txt").read().split())
rows_dir = {f[:-4] for f in os.listdir(f"{A}/rows") if f.endswith(".cpp")}
holds = {l.split("\t")[0] for l in open(f"{A}/work/holds.tsv").read().splitlines() if l.strip()} if os.path.exists(f"{A}/work/holds.tsv") else set()
ent = ns.landed_rows()
if done != rows_txt: problem("done != rows.txt", sorted(done ^ rows_txt)[:20])
if rows_dir != rows_txt: problem("rows/ != rows.txt", sorted(rows_dir ^ rows_txt)[:20])
for a in sorted(done):
    if a in holds: problem(a, "done but held")
    if a not in last or last[a][0] != "ACCEPT": problem(a, "not ACCEPT in the last final run:", last.get(a)); continue
    w = last[a][1]
    if not filecmp.cmp(f"{A}/rows/{a}.cpp", f"{w}/gen/{a}.cpp", shallow=False): problem(a, "rows/ copy != graded gen source", w)
    if not filecmp.cmp(f"{w}/pre/{a}/head.cpp", "rebuild/src/compiled/" + ent[a]["Source"], shallow=False):
        problem(a, "landed source changed since the grade", w)
mine = set()
for o in glob.glob(f"{S}/w22/nflocks/*/owner"):
    if open(o).read().strip() == "NF324": mine.add(os.path.basename(os.path.dirname(o)))
if mine != rows_txt:
    problem("locks != rows.txt: locked not done", sorted(mine - rows_txt)[:40], "done not locked", sorted(rows_txt - mine)[:40])
print(f"verify_final: done {len(done)}, rows {len(rows_dir)}, locks {len(mine)}, problems {bad}")
sys.exit(1 if bad else 0)
