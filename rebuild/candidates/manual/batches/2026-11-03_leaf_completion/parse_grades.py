# usage: parse_grades.py <plan.tsv> <out.tsv> <rdir>... -> one line per planned row: grade, behaviour, leaf hit, object symbols
import sys, re, csv, glob, subprocess, os
plan = {r["address"]: r for r in csv.DictReader(open(sys.argv[1]), delimiter="\t")}
out = sys.argv[2]; rdirs = sys.argv[3:]
grade, warn = {}, set()
G = re.compile(r"^([0-9a-f]{8})\s+([A-Z_]+(?:\(\d+v\d+\))?)\s+(\S+)\s+(.*)$")
W = re.compile(r"WARN leaf '(.*)' matches no symbol in ([0-9a-f]{8})")
for d in rdirs:
    for line in open(f"{d}/verify.log", errors="replace"):
        m = W.search(line)
        if m: warn.add(m.group(2)); continue
        m = G.match(line)
        if m and m.group(2) != "WARN" and m.group(1) in plan: grade[m.group(1)] = (m.group(2), m.group(3))
def objsyms(a):
    for d in rdirs:
        f = f"{d}/work/{a[:2]}/{a[2:4]}/{a}/{a}.obj"
        if os.path.exists(f):
            t = subprocess.run(["objdump", "-t", f], capture_output=True, text=True).stdout
            return [l.split()[-1] for l in t.splitlines() if "(ty   20)" in l and "(scl   2)" in l]
    return None
with open(out, "w") as fh:
    w = csv.writer(fh, delimiter="\t", lineterminator="\n")
    w.writerow(["address", "verdict", "grade", "behaviour", "completed_leaf_in_object", "result", "object_symbols_matching_old_prefix"])
    for a, r in sorted(plan.items()):
        g, b = grade.get(a, ("NOT_GRADED", "-"))
        syms = objsyms(a) or []
        hit = r["completed"] in syms or any(r["completed"] in s for s in syms)
        ok = g in ("MATCH", "RELOCATION_MATCH") and b == "PASS" and a not in warn and hit
        old = [s for s in syms if r["leaf"] in s]
        w.writerow([a, r["verdict"], g, b, "yes" if hit else "no", "HOLD" if ok else "FAIL", " ".join(old)])
