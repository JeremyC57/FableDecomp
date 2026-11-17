#!/usr/bin/env python3
"""promote_egod.py <batch> <agent-dir> <stage-dir> <audit-text> <addr8>...   (repo root)
Stages coordinator-verified rows of an egod author batch into pending/2026-11-17_egod_<batch>/ (src, test, hdrdiff, payload,
coordverify.log, notes, INDEX.tsv) and updates ../STATUS.tsv (MATCHED_STAGED, true length, folder) and ../NAME_FIXES.tsv."""
import csv, json, os, re, shutil, sys
b, A, ST, audit = sys.argv[1:5]; addrs = [x.lower() for x in sys.argv[5:]]
HERE = os.path.dirname(os.path.abspath(__file__)); RES = os.path.dirname(HERE); PEND = os.path.dirname(RES)
F = f"{PEND}/2026-11-17_egod_{b}"; rel = os.path.relpath(F, "/home/user/FableDecomp")
os.makedirs(F + "/src", exist_ok=True); os.makedirs(F + "/test", exist_ok=True)
log = open(ST + "/vland.log").read()
for a in addrs:
    if not re.search(rf"^{a}\s+(RELOCATION_MATCH|MATCH) PASS", log, re.M | re.I): sys.exit(f"FAILED {a}: no PASS line in {ST}/vland.log")
    shutil.copy(f"{A}/src/{a}.cpp", F + "/src/"); shutil.copy(f"{A}/test/{a}.cpp", F + "/test/")
if os.path.isdir(F + "/hdrdiff"): shutil.rmtree(F + "/hdrdiff")
shutil.copytree(ST + "/hdrdiff", F + "/hdrdiff"); shutil.copy(ST + "/vland.log", F + "/coordverify.log")
pl = json.load(open(A + "/payload.json"))["result"]["authored"]; pl = [r for r in pl if r["address"].lower().replace("0x", "") in addrs]
for r in pl:   # payload entries embed the source / test TEXT: refresh it from the files being staged
    a = r["address"].lower(); r["source_cpp"] = open(f"{A}/src/{a}.cpp").read(); r["test_cpp"] = open(f"{A}/test/{a}.cpp").read()
old = json.load(open(F + "/payload.json"))["result"]["authored"] if os.path.exists(F + "/payload.json") else []
old = [r for r in old if r["address"].lower() not in addrs] + pl
json.dump({"result": {"authored": old}}, open(F + "/payload.json", "w"), indent=1)
for n in ("NOTES.md", "LESSONS.md", "NAME_FIXES.tsv", "LABELS.tsv"):
    if os.path.exists(f"{A}/{n}"): shutil.copy(f"{A}/{n}", f"{F}/{n}")
if os.path.exists(A + "/hdr/README.md"): shutil.copy(A + "/hdr/README.md", F + "/HDR_README.md")
lens = {}
for l in open(A + "/oracle.tsv"):
    p = l.split("\t")
    if len(p) > 2 and p[2].isdigit(): lens[p[0].lower()] = int(p[2])
grade = {m.group(1).lower(): m.group(2) for m in re.finditer(r"^([0-9a-f]{8})\s+(RELOCATION_MATCH|MATCH) PASS", log, re.M | re.I)}
idx = F + "/INDEX.tsv"; rows = {}
if os.path.exists(idx): rows = {r["address"]: r for r in csv.DictReader(open(idx), delimiter="\t")}
for r in pl:
    a = r["address"].lower()
    rows[a] = {"address": a, "bytes": lens.get(a, ""), "module": r["module"], "name": r["name"], "state": "MATCHED_STAGED", "grade": grade.get(a, ""),
               "source": f"{rel}/src/{a}.cpp", "test": f"{rel}/test/{a}.cpp", "flags": " ".join(r.get("flags") or []), "headers": rel + "/hdrdiff", "audit": audit}
cols = ["address", "bytes", "module", "name", "state", "grade", "source", "test", "flags", "headers", "audit"]
with open(idx, "w") as f:
    f.write("\t".join(cols) + "\n")
    for a in sorted(rows): f.write("\t".join(str(rows[a][c]) for c in cols) + "\n")
with open(F + "/README.md", "w") as f:
    f.write(f"# egod lane hand-back {b}\n\nRows staged from author batch `{b}` (reserved list `../2026-11-17_egod_parked/`). Each was re-verified by the "
            "coordinator (`coordverify.log`: read-only vland, strict headers, minimal overlay = `hdrdiff/`) and rule-audited by an independent "
            "agent. `INDEX.tsv` lists the rows with true lengths and the audit notes; `NOTES.md` has what closed each row; `HDR_README.md` "
            "lists the header opt-ins (additions only). Nothing here is landed.\n")
L = RES + "/STATUS.tsv"; st = list(csv.reader(open(L), delimiter="\t"))
for r in st[1:]:
    if r[0] in addrs: r[3] = "MATCHED_STAGED"; r[4] = str(lens.get(r[0], "")); r[5] = rel; r[6] = f"batch {b}"
csv.writer(open(L, "w", newline=""), delimiter="\t", lineterminator="\n").writerows(st)
nf = RES + "/NAME_FIXES.tsv"; have = set()
if os.path.exists(nf): have = {l.split("\t")[0] for l in open(nf)}
else: open(nf, "w").write("address\tcorrect_symbol\tevidence\twrong_spellings_seen\tbatch\n")
if os.path.exists(A + "/NAME_FIXES.tsv"):
    with open(nf, "a") as f:
        for l in list(open(A + "/NAME_FIXES.tsv"))[1:]:
            if l.strip() and l.split("\t")[0] not in have: f.write(l.rstrip("\n") + f"\t{b}\n")
print("promoted", addrs, "->", rel)
