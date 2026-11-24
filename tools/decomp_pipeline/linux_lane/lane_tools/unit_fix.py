#!/usr/bin/env python3
"""Scripted unit route (2026-11-23): a landed row whose self symbol misses its PDB symbol only by access / const / virtual / return (the
access_fix.py edits) while landed CALLERS still reference the old spelling (rebuild/oracles/reloc_bindings.tsv binds it to this row).
The row and every landed caller that references the old spelling move together, or nothing moves. Generate-and-verify; writes only under <work>.
usage (repo root, lane env sourced, S set): python3 -u unit_fix.py <work> [--cap N]
Steps:
  1. access_fix.py --wide --virtual --ret --unit-self: the self edits, each proven as in access_fix (graded symbol = PDB symbol, objsec
     ALL_EQ under old -> PDB, grade PASS);
  2. callers: every other landed source naming the member's identifier as a whole word (a unit with more than --cap, default 60, is skipped)
     is compiled at HEAD; the unit's callers are those whose object contains the old spelling. A unit is skipped when a caller is reserved
     (T3_PORT_QUEUE.tsv, a live lock), or when a table witness of the old spelling is not among its callers. Units that share a source
     (a caller of two units, or a caller that is another unit's self) go in waves: the first taken, the rest deferred to the next run;
  3. access_fix.py --pairs: each caller's local-model declaration edited so its reference becomes the PDB spelling, proven by objsec ALL_EQ
     under the one rename old -> PDB, its own graded symbol unchanged, grade PASS;
  4. a unit is ACCEPTED only when its self and every caller are ACCEPTED. Output <work>/run/ for land_scripted.py: RESULTS.tsv, rows/,
     TABLE_CHANGES.tsv (replace old -> PDB for each table line binding the old spelling to the row), UNITS.tsv (address, unit)."""
import os, re, sys, json, subprocess, collections, shutil
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
LT = "tools/decomp_pipeline/linux_lane/lane_tools"
work = argv[0]; cap = int(argv[argv.index("--cap") + 1]) if "--cap" in argv else 60
S = os.environ["S"]
os.makedirs(work, exist_ok=True)
def run(args, log):
    with open(log, "w") as lf:
        subprocess.run(["python3", "-u"] + args, stdout=lf, stderr=subprocess.STDOUT)
def results(d):
    return [l.split("\t") for l in open(f"{d}/RESULTS.tsv").read().splitlines()[1:] if l]
# 1. self edits
if not os.path.exists(f"{work}/self/RESULTS.tsv"):
    run([f"{LT}/access_fix.py", f"{work}/self", "--wide", "--virtual", "--ret", "--unit-self", "--max", "5000"], f"{work}/self.log")
units = {l.split("\t")[0]: l.split("\t")[1:3] for l in open(f"{work}/self/UNITS.tsv").read().splitlines()[1:]}
sres = {r[0]: r for r in results(f"{work}/self")}
selves = {a: units[a] for a, r in sres.items() if r[1] == "ACCEPT" and a in units}
print("self accepted", len(selves), "of", len(units), flush=True)
# 2. candidate callers by text, then HEAD compile
ent = ns.landed_rows()
text = {}
for a, e in ent.items():
    try: text[a] = open("rebuild/src/compiled/" + e["Source"], encoding="utf-8", errors="replace").read()
    except OSError: pass
t3q = set()
for l in open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 2: t3q.add(q[1].lower().replace("0x", "")); t3q.update(x.lower() for x in re.findall(r'0x([0-9a-fA-F]{8})', q[2]))
def locked(a):
    o = f"{S}/w22/nflocks/{a}/owner"
    return os.path.exists(o) and open(o).read().strip() not in ("released", "COORD")
skip, cand = {}, {}
for a, (s, g) in selves.items():
    meth = re.match(r'\?([^@?]+)@', s).group(1)
    w = re.compile(r'(?<![\w])' + re.escape(meth) + r'(?![\w])')
    c = [b for b, t in text.items() if b != a and w.search(t)]
    if len(c) > cap: skip[a] = f"{len(c)} sources name {meth} (cap {cap})"; continue
    cand[a] = c
allc = sorted({b for c in cand.values() for b in c})
print("units", len(cand), "skipped", len(skip), "callers to scan", len(allc), flush=True)
scan = f"{work}/scan"
if allc and not os.path.exists(f"{scan}/vland.log"):
    os.makedirs(scan, exist_ok=True)
    cat = {l.split("\t")[0].lower(): l for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]}
    rs = []
    for b in allc:
        e = ent[b]; tst = open("rebuild/tests/" + e["TestSource"], encoding="utf-8").read() if e.get("TestSource") else ""
        rs.append(dict(address=b, module=e.get("Module", ""), name=b, source_cpp=text[b], test_cpp=tst, leaf=ns.leaf_of(e["Source"]) or "",
                       pass_pattern=e.get("PassPattern", f"{b}_TEST PASS")))
    json.dump({"result": {"authored": rs}}, open(f"{scan}/payload.json", "w"))
    open(f"{scan}/oracle.tsv", "w").write("address\tname\tlength\tbytes\n" + "".join("\t".join(cat[x["address"]].split("\t")[:4]) + "\n" for x in rs if x["address"] in cat))
    with open(f"{scan}/vland.log", "w") as lf:
        subprocess.run(["timeout", "3600", "python3", "-u", f"{LT}/vland.py", f"{scan}/payload.json", f"{scan}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{scan}/wd"))
def obj(b): return f"{scan}/wd/{b[:2]}/{b[2:4]}/{b}/{b}.obj"
callers, owner = {}, collections.defaultdict(set)
for a, c in cand.items():
    s = selves[a][0].encode()
    miss = [b for b in c if not os.path.exists(obj(b))]
    if miss: skip[a] = "callers did not compile at HEAD: " + ",".join(miss[:4]); continue
    callers[a] = [b for b in c if s in open(obj(b), "rb").read()]
    for b in callers[a]: owner[b].add(a)
bind = collections.defaultdict(list)
for l in open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 2: bind[(q[0], q[1].lower())].append(q[2].lower())
for a in list(callers):
    s, g = selves[a]; R = callers[a]; wit = bind.get((s, a), [])
    why = ("no caller references the old spelling" if not R else
           "caller reserved " + ",".join(b for b in R if b in t3q or locked(b)) if any(b in t3q or locked(b) for b in R) else
           "table witness not among the callers " + ",".join(w for w in wit if w not in R) if any(w not in R for w in wit) else "")
    if why: skip[a] = why; del callers[a]
# waves: one source changes once per run. A unit whose self or callers overlap a unit already taken is deferred to the next wave
# (run unit_fix.py again after landing this one; the overlapping source is then HEAD again with one edit done).
claimed = set()
for a in sorted(callers):
    mem = {a} | set(callers[a])
    if mem & claimed: skip[a] = "deferred to the next wave: shares " + ",".join(sorted(mem & claimed)[:3]); del callers[a]; continue
    claimed |= mem
print("units with callers", len(callers), flush=True)
# 3. caller edits
with open(f"{work}/pairs.tsv", "w") as o:
    o.write("caller\tP\tW\tT\n")
    for a, R in callers.items():
        for b in R: o.write(f"{b}\t{selves[a][1]}\t{selves[a][0]}\t{a}\n")
if callers and not os.path.exists(f"{work}/bind/RESULTS.tsv"):
    run([f"{LT}/access_fix.py", f"{work}/bind", "--pairs", f"{work}/pairs.tsv", "--max", "5000"], f"{work}/bind.log")
bres = {r[0]: r for r in results(f"{work}/bind")} if callers else {}
# 4. assemble the run for land_scripted.py
R_ = f"{work}/run"; os.makedirs(f"{R_}/rows", exist_ok=True)
out, tab, un = [], [], []
for a, R in sorted(callers.items()):
    bad = [b for b in R if bres.get(b, [b, "MISSING"])[1] != "ACCEPT"]
    if bad: skip[a] = "caller not accepted: " + "; ".join(f"{b} {bres.get(b, [b, 'MISSING', '', '', ''])[1]} {bres.get(b, ['']*5)[4][:80]}" for b in bad[:3]); continue
    out.append(sres[a]); shutil.copyfile(f"{work}/self/rows/{a}.cpp", f"{R_}/rows/{a}.cpp"); un.append(f"{a}\t{a}")
    for b in R:
        out.append(bres[b]); shutil.copyfile(f"{work}/bind/rows/{b}.cpp", f"{R_}/rows/{b}.cpp"); un.append(f"{b}\t{a}")
    s, g = selves[a]
    for w in bind.get((s, a), []): tab.append(f"replace\t{s} -> {g}\t{a}\t{w}\tunit_fix: the row and its callers moved to the PDB spelling together")
open(f"{R_}/RESULTS.tsv", "w").write("address\tverdict\told\tnew\treason\n" + "".join("\t".join(r) + "\n" for r in out))
open(f"{R_}/TABLE_CHANGES.tsv", "w").write("action\tsymbol\ttarget\twitness\tnote\n" + "".join(l + "\n" for l in tab))
open(f"{R_}/UNITS.tsv", "w").write("address\tunit\n" + "".join(l + "\n" for l in un))
open(f"{work}/SKIPPED.tsv", "w").write("address\treason\n" + "".join(f"{a}\t{w}\n" for a, w in sorted(skip.items())))
print(json.dumps(dict(units=len(units), self_accepted=len(selves), accepted_units=len(un and {l.split(chr(9))[1] for l in un}),
                      rows=len(out), table_lines=len(tab), skipped=collections.Counter(w.split(":")[0].split(" ")[0] for w in skip.values()))))
