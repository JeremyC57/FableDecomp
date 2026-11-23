#!/usr/bin/env python3
"""Scripted T2 route: a landed row whose own compiled symbol differs from the PDB symbol only in the member's access letter and / or its
`const` (naming tracker tier T2, fix `access`; docs/journal/2026-11/2026-11-23_naming_remainder.md). Generate-and-verify; writes candidate
sources only under <work>.
usage (repo root, lane env sourced, S set): python3 -u access_fix.py <work> [--max N] [--only addr,addr]
Edits, in the row's own local class model only (a declaration in a shared header is skipped: that is a header opt-in):
  - access: the declaration statement is wrapped as `<new>:` ... `<old>:`, so no member moves and no vtable slot changes;
  - const: `const` is added to or removed from the declaration and from the out-of-line definition `Class::Method(...)`.
Virtuality never changes (Q->U, Q->M ... are skipped: they need a vptr slot). The `// Leaf:` line becomes the PDB symbol.
A candidate is ACCEPTED only when, all compiled by vland on this lane: the new object's graded symbol is exactly the PDB symbol; the HEAD
and new objects are equal section by section under the one rename self -> PDB (objsec ALL_EQ, `.drectve` included); the grade is
MATCH / RELOCATION_MATCH with the test passing. Identity: the PDB symbol is the tracker's pdb_spelling, which callers already bind
(the table) -- the row only stops defining a near-miss spelling of it; a PDB name the catalog or another landed self claims is skipped.
Output: <work>/RESULTS.tsv and <work>/rows/<addr>.cpp for each ACCEPTED row."""
import os, re, sys, json, subprocess, contextlib, io, glob, collections
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
import verify_and_land as V
LT = "tools/decomp_pipeline/linux_lane/lane_tools"
OBJSEC = f"{LT}/naming_batch/agent_tools/nf314/objsec.py"
work = argv[0]; mx = int(argv[argv.index("--max") + 1]) if "--max" in argv else 30
only = set(argv[argv.index("--only") + 1].split(",")) if "--only" in argv else None
os.makedirs(work + "/rows", exist_ok=True)
ent = ns.landed_rows()
ACC = {"Q": ("public", "n"), "I": ("protected", "n"), "A": ("private", "n"), "U": ("public", "v"), "M": ("protected", "v"),
       "E": ("private", "v"), "S": ("public", "s"), "K": ("protected", "s"), "C": ("private", "s")}
SPLIT = re.compile(r'^(\?[^@?]+@((?:[^@]+@)*?)@)([A-Z])(.*)$')
selfsym = {}
for l in open("rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f = l.split("\t")
    if len(f) > 2 and f[1] and f[2] in ("MATCH", "RELOCATION_MATCH"): selfsym[f[0]] = f[1]
symaddr = collections.defaultdict(set)
for a, s in selfsym.items(): symaddr[s].add(a)
catname = collections.defaultdict(set)
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); catname[q[1]].add(q[0].lower())
rows = [l.split("\t") for l in open("rebuild/candidates/manual/naming/status/OPEN.tsv").read().splitlines()[1:]]
res, head, new = [], [], []
def block_end(t, i):
    """index just past the `}` matching the `{` at t[i-1]"""
    d = 1
    while d and i < len(t):
        d += {"{": 1, "}": -1}.get(t[i], 0); i += 1
    return i
for r in rows:
    if r[2] != "T2" or r[3] != "access": continue
    a, good, old = r[0], r[6], r[7]
    if only and a not in only: continue
    if len(new) >= mx: break
    if selfsym.get(a) and selfsym[a] != old: res.append((a, "SKIP", old, good, "the measured self symbol is " + selfsym[a])); continue
    mg, ms = SPLIT.match(good), SPLIT.match(old)
    if not (mg and ms and mg.group(1) == ms.group(1)): res.append((a, "SKIP", old, good, "names differ beyond the access / cv letters")); continue
    lg, lo = mg.group(3), ms.group(3)
    if lg not in ACC or lo not in ACC or ACC[lg][1] != ACC[lo][1]:
        res.append((a, "SKIP", old, good, f"access letter {lo}->{lg} changes virtuality or is not a member")); continue
    rg, ro = mg.group(4), ms.group(4)
    cv_new = None
    if rg != ro:
        if ACC[lg][1] == "s" or not (rg[1:] == ro[1:] and {rg[:1], ro[:1]} == {"A", "B"}):
            res.append((a, "SKIP", old, good, "the difference is not only access / const")); continue
        cv_new = rg[:1] == "B"
    meth = re.match(r'\?([^@]+)@', good).group(1); scope = mg.group(2).rstrip("@").split("@"); cls = scope[0]
    if len(scope) != 1 or meth.startswith("?"): res.append((a, "SKIP", old, good, "nested scope or operator / special member")); continue
    if (catname.get(cls + "::" + meth, set()) - {a}) or (symaddr.get(good, set()) - {a}):
        res.append((a, "SKIP", old, good, "identity: the PDB name is claimed by another address " +
                    ",".join(sorted((catname.get(cls + "::" + meth, set()) | symaddr.get(good, set())) - {a})[:4]))); continue
    path = "rebuild/src/compiled/" + ent[a]["Source"]; src = open(path, encoding="utf-8").read()
    cd = list(re.finditer(r'^[ \t]*(class|struct)\s+' + cls + r'\b[^;{()]*\{', src, re.M))
    if len(cd) != 1: res.append((a, "SKIP", old, good, f"{len(cd)} local definitions of {cls} (needs exactly one)")); continue
    kw = cd[0].group(1); b0 = cd[0].end(); b1 = block_end(src, b0) - 1
    body = src[b0:b1]
    # declarations of meth at class depth 0 (skip nested braces)
    depth, decls = 0, []
    for mm in re.finditer(r'[{}]|\b' + re.escape(meth) + r'\s*\(', body):
        tok = mm.group(0)
        if tok == "{": depth += 1
        elif tok == "}": depth -= 1
        elif depth == 0: decls.append(mm)
    if len(decls) != 1: res.append((a, "SKIP", old, good, f"{len(decls)} declarations of {meth} in the local {cls}")); continue
    d = decls[0]; ds = body.rfind("\n", 0, d.start()) + 1
    if re.search(r'[;{}]', body[ds:d.start()]) or re.search(r'\b(public|protected|private)\s*:', body[ds:d.start()]):
        res.append((a, "SKIP", old, good, "declaration does not start its line")); continue
    # statement end: `;` at depth 0 or the end of an inline body
    p = body.index("(", d.start()); pd = 1; i = p + 1
    while pd and i < len(body):
        pd += {"(": 1, ")": -1}.get(body[i], 0); i += 1
    close_paren = i
    j = i
    while j < len(body) and body[j] not in ";{": j += 1
    if j >= len(body): res.append((a, "SKIP", old, good, "declaration end not found")); continue
    de = j + 1 if body[j] == ";" else block_end(body, j + 1)
    decl_txt = body[ds:de]
    acc_now = "public" if kw == "struct" else "private"
    for lab in re.finditer(r'\b(public|protected|private)\s*:', body[:ds]): acc_now = lab.group(1)
    want_acc = ACC[lg][0]
    nd = decl_txt
    if cv_new is not None:
        rel = close_paren - ds; tail = nd[rel:]
        if cv_new:
            if re.match(r'\s*const\b', tail): res.append((a, "SKIP", old, good, "declaration already const")); continue
            nd = nd[:rel] + " const" + tail
        else:
            mc = re.match(r'(\s*)const\b', tail)
            if not mc: res.append((a, "SKIP", old, good, "declaration has no const to remove")); continue
            nd = nd[:rel] + tail[mc.end():]
    if want_acc != acc_now:
        ind = re.match(r'[ \t]*', nd).group(0)
        nd = f"{ind}{want_acc}:\n{nd}" + ("" if nd.endswith("\n") else "\n") + f"{ind}{acc_now}:" + ("" if body[de:].startswith("\n") else "\n")
    elif cv_new is None: res.append((a, "SKIP", old, good, f"access in the local model is already {acc_now}")); continue
    ns_src = src[:b0] + body[:ds] + nd + body[de:] + src[b1:]
    if cv_new is not None:
        od = [q for q in re.finditer(r'^[^\n;/]*?\b' + cls + r'\s*::\s*' + re.escape(meth) + r'\s*\([^;{]*\)\s*(?:const\s*)?\{', ns_src, re.M)]   # definitions, not calls
        if len(od) > 1: res.append((a, "SKIP", old, good, "several out-of-line definitions")); continue
        if od:
            k = ns_src.index("(", ns_src.index(meth, od[0].start())); pd = 1; k += 1
            while pd and k < len(ns_src):
                pd += {"(": 1, ")": -1}.get(ns_src[k], 0); k += 1
            tail = ns_src[k:]
            if cv_new: ns_src = ns_src[:k] + " const" + tail if not re.match(r'\s*const\b', tail) else ns_src
            else:
                mc = re.match(r'(\s*)const\b', tail)
                if mc: ns_src = ns_src[:k] + tail[mc.end():]
    if re.search(r'^//\s*Leaf:', ns_src, re.M): ns_src = re.sub(r'^//\s*Leaf:.*$', lambda q: "// Leaf: " + good, ns_src, count=1, flags=re.M)
    else: ns_src = "// Leaf: " + good + "\n" + ns_src
    kv = ent[a]
    tst = open("rebuild/tests/" + kv["TestSource"], encoding="utf-8").read() if kv.get("TestSource") else ""
    base = dict(address=a, module=kv.get("Module", cls), name=meth, test_cpp=tst, pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
    extra = [f for f in kv.get("CompilerFlags", "").split() if f not in ("/O2", "/Oy", "/W3")]
    if extra: base["flags"] = extra
    head.append(dict(base, source_cpp=src, leaf=old)); new.append(dict(base, source_cpp=ns_src, leaf=good))
    open(f"{work}/rows/{a}.cpp", "w").write(ns_src)
print("generated", len(new), flush=True)
def vland(tag, rs):
    d = f"{work}/{tag}"; os.makedirs(d, exist_ok=True)
    json.dump({"result": {"authored": rs}}, open(f"{d}/payload.json", "w"))
    cat = {l.split("\t")[0].lower(): l for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]}
    open(f"{d}/oracle.tsv", "w").write("address\tname\tlength\tbytes\n" + "".join("\t".join(cat[x["address"]].split("\t")[:4]) + "\n" for x in rs))
    with open(f"{d}/vland.log", "w") as lf:
        subprocess.run(["timeout", "900", "python3", "-u", f"{LT}/vland.py", f"{d}/payload.json", f"{d}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=dict(os.environ, LANDVERIFY_WORK=f"{d}/wd"))
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
print(collections.Counter(r[1] for r in res))
