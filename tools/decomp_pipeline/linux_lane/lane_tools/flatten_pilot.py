#!/usr/bin/env python3
"""Pilot: turn flattened `R __fastcall Class_Method(const Class* p)` rows back into `R Class::Method() const` in the row's own local model
(docs/journal/2026-11/2026-11-23_naming_remainder.md, route 2). Generate-and-verify; writes candidate sources only under <work>.
usage (repo root, lane env sourced, S set): python3 -u flatten_pilot.py <work> [--max N] [--symbols self_symbols.tsv]
Candidates: the row's graded symbol (self_symbols.tsv, else the compile gate's object_symbol) is `?Class_Method@@YI...` with ONE pointer
parameter, Class::Method has exactly one symbol in the PDB name sets and it takes no parameters (`...XZ`), the row defines or forward-declares
Class locally (v2: the header route needs add-only opt-ins, a header-lane task), and no other landed source or test names Class_Method. A candidate is ACCEPTED only when, all compiled by vland on this lane:
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
    return set(f for f, t in alltext.items() if name in t and os.path.basename(f) not in own)
res = [];  cands = []
# identity guard (landing pilot 1): the PDB name must belong to this address alone. Reject when the catalog names another address
# Class::Method, or another landed row's graded symbol already is the PDB symbol (0x00408360 v 0x00438440 ForceClose, ...).
catname = {}
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); catname.setdefault(q[1], set()).add(q[0].lower())
rtti = {}
for l in open("ghidra_out/labels_rtti_port.tsv", encoding="utf-8", errors="replace"):
    q = l.rstrip("\n").split("\t")
    if len(q) > 1: rtti.setdefault(q[0].lower(), set()).add(q[1])
tbl = {}
for l in open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 1: tbl.setdefault(q[0], set()).add(q[1].lower())
symaddr = {}
for k, v in sym.items(): symaddr.setdefault(v, set()).add(k)
for a in sorted(ent):
    s = sym.get(a, "")
    m = re.match(r'\?([A-Za-z0-9]+)_([A-Za-z0-9]+)@@YI', s)
    if not m: continue
    cls, meth = m.group(1), m.group(2)
    c = bymeth.get((meth, cls), [])
    if len(c) != 1 or not c[0].endswith("XZ"): continue
    if catname.get(cls + "::" + meth, set()) - {a} or symaddr.get(c[0], set()) - {a}:
        res.append((a, "SKIP", s, c[0], "identity: the PDB name is claimed by another address " + ",".join(sorted((catname.get(cls + "::" + meth, set()) | symaddr.get(c[0], set())) - {a})[:4]))); continue
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
# --switch (advisor, 2026-11-23): a class whose shared header already declares the member behind a landed class-model switch takes the
# switch route: the row defines the switch and includes the header (the preamble of the smallest landed precedent that defines a member of
# that class out of line under the same switch), drops its local model and defines the member with the header's own return type. No header
# edit. Only these class-model switches (pass-specific ones such as *_PASS27 stay out):
SWITCHES = {"FABLE_RETAIL_NSCRIPT", "FABLE_RETAIL_CCREATUREACTIONBASE", "FABLE_RETAIL_CIENGINE", "FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS"}
_prec = {}
def header_switch(cls, meth):
    """(switch, declaration match) when engine/<cls>.h declares meth() with no parameters inside `class cls`, under exactly one #ifdef of SWITCHES."""
    h = f"rebuild/include/engine/{cls}.h"
    if not os.path.exists(h): return None
    t = open(h, encoding="utf-8", errors="replace").read()
    found = []
    for d in re.finditer(r'^[ \t]*(?:virtual\s+)?(?P<r>[^;(){}\n]*?)\b' + meth + r'\s*\(\s*(?:void)?\s*\)\s*(?P<c>const)?\s*(?:=\s*0\s*)?;', t, re.M):
        stack = []
        for ln in t[:d.start()].splitlines():
            s = ln.strip()
            if re.match(r'#\s*if', s): stack.append(s)
            elif re.match(r'#\s*else|#\s*elif', s) and stack: stack[-1] = "ELSE " + stack[-1]
            elif re.match(r'#\s*endif', s) and stack: stack.pop()
        inner = [x for x in stack if not re.match(r'#\s*ifndef\s+(FABLE_RETAIL_HEADER_\w+|\w+_H|\w+_HPP|\w+_INCLUDED)\s*$', x)]
        sw = [re.match(r'#\s*ifdef\s+(\w+)\s*$', x) for x in inner]
        if inner and all(sw) and any(q.group(1) in SWITCHES for q in sw):
            cl = [q for q in re.finditer(r'^\s*(?:class|struct)\s+' + cls + r'\b[^;{]*\{', t[:d.start()], re.M)]
            if cl: found.append(([q.group(1) for q in sw], d))
    return found[0] if len(found) == 1 else None
def precedent(cls, sws):
    key = (cls, tuple(sws))
    if key in _prec: return _prec[key]
    best = None
    for f in glob.glob("rebuild/src/compiled/*/*/*.cpp"):
        tx = alltext.get(f) if alltext else None
        if tx is None: tx = open(f, encoding="utf-8", errors="replace").read()
        if not all(re.search(r'^#define\s+' + s + r'\b', tx, re.M) for s in sws): continue
        md = re.search(r'^[A-Za-z_][^;(\n]*\b' + cls + r'::\w+\s*\([^;]*\)\s*(?:const\s*)?\n?\s*\{', tx, re.M)
        if not md or ('#include "engine/' + cls + '.h"') not in tx[:md.start()]: continue
        pre = [l for l in tx[:md.start()].splitlines() if re.match(r'\s*#\s*(define|include|undef|pragma)\b', l)
               and not re.match(r'\s*#\s*define\s+__(fastcall|stdcall|cdecl|thiscall)\b', l)]
        if best is None or len(pre) < len(best[0]): best = (pre, os.path.basename(f))
    _prec[key] = best; return best
def switch_source(src, m, cls, meth, newsym, is_const):
    hs = header_switch(cls, meth)
    if not hs: return None, "no single class-model switch declares " + cls + "::" + meth + " in engine/" + cls + ".h"
    sws, d = hs
    if bool(d.group("c")) != is_const: return None, "header constness differs from the PDB symbol"
    pr = precedent(cls, sws)
    if not pr: return None, "no landed precedent defines a " + cls + " member under " + ",".join(sws)
    ret = re.sub(r'^(?:virtual\s+|static\s+|inline\s+)+', '', d.group("r").strip())
    if not ret: return None, "header return type not found"
    if re.fullmatch(r'E\w+|\w+Type|\w+Enum', ret) and '::' not in ret: ret = cls + "::" + ret   # a nested type needs the class scope outside it
    i = m.end(); depth = 1
    while depth and i < len(src):
        depth += {"{": 1, "}": -1}.get(src[i], 0); i += 1
    body = src[m.end():i - 1]
    pconst = 'const ' if m.group('c') else ''
    if m.group('p') is None: alias = ""
    elif m.group('t') == 'void': alias = f"\n    {pconst}void* {m.group('p')} = (void*)this;"
    else: alias = f"\n    {pconst}{cls}* {m.group('p')} = {'const_cast<' + cls + '*>(this)' if (is_const and not pconst) else 'this'};"
    lead = re.match(r'(?:\s*//[^\n]*\n)*', src).group(0)
    lead = re.sub(r'^//\s*Leaf:.*\n', '', lead, flags=re.M)
    out = ("// Leaf: " + newsym + "\n" + lead.lstrip("\n") +
           "// Switch route (flatten pilot --switch): the class model is engine/" + cls + ".h under " + ", ".join(sws) +
           "; preamble copied from the landed precedent " + pr[1] + ".\n" + "\n".join(pr[0]) + "\n\n" +
           f"{ret} {cls}::{meth}(){' const' if is_const else ''}\n{{{alias}{body}}}\n")
    return out, None
import struct
def coff_info(obj):
    """(.drectve text, set of undefined external symbols) of a COFF object."""
    d = open(obj, "rb").read(); ns, = struct.unpack_from("<H", d, 2); symp, nsym = struct.unpack_from("<II", d, 8)
    drv = ""
    for i in range(ns):
        o = 20 + i * 40
        if d[o:o + 8].rstrip(b"\0") == b".drectve":
            sz, ptr = struct.unpack_from("<II", d, o + 16); drv += d[ptr:ptr + sz].decode("latin-1")
    strt = symp + nsym * 18; und = set(); i = 0
    while i < nsym:
        o = symp + i * 18; nm = d[o:o + 8]
        if nm[:4] == b"\0\0\0\0":
            off, = struct.unpack_from("<I", nm, 4); e = d.index(b"\0", strt + off); name = d[strt + off:e].decode("latin-1")
        else: name = nm.rstrip(b"\0").decode("latin-1")
        val, sec, typ, cls, aux = struct.unpack_from("<IhHBB", d, o + 8)
        if sec == 0 and cls == 2 and val == 0: und.add(name)
        i += 1 + aux
    return drv, und
def libcp_only(o, oh, on):
    """Switch-route `.drectve` exception (coordinator ruling, advisor 2026-11-23): every non-.drectve section ALL_EQ, the new .drectve is
    HEAD's tokens plus exactly /DEFAULTLIB:"libcp", and the undefined-symbol sets are equal (so libcp can resolve nothing HEAD lacked)."""
    diffs = [l for l in o.splitlines() if l.startswith("DIFF")]
    if not diffs or any(" .drectve .drectve " not in l for l in diffs): return False
    if any(l.startswith(("ONLY", "MISSING", "EXTRA")) for l in o.splitlines()): return False
    dh, uh = coff_info(oh); dn, un = coff_info(on)
    th, tn = dh.split(), dn.split()
    if sorted(tn) != sorted(th + ['/DEFAULTLIB:"libcp"']): return False
    return uh == un
head, new = [], []
for a, cls, meth, old, newsym in cands:
    if len(new) >= mx: break
    kv = ent[a]; path = "rebuild/src/compiled/" + kv["Source"]; src = open(path, encoding="utf-8").read()
    fn = f"{cls}_{meth}"
    others = mentions(fn, {os.path.basename(path), os.path.basename(kv.get("TestSource", "") or "x")})
    if others: res.append((a, "SKIP", old, newsym, "other landed files name " + fn + ": " + ",".join(sorted(os.path.basename(o) for o in others)[:3]))); continue
    # source-level identity guard (advisor, 2026-11-23): most landed rows are not measured yet, so also reject when another landed source or
    # test already spells the PDB symbol or defines / declares Class::Method.
    own = {os.path.basename(path), os.path.basename(kv.get("TestSource", "") or "x")}
    # a claim is a definition, not a call: a `// Leaf:` line carrying the PDB symbol, a body `Class::Method(...) [const] {`, or a file
    # named for Class_Method at another address (callers that spell the PDB name agree with it; they are not claims).
    claim = [f for f in mentions(cls + "::" + meth, own) | set(mentions(newsym, own)) | set(mentions(fn + "_", own))
             if re.search(r'^\s*//\s*Leaf:.*' + re.escape(newsym), alltext[f], re.M)
             or (not f.endswith("_test.cpp") and re.search(r'\b' + cls + r'::' + meth + r'\s*\([^;{}]*\)\s*(?:const\s*)?\{', alltext[f]))
             or re.match(fn + r'_[0-9a-f]{8}(_test)?\.cpp$', os.path.basename(f))]
    claim += [f for f in alltext if re.match(fn + r'_[0-9a-f]{8}(_test)?\.cpp$', os.path.basename(f), re.I) and os.path.basename(f) not in own]
    claim += ["reloc_bindings.tsv -> " + g for g in sorted(tbl.get(newsym, set()) - {a})]   # the table binds the PDB name to another address
    if claim: res.append((a, "SKIP", old, newsym, "identity: another landed source spells " + cls + "::" + meth + ": " + ",".join(sorted({os.path.basename(o) if "/" in o else o for o in claim})[:3]))); continue
    # virtual targets need a second identity source (advisor, 2026-11-23): the retail RTTI vftable placement in labels_rtti_port.tsv must
    # name this address with exactly this PDB symbol and nothing else (pilot 1: all 70 virtual rows agree, no address carries two names).
    if re.match(r'\?[^@]+@[^@]+@@[UME]', newsym) and rtti.get(a, set()) != {newsym}:
        res.append((a, "SKIP", old, newsym, "identity: RTTI vftable placement does not confirm " + newsym + " (" + ",".join(sorted(rtti.get(a, {"none"}))) + ")")); continue
    # v3 parameter forms: (const C* p), (C* p), (C* const p), (struct / class C* p), (void* p), (const void* p), ()
    rx = re.compile(r'^(?P<ret>[^\n;{}()]*?)\b__fastcall\s+' + fn + r'\s*\(\s*(?:(?P<c>const\s+)?(?:class\s+|struct\s+)?(?P<t>' + cls +
                    r'|void)\s*\*\s*(?:const\s+)?(?P<p>\w+)|(?P<none>(?:void)?))\s*\)\s*\n?\s*\{', re.M)
    m = rx.search(src)
    if not m or len(rx.findall(src)) != 1: res.append((a, "SKIP", old, newsym, "definition shape not recognised")); continue
    acc = re.match(r'\?[^@]+@[^@]+@@([A-Z])([AB])E', newsym)
    if not acc: res.append((a, "SKIP", old, newsym, "PDB symbol is not a thiscall member")); continue
    letter, is_const = acc.group(1), acc.group(2) == "B"
    access = {"Q": "public", "U": "public", "I": "protected", "M": "protected", "A": "private", "E": "private"}.get(letter)
    if not access: res.append((a, "SKIP", old, newsym, "access letter " + letter)); continue
    virt = letter in "UME"
    ret = m.group('ret').strip()
    rc = re.match(r'\?[^@]+@[^@]+@@[A-Z][AB]E(_N|[DEFGHIJKMNX])XZ$', newsym)   # simple return codes: take the PDB's type
    if rc: ret = {"_N": "bool", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short", "H": "int", "I": "unsigned int",
                  "J": "long", "K": "unsigned long", "M": "float", "N": "double", "X": "void"}[rc.group(1)]
    ret = re.sub(r'^(?:static\s+|inline\s+|extern\s+"C"\s+)+', '', ret)
    decl = f"{access}: {'virtual ' if virt else ''}{ret} {meth}(){' const' if is_const else ''};"
    pconst = 'const ' if m.group('c') else ''
    if m.group('p') is None: alias = ""
    elif m.group('t') == 'void': alias = f"\n    {pconst}void* {m.group('p')} = (void*)this;"
    else: alias = f"\n    {pconst}{cls}* {m.group('p')} = {'' if (is_const and not pconst) else ''}{'const_cast<' + cls + '*>(this)' if (is_const and not pconst) else 'this'};"
    head_txt = f"{ret} {cls}::{meth}(){' const' if is_const else ''}\n{{{alias}"
    ns_src = src[:m.start()] + head_txt + src[m.end():]
    fwd = re.compile(r'^(\s*)class\s+' + cls + r'\s*;\s*$', re.M)
    dfn = re.compile(r'^\s*(?:class|struct)\s+' + cls + r'\b[^;{]*\{', re.M)
    md = dfn.search(ns_src)
    if md:
        i = md.end(); depth = 1
        while depth and i < len(ns_src):
            depth += {"{": 1, "}": -1}.get(ns_src[i], 0); i += 1
        body = ns_src[md.end():i - 1]
        if virt and not re.search(r'\bvirtual\b', body):
            b2 = re.sub(r'^(\s*(?:public:\s*)?)(?:void\s*\*|const\s+void\s*\*|void\s*\*\s*const)\s*(?:vtbl|__vftable|vtable|vft|vptr|m_vtbl|_vtbl)\s*;', r'\1', body, count=1)
            if b2 == body:
                mp = re.search(r'char\s+(\w+)\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]\s*;', body)
                if not mp or body.index(mp.group(0)) > body.find(';') or int(mp.group(2), 0) < 4:
                    res.append((a, "SKIP", old, newsym, "virtual target, local model has no leading vptr slot")); continue
                n = int(mp.group(2), 0) - 4
                b2 = body.replace(mp.group(0), (f"char {mp.group(1)}[{hex(n)}];" if n else ""), 1)
            body = b2
        ns_src = ns_src[:md.end()] + body.rstrip() + "\n    " + decl + "\n" + ns_src[i - 1:]
    elif fwd.search(ns_src) and os.path.exists(f"rebuild/include/engine/{cls}.h"):
        if "--switch" not in argv:
            res.append((a, "SKIP", old, newsym, "forward declaration only and engine/" + cls + ".h exists: needs a header opt-in (shared-header rule)")); continue
        ns_src, why = switch_source(src, m, cls, meth, newsym, is_const)
        if why: res.append((a, "SKIP", old, newsym, "switch route: " + why)); continue
    elif fwd.search(ns_src):
        ns_src = fwd.sub(lambda q: q.group(1) + f"class {cls} {{ {decl} }};", ns_src, count=1)
    elif not os.path.exists(f"rebuild/include/engine/{cls}.h") and not re.search(r'\b' + cls + r'\b', src[:m.start()]):
        ns_src = ns_src[:m.start()] + f"class {cls} {{ {decl} }};\n" + ns_src[m.start():]
    elif "--switch" in argv and os.path.exists(f"rebuild/include/engine/{cls}.h"):
        ns_src, why = switch_source(src, m, cls, meth, newsym, is_const)
        if why: res.append((a, "SKIP", old, newsym, "switch route: " + why)); continue
    else:
        res.append((a, "SKIP", old, newsym, "no local definition or forward declaration of " + cls + (" and engine/" + cls + ".h exists: needs a header opt-in" if os.path.exists(f"rebuild/include/engine/{cls}.h") else ""))); continue
    if "--switch" in argv and "// Switch route" not in ns_src: res.append((a, "SKIP", old, newsym, "local route (not run in --switch mode)")); continue
    if "// Switch route" in ns_src: pass
    elif "// Leaf:" in ns_src: ns_src = re.sub(r'^//\s*Leaf:.*$', lambda q: "// Leaf: " + newsym, ns_src, count=1, flags=re.M)
    elif ns_src.startswith('//'): ns_src = re.sub(r'^(//[^\n]*\n)', lambda q: q.group(1) + '// Leaf: ' + newsym + '\n', ns_src, count=1)
    else: ns_src = '// Leaf: ' + newsym + '\n' + ns_src   # landing pilot 1: 73 of 79 rows began with code, so the Leaf was never written
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
    sw_lib = "ALL_EQ" not in o and "// Switch route" in n["source_cpp"] and libcp_only(o, oh, on)
    if "ALL_EQ" not in o and not sw_lib: res.append((a, "REJECT", h["leaf"], n["leaf"], "objsec " + " / ".join(o.strip().splitlines()[-3:]))); continue
    if not (gst[0].startswith(("MATCH", "RELOCATION_MATCH")) and gst[1] == "PASS"): res.append((a, "REJECT", h["leaf"], n["leaf"], f"grade {gst}")); continue
    res.append((a, "ACCEPT", h["leaf"], n["leaf"], ("objsec ALL_EQ but .drectve +libcp (undefined sets equal)" if sw_lib else "objsec ALL_EQ") + f", {gst[0]} {gst[1]}"))
acc = {r[0] for r in res if r[1] == "ACCEPT"}
for f in glob.glob(f"{work}/rows/*.cpp"):
    if os.path.basename(f)[:8] not in acc: os.replace(f, f + ".rejected")
tbl = [l.split("\t") for l in open("rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read().splitlines()[1:]]
with open(f"{work}/TABLE_CHANGES.tsv", "w") as o:
    o.write("action\tsymbol\ttarget\twitness\tnote\n")
    for r in res:
        if r[1] != "ACCEPT": continue
        for c in tbl:
            if len(c) > 2 and c[0] == r[2] and c[1] == r[0]: o.write(f"delete\t{c[0]}\t{c[1]}\t{c[2]}\tflatten pilot: the self now defines {r[3]}\n")
with open(f"{work}/RESULTS.tsv", "w") as o:
    o.write("address\tverdict\told\tnew\treason\n" + "\n".join("\t".join(r) for r in res) + "\n")
import collections
print(collections.Counter(r[1] for r in res))
