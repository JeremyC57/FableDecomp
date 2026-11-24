#!/usr/bin/env python3
"""Scripted T2 route: a landed row whose own compiled symbol differs from the PDB symbol only in the member's access letter and / or its
`const` (naming tracker tier T2, fix `access`; docs/journal/2026-11/2026-11-23_naming_remainder.md). Generate-and-verify; writes candidate
sources only under <work>.
usage (repo root, lane env sourced, S set): python3 -u access_fix.py <work> [--max N] [--only addr,addr] [--wide] [--virtual] [--ret] [--alias] [--binders] [--rename] [--unit-self] [--pairs FILE]
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
RET = {"_N": "bool", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short", "H": "int", "I": "unsigned int", "J": "long",
       "K": "unsigned long", "M": "float", "N": "double", "X": "void"}
def tail_diff(lo, ro, lg, rg):
    """(cv_new or None, ret_code or None, virt_add) when the self tail (letter lo, rest ro) reaches the PDB tail (lg, rg) by the allowed
    edits, else None. Always: access letter, const. With --virtual: non-virtual -> virtual (RTTI-confirmed upstream). With --ret: a simple
    return type (RET) on a member."""
    if lo not in ACC or lg not in ACC: return None
    ko, kg = ACC[lo][1], ACC[lg][1]
    virt_add = ko == "n" and kg == "v"
    if ko != kg and not (virt_add and "--virtual" in argv): return None
    if ro == rg: return (None, None, virt_add)
    if kg == "s": return None
    cv_new = None if ro[:1] == rg[:1] else (rg[:1] == "B" if {ro[:1], rg[:1]} == {"A", "B"} else "bad")
    if cv_new == "bad" or ro[1:2] != rg[1:2]: return None
    if ro[2:] == rg[2:]: return (cv_new, None, virt_add)
    if "--ret" not in argv: return None
    a1, b1 = re.match(r'(_N|[DEFGHIJKMNX])(.*)$', ro[2:]), re.match(r'(_N|[DEFGHIJKMNX])(.*)$', rg[2:])
    if a1 and b1 and a1.group(2) == b1.group(2): return (cv_new, b1.group(1), virt_add)
    return None
VTBL = re.compile(r'^(\s*(?:(?:public|protected|private)\s*:\s*)*)(?:void\s*\*|const\s+void\s*\*|void\s*\*\s*const|void\s*\*\*|const\s+void\s*\*\*|unsigned\s+(?:int|long)|int|long|const\s+\w+\s*\*|\w+\s*\*)\s*(?:vtbl|__vftable|vtable|vft|vptr|m_vtbl|_vtbl|vfptr|m_vftable|m_pVtbl|vtbl_|pVtbl|m_vfptr)\s*;[^\n]*\n?')
PAD = re.compile(r'^(\s*(?:(?:public|protected|private)\s*:\s*)*)((?:unsigned\s+)?char)\s+(\w+)\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]\s*;')
def make_room_for_vptr(body):
    """the class body with its leading 4-byte vptr stand-in removed (a `void* vtbl`-like member, or 4 bytes off a leading char pad), or None"""
    lead = re.match(r'(\s*(?://[^\n]*\n\s*|/\*.*?\*/\s*)*)', body, re.S).group(0); rest = body[len(lead):]
    m = VTBL.match(rest)
    if m: return lead + m.group(1) + rest[m.end():]
    m = PAD.match(rest)
    if m:
        n = int(m.group(4), 0) - 4
        if n < 0: return None
        return lead + m.group(1) + (f"{m.group(2)} {m.group(3)}[{hex(n)}];" if n else "") + rest[m.end():]   # keep the element type
    return None
rows = [l.split("\t") for l in open("rebuild/candidates/manual/naming/status/OPEN.tsv").read().splitlines()[1:]]
if "--wide" in argv:
    # finish plan route 1 widened: any landed row whose trusted measured self symbol is not a PDB name and differs from exactly ONE PDB
    # symbol of the same `?name@scope@@` only in access / const. Identity: a virtual target needs the RTTI vftable placement to name that
    # symbol at this address; a non-virtual one needs the symbol in BOTH PDB name sets. A row with a table binder on its old spelling is a
    # unit (its binders move with it) and is skipped here.
    def nameset(fn):
        return {r[1] for r in ns.read_tsv(ns.P("ghidra_out", fn)) if len(r) > 1}
    egor, fwin = nameset("egor_pdb_names.tsv"), nameset("fablewin_pdb_names.tsv"); pdb = egor | fwin
    byprefix = collections.defaultdict(list)
    for s in pdb:
        m = SPLIT.match(s)
        if m: byprefix[m.group(1)].append(s)
    rtti = {}
    for l in open("ghidra_out/labels_rtti_port.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) > 1: rtti[q[0].lower()] = q[1]
    oldbind = collections.defaultdict(set)
    for l in open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
        q = l.split("\t")
        if len(q) > 1: oldbind[q[1].lower()].add(q[0])
    def near(s, g):
        a1, b1 = SPLIT.match(s), SPLIT.match(g)
        if not (a1 and b1 and a1.group(1) == b1.group(1)): return False
        return tail_diff(a1.group(3), a1.group(4), b1.group(3), b1.group(4)) is not None
    # an address in T3_PORT_QUEUE.tsv's address column or a unit column is reserved (NF316 Q3); earlier runs' rejects are queued there
    t3q = set()
    for l in open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
        q = l.split("\t")
        if len(q) > 2:
            t3q.add(q[1].lower().replace("0x", "")); t3q.update(x.lower() for x in re.findall(r'0x([0-9a-fA-F]{8})', q[2]))
    wide, why = [], collections.Counter()
    for a in sorted(ent):
        if a in t3q: why["reserved in T3_PORT_QUEUE"] += 1; continue
        s = selfsym.get(a)
        lf = ns.norm(ns.leaf_of(ent[a]["Source"]) or "")
        if lf.startswith("?") and ns.complete(lf): s = lf    # a full Leaf written since the measurement pass (landed today) wins
        if not s or s in pdb: continue
        m = SPLIT.match(s)
        if not m: continue
        g = [x for x in byprefix.get(m.group(1), []) if x != s and near(s, x)]
        alias = ""
        if not g and "--alias" in argv and m.group(2).count("@") == 1:
            # route 2b: the self is the right member on an alias class `X_Methods` / `X_Retail` / `X_<addr>`; the RTTI placement names X's member
            sc = m.group(2)[:-1]; r0 = rtti.get(a, ""); rm = SPLIT.match(r0)
            if rm and rm.group(2).count("@") == 1 and sc.startswith(rm.group(2)[:-1] + "_"):
                s2 = s.replace("@" + sc + "@@", "@" + rm.group(2)[:-1] + "@@", 1)
                if near(s2, r0): g, alias = [r0], sc
        if len(g) != 1: why["no single near PDB symbol" if not g else "several near PDB symbols"] += 1; continue
        g = g[0]; virt = ACC[SPLIT.match(g).group(3)][1] == "v"
        if virt and rtti.get(a) != g: why["virtual without RTTI agreement"] += 1; continue
        if not virt and not (g in egor and g in fwin): why["non-virtual not in both PDB sets"] += 1; continue
        if (s in oldbind.get(a, set())) != ("--unit-self" in argv):
            why["a table binder uses the old spelling (unit)" if "--unit-self" not in argv else "no table binder on the old spelling"] += 1; continue
        wide.append([a, "", "T2", "access", "", "1", g, s, alias])
    print("wide candidates", len(wide), dict(why), flush=True)
    if "--unit-self" in argv:
        # unit route (unit_fix.py): the self respell of a row whose old spelling callers still bind; unit_fix.py adds the callers
        open(f"{work}/UNITS.tsv", "w").write("address\told\tnew\n" + "".join(f"{w[0]}\t{w[7]}\t{w[6]}\n" for w in wide))
    rows = wide
BINDERS = "--binders" in argv or "--rename" in argv or "--pairs" in argv
RENAME = "--rename" in argv
if BINDERS:
    # T1 binder respell: the target's own row already defines the PDB symbol P; a caller (table witness) still references a near-miss
    # spelling W (access / const only). Edit the CALLER's local model declaration so its reference becomes P. Proof: the caller's
    # object is ALL_EQ under the one rename W -> P, its own grade still passes; the table line (W, T, caller) becomes (P, T, caller).
    t3q = set()
    for l in open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
        q = l.split("\t")
        if len(q) > 2: t3q.add(q[1].lower().replace("0x", "")); t3q.update(x.lower() for x in re.findall(r"0x([0-9a-fA-F]{8})", q[2]))
    pool, seen = [], set()
    for r in rows:
        if r[2] != "T1" or len(r) < 12: continue
        T, P = r[0], r[6]
        for part in r[11].split(" || "):
            mm = re.match(r"(\S+) \(([^)]*)\)$", part.strip())
            if not mm or mm.group(1) == P: continue
            W = mm.group(1)
            a1, b1 = SPLIT.match(W), SPLIT.match(P)
            ren = ""
            if RENAME:
                # --rename: same scope, same access letter and tail; only the member identifier differs (the caller's local model
                # misnamed the member it calls). Identity: the table binds W to T by relocation, and T's own row defines P.
                nw, np_ = re.match(r'\?([^@?]+)@', W), re.match(r'\?([^@?]+)@', P)
                if not (a1 and b1 and nw and np_ and nw.group(1) != np_.group(1) and a1.group(2) == b1.group(2)
                        and a1.group(3) + a1.group(4) == b1.group(3) + b1.group(4)): continue
                ren = nw.group(1) + "=" + np_.group(1)
            else:
                if not (a1 and b1 and a1.group(1) == b1.group(1)): continue
                td = tail_diff(a1.group(3), a1.group(4), b1.group(3), b1.group(4))
                if td is None or td[1] or td[2]: continue      # access / const only for callers
            for B in mm.group(2).split(","):
                B = B.strip().lower()
                if B in ("self", "...") or B not in ent or B in t3q or B in seen: continue
                seen.add(B); pool.append([B, "", "T2", "access", "", "1", P, W, "", T, ren])
    if "--pairs" in argv:
        pool = []
        for l in open(argv[argv.index("--pairs") + 1]).read().splitlines()[1:]:
            B, P, W, T = l.split("\t")[:4]
            if B in ent: pool.append([B, "", "T2", "access", "", "1", P, W, "", T])
    print("binder candidates", len(pool), flush=True)
    rows = pool
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
    # the alias class is column 8 only of the 9-column rows the --wide route builds; OPEN.tsv rows (13 columns) have `landed` there,
    # which read as an alias made every OPEN.tsv row skip (NF370, 2026-11-24)
    alias = r[8] if len(r) == 9 else ""
    if only and a not in only: continue
    if len(new) >= mx: break
    if selfsym.get(a) and selfsym[a] != old and "--wide" not in argv and not BINDERS: res.append((a, "SKIP", old, good, "the measured self symbol is " + selfsym[a])); continue
    if RENAME:
        wn, pn = r[10].split("=")
        path = "rebuild/src/compiled/" + ent[a]["Source"]; src_head = open(path, encoding="utf-8").read()
        if re.search(r'(?<![\w])' + re.escape(pn) + r'(?![\w])', src_head): res.append((a, "SKIP", old, good, f"the caller already uses the identifier {pn}")); continue
        if not re.search(r'(?<![\w])' + re.escape(wn) + r'(?![\w])', src_head): res.append((a, "SKIP", old, good, f"identifier {wn} not in the caller source")); continue
        if re.search(r'#\s*include\s*"engine/', src_head) and SPLIT.match(good).group(2).rstrip("@") and \
           f'engine/{SPLIT.match(good).group(2).rstrip("@").split("@")[-1]}.h' in src_head:
            res.append((a, "SKIP", old, good, "the callee's class comes from a shared header (a header respell, not a caller edit)")); continue
        # every whole-word use of the identifier is renamed; objsec ALL_EQ under the one rename W -> P proves nothing else moved
        ns_src = re.sub(r'(?<![\w])' + re.escape(wn) + r'(?![\w])', pn, src_head)
        kv = ent[a]
        tst = open("rebuild/tests/" + kv["TestSource"], encoding="utf-8").read() if kv.get("TestSource") else ""
        bn = os.path.basename(kv["Source"])[:-4]; nm = bn[len(kv.get("Module", "")) + 1:-9] if bn.startswith(kv.get("Module", "") + "_") else bn[:-9]
        base = dict(address=a, module=kv.get("Module", ""), name=nm or a, test_cpp=tst, pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
        extra = [f for f in kv.get("CompilerFlags", "").split() if f not in ("/O2", "/Oy", "/W3")]
        if extra: base["flags"] = extra
        own = ns.leaf_of(kv["Source"]) or ""
        if wn in own: res.append((a, "SKIP", old, good, "the caller's own symbol contains the identifier")); continue
        head.append(dict(base, source_cpp=src_head, leaf=own, ren=(old, good), target=r[9]))
        new.append(dict(base, source_cpp=ns_src, leaf=own, ren=(old, good), target=r[9]))
        open(f"{work}/rows/{a}.cpp", "w").write(ns_src); continue
    old_eff = old
    if alias:
        old_eff = old.replace("@" + alias + "@@", "@" + SPLIT.match(good).group(2)[:-1] + "@@", 1)
    mg, ms = SPLIT.match(good), SPLIT.match(old_eff)
    if not (mg and ms and mg.group(1) == ms.group(1)): res.append((a, "SKIP", old, good, "names differ beyond the access / cv letters")); continue
    lg, lo = mg.group(3), ms.group(3)
    td = tail_diff(lo, ms.group(4), lg, mg.group(4))
    if td is None: res.append((a, "SKIP", old, good, f"{lo}->{lg}: the difference is not an allowed access / const / virtual / return edit")); continue
    cv_new, ret_new, virt_add = td
    meth = re.match(r'\?([^@]+)@', good).group(1); scope = mg.group(2).rstrip("@").split("@"); cls = scope[0]
    if len(scope) != 1 or meth.startswith("?"): res.append((a, "SKIP", old, good, "nested scope or operator / special member")); continue
    if not BINDERS and ((catname.get(cls + "::" + meth, set()) - {a}) or (symaddr.get(good, set()) - {a})):
        res.append((a, "SKIP", old, good, "identity: the PDB name is claimed by another address " +
                    ",".join(sorted((catname.get(cls + "::" + meth, set()) | symaddr.get(good, set())) - {a})[:4]))); continue
    path = "rebuild/src/compiled/" + ent[a]["Source"]; src = open(path, encoding="utf-8").read(); src_head = src
    if alias:
        if re.search(r'(?<![\w])' + cls + r'(?![\w])', src) or f'engine/{cls}.h' in src:
            res.append((a, "SKIP", old, good, f"alias {alias}: the source already uses {cls}")); continue
        src = re.sub(r'(?<![\w])' + alias + r'(?![\w])', cls, src)
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
    if ret_new or virt_add:
        md = re.match(r'([ \t]*)((?:(?:virtual|inline|static)\s+)*)(.*?)\b' + re.escape(meth) + r'\s*\(', nd, re.S)
        if not md or "static" in md.group(2): res.append((a, "SKIP", old, good, "declaration head not recognised for a return / virtual edit")); continue
        quals = md.group(2) + ("virtual " if virt_add and "virtual" not in md.group(2) else "")
        rtxt = (RET[ret_new] + " ") if ret_new else md.group(3)
        nd = md.group(1) + quals + rtxt + nd[md.end(3):]
    if want_acc != acc_now:
        ind = re.match(r'[ \t]*', nd).group(0)
        nd = f"{ind}{want_acc}:\n{nd}" + ("" if nd.endswith("\n") else "\n") + f"{ind}{acc_now}:" + ("" if body[de:].startswith("\n") else "\n")
    elif cv_new is None and not ret_new and not virt_add: res.append((a, "SKIP", old, good, f"access in the local model is already {acc_now}")); continue
    nbody = body[:ds] + nd + body[de:]
    if virt_add and not re.search(r'\bvirtual\b', body):
        if ":" in re.sub(r'::', '', cd[0].group(0)): res.append((a, "SKIP", old, good, "virtual target, local model has a base clause")); continue
        nb = make_room_for_vptr(nbody)
        if nb is None: res.append((a, "SKIP", old, good, "virtual target, local model has no leading vptr slot")); continue
        nbody = nb
    ns_src = src[:b0] + nbody + src[b1:]
    if ret_new:
        od = [q for q in re.finditer(r'^([ \t]*)((?:inline\s+)?)([^\n;{}()]*?)\b' + cls + r'\s*::\s*' + re.escape(meth) + r'\s*\([^;{]*\)\s*(?:const\s*)?\{', ns_src, re.M)]
        if len(od) > 1: res.append((a, "SKIP", old, good, "several out-of-line definitions")); continue
        if od: ns_src = ns_src[:od[0].start(3)] + RET[ret_new] + " " + ns_src[od[0].end(3):]
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
    if BINDERS: pass        # a caller keeps its own Leaf
    elif re.search(r'^//\s*Leaf:', ns_src, re.M): ns_src = re.sub(r'^//\s*Leaf:.*$', lambda q: "// Leaf: " + good, ns_src, count=1, flags=re.M)
    else: ns_src = "// Leaf: " + good + "\n" + ns_src
    kv = ent[a]
    tst = open("rebuild/tests/" + kv["TestSource"], encoding="utf-8").read() if kv.get("TestSource") else ""
    base = dict(address=a, module=kv.get("Module", cls), name=meth, test_cpp=tst, pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
    extra = [f for f in kv.get("CompilerFlags", "").split() if f not in ("/O2", "/Oy", "/W3")]
    if extra: base["flags"] = extra
    if BINDERS:
        own = ns.leaf_of(kv["Source"]) or ""
        head.append(dict(base, source_cpp=src_head, leaf=own, ren=(old, good), target=r[9]))
        new.append(dict(base, source_cpp=ns_src, leaf=own, ren=(old, good), target=r[9]))
    else:
        head.append(dict(base, source_cpp=src_head, leaf=old)); new.append(dict(base, source_cpp=ns_src, leaf=good))
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
        if "ren" in n:
            with contextlib.redirect_stderr(io.StringIO()): gh, gn = V.obj_text(oh, h["leaf"])[2], V.obj_text(on, n["leaf"])[2]
            if gh != gn: res.append((a, "REJECT", n["ren"][0], n["ren"][1], f"caller's graded symbol moved {gh} -> {gn}")); continue
            o = subprocess.run(["python3", OBJSEC, oh, on, f"{n['ren'][0]}={n['ren'][1]}"], capture_output=True, text=True).stdout
            if "ALL_EQ" not in o: res.append((a, "REJECT", n["ren"][0], n["ren"][1], "objsec " + " / ".join(o.strip().splitlines()[-3:]))); continue
            if not (gst[0].startswith(("MATCH", "RELOCATION_MATCH")) and gst[1] == "PASS"): res.append((a, "REJECT", n["ren"][0], n["ren"][1], f"grade {gst}")); continue
            res.append((a, "ACCEPT", n["ren"][0], n["ren"][1], f"objsec ALL_EQ, {gst[0]} {gst[1]}; target {n['target']}")); continue
        with contextlib.redirect_stderr(io.StringIO()): g = V.obj_text(on, n["leaf"])[2]
        if g != n["leaf"]: res.append((a, "REJECT", h["leaf"], n["leaf"], f"graded symbol {g}")); continue
        o = subprocess.run(["python3", OBJSEC, oh, on, f"{h['leaf']}={n['leaf']}"], capture_output=True, text=True).stdout
        if "ALL_EQ" not in o: res.append((a, "REJECT", h["leaf"], n["leaf"], "objsec " + " / ".join(o.strip().splitlines()[-3:]))); continue
        if not (gst[0].startswith(("MATCH", "RELOCATION_MATCH")) and gst[1] == "PASS"): res.append((a, "REJECT", h["leaf"], n["leaf"], f"grade {gst}")); continue
        res.append((a, "ACCEPT", h["leaf"], n["leaf"], f"objsec ALL_EQ, {gst[0]} {gst[1]}"))
acc = {r[0] for r in res if r[1] == "ACCEPT"}
for f in glob.glob(f"{work}/rows/*.cpp"):
    if os.path.basename(f)[:8] not in acc: os.replace(f, f + ".rejected")
if BINDERS:
    with open(f"{work}/TABLE_CHANGES.tsv", "w") as o:
        o.write("action\tsymbol\ttarget\twitness\tnote\n")
        for r in res:
            if r[1] == "ACCEPT": o.write(f"replace\t{r[2]} -> {r[3]}\t{r[4].split('target ')[-1]}\t{r[0]}\taccess_fix --binders: the caller now references the PDB spelling\n")
with open(f"{work}/RESULTS.tsv", "w") as o:
    o.write("address\tverdict\told\tnew\treason\n" + "\n".join("\t".join(r) for r in res) + "\n")
print(collections.Counter(r[1] for r in res))
