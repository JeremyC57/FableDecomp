#!/usr/bin/env python3
"""flatten_pilot.py v5 (NF329, 2026-11-24; v4 by NF324 plus the generated `_VIRTUAL_MODEL` opt-in route, see V5 below): turn flattened `R __conv Class_Method(Class* p, ...)` rows back into the PDB member
`R Class::Method(...) [const]` (docs/journal/2026-11/2026-11-23_naming_remainder.md, route 2). Generate-and-verify; reads the repo, writes
only under <work> (and, in `grade`, under <work>/run/; the HEAD and new objects of one row are compiled at ONE work path).

usage (repo root, lane env sourced, S set):
  python3 -u flatten_pilot.py gen   <work> --entries ENTRIES.tsv [--only a,b] [--max N] [--routes local,switch,header] [--hdr-tree DIR]
                                            [--no-pre | --tests-only] [--no-min] [--jobs N] [--agent NF324]
  python3 -u flatten_pilot.py grade <work> [--chunk N] [--jobs N] [--vtimeout S] [--agent NF324] [--hdr-tree DIR] [--run NAME]
  python3 -u flatten_pilot.py export <work> <batch dir> [--holds FILE]        (RESULTS.tsv, rows/, head/payload.json + HEAD_FROM_REPO.txt, TABLE_CHANGES.tsv; no retail bytes)
ENTRIES.tsv columns: target, self_leaf, rtti_pdb_spelling (anything after is ignored). Without --entries the v3 candidate set is used
(self_symbols.tsv rows `?Class_Method@@YI...` whose Class::Method has one PDB symbol).

gen (no compile): per entry, identity guards, then the definition is rewritten and the class model chosen; every generated source, its
HEAD source and its landed test get a clang -fsyntax-only precheck (unless --no-pre). Writes <work>/GEN.tsv (address, verdict GEN / SKIP,
old, new, route, reason, bucket) and <work>/gen/<addr>.cpp. A GEN row whose new source fails clang where HEAD passes is a SKIP.
grade (a LATER call, after the rows are locked by --agent): vland --recheck-landed of HEAD then new for each chunk under one work path;
ACCEPT only when
  - the HEAD object's graded symbol is the ENTRIES self_leaf and the new object's graded symbol is exactly the PDB symbol;
  - objcheck: every non-debug section equal (bytes + relocations) under the ONE rename old -> PDB, the defined-symbol sets equal under that
    rename, the undefined sets equal, no new `??_7` / `??_R` symbol, and `.drectve` equal, or (switch / header routes only, NF318 Q1)
    HEAD's tokens plus exactly /DEFAULTLIB:"libcp";
  - the grade is MATCH / RELOCATION_MATCH with the test passing.
Identity (all must hold; any failure is a SKIP with an `identity:` reason):
  - the RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) at the address is exactly {PDB symbol}, and the symbol is in at least one
    PDB name set (RTTI + one PDB set = two sources, NF320 Q3); no other address carries the same RTTI label (both placed: held);
  - coordinator 2026-11-24, by FULL mangled symbol: (1) a catalog label Class::Method at another address that no landed self backs is a
    NAME_FIXES line when that address's own RTTI label / landed self proves another symbol, else a hold; (2) another address whose symbol
    is a different overload is no conflict; (3) a landed self (self_symbols.tsv or a `// Leaf:`) defining the SAME symbol at another
    address holds both rows as one unit ("duplicate strong definition unit"); also any other landed source with an out-of-line
    `Class::Method(` body that is not its own different-overload self, and a table line binding the symbol to another address;
  - no other landed source or test names Class_Method (binders / tests move with the row: a unit), and the row's own test does not call it.
  - no landed header defines the member inline in some branch, and no other landed source defines it in-class on a local model of the
    class (a pick-any COMDAT of the same symbol would meet the strong self: a unit).
Routes (class model of the new member):
  local   the row's own local class model gets the declaration (a virtual target turns a leading `void* vtbl` stand-in into the vptr or takes
          4 bytes off a leading pad, access_fix.make_room_for_vptr); a class with no local model and no engine/<Class>.h gets a one-member
          local declaration;
  switch  engine/<Class>.h already declares the member under class-model switches every one of which a landed row defines (no `_PASS`
          switch, NF318 Q2), or in its default branch: the row defines those switches and includes the header;
  header  engine/<Class>.h does not declare it: needs an add-only declaration opt-in (--hdr-tree DIR = the full include tree with the
          opt-ins; the header must then declare it under a switch whose macro this tool adds, see NOTES).
          With --hdr-tree, a `FABLE_RETAIL_<CLASS>_RTTI_DECLS` opt-in present in the tree counts as a landed switch (this run's own
          opt-ins, and ones another agent already landed in the repo header). Override gate (header route only): a probe
          `struct P : <class's base> { <decl> override; }` with the row's final preamble must compile, so the slot is not new.
Signatures come from the PDB symbol (llvm-undname). Self parameters: __fastcall (ecx = this, edx = an unused dummy when the member takes
stack parameters, then the stack parameters), __stdcall (all stack parameters, no this), __cdecl (none). The stack parameters must match
the PDB's in count; a parameter the body uses must already have the PDB type, except `T*` used where the PDB has `T&` (call sites
rewritten, NF313 Q2) and same-width integers / `char`-as-`bool` (retyped; tagged `retype` / `ptr2ref` in GEN notes). An enum return needs literal returns (`return (E)N;`, NF307
D3) and the enum's declaration from the class header or one shared enum header. By-value class returns and template types are T3.
The row's stale stand-in comment and any landed `// Retail...: <hex bytes>` byte-dump comment are dropped; `// Leaf:` becomes the PDB
symbol and one provenance line is appended.
V5 (NF329): with --hdr-tree, a `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` block (genmodel.py, NF325 style) counts as a switch like
_RTTI_DECLS: this run's own when no landed row defines it (route `header`), else a landed switch. Its `// Needs (defined by the including
row, above its first #include): A, B.` line adds A, B (landed base-model switches) to the switch set. Such rows take no precedent
preamble (v0 + error-driven prerequisites only). The header-route override gate accepts a declaration that the agent's
work/model_<Class>.tsv places on a NEW slot (past the base's retail slot count): the emitted-vftable probe is that proof. export: --holds FILE (address<TAB>reason) turns those ACCEPTs into HOLD; a HEAD source or test
with a byte-dump comment is left out of head/payload.json and listed in head/HEAD_FROM_REPO.txt (landing holds it until rebuilt)."""
import os, re, sys, json, subprocess, contextlib, io, glob, collections, struct, shutil, time
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, "tools/decomp_pipeline"); argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
LT = "tools/decomp_pipeline/linux_lane/lane_tools"
S = os.environ.get("S", "")
def opt(name, default=None):
    return argv[argv.index(name) + 1] if name in argv else default
MODE, work = argv[0], os.path.abspath(argv[1])
AGENT = opt("--agent", "NF429")
AGENT_DIR = os.environ.get("A", "")
NEWOPT = set()      # V5: this run's own _VIRTUAL_MODEL opt-in macros (no landed definer)
JOBS = int(opt("--jobs", "3"))
HDR_TREE = opt("--hdr-tree")
os.makedirs(work, exist_ok=True)

# ----------------------------------------------------------------------------------------------------------------------------- data
if MODE == "objcheck": pass
def rd(p):
    return open(p, encoding="utf-8", errors="replace").read()
ent = ns.landed_rows()
pdbset = collections.defaultdict(set)
for fn in ("egor", "fablewin"):
    for l in open(f"ghidra_out/{fn}_pdb_names.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) > 1: pdbset[q[1]].add(fn)
rtti, rtti_by = collections.defaultdict(set), collections.defaultdict(set)
for l in open("ghidra_out/labels_rtti_port.tsv", encoding="utf-8", errors="replace"):
    q = l.rstrip("\n").split("\t")
    if len(q) > 1: rtti[q[0].lower()].add(q[1]); rtti_by[q[1]].add(q[0].lower())
selfsym = {}
for l in rd("rebuild/oracles/self_symbols.tsv").splitlines()[1:]:
    f = l.split("\t")
    if len(f) > 1 and f[1]: selfsym[f[0].lower()] = f[1]
catname = collections.defaultdict(set)
for l in rd("rebuild/oracles/auto-re-candidates.tsv").splitlines()[1:]:
    q = l.split("\t", 2)
    if len(q) > 1: catname[q[1]].add(q[0].lower())
tbl = collections.defaultdict(set); tbl_lines = []
for l in rd("rebuild/oracles/reloc_bindings.tsv").splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 2: tbl[q[0]].add(q[1].lower()); tbl_lines.append(q)
LM = collections.Counter()      # FABLE_RETAIL_* macros a landed row defines -> number of rows
alltext = {}
if MODE == "gen":   # the whole-tree text indexes are only needed to generate
    for f in glob.glob("rebuild/src/compiled/*/*/*.cpp") + glob.glob("rebuild/tests/*/*/*.cpp"):
        alltext[f] = rd(f)
        if "/src/" in f:
            for m in set(re.findall(r'^\s*#\s*define\s+(FABLE_RETAIL_\w+)', alltext[f], re.M)): LM[m] += 1
    toks = {f: set(re.findall(r'\w+', t)) for f, t in alltext.items()}           # whole-word index
    bodies = collections.defaultdict(set)                                              # (cls, meth) -> sources with an out-of-line body
    for f, t in alltext.items():
        if "/src/" not in f or "::" not in t: continue
        for q in re.finditer(r'\b(\w+)::(\w+)\s*\([^;{}]*\)\s*(?:const\s*)?\{', re.sub(r'//[^\n]*|/\*.*?\*/', ' ', t, flags=re.S)):
            bodies[(q.group(1), q.group(2))].add(f)
    leafof = {}
    for f, t in alltext.items():
        m = re.search(r'^//\s*Leaf:\s*(\S+)', t, re.M)
        if m and "/src/" in f: leafof[f] = m.group(1)
    leaf_by = collections.defaultdict(set)
    src2addr = {"rebuild/src/compiled/" + e["Source"]: a for a, e in ent.items()}
    for f, s in leafof.items(): leaf_by[s].add(src2addr.get(f, os.path.basename(f)))
t3 = set()
Q1 = set()   # V5 (NF329): the brief releases NF324's own Q1 lines (source nf324, column 2 = an ENTRIES address) to this pass
if opt("--entries"):
    Q1 = {l.split("\t")[0].lower() for l in rd(opt("--entries")).splitlines()[1:] if l.strip()}
for l in rd("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv").splitlines()[1:]:
    r = l.split("\t")
    if len(r) > 2 and r[0] == "nf324" and r[1].lower().replace("0x", "") in Q1 and "0x" not in r[2]: continue
    if len(r) > 2:
        t3.add(r[1].lower().replace("0x", "")); t3.update(x.lower() for x in re.findall(r"0x([0-9a-fA-F]{8})", r[2]))
HELD = {x.lower() for x in "00B4D370 00BD3070 00890250 0091C080 0052C7E0 00A254A0 00ADC190 00ADC250 00ADC4E0 00ADCB90 00ADCE60 00BE8E60 00BFBD80 00BE6090 00BFB590".split()}
reserved = set()
for l in rd("rebuild/candidates/manual/pending/2026-11-22_codex_pass28/RESERVED.tsv").splitlines()[1:]:
    q = l.split("\t")
    if len(q) > 3 and "no longer reserved" not in q[3]: reserved.add(q[0].lower())
def lock_owner(a):
    p = f"{S}/w22/nflocks/{a}/owner"
    try: return open(p).read().strip()
    except OSError: return "released"

# ----------------------------------------------------------------------------------------------------------------------- helpers
_und = {}
def undname(syms):
    need = [s for s in syms if s not in _und]
    for i in range(0, len(need), 500):
        part = need[i:i + 500]
        out = subprocess.run(["llvm-undname"], input="\n".join(part) + "\n", capture_output=True, text=True).stdout.splitlines()
        lines = [l for l in out if l.strip()]
        k = 0
        for s in part:   # output: the input echoed, then the demangled line (or an error line)
            while k < len(lines) and lines[k] != s: k += 1
            _und[s] = lines[k + 1] if k + 1 < len(lines) and not lines[k + 1].startswith("?") else ""
            k += 1
    return [_und[s] for s in syms]
def split_top(s, sep=","):
    out, d, cur = [], 0, ""
    for ch in s:
        if ch in "(<[": d += 1
        elif ch in ")>]": d -= 1
        if ch == sep and d == 0: out.append(cur); cur = ""
        else: cur += ch
    if cur.strip(): out.append(cur)
    return [x.strip() for x in out]
def parse_und(u):
    """undname text -> dict(access, virt, static, ret, cls, meth, params[list], const) or None"""
    m = re.match(r'(public|protected|private): (virtual |static )?(.*?) __thiscall (.+)::([^:(]+)\((.*)\)( const)?$', u)
    if not m: return None
    ps = [] if m.group(6).strip() in ("void", "") else split_top(m.group(6))
    return dict(access=m.group(1), virt=bool(m.group(2) and "virtual" in m.group(2)), ret=m.group(3).strip(), cls=m.group(4),
                meth=m.group(5), params=ps, const=bool(m.group(7)))
def cpp_type(t):
    """undname type text -> C++ spelling (`class X const &` -> `const X&` keeps the elaborated key for class types)"""
    t = t.strip()
    m = re.match(r'(class|struct|enum|union) (\S+)( const)?\s*(\*|&)?( const)?$', t)
    if m:
        kw = m.group(1) if m.group(1) != "enum" else "enum"
        return ("const " if m.group(3) else "") + kw + " " + m.group(2) + ("" if not m.group(4) else m.group(4)) + (" const" if m.group(5) else "")
    return re.sub(r'\s+([*&])', r'\1', t)
def norm_type(t):
    t = re.sub(r'\b(class|struct|enum|union)\s+', '', t); t = re.sub(r'\s+', ' ', t).strip()
    t = re.sub(r'\s*([*&])\s*', r'\1', t)
    m = re.match(r'(.*?) const([*&]?)$', t)          # `X const&` == `const X&`
    if m and not m.group(1).startswith("const "): t = "const " + m.group(1) + m.group(2)
    return t.replace("unsigned int", "unsigned").replace("signed char", "char")
def block_end(t, i):
    d = 1
    while d and i < len(t):
        d += {"{": 1, "}": -1}.get(t[i], 0); i += 1
    return i
def strip_comments(t):
    return re.sub(r'//[^\n]*|/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), t, flags=re.S)
def uses(body, name):
    b = re.sub(r'\(\s*void\s*\)\s*' + re.escape(name) + r'\s*;', '', strip_comments(body))
    return bool(re.search(r'(?<![\w])' + re.escape(name) + r'(?![\w])', b))
def drop_void(body, name):
    return re.sub(r'[ \t]*\(\s*void\s*\)\s*' + re.escape(name) + r'\s*;[ \t]*\n?', '', body)
VTBL = re.compile(r'^(\s*(?:(?:public|protected|private)\s*:\s*)*)(?:void\s*\*|const\s+void\s*\*|void\s*\*\s*const|void\s*\*\*|const\s+void\s*\*\*|unsigned\s+(?:int|long)|int|long|const\s+\w+\s*\*|\w+\s*\*)\s*(?:vtbl|__vftable|vtable|vft|vptr|m_vtbl|_vtbl|vfptr|m_vftable|m_pVtbl|vtbl_|pVtbl|m_vfptr)\s*;[^\n]*\n?')
PAD = re.compile(r'^(\s*(?:(?:public|protected|private)\s*:\s*)*)((?:unsigned\s+)?char)\s+(\w+)\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]\s*;')
def make_room_for_vptr(body):
    """access_fix.py: the class body with its leading 4-byte vptr stand-in removed, or None; an empty body needs no room"""
    if not re.sub(r'\b(public|protected|private)\s*:', '', strip_comments(body)).strip(): return body
    lead = re.match(r'(\s*(?://[^\n]*\n\s*|/\*.*?\*/\s*)*)', body, re.S).group(0); rest = body[len(lead):]
    m = VTBL.match(rest)
    if m: return lead + m.group(1) + rest[m.end():]
    m = PAD.match(rest)
    if m:
        n = int(m.group(4), 0) - 4
        if n < 0: return None
        return lead + m.group(1) + (f"{m.group(2)} {m.group(3)}[{hex(n)}];" if n else "") + rest[m.end():]
    return None
def stack_at(t, pos):
    st = []
    for ln in t[:pos].splitlines():
        s = ln.strip()
        if re.match(r'#\s*if', s): st.append(s)
        elif re.match(r'#\s*(else|elif)', s) and st: st[-1] = "ELSE " + st[-1]
        elif re.match(r'#\s*endif', s) and st: st.pop()
    return st
def parse_stack(st):
    """an #if stack -> (switches to define, macros that must stay undefined) or None for a condition this tool does not read.
    `#ifdef X` / `#if defined(X)` / the #else of `#ifndef X` need X; `#if defined(A) || defined(B) ...` needs the first of them a landed row
    defines (no _PASS); `#ifndef X` / the #else of `#ifdef X` or of an `||` chain need them undefined."""
    need, forbid = [], set()
    for x in st:
        el = x.startswith("ELSE ")
        y = x[5:] if el else x
        m1 = re.match(r'#\s*ifdef\s+(\w+)\s*$', y) or re.match(r'#\s*if\s+defined\s*\(?\s*(\w+)\s*\)?\s*$', y)
        m2 = re.match(r'#\s*ifndef\s+(\w+)\s*$', y)
        ors = re.match(r'#\s*if\s+((?:!?\s*defined\s*\(\s*\w+\s*\)\s*(?:\|\|\s*)?)+)$', y)
        if m1: (forbid.add(m1.group(1)) if el else need.append(m1.group(1)))
        elif m2: (need.append(m2.group(1)) if el else forbid.add(m2.group(1)))
        elif ors and "!" not in ors.group(1):
            names = re.findall(r'defined\s*\(\s*(\w+)\s*\)', ors.group(1))
            if el: forbid.update(names)
            else:
                ok = [n for n in names if LM.get(n, 0) > 0 and "_PASS" not in n]
                if not ok: return None
                need.append(ok[0])
        else: return None
    return need, forbid
_enumhdr = None
def enum_header(qual):
    """the one shared header that defines enum `qual` (namespace-qualified allowed), or None"""
    global _enumhdr
    if _enumhdr is None:
        _enumhdr = collections.defaultdict(set)
        for h in glob.glob("rebuild/include/engine/*.h") + glob.glob("rebuild/include/*.h"):
            t = rd(h)
            for m in re.finditer(r'^\s*enum\s+(\w+)\s*(?::\s*\w+\s*)?\{', t, re.M):
                if not stack_at(t, m.start())[1:] or all(x.startswith("#ifndef") for x in stack_at(t, m.start())):
                    _enumhdr[m.group(1)].add(h)
    name = qual.split("::")[-1]
    hs = [h for h in _enumhdr.get(name, ()) if "::" not in qual or re.search(r'namespace\s+' + qual.split("::")[-2] + r'\b', rd(h))]
    return hs[0][len("rebuild/include/"):] if len(hs) == 1 else None

# ------------------------------------------------------------------------------------------------------------------------ entries
entries = []
if "--entries" in argv:
    for l in rd(opt("--entries")).splitlines()[1:]:
        q = l.split("\t")
        if len(q) >= 3: entries.append((q[0].lower(), q[1], q[2]))
else:   # v3 candidate set
    bymeth = collections.defaultdict(list)
    for p in pdbset:
        m = re.match(r'\?([^@?]+)@([^@]+)@@', p)
        if m: bymeth[(m.group(1), m.group(2))].append(p)
    for a, s in sorted(selfsym.items()):
        m = re.match(r'\?([A-Za-z0-9]+)_([A-Za-z0-9]+)@@YI', s)
        if m and a in ent and len(bymeth[(m.group(2), m.group(1))]) == 1: entries.append((a, s, bymeth[(m.group(2), m.group(1))][0]))
only = set(opt("--only").split(",")) if "--only" in argv else None
und = dict(zip([e[2] for e in entries], undname([e[2] for e in entries])))
pdb_at = collections.defaultdict(set)
for a, s, p in entries: pdb_at[p].add(a)

def identity(a, old, P, cls, meth):
    """(reason or None, namefix list)"""
    fixes = []
    if rtti.get(a, set()) != {P}: return "identity: RTTI placement at the address is " + (",".join(sorted(rtti.get(a, set()))) or "none"), fixes
    if not pdbset.get(P): return "identity: the symbol is in neither PDB name set", fixes
    o = rtti_by[P] - {a}
    if o: return "identity: RTTI places the same symbol at " + ",".join(sorted(o)) + " (held)", fixes
    o = pdb_at[P] - {a}
    if o: return "identity: another entry has the same PDB symbol " + ",".join(sorted(o)) + " (duplicate strong definition unit)", fixes
    dup = {b for b, s in selfsym.items() if s == P and b != a} | {b for b in leaf_by.get(P, set()) if b != a}
    if dup: return "identity: a landed self already defines the symbol at " + ",".join(sorted(dup)[:4]) + " (duplicate strong definition unit)", fixes
    o = tbl.get(P, set()) - {a}
    if o: return "identity: the table binds the symbol to " + ",".join(sorted(o)[:4]), fixes
    for b in sorted(catname.get(cls + "::" + meth, set()) - {a}):
        sb = selfsym.get(b) or ns.leaf_of(ent[b]["Source"]) if b in ent else selfsym.get(b)
        if sb and sb != P and sb.startswith(f"?{meth}@{cls}@@"): continue                      # (2) another overload: no conflict
        rb = rtti.get(b, set())
        if rb and P not in rb:                                                                  # (1) the label is provably wrong
            fixes.append((b, ",".join(sorted(rb)), f"RTTI vftable placement at {b} is {','.join(sorted(rb))}; {P} is RTTI-placed at {a} "
                          f"(labels_rtti_port.tsv) and in {'+'.join(sorted(pdbset[P]))}", f"catalog label {cls}::{meth}" + (f"; self {sb}" if sb else "")))
            continue
        if sb and not sb.startswith(f"?{meth}@{cls}@@") and sb in pdbset:                          # its landed self is another PDB name
            fixes.append((b, sb, f"landed self at {b} defines {sb} (PDB name); {P} is RTTI-placed at {a}", f"catalog label {cls}::{meth}"))
            continue
        mb = re.match(r'\?([^@?]+)@([^@]+)@@[A-Z]', sb or "")
        mf = re.match(r'\?([^@?]+)@@Y', sb or "")
        if sb and selfsym.get(b) and ((mb and (mb.group(1) != meth or mb.group(2) != cls)) or (mf and mf.group(1) != meth)):
            # (1) b's own byte-matched body is defined under another member name or class: the catalog label is a propagated one
            fixes.append((b, sb, f"catalog label {cls}::{meth} at {b} is wrong: the landed row there grades its own member {sb}; {P} is "
                          f"RTTI-placed at {a} (labels_rtti_port.tsv) and in {'+'.join(sorted(pdbset[P]))}", f"catalog label {cls}::{meth}"))
            continue
        return f"identity: catalog label {cls}::{meth} at {b} is not disproved (held)", []
    return None, fixes

DEF_RX = lambda fn: re.compile(r'^(?P<pre>[^\n;{}()#/]*?)\b(?:(?P<cc>__fastcall|__stdcall|__cdecl|FABLE_FASTCALL|__thiscall)\s+)?'
                               + re.escape(fn) + r'\s*\((?P<params>[^()]*)\)\s*(?P<cq>const\s*)?\{', re.M)
def parse_params(ptxt):
    ps = []
    for i, p in enumerate(split_top(ptxt)):
        if p in ("void", ""): continue
        m = re.match(r'(.*?[\s*&])(\w+)\s*$', p)
        if m and m.group(2) not in ("int", "long", "char", "float", "double", "bool", "short", "unsigned", "const"):
            ps.append((m.group(1).strip(), m.group(2)))
        else: ps.append((p, None))
    return ps

def class_def(src, cls):
    ds = list(re.finditer(r'^[ \t]*(class|struct)\s+' + re.escape(cls) + r'\b(?P<base>[^;{()]*)\{', src, re.M))
    return ds

def header_decl_sets(cls, meth, n_params):
    """[(switch list, decl match)] for each declaration of meth (with n_params parameters) inside class cls in engine/<cls>.h"""
    h = f"rebuild/include/engine/{cls}.h"
    if not os.path.exists(h): return None, []
    t = rd(h); out = []
    for d in re.finditer(r'^[ \t]*(?:(?:public|protected|private)\s*:\s*)?(?:virtual\s+)?[^;(){}\n#/]*?\b' + re.escape(meth) + r'\s*\(([^;{}]*?)\)\s*(const)?', t, re.M):
        pc = 0 if d.group(1).strip() in ("", "void") else len(split_top(d.group(1)))
        if pc != n_params: continue
        if not [q for q in re.finditer(r'^\s*(?:class|struct)\s+' + re.escape(cls) + r'\b[^;{]*\{', t[:d.start()], re.M)]: continue
        st = [x for x in stack_at(t, d.start()) if not re.match(r'#\s*ifndef\s+\w+_H\w*\s*$', x)]
        ps = parse_stack(st)
        out.append(((ps[0] + needs_of(t, ps[0])) if ps else None, d, ps[1] if ps else set()))
    return t, out

# ---------------------------------------------------------------------------------------------------------------------- generate
def generate(a, old, P):
    """(verdict, route, reason, new source or None, extra dict)"""
    u = parse_und(und.get(P, ""))
    if not u: return "SKIP", "-", "signature: the PDB symbol is not a plain member function (undname)", None, {}
    cls, meth = u["cls"], u["meth"]
    if "::" in cls: return "SKIP", "-", "signature: nested class scope", None, {}
    if a not in ent: return "SKIP", "-", "no landed row", None, {}
    if ns.leaf_of(ent[a]["Source"]) == P: return "SKIP", "-", "done: the landed row already defines the PDB symbol", None, {}
    mf = re.match(r'\?(\w+)@@Y[AGI]', old)
    fn = mf.group(1) if mf else ""
    if fn not in (f"{cls}_{meth}", f"{cls}__{meth}", f"{meth}_{cls}", meth): return "SKIP", "-", f"self {fn or old[:40]} is not a flattened {cls}_{meth}", None, {}
    if a in HELD or a in reserved: return "SKIP", "-", "reserved / held by the standing brief", None, {}
    if a in t3: return "SKIP", "-", "reserved: T3_PORT_QUEUE", None, {}
    lo = lock_owner(a)
    if lo not in ("released", AGENT): return "SKIP", "-", f"lock held by {lo}", None, {}
    why, fixes = identity(a, old, P, cls, meth)
    if why: return "SKIP", "-", why, None, {}
    kv = ent[a]; path = "rebuild/src/compiled/" + kv["Source"]; src = rd(path)
    tpath = "rebuild/tests/" + kv["TestSource"] if kv.get("TestSource") else None
    own = {path, tpath}
    others = sorted(os.path.basename(f) for f, tk in toks.items() if f not in own and fn in tk)
    if others: return "SKIP", "-", "unit: other landed files name " + fn + ": " + ",".join(others[:3]), None, {}
    if tpath and re.search(r'(?<![\w])' + re.escape(fn) + r'(?![\w])', alltext.get(tpath, "")) and not DEF_RX(fn).search(alltext.get(tpath, "")):
        return "SKIP", "-", "unit: the row's own test calls " + fn, None, {}
    for f in sorted(bodies.get((cls, meth), ())):   # an out-of-line body of Class::Method in another landed source, not its own other-overload self
        if f in own: continue
        if True:
            sb = leafof.get(f) or selfsym.get(src2addr.get(f, ""), "")
            if not (sb and sb.startswith(f"?{meth}@{cls}@@") and sb != P):
                return "SKIP", "-", "identity: another landed source defines " + cls + "::" + meth + ": " + os.path.basename(f) + " (duplicate strong definition unit)", None, {}
    for f, tk in toks.items():   # a local model in another landed source with an in-class body of the member: a pick-any COMDAT of the
        if f in own or "/src/" not in f or cls not in tk or meth not in tk: continue   # same symbol would clash with the strong self (census, 0x00BE8E60 / 0x009D5100)
        tx = strip_comments(alltext[f])
        for d in re.finditer(r'\b(?:class|struct)\s+' + re.escape(cls) + r'\b[^;{()]*\{', tx):
            if re.search(r'\b' + re.escape(meth) + r'\s*\([^;{}()]*\)\s*(?:const\s*)?\{', tx[d.end():block_end(tx, d.end())]):
                return "SKIP", "-", f"identity: {os.path.basename(f)} defines {cls}::{meth} in-class on a local model (pick-any v strong clash: unit)", None, {}
    for hp in [f"rebuild/include/engine/{cls}.h"] + ([f"{HDR_TREE}/engine/{cls}.h"] if HDR_TREE else []):
        if os.path.exists(hp) and re.search(r'\b' + re.escape(meth) + r'\s*\([^;{}()]*\)\s*(?:const\s*)?\{', strip_comments(rd(hp))):
            return "SKIP", "-", f"identity: engine/{cls}.h defines {meth} inline in some branch: a strong self would collide with its pick-any copies (nf86 / nf88 form)", None, {}
        hb = rd(hp) if os.path.exists(hp) else ""
        if re.search(r'\b' + re.escape(cls) + r'::' + re.escape(meth) + r'\s*\([^;{}]*\)\s*(?:const\s*)?\{', strip_comments(hb)):
            return "SKIP", "-", f"identity: engine/{cls}.h defines {cls}::{meth} out of class (nf86 / nf88 form)", None, {}
    rx = DEF_RX(fn); ms = list(rx.finditer(src))
    if len(ms) != 1: return "SKIP", "-", f"shape: {len(ms)} definitions of {fn} recognised", None, {}
    m = ms[0]
    if m.group("cq"): return "SKIP", "-", "shape: const free function", None, {}
    conv = re.match(r'\?\w+@@Y([AGI])', old).group(1)
    sp = parse_params(m.group("params")); n = len(sp); pp = u["params"]; k = len(pp)
    body_start = m.end(); body_end = block_end(src, body_start) - 1; body = src[body_start:body_end]
    if conv == "I":
        if k > 0 and n != k + 2: return "SKIP", "-", f"params: __fastcall self has {n} parameters, the member {k} (stack counts differ)", None, {}
        if k == 0 and n > 2: return "SKIP", "-", f"params: __fastcall self has {n} parameters, the member none", None, {}
        this_p = sp[0] if n >= 1 else None; dummy = sp[1:2] if n >= 2 else []; stackp = sp[2:]
    elif conv == "G":
        if n != k: return "SKIP", "-", f"params: __stdcall self has {n} stack parameters, the member {k}", None, {}
        this_p, dummy, stackp = None, [], sp
    else:
        if n or k: return "SKIP", "-", "params: __cdecl self with parameters", None, {}
        this_p, dummy, stackp = None, [], []
    for t, nm in dummy:
        if nm and uses(body, nm): return "SKIP", "-", f"params: the edx parameter {nm} is used", None, {}
        if nm: body = drop_void(body, nm)
    for t in [u["ret"]] + pp:
        if "<" in t: return "SKIP", "-", "signature: template type (T3)", None, {}
        if "(" in t: return "SKIP", "-", "signature: function-pointer type", None, {}
    newp = []; retypes = []
    for i, ((st, nm), pt) in enumerate(zip(stackp, pp)):
        ct = cpp_type(pt)
        if nm and uses(body, nm) and norm_type(st) != norm_type(ct):
            a_, b_ = norm_type(st), norm_type(ct)
            INTS = [{"int", "long"}, {"unsigned", "unsigned long"}, {"char", "bool"}, {"unsigned char", "bool"}]
            if a_.endswith("*") and b_.endswith("&") and a_[:-1] == b_[:-1]:     # pointer -> reference (NF313 Q2 call-site edits)
                body = re.sub(r'(?<![\w.>])' + nm + r'\s*->', "@@R@@.", body)
                body = re.sub(r'(?<![\w)\]])\*\s*' + nm + r'(?![\w])', "@@R@@", body)
                body = re.sub(r'(?<![\w.>@])' + nm + r'(?![\w])', "&@@R@@", body).replace("@@R@@", nm)
                retypes.append(f"{nm}: {st} -> {ct} (pointer -> reference, NF313 Q2)")
            elif {a_, b_} in INTS: retypes.append(f"{nm}: {st} -> {ct}")             # same-width integer retype to the PDB type (tagged)
            else: return "SKIP", "-", f"params: used parameter {nm} is `{st}`, the PDB type is `{ct}` (alias pattern)", None, {}
        newp.append(ct + (" " + nm if nm else ""))
    if any("::" in x and re.match(r'(const )?(class|struct|union) ', x) for x in map(cpp_type, pp)):
        nsparam = True
    else: nsparam = False
    ret = u["ret"]
    if re.match(r'(class|struct|union) \S+$', ret): return "SKIP", "-", "signature: by-value class return (T3)", None, {}
    enum_ret = None
    if ret.startswith("enum "):
        enum_ret = ret[5:]
        rets = re.findall(r'\breturn\b([^;]*);', strip_comments(body))
        if not rets or not all(re.fullmatch(r'\s*\(?\s*-?(0x[0-9a-fA-F]+|\d+)\s*\)?\s*', r) for r in rets):
            return "SKIP", "-", "signature: enum return with non-literal returns", None, {}
        body = re.sub(r'\breturn\s+\(?\s*(-?(?:0x[0-9a-fA-F]+|\d+))\s*\)?\s*;', lambda q: f"return ({enum_ret}){q.group(1)};", body)
        ret_txt = enum_ret
    else: ret_txt = cpp_type(ret)
    # `this`
    alias = ""
    if this_p:
        tt, nm = this_p
        if nm and uses(body, nm):
            tn = norm_type(tt).replace("const ", "")
            if tn not in (cls + "*", "void*"): return "SKIP", "-", f"this: the body uses {nm} typed `{tt}` (another class)", None, {}
            pconst = "const " if tt.strip().startswith("const") else ""
            if tn == "void*": alias = f"\n    {pconst}void* {nm} = (void*)this;"
            else: alias = f"\n    {pconst}{cls}* {nm} = " + ("const_cast<" + cls + "*>(this)" if (u["const"] and not pconst) else "this") + ";"
        elif nm: body = drop_void(body, nm)
    head_txt = f"{ret_txt} {cls}::{meth}({', '.join(newp)}){' const' if u['const'] else ''}\n{{{alias}"
    decl = f"{u['access']}: {'virtual ' if u['virt'] else ''}{ret_txt} {meth}({', '.join(cpp_type(x) for x in pp)}){' const' if u['const'] else ''};"
    nsrc = src[:m.start()] + head_txt + body + src[body_end:]
    routes = opt("--routes", "local,switch,header").split(",")
    hdr = f"rebuild/include/engine/{cls}.h"; hx = os.path.exists(hdr) or (HDR_TREE and os.path.exists(f"{HDR_TREE}/engine/{cls}.h"))
    cd = class_def(nsrc, cls)
    extra = dict(fixes=fixes, cls=cls, meth=meth, retypes=retypes, virt=u["virt"], pdb=P)
    if len(cd) > 1: return "SKIP", "-", f"model: {len(cd)} local definitions of {cls}", None, extra
    if cd:
        route = "local"
        if "local" not in routes: return "SKIP", route, "route not selected", None, extra
        if re.search(r'\b' + re.escape(meth) + r'\s*\(', strip_comments(nsrc[cd[0].end():block_end(nsrc, cd[0].end())])):
            return "SKIP", route, "model: the local class already declares " + meth, None, extra
        b0 = cd[0].end(); b1 = block_end(nsrc, b0) - 1; cbody = nsrc[b0:b1]
        if u["virt"] and not re.search(r'\bvirtual\b', cbody):
            if re.search(r':(?!:)', cd[0].group("base")): return "SKIP", route, "model: virtual target, local model has a base clause", None, extra
            nb = make_room_for_vptr(cbody)
            lead_ = re.match(r'(\s*(?://[^\n]*\n\s*|/\*.*?\*/\s*)*)', cbody, re.S).group(0); mp_ = PAD.match(cbody[len(lead_):])
            padecl = r'(?:unsigned\s+)?char\s+' + (mp_.group(3) if mp_ else "x") + r'\s*\[[^\]]*\]\s*;'   # other classes' same-named pads are declarations, not uses
            if nb is not None and mp_ and re.search(r'(?<![\w])' + mp_.group(3) + r'(?![\w])', re.sub(padecl, ' ', strip_comments(nsrc[:b0] + nsrc[b1:]) + strip_comments(cbody[len(lead_) + mp_.end():]))):
                nb = None   # the leading pad is used by name (`_tail + 0x40`): shrinking it would move what the body reads (0x00452BB4)
            if nb is None:   # a leading 4-byte scalar placeholder (`int dummy;`) whose name nothing else in the TU uses is the vptr word
                lm = re.match(r'(\s*(?:(?:public|protected|private)\s*:\s*)*)(?:int|long|unsigned(?:\s+(?:int|long))?|ULONG|DWORD|void\s*\*)\s+(\w+)\s*;[^\n]*\n?', cbody)
                rest_src = nsrc[:b0] + nsrc[b1:]
                if lm and not re.search(r'(?<![\w])' + lm.group(2) + r'(?![\w])', strip_comments(rest_src) + strip_comments(cbody[lm.end():])):
                    nb = lm.group(1) + cbody[lm.end():]
            if nb is None: return "SKIP", route, "model: virtual target, local model has no leading vptr slot", None, extra
            cbody = nb
        acc_now = "public" if cd[0].group(1) == "struct" else "private"
        for lab in re.finditer(r'\b(public|protected|private)\s*:', strip_comments(cbody)): acc_now = lab.group(1)
        ins = cbody.rstrip() + "\n" + (decl.replace(": ", ":\n    ", 1) if acc_now != u["access"] else "    " + decl.split(": ", 1)[1]) + "\n"
        nsrc = nsrc[:b0] + ins + nsrc[b1:]
        if enum_ret:
            eh = enum_header(enum_ret)
            if not eh: return "SKIP", route, f"enum: {enum_ret} has no shared declaration (a local enum model would be new)", None, extra
            nsrc = add_includes(nsrc, [], [eh])
    elif hx:
        hsrc, decls = header_decl_sets(cls, meth, k) if not HDR_TREE else header_decl_sets_tree(cls, meth, k)
        cands = sorted([d for d in decls if d[0] is not None and all(LM.get(x, 0) > 0 and "_PASS" not in x for x in d[0])], key=lambda d: len(d[0]))
        if not decls:
            route = "header"
            flat = not re.search(r'(?:class|struct)\s+' + re.escape(cls) + r'\s*:\s*(?:public|private|protected)?\s*\w', rd(hdr))
            return "SKIP", route, ("model: engine/" + cls + ".h does not declare " + meth + (" and has no polymorphic class model (port)" if flat and u["virt"] else
                                   " (add-only opt-in)")), None, extra
        if not cands:
            why = "; ".join(sorted({"#if " + ("complex" if d[0] is None else ",".join(d[0])) for d in decls}))[:200]
            return "SKIP", "switch", "model: engine/" + cls + ".h declares " + meth + " only under " + why + " (no landed / non-_PASS switch)", None, extra
        route = "switch" if not (HDR_TREE and any("_RTTI_DECLS" in x or "_FLAT_DECLS" in x or x in NEWOPT for x in cands[0][0])) else "header"
        if route not in routes: return "SKIP", route, "route not selected", None, extra
        sws = cands[0][0]
        fb = [x for x in cands[0][2] if re.search(r'^\s*#\s*define\s+' + x + r'\b', nsrc, re.M) or x in sws]
        if fb: return "SKIP", "switch", "model: the declaring branch needs " + ",".join(fb) + " undefined, the row or the switch set defines it", None, extra
        nsrc = re.sub(r'^[ \t]*(class|struct)\s+' + re.escape(cls) + r'\s*;[ \t]*\n', '', nsrc, flags=re.M)
        base_src = nsrc
        nsrc = add_includes(base_src, sws, [f"engine/{cls}.h"])
        extra["switches"] = sws; variants = []
        for alt in ([] if any(x in NEWOPT for x in cands[0][0]) else cands[:3]):   # V5: no precedent exists for this run's own model
            cs = [x for x in alt[0] if not x.endswith("_RTTI_DECLS")]           # precedents exist for the class switches only
            pr = precedent(cls, cs)
            if pr: variants.append((add_preamble(base_src, [f"#define {x}" for x in alt[0] if x not in cs] + pr[0]), alt[0], "preamble of " + pr[1]))
        extra["variants"] = variants
        bm = re.search(r'^\s*(?:class|struct)\s+' + re.escape(cls) + r'\s*:\s*(?:public\s+|private\s+|protected\s+)?([\w:<>, ]+?)\s*\{', hsrc[:cands[0][1].start()][::-1][::-1], re.M)
        defs_before = list(re.finditer(r'^\s*(?:class|struct)\s+' + re.escape(cls) + r'\b([^;{]*)\{', hsrc[:cands[0][1].start()], re.M))
        if defs_before:
            bb = re.match(r'\s*:\s*(?:public\s+|private\s+|protected\s+)?(\w+)', defs_before[-1].group(1))
            extra["base"] = bb.group(1) if bb else ""
        extra["decl"] = decl.split(": ", 1)[1]; extra["route"] = route
    else:
        route = "local"
        if "local" not in routes: return "SKIP", route, "route not selected", None, extra
        if enum_ret:
            eh = enum_header(enum_ret)
            if not eh: return "SKIP", route, f"enum: {enum_ret} has no shared declaration (a local enum model would be new)", None, extra
        fwd = re.compile(r'^([ \t]*)(?:class|struct)\s+' + re.escape(cls) + r'\s*;[ \t]*$', re.M)
        one = f"class {cls} {{ {decl} }};"
        if fwd.search(nsrc): nsrc = fwd.sub(lambda q: q.group(1) + one, nsrc, count=1)
        elif re.search(r'(?<![\w])' + re.escape(cls) + r'(?![\w])', strip_comments(nsrc[:m.start()])):
            return "SKIP", route, f"model: {cls} is used before the definition without a local model", None, extra
        else:
            mm = re.search(re.escape(head_txt[:40]), nsrc); nsrc = nsrc[:mm.start()] + one + "\n" + nsrc[mm.start():]
        if enum_ret: nsrc = add_includes(nsrc, [], [eh])
    if nsparam and route == "local": return "SKIP", route, "signature: namespaced class parameter in a local model", None, extra
    def fin(s, sws, how):
        s = re.sub(r'^//\s*Stand-in \(owner rule[^\n]*\b' + re.escape(fn) + r'\b[^\n]*\n?', '', s, flags=re.M)   # stale once the row is the member
        s = re.sub(r'^//[^\n]*\b[Rr]etail\b[^\n]*\b(?:[0-9a-fA-F]{2} ){3,}[0-9a-fA-F]{2}\b[^\n]*\n?', '', s, flags=re.M)   # a retail byte-dump comment (never handed back)
        if re.search(r'^//\s*Leaf:', s, re.M): s = re.sub(r'^//\s*Leaf:.*$', lambda q: "// Leaf: " + P, s, count=1, flags=re.M)
        else: s = "// Leaf: " + P + "\n" + s
        tag = {"local": "the row's local class model", "switch": "engine/" + cls + ".h under " + (", ".join(sws or []) or "its default branch"),
               "header": "engine/" + cls + ".h opt-in " + ", ".join(sws or [])}[route] + (f" ({how})" if how else "")
        return s.rstrip("\n") + f"\n// Flatten route (flatten_pilot v5, NF329): {fn} -> {cls}::{meth}, the RTTI-placed PDB member; class model: {tag}.\n"
    extra["variants"] = [(fin(s, w, h), w) for s, w, h in extra.get("variants", [])]
    return "GEN", route, "", fin(nsrc, extra.get("switches"), ""), extra

_prec = {}
def precedent(cls, sws):
    """(#define / #include lines, file) of the landed source with the shortest preamble that includes engine/<cls>.h and defines every switch
    in sws; pragmas and calling-convention defines are not copied"""
    key = (cls, tuple(sws))
    if key in _prec: return _prec[key]
    best = None; inc = '#include "engine/' + cls + '.h"'
    for f, tx in alltext.items():
        if "/src/" not in f or inc not in tx: continue
        if not all(re.search(r'^\s*#\s*define\s+' + s + r'\b', tx, re.M) for s in sws): continue
        pre = []
        for l in tx[:tx.index(inc) + len(inc)].splitlines():
            if re.match(r'\s*#\s*(define|include|undef)\b', l) and not re.match(r'\s*#\s*define\s+__(fastcall|stdcall|cdecl|thiscall)\b', l): pre.append(l.strip())
        if best is None or len(pre) < len(best[0]): best = (pre, os.path.basename(f))
    _prec[key] = best; return best
def add_preamble(src, lines):
    """put a precedent's preamble lines (those the row lacks) after the leading comment block"""
    out = src.split("\n"); i = 0
    while i < len(out) and (out[i].startswith("//") or not out[i].strip()): i += 1
    have = set(l.strip() for l in out)
    return "\n".join(out[:i] + [l for l in lines if l not in have] + out[i:])
def add_includes(src, defines, includes):
    """put `#define` lines and includes after the leading comment block (defines before every include, GOTCHAS 127)"""
    lines = src.split("\n"); i = 0
    while i < len(lines) and (lines[i].startswith("//") or not lines[i].strip()): i += 1
    have = set(re.findall(r'^\s*#\s*define\s+(\w+)', src, re.M))
    add = [f"#define {d}" for d in defines if d not in have] + [f'#include "{h}"' for h in includes if f'"{h}"' not in src]
    # the row's own defines may sit below its pragmas: keep them, ours go first
    return "\n".join(lines[:i] + add + lines[i:])

def header_decl_sets_tree(cls, meth, n):
    """--hdr-tree: like header_decl_sets on <tree>/engine/<cls>.h (opt-in macros added by this run count as landed once their rows land)"""
    h = f"{HDR_TREE}/engine/{cls}.h"
    if not os.path.exists(h): return header_decl_sets(cls, meth, n)
    t = rd(h); out = []
    for d in re.finditer(r'^[ \t]*(?:(?:public|protected|private)\s*:\s*)?(?:virtual\s+)?[^;(){}\n#/]*?\b' + re.escape(meth) + r'\s*\(([^;{}]*?)\)\s*(const)?', t, re.M):
        pc = 0 if d.group(1).strip() in ("", "void") else len(split_top(d.group(1)))
        if pc != n: continue
        if not list(re.finditer(r'^\s*(?:class|struct)\s+' + re.escape(cls) + r'\b[^;{]*\{', t[:d.start()], re.M)): continue
        st = [x for x in stack_at(t, d.start()) if not re.match(r'#\s*ifndef\s+\w+_H\w*\s*$', x)]
        for x in st:
            mm = re.match(r'#\s*ifdef\s+(\w+_RTTI_DECLS)\s*$', x)
            if mm: LM[mm.group(1)] = max(LM[mm.group(1)], 1)   # this run's opt-in macro (the rows that land with it define it)
            mv = re.match(r'#\s*ifdef\s+(\w+_VIRTUAL_MODEL)\s*$', x)
            if mv and LM.get(mv.group(1), 0) == 0:                # V5: this run's generated model (no landed definer yet)
                NEWOPT.add(mv.group(1)); LM[mv.group(1)] = 1
        ps = parse_stack(st)
        out.append(((ps[0] + needs_of(t, ps[0])) if ps else None, d, ps[1] if ps else set()))
    return t, out
def needs_of(t, sws):
    """V5: the `// Needs (defined by the including row, above its first #include): A, B.` line of each _VIRTUAL_MODEL block in sws"""
    add = []
    for x in sws:
        if not x.endswith("_VIRTUAL_MODEL"): continue
        k = t.find("#ifdef " + x)
        if k < 0: continue
        blk = t[k:t.find("\n#else", k) if t.find("\n#else", k) > 0 else len(t)]
        mm = re.search(r'^// Needs \(defined by the including row, above its first #include\): ([^\n]*?)\.?\s*$', blk, re.M)
        if mm: add += [y.strip() for y in mm.group(1).split(",") if y.strip() and y.strip() not in sws + add]
    return add

# ------------------------------------------------------------------------------------------------------------------------ precheck
VCINC = f"{S}/wine/vc71rtm/include"
def clang(path, defs=(), tree=None):
    inc = ([f"-I{tree}"] if tree else []) + ["-I" + os.path.abspath("rebuild/include")]
    cmd = ["timeout", "60", "clang++", "-target", "i686-pc-windows-msvc", "-fms-extensions", "-fms-compatibility",
           "-fms-compatibility-version=13.10", "-Xclang", "-fno-wchar", "-fsyntax-only", "-w"] + inc + ["-isystem", VCINC] + \
          [f"-D{d}" for d in defs] + [path]
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", cwd=os.path.dirname(path))
    errs = [l for l in r.stderr.splitlines() if re.search(r'\b(error|fatal error)\b', l)]
    return ("PASS" if r.returncode == 0 else "FAIL"), errs[:6]
def clang_text(text, d, defs=()):
    open(f"{d}/probe.cpp", "w").write(text); return clang(f"{d}/probe.cpp", defs, HDR_TREE)[0] == "PASS"
def rowdefs(kv):
    return [f[2:] for f in kv.get("CompilerFlags", "").split() if f.startswith("/D")]

# ---------------------------------------------------------------------------------------------------------------------- objcheck
def coff(obj):
    d = open(obj, "rb").read(); nsec, = struct.unpack_from("<H", d, 2); symp, nsym = struct.unpack_from("<II", d, 8); opt_ = struct.unpack_from("<H", d, 16)[0]
    strt = symp + nsym * 18
    def nm(e):
        if e[:4] == b"\0\0\0\0":
            o, = struct.unpack_from("<I", e, 4); return d[strt + o:d.index(b"\0", strt + o)].decode("latin-1")
        return e[:8].rstrip(b"\0").decode("latin-1")
    syms = {}; dfn, und_ = set(), set(); i = 0
    while i < nsym:
        e = d[symp + i * 18:symp + i * 18 + 18]; name = nm(e); syms[i] = name
        val, sec, typ, cl, aux = struct.unpack_from("<IhHBB", e, 8)
        if cl == 2 and sec > 0: dfn.add(name)
        if cl == 2 and sec == 0 and val == 0: und_.add(name)
        i += 1 + aux
    secs = []; drv = ""
    for k in range(nsec):
        h = d[20 + opt_ + 40 * k:20 + opt_ + 40 * (k + 1)]; name = h[:8].rstrip(b"\0").decode("latin-1")
        size, ptr, prel, _, nrel = struct.unpack_from("<IIIIH", h, 16)
        if name.startswith(".debug"): continue
        raw = d[ptr:ptr + size] if ptr else b""
        if name == ".drectve": drv += raw.decode("latin-1");
        rel = [(struct.unpack_from("<I", d, prel + 10 * j)[0], syms.get(struct.unpack_from("<I", d, prel + 10 * j + 4)[0], "?"),
                struct.unpack_from("<H", d, prel + 10 * j + 8)[0]) for j in range(nrel)]
        secs.append((name, raw, rel))
    return secs, dfn, und_, drv
def objcheck(oh, on, old, new, allow_libcp):
    """'' when equal under the one rename old -> new, else the reason"""
    sh, dh, uh, rh = coff(oh); sn, dn, un, rn = coff(on)
    mp = lambda s: new if s == old else s
    if {mp(x) for x in dh} != dn:
        return "defined symbols differ: +" + ",".join(sorted(dn - {mp(x) for x in dh}))[:150] + " -" + ",".join(sorted({mp(x) for x in dh} - dn))[:150]
    if uh != un: return "undefined symbols differ: +" + ",".join(sorted(un - uh))[:150] + " -" + ",".join(sorted(uh - un))[:150]
    if old not in dh or new not in dn: return "the self symbol is not defined as expected"
    if any(x.startswith(("??_7", "??_R")) for x in dn - dh): return "new vftable / RTTI symbol"
    if len(sh) != len(sn): return f"section count {len(sh)} v {len(sn)}"
    libcp = False
    for (a, ra, la), (b, rb, lb) in zip(sh, sn):
        if a != b: return f"section {a} v {b}"
        if a == ".drectve":
            if ra == rb: continue
            if allow_libcp and sorted(rb.decode("latin-1").split()) == sorted(ra.decode("latin-1").split() + ['/DEFAULTLIB:"libcp"']): libcp = True; continue
            return ".drectve differs"
        if ra != rb: return f"section {a} bytes differ"
        if [(o, mp(s), t) for o, s, t in la] != lb: return f"section {a} relocations differ"
    return "" if not libcp else "LIBCP"

# -------------------------------------------------------------------------------------------------------------------------- modes
def gen_mode():
    mx = int(opt("--max", "100000"))
    os.makedirs(f"{work}/gen", exist_ok=True); os.makedirs(f"{work}/pre", exist_ok=True)
    res = []; gens = []; global genx; genx = {}
    for a, old, P in entries:
        if only and a not in only: continue
        if len(gens) >= mx: res.append((a, "SKIP", old, P, "-", "over --max", {})); continue
        try: v, route, why, nsrc, extra = generate(a, old, P)
        except Exception as ex: v, route, why, nsrc, extra = "SKIP", "-", f"tool error {type(ex).__name__}: {ex}", None, {}
        res.append((a, v, old, P, route, why, extra)); genx[a] = extra
        if v == "GEN":
            open(f"{work}/gen/{a}.cpp", "w").write(nsrc); gens.append(a)
            for i, (vs, w) in enumerate(extra.get("variants", [])): open(f"{work}/gen/{a}.v{i + 1}.cpp", "w").write(vs)
    print("generated", len(gens), flush=True)
    pre = {}
    if "--no-pre" not in argv and gens:
        def one(a):
            kv = ent[a]; dfs = rowdefs(kv); out = {}
            d = f"{work}/pre/{a}"; os.makedirs(d, exist_ok=True)
            if "--tests-only" in argv:   # the pre-lock phase: only the landed test is prechecked (BRIEF_NF_BATCH: tests before the lock)
                if kv.get("TestSource"): shutil.copy("rebuild/tests/" + kv["TestSource"], f"{d}/test.cpp"); out["test"] = clang(f"{d}/test.cpp")
                else: out["test"] = ("NONE", [])
                out["new"] = out["head"] = ("PASS", []); out["variant"] = "tests-only"
                return a, out
            shutil.copy(f"{work}/gen/{a}.cpp", f"{d}/new.cpp"); shutil.copy("rebuild/src/compiled/" + kv["Source"], f"{d}/head.cpp")
            out["new"] = clang(f"{d}/new.cpp", dfs, HDR_TREE); out["head"] = clang(f"{d}/head.cpp", dfs); out["variant"] = ""
            i = 1
            while out["new"][0] != "PASS" and os.path.exists(f"{work}/gen/{a}.v{i}.cpp"):   # precedent-preamble variants, in order
                shutil.copy(f"{work}/gen/{a}.v{i}.cpp", f"{d}/new.cpp"); r = clang(f"{d}/new.cpp", dfs, HDR_TREE)
                if r[0] == "PASS": out["new"] = r; out["variant"] = f"v{i}"; shutil.copy(f"{work}/gen/{a}.v{i}.cpp", f"{work}/gen/{a}.cpp")
                i += 1
            if out["new"][0] != "PASS" and "// Flatten route" in rd(f"{work}/gen/{a}.cpp") and "class model: engine/" in rd(f"{work}/gen/{a}.cpp"):
                # error-driven prerequisites (NF320 addpre.py idea): a header `#error ... define FABLE_RETAIL_X` adds that landed switch, an
                # `unknown type name 'X'` / `incomplete type 'X'` adds engine/X.h ahead of the first include; at most 6 rounds
                cur = rd(f"{work}/gen/{a}.cpp"); r = clang(f"{d}/new.cpp", dfs, HDR_TREE) if False else out["new"]
                for _ in range(6):
                    errs = " ".join(r[1]); add = None
                    md = re.search(r'define (FABLE_RETAIL_\w+)', errs) or re.search(r'needs [^|]*?(FABLE_RETAIL_\w+)', errs)
                    mt = re.search(r"(?:unknown type name|incomplete (?:result )?type|unknown class name|no template named) '(?:class )?(\w+)'", errs)
                    if md and LM.get(md.group(1), 0) and "_PASS" not in md.group(1) and f"#define {md.group(1)}" not in cur: add = f"#define {md.group(1)}"
                    elif mt and os.path.exists(f"rebuild/include/engine/{mt.group(1)}.h") and f'engine/{mt.group(1)}.h"' not in cur: add = f'#include "engine/{mt.group(1)}.h"'
                    if not add: break
                    L = cur.split("\n")
                    k = next((i for i, l in enumerate(L) if l.startswith("#include")), None) if add.startswith("#include") else \
                        next((i for i, l in enumerate(L) if l.startswith(("#define", "#include"))), None)
                    if k is None: break
                    L.insert(k, add); cur = "\n".join(L)
                    open(f"{d}/new.cpp", "w").write(cur); r = clang(f"{d}/new.cpp", dfs, HDR_TREE)
                    if r[0] == "PASS":
                        open(f"{work}/gen/{a}.cpp", "w").write(cur); out["new"] = r; out["variant"] = (out["variant"] + " prereq").strip(); break
            if out["new"][0] != "PASS": shutil.copy(f"{work}/gen/{a}.cpp", f"{d}/new.cpp")
            elif out["variant"] and "--no-min" not in argv and "prereq" not in out["variant"]:
                # minimise the copied precedent preamble (NF320 lesson: minimal switch sets): drop the added includes other than the
                # class header in one step, then each added line alone, keeping a removal only while clang still passes
                cur = rd(f"{work}/gen/{a}.cpp").split("\n"); headl = set(l.strip() for l in rd(f"{d}/head.cpp").split("\n"))
                cls_inc = re.search(r'^// Flatten route .*?-> (\w+)::', "\n".join(cur), re.M).group(1)
                added = [i for i, l in enumerate(cur) if re.match(r'#\s*(define|include|undef)\b', l) and l.strip() not in headl]
                def test(lines):
                    open(f"{d}/min.cpp", "w").write("\n".join(lines)); return clang(f"{d}/min.cpp", dfs, HDR_TREE)[0] == "PASS"
                incs = {i for i in added if cur[i].startswith("#include") and f'engine/{cls_inc}.h"' not in cur[i]}
                if incs and test([l for i, l in enumerate(cur) if i not in incs]): drop = set(incs)
                else: drop = set()
                for i in reversed(added):
                    if i in drop or f'engine/{cls_inc}.h"' in cur[i]: continue
                    mdef = re.match(r'#\s*define\s+(\w+)', cur[i])
                    if mdef and re.search(r'^// Flatten route .*\b' + mdef.group(1) + r'\b', "\n".join(cur), re.M): continue   # the class switches stay
                    if test([l for j, l in enumerate(cur) if j not in drop | {i}]): drop.add(i)
                cur = [l for j, l in enumerate(cur) if j not in drop]
                open(f"{work}/gen/{a}.cpp", "w").write("\n".join(cur)); shutil.copy(f"{work}/gen/{a}.cpp", f"{d}/new.cpp")
                out["new"] = clang(f"{d}/new.cpp", dfs, HDR_TREE); out["variant"] += f" min-{len(drop)}/{len(added)}"
            if kv.get("TestSource"): shutil.copy("rebuild/tests/" + kv["TestSource"], f"{d}/test.cpp"); out["test"] = clang(f"{d}/test.cpp")
            else: out["test"] = ("NONE", [])
            ex = genx.get(a, {})
            if out["new"][0] == "PASS" and ex.get("route") == "header" and ex.get("virt") and ex.get("base") and ex.get("decl"):
                # override gate (advisor 2026-11-24), header route only (the declaration is this run's opt-in): with the row's FINAL preamble
                # the declaring class's base chain must already declare this virtual (no new slot), and a made-up name must fail; else retry
                # the unminimised variant, else SKIP. (On the switch route the header's own model declares the member; its sizeof / offset
                # asserts, which the precheck compiles, pin the layout the switch set gives.)
                def gate(path):
                    s = rd(path); k = s.find(f"{ex['cls']}::{ex['meth']}(")
                    pre_ = s[:s.rfind("\n", 0, k)] if k > 0 else s
                    okp = clang_text(pre_ + f"\nstruct NF324Probe : {ex['base']} {{ {ex['decl'].replace('virtual ', '', 1).rstrip(';')} override; }};\n", d, dfs)
                    ctl = clang_text(pre_ + f"\nstruct NF324Ctl : {ex['base']} {{ virtual void NF324MadeUpName() override; }};\n", d, dfs)
                    return okp and not ctl
                newslot = False
                mp = f"{AGENT_DIR}/work/model_{ex['cls']}.tsv"
                if os.path.exists(mp):
                    for l in open(mp).read().splitlines()[1:]:
                        q = l.split("\t")
                        if len(q) > 3 and q[2] == ex.get("pdb") and q[3] == "new": newslot = True
                if newslot: out["gate"] = "PASS new-slot (model tsv; the vftable probe is the proof)"
                elif not gate(f"{work}/gen/{a}.cpp"):
                    out["gate"] = "FAIL"
                    for i in range(1, 4):
                        vp = f"{work}/gen/{a}.v{i}.cpp"
                        if os.path.exists(vp) and clang(vp, dfs, HDR_TREE)[0] == "PASS" and gate(vp):
                            shutil.copy(vp, f"{work}/gen/{a}.cpp"); shutil.copy(vp, f"{d}/new.cpp"); out["gate"] = f"PASS v{i} unminimised"; break
                else: out["gate"] = "PASS"
            return a, out
        with ThreadPoolExecutor(JOBS) as ex:
            for a, out in ex.map(one, gens): pre[a] = out
        with open(f"{work}/PRECHECK.tsv", "w") as o:
            o.write("address\tnew\thead\ttest\tvariant\tnew_errors\thead_errors\n")
            for a in gens:
                p = pre[a]; o.write(f"{a}\t{p['new'][0]}\t{p['head'][0]}\t{p['test'][0]}\t{p['variant']}\t{' | '.join(p['new'][1])[:400]}\t{' | '.join(p['head'][1])[:300]}\n")
    out = []
    for a, v, old, P, route, why, extra in res:
        if v == "GEN" and a in pre:
            p = pre[a]
            if p["new"][0] != "PASS":
                v = "SKIP"; why = ("precheck: new source fails clang where HEAD passes: " if p["head"][0] == "PASS" else
                                   "precheck: new and HEAD sources fail clang (D1 cl /Zs route): ") + (p["new"][1][0] if p["new"][1] else "")[:200]
            elif p["head"][0] != "PASS": v = "SKIP"; why = "precheck: HEAD source fails clang (D1 cl /Zs route)"
            elif p["test"][0] == "FAIL": v = "SKIP"; why = "precheck: the landed test fails clang (D1 cl /Zs route)"
            elif p.get("gate", "").startswith("FAIL"): v = "SKIP"; why = f"override gate: with the final preamble {extra.get('base')} does not declare the virtual (a new slot)"
        out.append((a, v, old, P, route, why, bucket(why), json.dumps(extra.get("fixes", [])), ",".join(extra.get("switches") or []),
                    "; ".join([("ptr2ref " if "NF313" in x else "retype ") + x for x in extra.get("retypes", [])] + ([f"gate {pre[a].get('gate')}"] if a in pre and pre[a].get("gate") else [])
                              + ([f"pre {pre[a]['variant']}"] if a in pre and pre[a].get("variant") else []))))
    with open(f"{work}/GEN.tsv", "w") as o:
        o.write("address\tverdict\told\tnew\troute\treason\tbucket\tname_fixes\tswitches\tnotes\n")
        for r in out: o.write("\t".join(r) + "\n")
    print(collections.Counter(r[1] for r in out)); print(collections.Counter(r[6] for r in out).most_common(40))

def bucket(why):
    if not why: return "generated"
    b = re.sub(r'\b0x[0-9a-fA-F]+\b|\b[0-9a-f]{8}\b', 'A', why)
    b = re.sub(r'\b(declare|declares|defines|parameter|uses|named) \w+', r'\1 M', b); b = re.sub(r'`[^`]*`', 'T', b); b = re.sub(r'engine/\w+\.h', 'H', b); b = re.sub(r'\b[CN][A-Z]\w*(::\w+)?|\?[^ ]+', 'X', b)
    b = re.sub(r'\b(FABLE_\w+)', 'M', b); b = re.sub(r'\d+', 'n', b)
    return b.split(":")[0] + ": " + ":".join(b.split(":")[1:])[:90] if ":" in b else b[:90]

def vland(d, rows, vt):
    os.makedirs(d, exist_ok=True)
    json.dump({"result": {"authored": rows}}, open(f"{d}/payload.json", "w"))
    cat = {}
    want = {r["address"] for r in rows}
    for l in open("rebuild/oracles/auto-re-candidates.tsv"):
        a = l[:8].lower()
        if a in want: cat[a] = l.rstrip("\n")
    open(f"{d}/oracle.tsv", "w").write("address\tname\tlength\tbytes\n" + "".join("\t".join(cat[r["address"]].split("\t")[:4]) + "\n" for r in rows))
    env = dict(os.environ, LANDVERIFY_WORK=f"{os.path.dirname(d)}/wd")
    if HDR_TREE and d.endswith("/new"): env["FABLE_HDR_OVERLAY"] = HDR_TREE
    with open(f"{d}/vland.log", "w") as lf:
        subprocess.run(["timeout", str(vt), "python3", "-u", f"{LT}/vland.py", f"{d}/payload.json", f"{d}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=env)
    os.rename(f"{os.path.dirname(d)}/wd", f"{d}/wd")
    st = {}
    for l in open(f"{d}/vland.log", errors="replace"):
        m = re.match(r"([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if m and m.group(2) != "WARN": st.setdefault(m.group(1), (m.group(2), m.group(3)))
    return st

def grade_mode():
    import verify_and_land as V
    gen = [l.split("\t") for l in rd(f"{work}/GEN.tsv").splitlines()[1:]]
    rows = [g for g in gen if g[1] == "GEN" and (not only or g[0] in only)]
    bad = [g[0] for g in rows if lock_owner(g[0]) != AGENT]
    if bad: print("NOT LOCKED by", AGENT, ":", " ".join(bad[:20]), "..." if len(bad) > 20 else ""); sys.exit(2)
    for g in rows:   # staleness: the source generated from must still be the landed source
        if not os.path.exists(f"{work}/pre/{g[0]}/head.cpp") or rd(f"{work}/pre/{g[0]}/head.cpp") != rd("rebuild/src/compiled/" + ent[g[0]]["Source"]):
            print("STALE", g[0]); sys.exit(2)
    chunk = int(opt("--chunk", "25")); vt = int(opt("--vtimeout", "900"))
    run = f"{work}/{opt('--run', 'run')}"; assert not os.path.exists(run), "stale run dir: use a fresh --run name"
    os.makedirs(run)
    def payload(g, new):
        a = g[0]; kv = ent[a]
        tst = rd("rebuild/tests/" + kv["TestSource"]) if kv.get("TestSource") else ""
        base = dict(address=a, module=kv.get("Module", ""), name=g[3] and re.match(r'\?([^@]+)@', g[3]).group(1),
                    test_cpp=tst, pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
        extra = [f for f in kv.get("CompilerFlags", "").split() if f not in ("/O2", "/Oy", "/W3")]
        if extra: base["flags"] = extra
        return dict(base, source_cpp=rd(f"{work}/gen/{a}.cpp") if new else rd("rebuild/src/compiled/" + kv["Source"]), leaf=g[3] if new else g[2])
    chunks = [rows[i:i + chunk] for i in range(0, len(rows), chunk)]
    def do(ci):
        c = chunks[ci]; d = f"{run}/c{ci:03d}"
        t0 = time.time(); sh = vland(f"{d}/head", [payload(g, False) for g in c], vt); sn = vland(f"{d}/new", [payload(g, True) for g in c], vt)
        print(f"chunk {ci} ({len(c)} rows) {time.time() - t0:.0f}s", flush=True)
        return ci, sh, sn
    results = {}
    with ThreadPoolExecutor(JOBS) as ex:
        for ci, sh, sn in ex.map(do, range(len(chunks))): results[ci] = (sh, sn)
    res = []
    for ci, c in enumerate(chunks):
        sh, sn = results[ci]; d = f"{run}/c{ci:03d}"
        for g in c:
            a, old, P, route = g[0], g[2], g[3], g[4]
            oh = f"{d}/head/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"; on = f"{d}/new/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"
            gh, gn = sh.get(a, ("NOT_GRADED", "-")), sn.get(a, ("NOT_GRADED", "-"))
            if not (os.path.exists(oh) and os.path.exists(on)): res.append((a, "REJECT", old, P, route, f"no object (head {gh}, new {gn})")); continue
            with contextlib.redirect_stderr(io.StringIO()): ygh, ygn = V.obj_text(oh, old)[2], V.obj_text(on, P)[2]
            if ygh != old: res.append((a, "REJECT", old, P, route, f"HEAD graded symbol {ygh} (ENTRIES self {old})")); continue
            if ygn != P: res.append((a, "REJECT", old, P, route, f"new graded symbol {ygn}")); continue
            why = objcheck(oh, on, old, P, route in ("switch", "header"))
            if why not in ("", "LIBCP"): res.append((a, "REJECT", old, P, route, "objcheck: " + why)); continue
            if not (gn[0].startswith(("MATCH", "RELOCATION_MATCH")) and gn[1] == "PASS"): res.append((a, "REJECT", old, P, route, f"grade {gn} (HEAD {gh})")); continue
            res.append((a, "ACCEPT", old, P, route, ("objcheck equal but .drectve +libcp (NF318 Q1)" if why == "LIBCP" else "objcheck ALL_EQ") + f", {gn[0]} {gn[1]}"))
    with open(f"{run}/GRADE.tsv", "w") as o:
        o.write("address\tverdict\told\tnew\troute\treason\n" + "".join("\t".join(r) + "\n" for r in res))
    print(collections.Counter(r[1] for r in res))

def export_mode():
    out = os.path.abspath(argv[2]); os.makedirs(f"{out}/rows", exist_ok=True); os.makedirs(f"{out}/head", exist_ok=True)
    gen = [l.split("\t") for l in rd(f"{work}/GEN.tsv").splitlines()[1:]]
    gr = {}
    for gp in sorted(glob.glob(f"{work}/run*/GRADE.tsv")):    # later runs win
        gr.update({l.split("\t")[0]: l.split("\t") for l in rd(gp).splitlines()[1:]})
    res = []
    for g in gen:
        if only and g[0] not in only: continue      # V5: one batch per export (--only)
        r = gr.get(g[0])
        res.append(r[:4] + [r[5] + f" [route {r[4]}]"] if r else [g[0], "SKIP", g[2], g[3], g[5] or "not graded"])
    if "--holds" in argv:   # addresses held after grading (e.g. a census hit): never exported as ACCEPT
        hold = {l.split("\t")[0]: l.split("\t", 1)[1] for l in rd(opt("--holds")).splitlines() if l.strip()}
        res = [([r[0], "HOLD", r[2], r[3], hold[r[0]]] if r[0] in hold and r[1] == "ACCEPT" else r) for r in res]
    acc = {r[0] for r in res if r[1] == "ACCEPT"}
    for a in acc: shutil.copy(f"{work}/gen/{a}.cpp", f"{out}/rows/{a}.cpp")
    heads = []
    for p in sorted(glob.glob(f"{work}/run*/c*/head/payload.json")):
        heads += [x for x in json.load(open(p))["result"]["authored"] if x["address"] in acc]
    heads = list({x["address"]: x for x in heads}.values())
    # a HEAD source or test that carries a retail byte-dump comment is never handed back: it is left out of the payload and listed, and
    # land_scripted.py's stale check holds the row until the coordinator rebuilds its HEAD entry from the repo (equal by construction)
    dump = re.compile(r'^//[^\n]*\b[Rr]etail\b[^\n]*\b(?:[0-9a-fA-F]{2} ){3,}[0-9a-fA-F]{2}\b', re.M)
    out_heads = [x for x in heads if dump.search(x.get("source_cpp") or "") or dump.search(x.get("test_cpp") or "")]
    heads = [x for x in heads if x not in out_heads]
    open(f"{out}/head/HEAD_FROM_REPO.txt", "w").write("".join(x["address"] + "\n" for x in out_heads))
    json.dump({"result": {"authored": heads}}, open(f"{out}/head/payload.json", "w"))
    with open(f"{out}/RESULTS.tsv", "w") as o:
        o.write("address\tverdict\told\tnew\treason\n" + "".join("\t".join(r) + "\n" for r in res))
    with open(f"{out}/TABLE_CHANGES.tsv", "w") as o:
        o.write("action\tsymbol\ttarget\twitness\tnote\n")
        for r in res:
            if r[1] != "ACCEPT": continue
            for c in tbl_lines:
                if c[0] == r[2] and c[1].lower() == r[0]:
                    if c[2].lower().replace("0x", "") == r[0]: o.write(f"delete\t{c[0]}\t{c[1]}\t{c[2]}\tflatten v4: the self now defines {r[3]}\n")
                    else: o.write(f"UNIT?\t{c[0]}\t{c[1]}\t{c[2]}\tflatten v4: another witness binds the old self spelling; not a delete\n")
    print(collections.Counter(r[1] for r in res))

def objcheck_mode():   # objcheck <work(ignored)> <head.obj> <new.obj> <old> <new> [libcp]: the grade's object comparison alone (controls)
    r = objcheck(argv[2], argv[3], argv[4], argv[5], len(argv) > 6)
    print("ALL_EQ" if r == "" else r)
{"gen": gen_mode, "grade": grade_mode, "export": export_mode, "objcheck": objcheck_mode}[MODE]()
