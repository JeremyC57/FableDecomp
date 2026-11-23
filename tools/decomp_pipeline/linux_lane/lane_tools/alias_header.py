#!/usr/bin/env python3
"""Scripted route 2b (docs/journal/2026-11/2026-11-23_naming_finish_plan.md): a landed row that defines a member on an alias class
`struct X_Methods : X` (X_Retail, X_<addr>, ...) derived from the real class X of its shared header engine/X.h, where the retail RTTI
vftable placement (ghidra_out/labels_rtti_port.tsv) names X's member at the row's address. Generate-and-verify; writes only under <work>.
usage (repo root, lane env sourced, S set): python3 -u alias_header.py <work> [--max N]
Edit: the out-of-line definition `R X_Methods::M(...)` becomes `R X::M(...)`; M's declaration leaves the alias class; when engine/X.h
declares M only under `#ifdef SW` (one class-model switch, no #else branch) and the row does not define SW, `#define SW` goes right
before the row's `#include "engine/X.h"`. No header is edited. ACCEPTED only when the new object's graded symbol is exactly the RTTI
symbol, the HEAD and new objects are equal section by section under the one rename (objsec ALL_EQ; `.drectve` included), and the grade
is MATCH / RELOCATION_MATCH PASS. Skips: an address reserved in T3_PORT_QUEUE.tsv, a PDB name the catalog or a landed self gives another
address, a table binder on the old spelling, an inline definition inside the alias class.
Output: <work>/RESULTS.tsv and <work>/rows/<addr>.cpp for each ACCEPTED row."""
import os, re, sys, json, subprocess, contextlib, io, glob, collections
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
import verify_and_land as V
LT = "tools/decomp_pipeline/linux_lane/lane_tools"; OBJSEC = f"{LT}/naming_batch/agent_tools/nf314/objsec.py"
work = argv[0]; mx = int(argv[argv.index("--max") + 1]) if "--max" in argv else 30
os.makedirs(work + "/rows", exist_ok=True)
ent = ns.landed_rows()
rtti = {}
for l in open("ghidra_out/labels_rtti_port.tsv", encoding="utf-8", errors="replace"):
    q = l.rstrip("\n").split("\t")
    if len(q) > 1: rtti[q[0].lower()] = q[1]
selfs = {}
for r in ns.read_tsv("rebuild/oracles/self_symbols.tsv")[1:]:
    if len(r) > 2 and r[1] and r[2] in ("MATCH", "RELOCATION_MATCH"): selfs[r[0]] = r[1]
t3q = set()
for l in open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 2: t3q.add(q[1].lower().replace("0x", "")); t3q.update(x.lower() for x in re.findall(r"0x([0-9a-fA-F]{8})", q[2]))
bind = collections.defaultdict(set)
for l in open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 1: bind[q[1].lower()].add(q[0])
catname = collections.defaultdict(set)
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); catname[q[1]].add(q[0].lower())
symaddr = collections.defaultdict(set)
for a, s in selfs.items(): symaddr[s].add(a)
GUARD = re.compile(r"#\s*ifndef\s+(FABLE_RETAIL_HEADER_\w+|\w+_H|\w+_HPP|\w+_INCLUDED)\s*$")
def header_switch(h, meth):
    """the one `#ifdef SW` guarding every declaration of meth in h ('' when one is unconditional), or None"""
    t = open(h, encoding="utf-8", errors="replace").read(); sws = set()
    for d in re.finditer(r"\b" + re.escape(meth) + r"\s*\(", t):
        stack = []
        for ln in t[:d.start()].splitlines():
            s = ln.strip()
            if re.match(r"#\s*if", s): stack.append(s)
            elif re.match(r"#\s*(else|elif)", s) and stack: stack[-1] = "ELSE"
            elif re.match(r"#\s*endif", s) and stack: stack.pop()
        inner = [x for x in stack if not GUARD.match(x)]
        if not inner: return ""
        if len(inner) != 1 or not re.match(r"#\s*ifdef\s+\w+\s*$", inner[0]): return None
        sws.add(inner[0].split()[-1])
    return sws.pop() if len(sws) == 1 else None
def block_end(t, i):
    d = 1
    while d and i < len(t):
        d += {"{": 1, "}": -1}.get(t[i], 0); i += 1
    return i
res, head, new = [], [], []
for a in sorted(rtti):
    if len(new) >= mx: break
    if a not in ent: continue
    g = rtti[a]; s = selfs.get(a) or ns.norm(ns.leaf_of(ent[a]["Source"]) or "")
    m, mg = re.match(r"\?([^@]+)@([^@]+)@@", s), re.match(r"\?([^@]+)@([^@]+)@@", g)
    if not (m and mg and m.group(1) == mg.group(1) and m.group(2).startswith(mg.group(2) + "_")): continue
    X, AL, meth = mg.group(2), m.group(2), mg.group(1)
    # the parameter list must already be the PDB's (stand-in parameter types make it a signature port, C2511): compare the encodings
    # after `@@<access><cv>E<return>`
    ts, tg = re.match(r"\?[^@]+@[^@]+@@[A-Z][AB]?E?(_N|[A-Z]|\?[^@]*@@)?(.*)$", s), re.match(r"\?[^@]+@[^@]+@@[A-Z][AB]?E?(_N|[A-Z]|\?[^@]*@@)?(.*)$", g)
    if not (ts and tg and ts.group(2) == tg.group(2)):
        res.append((a, "SKIP", s, g, "parameter types differ from the PDB (a signature port)")); continue
    if a in t3q: res.append((a, "SKIP", s, g, "reserved in T3_PORT_QUEUE")); continue
    if s in bind.get(a, set()): res.append((a, "SKIP", s, g, "a table binder uses the old spelling (unit)")); continue
    if (catname.get(X + "::" + meth, set()) - {a}) or (symaddr.get(g, set()) - {a}):
        res.append((a, "SKIP", s, g, "identity: the PDB name is claimed by another address")); continue
    path = "rebuild/src/compiled/" + ent[a]["Source"]; src = open(path, encoding="utf-8").read()
    ma = re.search(r"^[ \t]*(?:struct|class)\s+" + re.escape(AL) + r"\s*:\s*(?:public\s+)?" + re.escape(X) + r"\b[^{;]*\{", src, re.M)
    inc = re.search(r'^[ \t]*#\s*include\s+"engine/' + re.escape(X) + r'\.h"[^\n]*\n', src, re.M)
    if not ma or not inc: res.append((a, "SKIP", s, g, "alias class is not derived from X, or engine/X.h is not included")); continue
    h = f"rebuild/include/engine/{X}.h"
    sw = header_switch(h, meth)
    if sw is None: res.append((a, "SKIP", s, g, f"engine/{X}.h does not declare {meth} under one plain switch")); continue
    od = list(re.finditer(r"\b" + re.escape(AL) + r"\s*::\s*" + re.escape(meth) + r"\s*\(", src))
    if len(od) != 1: res.append((a, "SKIP", s, g, f"{len(od)} out-of-line definitions of {AL}::{meth}")); continue
    ns_src = re.sub(r"\b" + re.escape(AL) + r"\s*::\s*" + re.escape(meth) + r"(\s*\()", X + "::" + meth + r"\1", src, count=1)
    # the definition takes the target's constness (the header declares it; the alias class often did not)
    cvm = re.match(r"\?[^@]+@[^@]+@@[A-Z]([AB])", g)
    if cvm:
        k = re.search(r"\b" + re.escape(X) + r"::" + re.escape(meth) + r"\s*\(", ns_src); k = k.end(); pd = 1
        while pd and k < len(ns_src):
            pd += {"(": 1, ")": -1}.get(ns_src[k], 0); k += 1
        tail = ns_src[k:]; has = re.match(r"\s*const\b", tail)
        if cvm.group(1) == "B" and not has: ns_src = ns_src[:k] + " const" + tail
        elif cvm.group(1) == "A" and has: ns_src = ns_src[:k] + tail[has.end():]
    # drop meth's declaration(s) from the alias class body
    ma2 = re.search(r"^[ \t]*(?:struct|class)\s+" + re.escape(AL) + r"\s*:\s*(?:public\s+)?" + re.escape(X) + r"\b[^{;]*\{", ns_src, re.M)
    b0 = ma2.end(); b1 = block_end(ns_src, b0) - 1; body = ns_src[b0:b1]
    dd = [q for q in re.finditer(r"^[^\n;{}]*\b" + re.escape(meth) + r"\s*\([^;{}]*\)[^;{}]*(;|\{)", body, re.M)]
    if any(q.group(1) == "{" for q in dd): res.append((a, "SKIP", s, g, "inline definition inside the alias class")); continue
    for q in reversed(dd): body = body[:q.start()] + body[q.end():]
    ns_src = ns_src[:b0] + body + ns_src[b1:]
    if sw and not re.search(r"^\s*#\s*define\s+" + sw + r"\b", ns_src, re.M):
        i = ns_src.index(inc.group(0)) if inc.group(0) in ns_src else -1
        if i < 0: res.append((a, "SKIP", s, g, "include line not found after the edit")); continue
        ns_src = ns_src[:i] + f"#define {sw}   // alias route: engine/{X}.h declares {meth} under this class-model switch\n" + ns_src[i:]
    if re.search(r"^//\s*Leaf:", ns_src, re.M): ns_src = re.sub(r"^//\s*Leaf:.*$", lambda q: "// Leaf: " + g, ns_src, count=1, flags=re.M)
    else: ns_src = "// Leaf: " + g + "\n" + ns_src
    kv = ent[a]
    tst = open("rebuild/tests/" + kv["TestSource"], encoding="utf-8").read() if kv.get("TestSource") else ""
    base = dict(address=a, module=kv.get("Module", X), name=meth, test_cpp=tst, pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
    extra = [f for f in kv.get("CompilerFlags", "").split() if f not in ("/O2", "/Oy", "/W3")]
    if extra: base["flags"] = extra
    head.append(dict(base, source_cpp=src, leaf=s)); new.append(dict(base, source_cpp=ns_src, leaf=g))
    open(f"{work}/rows/{a}.cpp", "w").write(ns_src)
print("generated", len(new), flush=True)
def vland(tag, rs):
    d = f"{work}/{tag}"; os.makedirs(d, exist_ok=True)
    assert not os.path.exists(f"{work}/wd") and not os.path.exists(f"{d}/wd"), "stale work dir: use a fresh run directory"
    json.dump({"result": {"authored": rs}}, open(f"{d}/payload.json", "w"))
    cat = {l.split("\t")[0].lower(): l for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]}
    open(f"{d}/oracle.tsv", "w").write("address\tname\tlength\tbytes\n" + "".join("\t".join(cat[x["address"]].split("\t")[:4]) + "\n" for x in rs))
    with open(f"{d}/vland.log", "w") as lf:
        subprocess.run(["timeout", "900", "python3", "-u", f"{LT}/vland.py", f"{d}/payload.json", f"{d}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{work}/wd"))
    os.rename(f"{work}/wd", f"{d}/wd")   # HEAD and new compile at ONE path (the `?A0x` anonymous-namespace hash follows it), then move aside
    st = {}
    for l in open(f"{d}/vland.log", errors="replace"):
        m = re.match(r"([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if m and m.group(2) != "WARN": st.setdefault(m.group(1), (m.group(2), m.group(3)))
    return d, st
if new:
    dh, sh = vland("head", head); dn, sn = vland("new", new)
    for h, n in zip(head, new):
        a = h["address"]; oh = f"{dh}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"; on = f"{dn}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"
        gst = sn.get(a, ("NOT_GRADED", "-"))
        if not (os.path.exists(oh) and os.path.exists(on)): res.append((a, "REJECT", h["leaf"], n["leaf"], f"no object (head {sh.get(a)}, new {gst})")); continue
        with contextlib.redirect_stderr(io.StringIO()): gs = V.obj_text(on, n["leaf"])[2]
        if gs != n["leaf"]: res.append((a, "REJECT", h["leaf"], n["leaf"], f"graded symbol {gs}")); continue
        o = subprocess.run(["python3", OBJSEC, oh, on, f"{h['leaf']}={n['leaf']}"], capture_output=True, text=True).stdout
        if "ALL_EQ" not in o: res.append((a, "REJECT", h["leaf"], n["leaf"], "objsec " + " / ".join(o.strip().splitlines()[-3:]))); continue
        if not (gst[0].startswith(("MATCH", "RELOCATION_MATCH")) and gst[1] == "PASS"): res.append((a, "REJECT", h["leaf"], n["leaf"], f"grade {gst}")); continue
        res.append((a, "ACCEPT", h["leaf"], n["leaf"], f"objsec ALL_EQ, {gst[0]} {gst[1]}"))
acc = {r[0] for r in res if r[1] == "ACCEPT"}
for f in glob.glob(f"{work}/rows/*.cpp"):
    if os.path.basename(f)[:8] not in acc: os.replace(f, f + ".rejected")
with open(f"{work}/RESULTS.tsv", "w") as o:
    o.write("address\tverdict\told\tnew\treason\n" + "\n".join("\t".join(r) for r in res) + "\n")
print(collections.Counter(r[1] for r in res))
