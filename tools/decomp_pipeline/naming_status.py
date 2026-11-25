#!/usr/bin/env python3
"""naming_status.py [--out DIR] [--history]: the naming tracker (owner 2026-11-22: "make it easier to track our naming progress
and which are done and which aren't, and batch items that are easier to work together").

Generated, never hand-edited. Reads only committed files:
  rebuild/oracles/reloc_bindings.tsv      the spelling each landed caller binds at a retail target (one witness line per spelling)
  rebuild/build_candidates.ps1            landed rows (Address -> Source)
  rebuild/src/compiled/**/<Source>        each landed row's `// Leaf:` line = the spelling the row itself defines
  rebuild/oracles/auto-re-candidates.tsv  catalog names
  ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv   the two retail-era PDB name sets (names only)
  rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv   the queue (free-text log; lines in NAME_FIXES shape are tracked)

A retail address's spellings = every symbol the table binds at it plus its landed row's own Leaf. What this measures is spelling
agreement and PDB-name membership, by NAME only: a PDB name on the wrong body (the nf265 Delete trio) still reads as agreeing.
Self leaves of the ~16k landed rows without a Leaf line and vtable-slot-only references are not seen (v1 limits; the census in an
agent pass is object-level and remains the proof).

Status of an address (measured, not "done"):
  agree_pdb        one spelling, and it is a PDB name                       -> nothing to do
  agree_std        one spelling, not a PDB name, std:: / compiler-generated / C name  -> nothing to do (no PDB spelling expected)
  agree_unverified one spelling, not a PDB name, no stand-in marker          -> check later (often a cross-build signature difference)
  standin          one spelling with a stand-in marker (struct T, _<addr>, Native::, DAT_, ...) -> open
  split_fixable    2+ spellings, exactly one a PDB name                      -> open, the target spelling is known
  split_pdb_multi  2+ spellings, 2+ of them PDB names                        -> ruling (byte-identical bodies folded by the linker?)
  split_unknown    2+ spellings, none a PDB name                             -> open, needs research
Tier of an open address (what the fix touches; batch by class within a tier):
  T0 metadata      catalog name is a stand-in while every code spelling is the PDB one (catalog / file rename only, no compile)
  T1 binders       the PDB spelling is the row's own leaf (or no landed row): only callers bind wrong spellings (in-place respell)
  T2 self-access   the row's own leaf is wrong and differs from the PDB spelling only in the access / virtual / const letters
  T3 unit          anything else (self rename with a new class or signature, data / vtable targets, ports, folds)
Outputs (deterministic, sorted): <out>/OPEN.tsv, <out>/BATCHES.tsv, <out>/QUEUE_STATUS.tsv, <out>/SUMMARY.md; with --history,
<out>/HISTORY.tsv (table-only split count at each day's first commit, from git; the comparable long-run trend).
"""
import argparse, collections, os, re, subprocess, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
P = lambda *a: os.path.join(ROOT, *a)

STANDIN = re.compile(r"(?:_[0-9a-fA-F]{8}(?![0-9a-fA-F])|@T@@|^\?\w+@T@@|@S@@|^\?\?_?[0-9A-Z][TS]@@|DAT_|FUN_|_vtbl|Unproven|@Native@@|ForwardTarget_|g_obj_|"
                     # @S@@ / ??1T@@ / ??0S@@: the placeholder classes T and S (no PDB class has either name; finish plan route 0)
                     r"_Retail@@|Methods@@|\bv\d\d@|Virtual\d+@|LAB_|thunk_)")
CAT_STANDIN = re.compile(r"(?:_[0-9a-fA-F]{8}(?![0-9a-fA-F])|FUN_|DAT_|::void |Unproven|Virtual\d+|\bT::)")
# MSVC function access/storage letter after the name@scope@@ : A-H private/protected/public member (Q..Z for publics etc.)
ACCESS = re.compile(r"^(\?[^@]+@[^?]*?@@)([A-Z])([A-E]?)(.*)$")


def read_tsv(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        return [l.rstrip("\r\n").split("\t") for l in f]


def pdb_names():
    s = set()
    for fn in ("egor_pdb_names.tsv", "fablewin_pdb_names.tsv"):
        for r in read_tsv(P("ghidra_out", fn)):
            if len(r) > 1: s.add(r[1])
    return s


def landed_rows():
    ps = open(P("rebuild", "build_candidates.ps1"), encoding="utf-8-sig", errors="replace").read()
    ent = {}
    for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
        kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
        a = kv.get("Address", "").lower().replace("0x", "")
        if a and kv.get("Source"): ent[a] = kv
    return ent


# Rows that compile another row's file (option S, owner ruling 2026-11-24; NF376 pilot, NF379 tools). A THIN row is comments, its own
# `// Leaf:` and one `#include "../src/compiled/<owner Source>"` (EH funclets, the nf251 0x00B2AB10 fold, option S members); a FOLD row
# includes another row's file and adds code of its own (0x00BEB490). Both re-grade when the included file changes; only a thin row
# hands its edits to the owner.
_COMPILED_INC = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"\.\./src/compiled/([^"]+)"', re.M)


def _row_text(src):
    try:
        with open(P("rebuild", "src", "compiled", src), encoding="utf-8", errors="replace") as f: return f.read()
    except OSError:
        return None


def owner_source(src):
    """The catalog Source that holds a row's code: a thin row resolves to the file it includes, any other row to itself. Tools that
    EDIT a row's text (respells, retypes, access / unit fixes, NAME_FIXES) must edit owner_source(src); then every row in
    includers_of(owner) re-grades (recheck.py --with-includers; land_scripted.py does it at landing)."""
    t = _row_text(src)
    if t is None: return src
    code = re.sub(r"//[^\n]*", "", t)
    m = _COMPILED_INC.findall(code)
    return m[0] if len(m) == 1 and not re.sub(r'#[ \t]*include[ \t]+"[^"]+"', "", code).strip() else src


_INCLUDERS = {}
def includers_map(ent=None):
    """{included Source: [addresses of the landed rows that include it, directly]}, one pass over the landed sources (cached per catalog)."""
    ent = ent or landed_rows(); key = id(ent)
    if key not in _INCLUDERS:
        m = {}
        for a, kv in ent.items():
            t = _row_text(kv.get("Source", ""))
            if not t or "../src/compiled/" not in t: continue
            for inc in set(_COMPILED_INC.findall(re.sub(r"//[^\n]*", "", t))):
                if inc != kv["Source"]: m.setdefault(inc, []).append(a)
        _INCLUDERS.clear(); _INCLUDERS[key] = (ent, m)      # keep `ent` alive so its id is not reused
    return _INCLUDERS[key][1]


def includers_of(src, ent=None):
    """The landed addresses whose Source compiles <src> through `#include "../src/compiled/<src>"`, thin or fold, transitively, sorted.
    A change to <src> must re-grade every one of them."""
    ent = ent or landed_rows(); m = includers_map(ent); out = set(); todo = [src]
    while todo:
        for a in m.get(todo.pop(), []):
            if a not in out:
                out.add(a); todo.append(ent[a]["Source"])
    return sorted(out)


def leaf_of(src):
    p = P("rebuild", "src", "compiled", src)
    try:
        with open(p, encoding="utf-8", errors="replace") as f:
            for i, l in enumerate(f):
                if l.startswith("// Leaf:"):
                    t = l[8:].strip().split()
                    return t[0] if t else ""
                if i > 400: break
    except OSError:
        pass
    return ""


_LIBSYMS = None
def lib_symbols():
    """symbols of proven toolchain / vendor library code (libsig: library_signatures.tsv, vendor_signatures.tsv `symbol` columns)."""
    global _LIBSYMS
    if _LIBSYMS is None:
        _LIBSYMS = set()
        for f in ("library_signatures.tsv", "vendor_signatures.tsv"):
            p = P("rebuild", "oracles", f)
            if not os.path.exists(p): continue
            rows = read_tsv(p)
            if not rows or "symbol" not in rows[0]: continue
            k = rows[0].index("symbol")
            _LIBSYMS.update(r[k] for r in rows[1:] if len(r) > k and r[k])
    return _LIBSYMS


def is_std(sym):
    # A C name (no `?`) counts only when it is a proven library symbol (finish plan route 0, 2026-11-23: is_std used to accept every C
    # name, so 1,270 made-up C names such as `_Construct` / `@VecDelDtor@12` counted as done).
    # Import-table names (`__imp__GetFileSize@8`) and SDK GUID data (`_GUID_SysMouse`, `_IID_...`) are the real names. `_$E1` / `_$E2`
    # are the compiler's placeholder for a dynamic initializer whose PDB name is `dynamic initializer for 'X'`: open.
    if not sym.startswith("?"):
        return sym in lib_symbols() or sym.startswith("__imp_") or re.match(r"_(GUID|IID|CLSID|DIID|LIBID)_\w+$", sym) is not None
    return "@std@@" in sym or "@_STL@@" in sym or sym.startswith("??_")


def klass(sym):
    """the batch key of a mangled name: its innermost class / namespace (a template's base name, e.g. vector), '' for free names."""
    if not sym.startswith("?"): return ""
    if sym.startswith("??$"):                      # a function template ??$name@<args>@scope@@: key on the template name
        m = re.match(r"^\?\?\$([^@]+)@", sym); return m.group(1) if m else ""
    if sym.startswith("??_") and len(sym) > 4:     # ??_G / ??_7 / ??_E <type>@@
        rest = sym[4:]
    elif sym.startswith("??") and len(sym) > 3:    # ??0 ctor, ??1 dtor, ??R operator(), ??4 operator= ... <class>@@
        rest = sym[3:]
    else:                                          # ?name@scope@...@@
        m = re.match(r"^\?[^@]*@", sym)
        if not m: return ""
        rest = sym[m.end():]
        if rest.startswith("@"): return ""         # a free function ?name@@...
    if rest.startswith("?$"):
        m = re.match(r"^\?\$([^@]+)@", rest); return m.group(1) if m else ""
    m = re.match(r"^([^@?]+)@", rest)
    return m.group(1) if m else ""


def access_only(a, b):
    """True when two mangled function names differ only in the access / virtual / const letters after @@."""
    ma, mb = ACCESS.match(a), ACCESS.match(b)
    if not (ma and mb): return False
    return ma.group(1) == mb.group(1) and ma.group(4) == mb.group(4) and (ma.group(2), ma.group(3)) != (mb.group(2), mb.group(3))


FIXORDER = {"catalog": -1, "access": 0, "standin_alias": 1, "signature": 2, "other": 3, "standin": 4, "-": 5}


def fix_kind(good, wrong):
    """how the wrong spellings differ from the PDB one: access (access / virtual / const letters only), standin_alias (a stand-in
    name), signature (same ?name@scope@@, other parameter / return types), other."""
    if not wrong: return "other"
    ks = set()
    for w in wrong:
        if access_only(w, good): ks.add("access")
        elif STANDIN.search(w): ks.add("standin_alias")
        elif w.split("@@")[0] == good.split("@@")[0]: ks.add("signature")
        else: ks.add("other")
    return min(ks, key=lambda k: FIXORDER[k]) if len(ks) == 1 else "other"


def table_spellings(text):
    by = collections.defaultdict(lambda: collections.defaultdict(set))
    lines = text.split("\n")
    for l in lines[1:]:
        f = l.rstrip("\r").split("\t")
        if len(f) >= 3 and f[0] and f[1]: by[f[1].lower()][f[0]].add(f[2].lower())
        elif len(f) >= 2 and f[0] and f[1]: by[f[1].lower()][f[0]]
    return by


def norm(sym):
    """one spelling per address for the compiler's deleting-dtor alias pair: the vector form ??_E is reported as ??_G."""
    return "??_G" + sym[4:] if sym.startswith("??_E") else sym


def complete(sym):
    if sym.startswith("?"): return "@@" in sym and not sym.endswith("@")
    return bool(sym)


def resolve_leaf(lf, sp):
    """(spelling, how): a `// Leaf:` line is a full mangled name, a prefix of one, or a plain / C name the grader matches.
    A prefix or plain leaf that matches a table spelling agrees with it; one that matches none leaves the self spelling unmeasured."""
    if not lf: return "", "none"
    lf = norm(lf)
    if lf in sp: return lf, "full"
    if lf.startswith("?") and not complete(lf):
        m = [s for s in sp if s.startswith(lf)]
        return (m[0], "prefix") if len(m) == 1 else ("", "prefix_unmatched")
    if lf.startswith("?"): return lf, "full"
    pat = re.compile(r"^[?_@]*" + re.escape(lf.lstrip("_@")) + r"(@|$)")
    m = [s for s in sp if pat.match(s)]
    if len(m) == 1: return m[0], "plain"
    if not sp and re.match(r"^[_@]", lf): return lf, "full"      # a decorated C name with no callers
    return "", "plain_unmatched"


def git(*a):
    return subprocess.run(["git", "-C", ROOT] + list(a), capture_output=True, text=True, errors="replace").stdout


def history(out):
    rows = []
    days = git("log", "--format=%cs %H", "--", "rebuild/oracles/reloc_bindings.tsv").split("\n")
    first = {}
    for l in days:
        if not l.strip(): continue
        d, h = l.split()
        first[d] = h            # log is newest first: the last seen per day is that day's first commit
    pdb = pdb_names()
    for d in sorted(first)[-60:]:
        by = table_spellings(git("show", first[d] + ":rebuild/oracles/reloc_bindings.tsv"))
        multi = [t for t, s in by.items() if len(s) > 1]
        fix = sum(1 for t in multi if sum(1 for x in by[t] if x in pdb) == 1)
        rows.append((d, first[d][:10], len(by), len(multi), fix))
    by = table_spellings(open(P("rebuild", "oracles", "reloc_bindings.tsv"), encoding="utf-8", errors="replace").read())
    multi = [t for t, s in by.items() if len(s) > 1]
    rows.append(("now", git("rev-parse", "--short=10", "HEAD").strip(), len(by), len(multi),
                 sum(1 for t in multi if sum(1 for x in by[t] if x in pdb) == 1)))
    with open(os.path.join(out, "HISTORY.tsv"), "w") as o:
        o.write("date\tcommit\ttable_targets\ttable_split_targets\ttable_split_one_pdb\n")
        for r in rows: o.write("\t".join(map(str, r)) + "\n")
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=P("rebuild", "candidates", "manual", "naming", "status"))
    ap.add_argument("--history", action="store_true")
    ap.add_argument("--dump-rec", default="", help="write every measured address record as JSON (for naming/order_match.py)")
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    pdb = pdb_names()
    ent = landed_rows()
    cat = {r[0].lower(): r[1] for r in read_tsv(P("rebuild", "oracles", "auto-re-candidates.tsv"))[1:] if len(r) > 1}
    by = table_spellings(open(P("rebuild", "oracles", "reloc_bindings.tsv"), encoding="utf-8", errors="replace").read())
    leaf = {ad: leaf_of(e["Source"]) for ad, e in ent.items()}
    # the measurement pass (lane_tools/self_symbols.py -> rebuild/oracles/self_symbols.tsv, ROADMAP NF302 Q3): the symbol each landed row's
    # own object defines, read from the grade's own comparison. Trusted only when the grade matched (a DIFFER row may have graded another
    # function); used when the row's Leaf line is not a usable full spelling. Last line per address wins.
    measured = {}
    msp = P("rebuild", "oracles", "self_symbols.tsv")
    if os.path.exists(msp):
        for r in read_tsv(msp)[1:]:
            if len(r) > 2: measured[r[0].lower()] = r[1] if (r[1] and r[2] in ("MATCH", "RELOCATION_MATCH")) else ""

    status, rec = {}, {}
    unmeasured = collections.Counter()
    for t in sorted(set(by) | set(ent)):
        sp = {}
        for s, c in by.get(t, {}).items(): sp.setdefault(norm(s), set()).update(c)
        self_leaf, self_how = resolve_leaf(leaf.get(t, ""), sp)
        if t in ent and self_how != "full" and measured.get(t):
            self_leaf, self_how = norm(measured[t]), "full"     # what the object defines (measurement pass)
        if self_leaf: sp.setdefault(self_leaf, set()).add("self")
        if t in ent and self_how != "full": unmeasured[self_how] += 1
        if not sp:
            status[t] = "unmeasured"
            rec[t] = dict(target=t, status="unmeasured", tier="", landed="y", catalog=cat.get(t, ""), self_leaf=leaf.get(t, ""),
                          pdb_spelling="", klass="", spellings="", n_spellings=0, source=ent.get(t, {}).get("Source", ""))
            continue
        names = sorted(sp)
        pd = [s for s in names if s in pdb]
        if len(names) == 1:
            s = names[0]
            st = "agree_pdb" if s in pdb else ("standin" if STANDIN.search(s) else ("agree_std" if is_std(s) else "agree_unverified"))
        else:
            st = "split_fixable" if len(pd) == 1 else ("split_pdb_multi" if len(pd) > 1 else "split_unknown")
        cn = cat.get(t, "")
        tier = ""
        if st in ("standin", "split_fixable", "split_unknown"):
            good = pd[0] if len(pd) == 1 else ""
            if st == "split_fixable" and (not self_leaf or self_leaf == good):
                tier = "T1"
            elif st == "split_fixable" and self_leaf and access_only(self_leaf, good):
                tier = "T2"
            else:
                tier = "T3"
        elif st in ("agree_pdb",) and cn and CAT_STANDIN.search(cn):
            # T0 (catalog / file rename only) needs the row's own Leaf to confirm the PDB spelling; a row with no Leaf line may
            # still define its stand-in (0x0073D180 / 0x00863090, landing nf295: callers moved, the self row defines
            # CCountedPointer_0073cc90), and a plain / prefix Leaf can resolve onto the PDB spelling while the object defines a stand-in
            # (the AutoTinyVirtualObject DeleteData rows, NF284), so only a full mangled Leaf makes it T0; otherwise a T3 self port.
            st, tier = "catalog_standin", ("T0" if self_leaf and self_leaf in pdb and self_how == "full" else "T3")
        status[t] = st
        good = pd[0] if len(pd) == 1 else ""
        rec[t] = dict(target=t, status=st, tier=tier, landed="y" if t in ent else "n", catalog=cn, self_leaf=self_leaf,
                      pdb_spelling=good, klass=klass(good or self_leaf or (names[0] if names else "")),
                      spellings=" || ".join("%s (%s)" % (s[:160], ",".join(sorted(sp[s])[:6]) + (",..." if len(sp[s]) > 6 else "")) for s in names)[:700],
                      n_spellings=len(names), names=names, source=ent.get(t, {}).get("Source", ""),
                      fix=("catalog" if tier == "T0" else "self" if st == "catalog_standin" else fix_kind(good, [x for x in names if x != good]) if good else
                           ("standin" if st == "standin" else "")))

    # Shared names (finish plan route 0, 2026-11-23): one non-PDB, non-std name that is the only spelling of several addresses is at most
    # right once (`AutoTinyCountedHandle::Reset` on 341 addresses, `CTCCarryable_OnKill` on 173), so those rows are stand-ins (T3 identity).
    # A PDB name on several addresses is listed for audit (SHARED_PDB.tsv) and keeps its status: overloads of one name across classes do
    # not share a full mangled name, so a shared full PDB symbol is an identity conflict or an ICF fold to check.
    # Not counted as a second address of a name (NF385 q1 / q6, 2026-11-24): an EH unwind funclet row (its Leaf is its parent
    # function's symbol, sliced at the funclet offset), and a data symbol (storage-class digit after the scope, e.g.
    # ?NullCoord@CScriptThing@@1VC3DVector@@A, whose +offset entries share the base symbol).
    def _not_a_second_address(t2, s2):
        src = ent.get(t2, {}).get("Source", "")
        return "Unwind" in os.path.basename(src) or bool(re.match(r"\?[^@]+@(?:[^@]+@)*@[0-4]", s2))
    single = collections.defaultdict(list)
    for t2, r2 in rec.items():
        if r2["status"] in ("agree_unverified", "agree_std", "agree_pdb") and r2["n_spellings"] == 1:
            s2 = r2["self_leaf"] or r2["pdb_spelling"] or r2["spellings"].split(" (")[0]
            if _not_a_second_address(t2, s2): continue
            single[s2].append(t2)
    shared_pdb = []
    for s2, ts in single.items():
        if len(ts) < 2: continue
        for t2 in ts:
            if rec[t2]["status"] == "agree_pdb": shared_pdb.append((s2, t2)); continue
            if rec[t2]["status"] == "agree_std" and s2.startswith("??_"): continue   # compiler-generated thunks fold legitimately
            rec[t2].update(status="standin", tier="T3", fix="shared name (%d addresses)" % len(ts)); status[t2] = "standin"
    with open(os.path.join(a.out, "SHARED_PDB.tsv"), "w") as o:
        o.write("symbol\ttarget\n" + "".join("%s\t%s\n" % x for x in sorted(shared_pdb)))

    # RTTI vftable placement (ghidra_out/labels_rtti_port.tsv, tools/rtti_port_all.py: own-class slots only, ICF-collided addresses dropped):
    # the retail second source for a virtual member's name (finish plan route 0).
    rtti = {}
    for fn in ("labels_rtti_port.tsv", "labels_rtti_port_dtors.tsv"):     # member slots; deleting destructors (tools/rtti_port_dtors.py)
        rp = P("ghidra_out", fn)
        if os.path.exists(rp):
            for r in read_tsv(rp):
                if len(r) > 1: rtti.setdefault(r[0].lower(), r[1])
    # RTTI against the spelling (advisor, 2026-11-23): the retail vftable placement is a retail second source, so a row whose only spelling
    # contradicts it is not done (rtti_conflict, T3 identity), and a row whose RTTI name is not what it spells yet is identified but not
    # emitted (rtti_identified: the name is known; the code still has to define it, e.g. the deleting destructors' ctor-TU ports).
    for t2, r2 in rec.items():
        r2["rtti"] = rn = rtti.get(t2, "")
        if not rn or rn in r2.get("names", ()): continue   # full spellings (the spellings column is truncated)
        # deleting destructors: the CLASS comes from the RTTI slot, the KIND (??_G scalar / ??_E vector) from the retail body, which the
        # grade enforces (owner question 2026-11-23, advisor ruling): ??_G and ??_E of the same class and tail agree here
        if rn[:4] in ("??_G", "??_E") and any(x[:4] in ("??_G", "??_E") and x[4:] == rn[4:] for x in r2.get("names", ())): continue
        if r2["status"] in ("agree_pdb", "agree_std"):
            r2.update(status="rtti_conflict", tier="T3", fix="RTTI names " + rn[:80]); status[t2] = "rtti_conflict"
        elif r2["status"] in ("agree_unverified", "standin", "split_unknown"):
            r2.update(status="rtti_identified", tier="T3", fix="emit " + rn[:80]); status[t2] = "rtti_identified"
    cols = ["target", "status", "tier", "fix", "klass", "n_spellings", "pdb_spelling", "self_leaf", "landed", "catalog", "source", "spellings", "rtti"]
    if a.dump_rec:
        import json as _json
        with open(a.dump_rec, "w") as _f: _json.dump(rec, _f)
    openst = ("standin", "split_fixable", "split_unknown", "split_pdb_multi", "catalog_standin", "rtti_conflict", "rtti_identified")
    with open(os.path.join(a.out, "OPEN.tsv"), "w") as o:
        o.write("\t".join(cols) + "\n")
        for t in sorted(rec, key=lambda t: (rec[t]["tier"] or "T9", rec[t]["status"], rec[t]["klass"], t)):
            if rec[t]["status"] in openst: o.write("\t".join(str(rec[t][c]) for c in cols) + "\n")

    # batches: tier x class, with the binder rows a T1 batch would touch
    bat = collections.defaultdict(list)
    for t, r in rec.items():
        if r["tier"]: bat[(r["tier"], r["fix"] or "-", r["klass"] or "(none)")].append(t)
    with open(os.path.join(a.out, "BATCHES.tsv"), "w") as o:
        o.write("tier\tfix\tklass\tn_targets\tn_rows_touched\theader\ttargets\n")
        for (tier, fx, k), ts in sorted(bat.items(), key=lambda kv: (kv[0][0], FIXORDER.get(kv[0][1], 9), -len(kv[1]), kv[0][2])):
            rows = set()
            for t in ts:
                for s, c in by.get(t, {}).items():
                    if s != rec[t]["pdb_spelling"]: rows |= c
                if tier != "T1" and t in ent: rows.add(t)
            hdr = "engine/%s.h" % k if k and os.path.exists(P("rebuild", "include", "engine", k + ".h")) else ""
            o.write("\t".join([tier, fx, k, str(len(ts)), str(len(rows)), hdr, " ".join(sorted(ts)[:200])]) + "\n")

    # queue lines in NAME_FIXES shape: <tag> <0xADDR...> <?mangled correct symbol> ...
    qs = collections.Counter()
    qpath = P("rebuild", "candidates", "manual", "naming", "NAME_FIXES_QUEUE.tsv")
    with open(os.path.join(a.out, "QUEUE_STATUS.tsv"), "w") as o:
        o.write("line\ttag\taddress\tcorrect_symbol\tstatus\tother_spellings\n")
        for i, f in enumerate(read_tsv(qpath), 1):
            if len(f) < 3: continue
            m = re.match(r"^\s*(?:0x)?([0-9A-Fa-f]{8})\b", f[1])
            sym = f[2].split()[0] if f[2].strip() else ""
            if not m or not sym.startswith("?"):
                qs["untracked"] += 1; continue
            t = m.group(1).lower()
            sp = set(by.get(t, {})) | ({leaf[t]} if leaf.get(t) else set())
            st = "closed" if sp == {sym} else ("partial" if sym in sp else ("open" if sp else "no_binding"))
            qs[st] += 1
            o.write("\t".join([str(i), f[0], t, sym, st, " || ".join(sorted(sp - {sym}))[:400]]) + "\n")

    c = collections.Counter(status.values())
    tiers = collections.Counter(r["tier"] for r in rec.values() if r["tier"])
    hist = history(a.out) if a.history else []
    total = len(rec)
    done = c["agree_pdb"] + c["agree_std"]
    with open(os.path.join(a.out, "SUMMARY.md"), "w") as o:
        o.write("# Naming status (generated by `tools/decomp_pipeline/naming_status.py`; do not edit)\n\n")
        o.write("Commit: `%s`. Every retail address the landed code names: the table's caller spellings plus each landed row's own "
                "`// Leaf:` (rows without a Leaf line contribute callers only). Name agreement and PDB membership are by name, "
                "not body; an agent's object-level census stays the proof.\n\n" % git("rev-parse", "--short=10", "HEAD").strip())
        o.write("| Status | Addresses | Share | Meaning |\n|---|---:|---:|---|\n")
        mean = dict(agree_pdb="one spelling, a PDB name: done", agree_std="one spelling, std / compiler / C name: done",
                    agree_unverified="one spelling, not a PDB name, no stand-in marker: check later",
                    catalog_standin="callers agree on the PDB name, catalog / file name is a stand-in (T0 when the row's own Leaf confirms it, else T3 self)",
                    standin="one spelling with a stand-in marker: open", split_fixable="2+ spellings, one PDB name: open, target known",
                    split_pdb_multi="2+ PDB names at one address: ruling", split_unknown="2+ spellings, no PDB name: open, research")
        mean["unmeasured"] = "landed row, no caller binding and no usable mangled Leaf line: not measured"
        mean["rtti_conflict"] = "one spelling, but the retail RTTI vftable placement names another symbol: open, identity"
        mean["rtti_identified"] = "the retail RTTI placement names it; the code does not define that name yet: identified, emission open"
        for k in ["agree_pdb", "agree_std", "agree_unverified", "catalog_standin", "standin", "split_fixable", "split_pdb_multi", "split_unknown",
                  "rtti_conflict", "rtti_identified", "unmeasured"]:
            o.write("| %s | %d | %.2f%% | %s |\n" % (k, c[k], 100.0 * c[k] / max(total, 1), mean[k]))
        o.write("| **total** | **%d** | | done (agree_pdb + agree_std): **%d (%.2f%%)** |\n\n" % (total, done, 100.0 * done / max(total, 1)))
        o.write("| Tier | Open addresses | Batches (by class) | What the fix touches |\n|---|---:|---:|---|\n")
        tm = dict(T0="catalog / file rename only, no compile", T1="callers' spellings only (the row already defines the PDB name, or there is no landed row)",
                  T2="the row's own leaf, access / virtual / const letters only (zero line delta)",
                  T3="a unit: self rename, port, vtable, fold, data target, research")
        for k in ["T0", "T1", "T2", "T3"]:
            o.write("| %s | %d | %d | %s |\n" % (k, tiers[k], sum(1 for (tt, _f, _k) in bat if tt == k), tm[k]))
        fx = collections.Counter((r["tier"], r["fix"]) for r in rec.values() if r["tier"] in ("T1", "T2"))
        o.write("\nT1 / T2 by fix kind (`access` = access / virtual / const letters only; `standin_alias` = a stand-in name; "
                "`signature` = same name and scope, other types; `other`): " +
                ", ".join("%s %s %d" % (a_, b_, n) for (a_, b_), n in sorted(fx.items())) + ".\n")
        o.write("\nLanded rows whose own Leaf could not be read as a full spelling: " + ", ".join("%s %d" % kv for kv in sorted(unmeasured.items())) +
                " (`none` = no Leaf line; `prefix` / `plain` matched one caller spelling and count as agreeing; `*_unmatched` = self spelling unknown).\n")
        o.write("\nQueue lines in NAME_FIXES shape (`QUEUE_STATUS.tsv`): " + ", ".join("%s %d" % kv for kv in sorted(qs.items())) +
                ". Free-text lines are `untracked`; new coordinator lines go one address per line in that shape.\n")
        if hist:
            o.write("\nTrend (`HISTORY.tsv`, table-only split targets at each day's first commit; comparable over time, lower than "
                    "the split counts above because self leaves are not included):\n\n| Date | Split targets | One PDB name |\n|---|---:|---:|\n")
            for r in hist[-14:]: o.write("| %s | %d | %d |\n" % (r[0], r[3], r[4]))
        o.write("\nFiles: `OPEN.tsv` (every open address, sorted by tier, status, class), `BATCHES.tsv` (tier x class, rows touched, "
                "shared header if any), `QUEUE_STATUS.tsv`, `HISTORY.tsv`.\n")
    print("total", total, dict(c), "tiers", dict(tiers), "queue", dict(qs))


if __name__ == "__main__":
    main()
