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

STANDIN = re.compile(r"(?:_[0-9a-fA-F]{8}(?![0-9a-fA-F])|@T@@|^\?\w+@T@@|DAT_|FUN_|_vtbl|Unproven|@Native@@|ForwardTarget_|g_obj_|"
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


def is_std(sym):
    return (not sym.startswith("?")) or "@std@@" in sym or "@_STL@@" in sym or sym.startswith("??_") or "?$" in sym and "@std@@" in sym


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
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    pdb = pdb_names()
    ent = landed_rows()
    cat = {r[0].lower(): r[1] for r in read_tsv(P("rebuild", "oracles", "auto-re-candidates.tsv"))[1:] if len(r) > 1}
    by = table_spellings(open(P("rebuild", "oracles", "reloc_bindings.tsv"), encoding="utf-8", errors="replace").read())
    leaf = {ad: leaf_of(e["Source"]) for ad, e in ent.items()}

    status, rec = {}, {}
    unmeasured = collections.Counter()
    for t in sorted(set(by) | set(ent)):
        sp = {}
        for s, c in by.get(t, {}).items(): sp.setdefault(norm(s), set()).update(c)
        self_leaf, self_how = resolve_leaf(leaf.get(t, ""), sp)
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
            st, tier = "catalog_standin", "T0"
        status[t] = st
        good = pd[0] if len(pd) == 1 else ""
        rec[t] = dict(target=t, status=st, tier=tier, landed="y" if t in ent else "n", catalog=cn, self_leaf=self_leaf,
                      pdb_spelling=good, klass=klass(good or self_leaf or (names[0] if names else "")),
                      spellings=" || ".join("%s (%s)" % (s[:160], ",".join(sorted(sp[s])[:6]) + (",..." if len(sp[s]) > 6 else "")) for s in names)[:700],
                      n_spellings=len(names), source=ent.get(t, {}).get("Source", ""),
                      fix=("catalog" if tier == "T0" else fix_kind(good, [x for x in names if x != good]) if good else
                           ("standin" if st == "standin" else "")))

    cols = ["target", "status", "tier", "fix", "klass", "n_spellings", "pdb_spelling", "self_leaf", "landed", "catalog", "source", "spellings"]
    openst = ("standin", "split_fixable", "split_unknown", "split_pdb_multi", "catalog_standin")
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
                    catalog_standin="code agrees on the PDB name, catalog / file name is a stand-in (T0)",
                    standin="one spelling with a stand-in marker: open", split_fixable="2+ spellings, one PDB name: open, target known",
                    split_pdb_multi="2+ PDB names at one address: ruling", split_unknown="2+ spellings, no PDB name: open, research")
        mean["unmeasured"] = "landed row, no caller binding and no usable mangled Leaf line: not measured"
        for k in ["agree_pdb", "agree_std", "agree_unverified", "catalog_standin", "standin", "split_fixable", "split_pdb_multi", "split_unknown", "unmeasured"]:
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
