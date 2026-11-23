#!/usr/bin/env python3
"""Pilot: turn flattened `R __fastcall Class_Method(const Class* p)` rows back into `R Class::Method() const` on the shared header
(docs/journal/2026-11/2026-11-23_naming_remainder.md, route 2). Generate-and-verify; writes candidate sources only under <work>.
usage (repo root, lane env sourced, S set): python3 -u flatten_pilot.py <work> [--max N] [--symbols self_symbols.tsv]
Candidates: the row's graded symbol (self_symbols.tsv, else the compile gate's object_symbol) is `?Class_Method@@YI...` with ONE pointer
parameter, Class::Method has exactly one symbol in the PDB name sets and it takes no parameters (`...XZ`), engine/<Class>.h declares
Method, and no other landed source or test names Class_Method. A candidate is ACCEPTED only when, all compiled by vland on this lane:
  - the new object's graded symbol is exactly the PDB symbol;
  - the HEAD and new objects are equal section by section (bytes and relocations) under the one rename old -> PDB (objsec);
  - the new grade is MATCH / RELOCATION_MATCH with the test passing.
Output: <work>/RESULTS.tsv (address, verdict, old, new, reason) and <work>/rows/<addr>.cpp for each ACCEPTED row."""
import os, re, sys, json, subprocess, contextlib, io, glob
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
import verify_and_land as V
LT = "tools/decomp_pipeline/linux_lane/lane_tools"
OBJSEC = "tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf314/objsec.py"
work = argv[0]; mx = int(argv[argv.index("--max") + 1]) if "--max" in argv else 30
os.makedirs(work + "/rows", exist_ok=True)
pdb = ns.pdb_names(); ent = ns.landed_rows()
sym = {}
src_tsv = argv[argv.index("--symbols") + 1] if "--symbols" in argv else None
if src_tsv:
    for l in open(src_tsv).read().splitlines()[1:]:
        c = l.split("\t")
        if len(c) > 1 and c[1]: sym[c[0]] = c[1]
for l in open("rebuild/compile-gate/retail-parity.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    c = l.split("\t")
    if len(c) > 5 and c[5] and c[0].lower() not in sym: sym[c[0].lower()] = c[5]
bymeth = {}
for p in pdb:
    m = re.match(r'\?([^@?]+)@([^@]+)@@', p)
    if m: bymeth.setdefault((m.group(1), m.group(2)), []).append(p)
alltext = None
def mentions(name, own):
    global alltext
    if alltext is None:
        alltext = {}
        for f in glob.glob("rebuild/src/compiled/*/*/*.cpp") + glob.glob("rebuild/tests/*/*/*.cpp"):
            alltext[f] = open(f, encoding="utf-8", errors="replace").read()
    return [f for f, t in alltext.items() if name in t and os.path.basename(f) not in own]
res = [];  cands = []
for a in sorted(ent):
    s = sym.get(a, "")
    m = re.match(r'\?([A-Za-z0-9]+)_([A-Za-z0-9]+)@@YI', s)
    if not m: continue
    cls, meth = m.group(1), m.group(2)
    c = bymeth.get((meth, cls), [])
    if len(c) != 1 or not c[0].endswith("XZ"): continue
    hdr = f"rebuild/include/engine/{cls}.h"
    if not os.path.exists(hdr) or not re.search(r'\b' + meth + r'\s*\(', open(hdr, errors="replace").read()): continue
    cands.append((a, cls, meth, s, c[0]))
print("candidates", len(cands), flush=True)
def vland(tag, rows):
    d = f"{work}/{tag}"; os.makedirs(d, exist_ok=True)
    json.dump({"result": {"authored": rows}}, open(f"{d}/payload.json", "w"))
    orc = ["address\tname\tlength\tbytes"]
    cat = {l.split("\t")[0].lower(): l for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]}
    for r in rows: orc.append("\t".join(cat[r["address"]].split("\t")[:4]))
    open(f"{d}/oracle.tsv", "w").write("\n".join(orc) + "\n")
    with open(f"{d}/vland.log", "w") as lf:
        subprocess.run(["timeout", "900", "python3", "-u", f"{LT}/vland.py", f"{d}/payload.json", f"{d}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{d}/wd"))
    st = {}
    for l in open(f"{d}/vland.log", errors="replace"):
        m = re.match(r"([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if m and m.group(2) != "WARN": st.setdefault(m.group(1), (m.group(2), m.group(3)))
    return d, st
head, new = [], []
for a, cls, meth, old, newsym in cands:
    if len(new) >= mx: break
    kv = ent[a]; path = "rebuild/src/compiled/" + kv["Source"]; src = open(path, encoding="utf-8").read()
    fn = f"{cls}_{meth}"
    others = mentions(fn, {os.path.basename(path), os.path.basename(kv.get("TestSource", "") or "x")})
    if others: res.append((a, "SKIP", old, newsym, "other landed files name " + fn + ": " + ",".join(os.path.basename(o) for o in others[:3]))); continue
    rx = re.compile(r'^(?P<ret>[^\n;{}()]*?)\b__fastcall\s+' + fn + r'\s*\(\s*(?P<c>const\s+)?(?:class\s+|struct\s+)?' + cls +
                    r'\s*\*\s*(?P<p>\w+)\s*\)\s*\n?\s*\{', re.M)
    m = rx.search(src)
    if not m or len(rx.findall(src)) != 1: res.append((a, "SKIP", old, newsym, "definition shape not recognised")); continue
    acc = re.match(r'\?[^@]+@[^@]+@@([A-Z])([AB])E', newsym)
    if not acc: res.append((a, "SKIP", old, newsym, "PDB symbol is not a thiscall member")); continue
    is_const = acc.group(2) == "B"
    body_alias = f"{'const ' if is_const else ''}{cls}* {m.group('p')} = this;"
    head_txt = f"{m.group('ret').strip()} {cls}::{meth}(){' const' if is_const else ''}\n{{\n    {body_alias}"
    ns_src = src[:m.start()] + head_txt + src[m.end():]
    fwd = re.compile(r'^\s*class\s+' + cls + r'\s*;\s*$', re.M)
    inc = f'#include "engine/{cls}.h"'
    ns_src = fwd.sub(inc, ns_src, count=1) if fwd.search(ns_src) else re.sub(r'^(?!//)', inc + "\n", ns_src, count=1, flags=re.M)
    if "// Leaf:" in ns_src: ns_src = re.sub(r'^//\s*Leaf:.*$', "// Leaf: " + newsym, ns_src, count=1, flags=re.M)
    else: ns_src = re.sub(r'^(//[^\n]*\n)', r'\1// Leaf: ' + newsym.replace('\\', '\\\\') + '\n', ns_src, count=1)
    tst = open("rebuild/tests/" + kv["TestSource"], encoding="utf-8").read() if kv.get("TestSource") else ""
    base = dict(address=a, module=kv.get("Module", cls), name=meth, test_cpp=tst, pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
    fl = kv.get("CompilerFlags", "").split()
    extra = [f for f in fl if f not in ("/O2", "/Oy", "/W3")]
    if extra: base["flags"] = extra
    head.append(dict(base, source_cpp=src, leaf=old)); new.append(dict(base, source_cpp=ns_src, leaf=newsym))
    open(f"{work}/rows/{a}.cpp", "w").write(ns_src)
print("generated", len(new), flush=True)
dh, sh = vland("head", head); dn, sn = vland("new", new)
for h, n in zip(head, new):
    a = h["address"]; oh = f"{dh}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"; on = f"{dn}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"
    gst = sn.get(a, ("NOT_GRADED", "-"))
    if not (os.path.exists(oh) and os.path.exists(on)): res.append((a, "REJECT", h["leaf"], n["leaf"], f"no object (head {sh.get(a)}, new {gst})")); continue
    with contextlib.redirect_stderr(io.StringIO()): g = V.obj_text(on, n["leaf"])[2]
    if g != n["leaf"]: res.append((a, "REJECT", h["leaf"], n["leaf"], f"graded symbol {g}")); continue
    o = subprocess.run(["python3", OBJSEC, oh, on, f"{h['leaf']}={n['leaf']}"], capture_output=True, text=True).stdout
    if "ALL_EQ" not in o: res.append((a, "REJECT", h["leaf"], n["leaf"], "objsec " + " / ".join(o.strip().splitlines()[-3:]))); continue
    if not (gst[0].startswith(("MATCH", "RELOCATION_MATCH")) and gst[1] == "PASS"): res.append((a, "REJECT", h["leaf"], n["leaf"], f"grade {gst}")); continue
    res.append((a, "ACCEPT", h["leaf"], n["leaf"], f"objsec ALL_EQ, {gst[0]} {gst[1]}"))
acc = {r[0] for r in res if r[1] == "ACCEPT"}
for f in glob.glob(f"{work}/rows/*.cpp"):
    if os.path.basename(f)[:8] not in acc: os.replace(f, f + ".rejected")
with open(f"{work}/RESULTS.tsv", "w") as o:
    o.write("address\tverdict\told\tnew\treason\n" + "\n".join("\t".join(r) for r in res) + "\n")
import collections
print(collections.Counter(r[1] for r in res))
