#!/usr/bin/env python3
"""Coordinator: land the ACCEPTed rows of a scripted naming route (flatten_pilot.py / access_fix.py run directory).
usage (repo root, lane env sourced, S set): python3 -u land_scripted.py <run dir> <batch name> <queue tag> [--fix-unsigned-pad] [--release-t3 TAG[,TAG...]]
--release-t3 TAG[,TAG...]: a T3_PORT_QUEUE.tsv line of a source TAG whose column-2 address is an accepted row of this run, and which is that address's
  only mention anywhere in the queue, does not reserve it (an agent pass the coordinator sent into those lines); the lines of the rows that
  land are closed: removed from the queue and kept in the batch's T3_CLOSED.tsv. An address with several lines is released when EVERY line
  mentioning it (column 2 or the unit column) has a released TAG and the address in column 2; all of them close with the row (NF339 D6).
Steps, in order (nothing is committed; the coordinator writes the README / HANDOFF / claims line and commits):
  1. reservations: an accepted address is HELD when it is in T3_PORT_QUEUE.tsv's address column or a unit column (NF316 Q3), under a live
     lane lock ($S/w22/nflocks/<addr>/owner not `released`), or named by an open (not done / released) COORDINATION_CLAIMS.tsv line;
  2. lock the rest as COORD and copy <run>/rows/<addr>.cpp over the landed source;
     --fix-unsigned-pad restores `unsigned char` on a pad that an older access_fix.py (before 2026-11-23 evening) shrank as `char`;
  3. recheck.py + vland --recheck-landed on the landed tree (timeout 900), and the graded symbol of every object must be the run's new
     symbol; any failure restores that row's HEAD source and holds it;
  4. a binder run (access_fix.py --binders) also applies its TABLE_CHANGES.tsv for the landed callers (apply_table.py);
  5. write rebuild/candidates/manual/batches/<batch>/ (RESULTS.tsv, rows.txt, rows_held.txt, logs/recheck.log), append the run's REJECT
     rows to T3_PORT_QUEUE.tsv as <queue tag>, release the locks (naming_batch/clean.py).
A unit_fix.py run carries UNITS.tsv (address, unit): a unit lands whole or not at all (held or failed member -> every member restored / held)."""
import os, re, sys, json, shutil, subprocess, contextlib, io
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
import verify_and_land as V
run, batch, tag = argv[0], argv[1], argv[2]
S = os.environ["S"]; LT = "tools/decomp_pipeline/linux_lane/lane_tools"
ent = ns.landed_rows()
res = [l.split("\t") for l in open(f"{run}/RESULTS.tsv").read().splitlines()[1:]]
acc = {r[0]: r for r in res if r[1] == "ACCEPT"}
q = [l.split("\t") for l in open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv").read().splitlines()[1:]]
rel_tag = set(argv[argv.index("--release-t3") + 1].split(",")) if "--release-t3" in argv else None  # one tag or a comma list
mentions = {}
for i, r in enumerate(q):
    if len(r) > 2:
        for x in {r[1].lower().replace("0x", "")} | {x.lower() for x in re.findall(r"0x([0-9a-fA-F]{8})", r[2])}: mentions.setdefault(x, []).append(i)
released = {}                                                   # address -> its queue line indices (released for this run)
if rel_tag:
    for a in acc:
        m = mentions.get(a, [])
        if m and all(q[i][0] in rel_tag and q[i][1].lower().replace("0x", "") == a for i in m): released[a] = m
t3 = {a for a in mentions if a not in released}
held = {}
for a in acc:
    o = f"{S}/w22/nflocks/{a}/owner"
    if a in t3: held[a] = "T3_PORT_QUEUE"
    elif os.path.exists(o) and open(o).read().strip() != "released": held[a] = "lock " + open(o).read().strip()
# COORDINATION.md section 2: the newest line for an item wins, so a claim closed by a later `done` / `released` line no longer holds.
newest = {}
for l in open("rebuild/candidates/manual/COORDINATION_CLAIMS.tsv").read().splitlines():
    f = l.split("\t")
    if len(f) > 4: newest[(f[1], f[2])] = l
for l in newest.values():
    if l.split("\t")[4] not in ("done", "released"):
        for a in acc:
            if a in l.lower() and a not in held: held[a] = "open claim"
# staleness: the source a run generated its candidate from must still be the landed source (a later landing may have changed it); the run's
# HEAD payloads are <run>/head/payload.json, or for a unit run (<work>/run) <work>/self/head and <work>/bind/head
import glob as _g
heads = {}
for p in _g.glob(f"{run}/head/payload.json") + _g.glob(f"{os.path.dirname(run.rstrip('/'))}/*/head/payload.json"):
    for x in json.load(open(p))["result"]["authored"]: heads.setdefault(x["address"], x["source_cpp"])
for a in acc:
    if a in held or a not in ent: continue
    if a not in heads: held[a] = "stale check: no HEAD payload for the row"; continue
    if open("rebuild/src/compiled/" + ent[a]["Source"], encoding="utf-8").read() != heads[a]: held[a] = "stale: the landed source changed since the run"
# a unit run (unit_fix.py) lands whole or not at all: one held member holds its unit
unit = {}
if os.path.exists(f"{run}/UNITS.tsv"):
    unit = {l.split("\t")[0]: l.split("\t")[1] for l in open(f"{run}/UNITS.tsv").read().splitlines()[1:] if l}
def hold_units(why_of):
    for u in {unit[a] for a in why_of if a in unit}:
        for a in acc:
            if unit.get(a) == u and a not in held: held[a] = f"unit {u} member held"
hold_units(dict(held))
land = sorted(a for a in acc if a not in held)
orig = {}
for a in land:
    d = f"{S}/w22/nflocks/{a}"; os.makedirs(d, exist_ok=True); open(d + "/owner", "w").write("COORD\n")
    dst = "rebuild/src/compiled/" + ent[a]["Source"]; orig[a] = open(dst, encoding="utf-8").read()
    new = open(f"{run}/rows/{a}.cpp", encoding="utf-8").read()
    if "--fix-unsigned-pad" in argv:
        for m in re.finditer(r"\bunsigned\s+char\s+(\w+)\s*\[", orig[a]):
            new = re.sub(r"(?<!unsigned )\bchar(\s+" + m.group(1) + r"\s*\[)", r"unsigned char\1", new)
    open(dst, "w", encoding="utf-8").write(new)
W = f"{S}/land_{batch}"; os.makedirs(f"{W}/p", exist_ok=True)
subprocess.run(["python3", f"{LT}/recheck.py", f"{W}/p"] + land, capture_output=True, text=True)
with open(f"{W}/recheck.log", "w") as lf:
    subprocess.run(["timeout", "900", "python3", "-u", f"{LT}/vland.py", f"{W}/p/payload.json", f"{W}/p/oracle.tsv", "--recheck-landed"],
                   stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{W}/wd"))
st = {}
for l in open(f"{W}/recheck.log", errors="replace"):
    m = re.match(r"([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
    if m and m.group(2) != "WARN": st.setdefault(m.group(1), (m.group(2), m.group(3)))
bad = []
for a in land:
    g = st.get(a, ("NOT_GRADED", "-")); o = f"{W}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"; sym = ""
    if os.path.exists(o):
        with contextlib.redirect_stderr(io.StringIO()): sym = V.obj_text(o, acc[a][3])[2]
    binder = "; target " in acc[a][4] or (a in unit and unit[a] != a)          # access_fix --binders: a caller keeps its own symbol; its proof is objsec + grade
    # A leaf in an anonymous namespace is written `...@?A0x` (a prefix): its 8-hex hash depends on the build path (landing nf365 g2).
    anon = acc[a][3].endswith("?A0x") and re.sub(r"(\?A0x)[0-9a-f]{8}.*", r"\1", sym) == acc[a][3]
    if not (g[0].startswith(("MATCH", "RELOCATION_MATCH")) and g[1] == "PASS" and (binder or sym == acc[a][3] or anon)):
        bad.append(a); held[a] = f"landing recheck {g} symbol {sym}"
        open("rebuild/src/compiled/" + ent[a]["Source"], "w", encoding="utf-8").write(orig[a])
for u in {unit[a] for a in bad if a in unit}:
    for a in land:
        if unit.get(a) == u and a not in bad:
            bad.append(a); held[a] = f"unit {u}: a member failed the landing recheck"
            open("rebuild/src/compiled/" + ent[a]["Source"], "w", encoding="utf-8").write(orig[a])
land = [a for a in land if a not in bad]
B = f"rebuild/candidates/manual/batches/{batch}"; os.makedirs(f"{B}/logs", exist_ok=True)
shutil.copyfile(f"{run}/RESULTS.tsv", f"{B}/RESULTS.tsv"); shutil.copyfile(f"{W}/recheck.log", f"{B}/logs/recheck.log")
open(f"{B}/rows.txt", "w").write("".join(a + "\n" for a in land))
open(f"{B}/rows_held.txt", "w").write("".join(f"{a}\t{why}\n" for a, why in sorted(held.items())))
# a binder run's table changes, for the landed callers only (naming_batch/apply_table.py; nothing is written if any line misses)
if os.path.exists(f"{run}/TABLE_CHANGES.tsv"):
    tl = open(f"{run}/TABLE_CHANGES.tsv").read().splitlines()
    keep = [tl[0]] + [l for l in tl[1:] if l.split("\t")[3] in land]
    open(f"{B}/TABLE_CHANGES.tsv", "w").write("\n".join(keep) + "\n")
    ap = subprocess.run(["python3", f"{LT}/naming_batch/apply_table.py", f"{B}/TABLE_CHANGES.tsv"], capture_output=True, text=True)
    print("table:", ap.stdout.strip().replace("\n", " | "), ap.stderr.strip()[-300:])
if released:
    closed = {i for a in land if a in released for i in released[a]}
    qp = "rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv"; ql = open(qp).read().splitlines()
    open(f"{B}/T3_CLOSED.tsv", "w").write(ql[0] + "\n" + "".join(ql[i + 1] + "\n" for i in sorted(closed)))
    open(qp, "w").write("\n".join(l for k, l in enumerate(ql) if k - 1 not in closed) + "\n")
    print("t3 closed:", len(closed))
rej = [r for r in res if r[1] == "REJECT"]
if rej:
    with open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv", "a") as o:
        for r in rej: o.write(f"{tag}\t0x{r[0].upper()}\tself respell\t{tag} measured {r[2]} -> {r[3]} ({r[4][:70]})\n")
for a in acc:
    if os.path.exists(f"{S}/w22/nflocks/{a}/owner") and open(f"{S}/w22/nflocks/{a}/owner").read().strip() == "COORD":
        subprocess.run(["python3", f"{LT}/naming_batch/clean.py", f"{S}/w22/nflocks", f"{S}/w22/nflocks/{a}"], capture_output=True)
print(json.dumps(dict(accepted=len(acc), landed=len(land), held=len(held), recheck_failed=len(bad), rejects_queued=len(rej))))
