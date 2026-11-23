#!/usr/bin/env python3
"""Measure each landed row's own compiled symbol (the naming tracker's "unmeasured" rows; docs/journal/2026-11/2026-11-23_naming_remainder.md).
usage (repo root, lane env sourced, S set):  python3 -u self_symbols.py <out.tsv> <workdir> [--chunk 600] [--all] [--max N]
Picks the landed rows whose `// Leaf:` is not a full mangled symbol (all landed rows with --all), runs recheck.py + vland --recheck-landed
in chunks (each under `timeout 900`), reads the graded symbol of every row's object with verify_and_land.obj_text (the symbol the
grade itself compared: the Leaf's match, else the largest function), appends `address  symbol  status  leaf_given` and cleans the chunk's
build dir with naming_batch/clean.py. Resumable: addresses already in <out.tsv> are skipped. Writes nothing under the repo but <out.tsv>
if that path is in the repo."""
import os, re, subprocess, sys, contextlib, io
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
import verify_and_land as V
out, work = argv[0], argv[1]
chunk = int(argv[argv.index("--chunk") + 1]) if "--chunk" in argv else 600
LT = "tools/decomp_pipeline/linux_lane/lane_tools"
ent = ns.landed_rows()
by = ns.table_spellings(open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read())
want = []
for t in sorted(ent):
    sp = {}
    for s, c in by.get(t, {}).items(): sp.setdefault(ns.norm(s), set()).update(c)
    _, how = ns.resolve_leaf(ns.leaf_of(ent[t]["Source"]), sp)
    if "--all" in argv or how != "full": want.append(t)
done = set()
if os.path.exists(out):
    done = {l.split("\t")[0] for l in open(out).read().splitlines()[1:]}
else:
    open(out, "w").write("address\tsymbol\tstatus\tleaf_given\n")
todo = [t for t in want if t not in done]
if "--max" in argv: todo = todo[:int(argv[argv.index("--max") + 1])]
print(f"rows to measure {len(want)}, done {len(done)}, left {len(todo)}", flush=True)
os.makedirs(work, exist_ok=True)
for i in range(0, len(todo), chunk):
    part = todo[i:i + chunk]; d = os.path.join(work, f"c{i // chunk:03d}"); os.makedirs(d, exist_ok=True)
    subprocess.run(["python3", f"{LT}/recheck.py", d] + part, capture_output=True, text=True)
    import json
    pay = json.load(open(f"{d}/payload.json"))["result"]["authored"]
    log = f"{d}/vland.log"
    with open(log, "w") as lf:
        subprocess.run(["timeout", "900", "python3", "-u", f"{LT}/vland.py", f"{d}/payload.json", f"{d}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{d}/wd"))
    st = {}
    for l in open(log, errors="replace"):
        m = re.match(r"([0-9a-f]{8})\s+([A-Z_]+[A-Z_()0-9v]*)\s", l)
        if m and m.group(2) not in ("WARN",): st.setdefault(m.group(1), m.group(2))
    rows = []
    for e in pay:
        a = e["address"]; obj = f"{d}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"; sym = ""
        if os.path.exists(obj):
            try:
                with contextlib.redirect_stderr(io.StringIO()): sym = V.obj_text(obj, e.get("leaf", ""))[2]
            except Exception as ex: sym = f"ERROR:{type(ex).__name__}"
        rows.append(f"{a}\t{sym}\t{st.get(a, 'NOT_GRADED')}\t{e.get('leaf', '')}")
    with open(out, "a") as o: o.write("\n".join(rows) + "\n")
    subprocess.run(["python3", f"{LT}/naming_batch/clean.py", work, f"{d}/wd"], capture_output=True)
    print(f"chunk {i // chunk}: {len(part)} rows, {sum(1 for r in rows if r.split(chr(9))[1])} symbols", flush=True)
