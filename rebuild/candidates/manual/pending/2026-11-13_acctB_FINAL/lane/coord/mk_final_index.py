#!/usr/bin/env python3
# mk_final_index.py -- build pending/2026-11-13_acctB_FINAL/: one index of the best version of every acctB reserved row,
# one combined payload of the matched rows (no oracle bytes), and a map of every header the lane made or edited.
import csv, glob, hashlib, json, os, re, subprocess, sys
ROOT = "/home/user/FableDecomp"; P = "rebuild/candidates/manual/pending"; OUT = f"{ROOT}/{P}/2026-11-13_acctB_FINAL"
os.chdir(ROOT); os.makedirs(OUT, exist_ok=True)
res = list(csv.DictReader(open(f"{P}/2026-11-11_acctB/RESERVED.tsv"), delimiter="\t"))

# newest commit time of every tracked file under the acctB folders
ct = {}
log = subprocess.run(["git", "log", "--format=@%ct", "--name-only", "--", P], capture_output=True, text=True).stdout
t = 0
for l in log.splitlines():
    if l.startswith("@"): t = int(l[1:])
    elif l and "_acctB" in l and l not in ct: ct[l] = t

folders = sorted(d for d in glob.glob(f"{P}/*_acctB*") if os.path.isdir(d) and not d.endswith("_FINAL"))
status = {}      # (folder, addr) -> STATUS line dict
for d in folders:
    f = d + "/STATUS.tsv"
    if not os.path.exists(f): continue
    for r in csv.DictReader(open(f, errors="replace"), delimiter="\t"):
        a = (r.get("address") or "").lower().replace("0x", "")
        if a: status[(d, a)] = r

def grade_of(r):
    if not r: return ""
    for k in ("grade", "status"):
        v = (r.get(k) or "")
        m = re.match(r"(RELOCATION_MATCH|MATCH|DIFFER\S*|PARKED\S*|BLOCKED\S*)", v)
        if m: return m.group(1)
    return (r.get("grade") or r.get("status") or "")[:40]

wf = {}
try:
    for r in json.load(open(f"{P}/2026-11-13_acctB_parked/BWF2_WORKFLOW_RESULT.json")):
        if r: wf[r["addr"]] = r
except Exception: pass
parked = {r["address"]: r for r in csv.DictReader(open(f"{P}/2026-11-13_acctB_parked/STATUS.tsv"), delimiter="\t")}

def payload_row(d, a):
    for p in (d + "/payload.json",):
        if os.path.exists(p):
            try:
                for c in json.load(open(p))["result"]["authored"]:
                    if c["address"].lower().replace("0x", "") == a: return c
            except Exception: pass
    return None

rows = []; combined = []; n = dict(MATCHED=0, DRAFT=0, NO_DRAFT=0, NOT_AUTHORED=0)
for r in res:
    a = r["address"].lower(); ln = r["length_estimate"]
    cands = [(ct.get(f"{d}/rows/{a}.cpp", 0), d) for d in folders if os.path.exists(f"{d}/rows/{a}.cpp")]
    matched = [(t, d) for t, d in cands if grade_of(status.get((d, a))) in ("RELOCATION_MATCH", "MATCH")]
    pk = [(ct.get(f"{d}/rows/parked/{a}.cpp", 0), d) for d in folders if os.path.exists(f"{d}/rows/parked/{a}.cpp")]
    e = dict(address=a, bytes=ln, module=r["catalog_module"], name=r["catalog_name"], state="", grade="", best_source="", test="", headers="", notes="",
             older_copies="", open_items="")
    if matched:
        t, d = max(matched); st = status[(d, a)]; e["state"] = "MATCHED"; e["grade"] = grade_of(st)
        e["best_source"] = f"{d}/rows/{a}.cpp"; e["test"] = f"{d}/rows/{a}.test.cpp" if os.path.exists(f"{d}/rows/{a}.test.cpp") else ""
        e["headers"] = f"{d}/hdr/{a}" if os.path.isdir(f"{d}/hdr/{a}") else (f"{d}/hdr" if os.path.isdir(d + "/hdr") else "")
        e["notes"] = f"{d}/notes/{a}" if os.path.isdir(f"{d}/notes/{a}") else f"{d}/README.md"
        e["older_copies"] = " ".join(os.path.basename(x) for _, x in sorted(matched)[:-1])
        w = wf.get(a)
        items = []
        if w and w.get("status") == "MATCH_WITH_OPEN_ISSUES": items.append(f"{len(w.get('open_issues', []))} open verifier finding(s): see 2026-11-13_acctB_parked/README.md")
        note = " ".join(str(st.get(k) or "") for k in ("notes", "note", "flags"))
        if re.search(r"volatile", note) and "purity clean" not in note: items.append("uses volatile: owner ruling 2026-11-13 (portable + retail did it); check the Retail-volatile line")
        if re.search(r"RELOC_CONFLICT|conditional|CONDITIONAL|open", note): items.append("see the folder README for the open review item")
        e["open_items"] = "; ".join(items)
        c = payload_row(d, a)
        if c: combined.append(c)
        else: e["open_items"] = (e["open_items"] + "; " if e["open_items"] else "") + "no payload entry in the folder's payload.json: build it from rows/"
        n["MATCHED"] += 1
    elif a in parked:
        pr = parked[a]; d = f"{P}/2026-11-13_acctB_parked"
        if pr["draft_grade"] != "no draft":
            e["state"] = "DRAFT"; e["grade"] = pr["draft_grade"]; e["best_source"] = f"{d}/rows/parked/{a}.cpp"; n["DRAFT"] += 1
        else:
            e["state"] = "NO_DRAFT"; e["grade"] = "dossier+outline" if pr["dossier"] == "y" else "nothing yet"; n["NO_DRAFT"] += 1
        e["headers"] = f"{d}/hdr/{a}" if os.path.isdir(f"{d}/hdr/{a}") else ""; e["notes"] = f"{d}/notes/{a}"
        e["older_copies"] = " ".join(os.path.basename(x) for _, x in sorted(pk) if not x.endswith("_parked"))
    else:
        e["state"] = "NOT_AUTHORED"; n["NOT_AUTHORED"] += 1
        if pk:
            t, d = max(pk); e["best_source"] = f"{d}/rows/parked/{a}.cpp"; e["grade"] = grade_of(status.get((d, a))) or "parked"; e["notes"] = f"{d}/README.md"
        else:
            hits = [d for d in folders if (d, a) in status]
            e["notes"] = (hits[-1] + "/README.md") if hits else ""; e["grade"] = grade_of(status.get((hits[-1], a))) if hits else ""
        e["open_items"] = "blocked or a fragment of another function: see notes"
    rows.append(e)

cols = ["address", "bytes", "module", "name", "state", "grade", "best_source", "test", "headers", "notes", "older_copies", "open_items"]
with open(OUT + "/INDEX.tsv", "w") as f:
    f.write("\t".join(cols) + "\n")
    for e in rows: f.write("\t".join(str(e[c]).replace("\t", " ").replace("\n", " ") for c in cols) + "\n")
json.dump({"result": {"authored": combined}}, open(OUT + "/payload.json", "w"), indent=1)

# header map: every header file in any acctB overlay v the repo's rebuild/include
hm = {}
for d in folders:
    for h in glob.glob(d + "/hdr/**/*", recursive=True):
        if not os.path.isfile(h) or not h.endswith((".h", ".inl", ".hpp")): continue
        rel = h.split("/hdr/", 1)[1]; parts = rel.split("/")
        row = parts[0] if re.fullmatch(r"[0-9a-f]{8}", parts[0]) else ""
        inc = "/".join(parts[1:]) if row else rel
        sha = hashlib.sha1(open(h, "rb").read()).hexdigest()[:10]
        repo = f"rebuild/include/{inc}"
        kind = "new (not in rebuild/include)" if not os.path.exists(repo) else ("same as repo" if hashlib.sha1(open(repo, "rb").read()).hexdigest()[:10] == sha else "edited (differs from repo)")
        hm.setdefault(inc, []).append((os.path.basename(d), row, sha, kind, h))
best_hdr = {e["headers"] for e in rows if e["state"] == "MATCHED" and e["headers"]}
with open(OUT + "/HEADERS.tsv", "w") as f:
    f.write("header\tkind\tdistinct_versions\tused_by_best_matched_rows\tcopies (folder:row:sha)\n")
    for inc in sorted(hm):
        v = hm[inc]; kinds = sorted({k for _, _, _, k, _ in v})
        if kinds == ["same as repo"]: continue
        used = sorted({(row or folder) for folder, row, sha, k, h in v if any(h.startswith(b + "/") for b in best_hdr)})
        f.write(f"{inc}\t{' / '.join(kinds)}\t{len({s for _, _, s, _, _ in v})}\t{' '.join(used)}\t{' '.join(f'{fo}:{ro}:{s}' for fo, ro, s, _, _ in v)}\n")
print(n, "combined payload rows", len(combined), "headers", sum(1 for l in open(OUT + '/HEADERS.tsv')) - 1)
bad = [e["address"] for e in rows if e["state"] == "MATCHED" and not e["test"]]
print("matched without test file:", bad)
print("matched bytes", sum(int(e["bytes"]) for e in rows if e["state"] == "MATCHED"))
