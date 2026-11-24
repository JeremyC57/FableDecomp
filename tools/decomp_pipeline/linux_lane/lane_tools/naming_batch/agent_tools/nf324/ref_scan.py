#!/usr/bin/env python3
"""Which landed rows' objects reference a symbol? (2026-11-23) A table `replace W -> P` is only safe when no landed row still references
W; reloc_bindings.tsv keeps one witness per (symbol, target), so the other references must be found by compiling.
usage (repo root, lane env sourced, S set): python3 -u ref_scan.py <work> <symbols file> [--cap N]
<symbols file>: one mangled symbol per line (optionally `symbol<TAB>address,address` = rows to leave out, e.g. the callers already edited).
Candidates: landed sources naming the symbol's identifier as a whole word (more than --cap, default 80, is reported as TOO_MANY).
They are compiled at HEAD by vland under <work>/wd; a row references the symbol when its object contains the symbol's bytes.
Output <work>/REFS.tsv: symbol, status (REFS / NONE / TOO_MANY / NOT_COMPILED), addresses."""
import os, re, sys, json, subprocess
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
LT = "tools/decomp_pipeline/linux_lane/lane_tools"
work, symf = argv[0], argv[1]; cap = int(argv[argv.index("--cap") + 1]) if "--cap" in argv else 80
os.makedirs(work, exist_ok=True)
ent = ns.landed_rows()
text = {}
for a, e in ent.items():
    try: text[a] = open("rebuild/src/compiled/" + e["Source"], encoding="utf-8", errors="replace").read()
    except OSError: pass
syms = []
for l in open(symf).read().splitlines():
    if not l.strip(): continue
    f = l.split("\t"); syms.append((f[0], set(f[1].split(",")) if len(f) > 1 and f[1] else set()))
cand, status = {}, {}
for s, ex in syms:
    # the member's identifier, or for a special member (`??0X@`, `??_GX@`, an operator) the class's: a superset either way
    m = re.match(r'\?([^@?]+)@', s) or re.match(r'\?\?(?:_[0-9A-Z]|[0-9A-Z])([A-Za-z_]\w*)@', s) or re.match(r'_?([A-Za-z_]\w*)', s)
    if not m: status[s] = ("TOO_MANY", "no identifier"); continue
    w = re.compile(r'(?<![\w])' + re.escape(m.group(1)) + r'(?![\w])')
    c = [a for a, t in text.items() if a not in ex and w.search(t)]
    if len(c) > cap: status[s] = ("TOO_MANY", f"{len(c)} sources"); continue
    cand[s] = c
allc = sorted({a for c in cand.values() for a in c})
print("symbols", len(syms), "rows to compile", len(allc), flush=True)
if allc and not os.path.exists(f"{work}/vland.log"):
    cat = {l.split("\t")[0].lower(): l for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]}
    rs = []
    for a in allc:
        e = ent[a]; tst = open("rebuild/tests/" + e["TestSource"], encoding="utf-8").read() if e.get("TestSource") else ""
        rs.append(dict(address=a, module=e.get("Module", ""), name=a, source_cpp=text[a], test_cpp=tst, leaf=ns.leaf_of(e["Source"]) or "",
                       pass_pattern=e.get("PassPattern", f"{a}_TEST PASS")))
    json.dump({"result": {"authored": rs}}, open(f"{work}/payload.json", "w"))
    open(f"{work}/oracle.tsv", "w").write("address\tname\tlength\tbytes\n" + "".join("\t".join(cat[x["address"]].split("\t")[:4]) + "\n" for x in rs if x["address"] in cat))
    with open(f"{work}/vland.log", "w") as lf:
        subprocess.run(["timeout", "3600", "python3", "-u", f"{LT}/vland.py", f"{work}/payload.json", f"{work}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{work}/wd"))
def obj(a): return f"{work}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"
for s, c in cand.items():
    miss = [a for a in c if not os.path.exists(obj(a))]
    if miss: status[s] = ("NOT_COMPILED", ",".join(miss)); continue
    r = [a for a in c if s.encode() in open(obj(a), "rb").read()]
    status[s] = ("REFS" if r else "NONE", ",".join(r))
with open(f"{work}/REFS.tsv", "w") as o:
    o.write("symbol\tstatus\taddresses\n" + "".join(f"{s}\t{st}\t{ad}\n" for s, (st, ad) in status.items()))
print({k: sum(1 for v in status.values() if v[0] == k) for k in ("REFS", "NONE", "TOO_MANY", "NOT_COMPILED")})
