#!/usr/bin/env python3
"""Complete truncated `// Leaf:` lines of landed sources to the one PDB symbol they abbreviate.

Many landed sources carry a Leaf that is only the start of a mangled symbol (`??1CAIStateGroup_HoverWanderAroundThing@`,
`?RemoveUIActionParam@CManager@NUISystem`): enough for verify_and_land.py, which grades the object function whose symbol
CONTAINS the leaf, but not a symbol llvm-undname can demangle, so tools/sync_catalog_names.py cannot verify the row's
name. This tool completes each such prefix to the single symbol of the PDB name sets
(ghidra_out/fablewin_pdb_names.tsv + ghidra_out/egor_pdb_names.tsv, used as name SETS, never by address) that starts
with it, when exactly one does, and only when the completed symbol demangles to the qualified name the catalog already
carries (the same qualified()/norm() rule as sync_catalog_names.py). Everything else is left alone and reported:

    complete        exactly one PDB symbol, its name equals the catalog name     -> Leaf completed
    refine          exactly one PDB symbol, its name is a more specific spelling of the catalog name (template arguments
                    or outer scopes the catalog left out)                         -> NAME_CONFLICTS; completed only
                    with --allow-refine (sync_catalog_names.py then respells the catalog to the PDB name)
    conflict        exactly one PDB symbol, its name differs from the catalog     -> NAME_CONFLICTS (naming finding)
    ambiguous       several PDB symbols start with the prefix
    no-match        none does (STLport / template instantiations, invented names)
    c-decorated     the Leaf is not `?`-mangled (`@ov_pcm_tell@4`, `_fn@8`): not a prefix at all
    anon-namespace  the only PDB symbol carries a `?A0x<hash>` anonymous namespace (the hash is per TU, so the full
                    symbol never names the reconstruction's object symbol): the prefix is the right leaf
    skip-respell    catalog name is a placeholder family another pass is respelling (--skip-pattern)
    excluded        listed in --exclude (e.g. a recheck that did not hold the grade)

Only the Leaf token is replaced; the rest of the line and the file (encoding, line endings) are kept.

A PDB symbol is retail's spelling, not necessarily the reconstruction's: on 2026-11-03 636 of 1,709 unique completions
(37%) named a symbol the landed source does not compile (other parameter types, CRetailVector-style stand-ins,
Dinkumware iterators in the PDB name sets), and verify_and_land.py then silently grades the largest function. So a
completion is applied only for rows a recheck proved: build the payload from --plan, regrade with
`vland.py --recheck-landed` (the completed symbol must be in the object and the grade MATCH / RELOCATION_MATCH + PASS),
and pass the proven addresses with --only. Applying without --only is refused.
Records: rebuild/candidates/manual/batches/2026-11-03_leaf_completion/.

    python3 tools/complete_leaf_prefixes.py [--dry-run] [--report R.tsv] [--conflicts C.tsv] [--plan P.tsv]
                                            [--only ADDRS.txt] [--exclude ADDRS.txt]
"""
import argparse, bisect, collections, csv, re, subprocess, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import sync_catalog_names as S  # noqa: E402  (landed_sources, leaf_of, qualified, norm, UNDNAME, CATALOG)

ROOT = S.ROOT
PDB_SETS = [ROOT / "ghidra_out" / "fablewin_pdb_names.tsv", ROOT / "ghidra_out" / "egor_pdb_names.tsv"]
SKIP_PATTERNS = ["CVtable_", "ScalarDeletingDtor_", "CCountedPointer_Release_", "CIVCountedPointer_Release_"]
LEAF_RE = re.compile(r"^(//\s*Leaf:\s*)(\S+)", re.M)


def undname_ok(symbols, flags=()):
    """{symbol: demangled line or None}. llvm-undname echoes every input line on stdout followed by its demangling;
    an invalid symbol gets only the echo (its `error: Invalid mangled name` goes to stderr)."""
    res = {}
    if not symbols:
        return res
    out = subprocess.run([S.UNDNAME, *flags], input="\n".join(symbols) + "\n", capture_output=True, text=True).stdout
    for block in out.split("\n\n"):
        lines = [l for l in block.split("\n") if l.strip()]
        if lines:
            res[lines[0]] = lines[1] if len(lines) >= 2 else None
    return res


def pdb_symbols():
    syms = set()
    for p in PDB_SETS:
        with open(p, encoding="utf-8", errors="replace") as f:
            for line in f:
                q = line.rstrip("\r\n").split("\t")
                if len(q) >= 2 and q[1]:
                    syms.add(q[1])
    return sorted(syms)


def starting_with(sorted_syms, prefix):
    i = bisect.bisect_left(sorted_syms, prefix)
    out = []
    while i < len(sorted_syms) and sorted_syms[i].startswith(prefix):
        out.append(sorted_syms[i]); i += 1
    return out


def _targs(comp):
    """`vector<A,B>` -> ("vector", ["A", "B"]); no template arguments -> (comp, None). Operators stay atomic."""
    if comp.startswith("operator") or comp.startswith("~operator") or "<" not in comp or not comp.endswith(">"):
        return comp, None
    i = comp.index("<")
    args, depth, cur = [], 0, ""
    for ch in comp[i + 1:-1]:
        if ch == "<": depth += 1
        elif ch == ">": depth -= 1
        if ch == "," and depth == 0:
            args.append(cur); cur = ""; continue
        cur += ch
    args.append(cur)
    return comp[:i], args


def _comp_refines(c, p):
    if c == p:
        return True
    cb, ca = _targs(c); pb, pa = _targs(p)
    if cb != pb or pa is None:
        return False
    if ca is None:                      # the catalog left the template arguments out
        return True
    return len(ca) <= len(pa) and all(_qual_refines(x, y) for x, y in zip(ca, pa))   # trailing defaults left out


def _qual_refines(c, p):
    cs = [x for x in S.split_top(c) if x != "_global"]
    ps = S.split_top(p)
    if not cs or len(cs) > len(ps):
        return False
    return all(_comp_refines(x, y) for x, y in zip(cs, ps[len(ps) - len(cs):]))   # outer scopes left out


def refines(catalog_name, pdb_name):
    """True when the catalog name is the PDB name with template arguments, default template arguments, `std::` or
    outer scopes (namespaces, enclosing classes) left out: the same function, spelled less specifically."""
    canon = lambda x: S.norm(x).replace("std::", "")
    return _qual_refines(canon(catalog_name), canon(pdb_name))


def read_addrs(path):
    if not path:
        return None
    return {l.split()[0].lower().replace("0x", "").zfill(8) for l in open(path) if l.strip() and not l.startswith("#")}


def classify(skip_patterns=SKIP_PATTERNS, exclude=None, only=None, allow_refine=False):
    """Rows of every landed source whose `?` Leaf undname rejects, plus the C-decorated ones, with their verdict."""
    with open(S.CATALOG, encoding="utf-8-sig", newline="") as f:
        cat = {r["address"].lower(): r["name"] for r in csv.DictReader(f, delimiter="\t")}
    src = {}
    for addr, path in S.landed_sources():
        leaf = S.leaf_of(path)
        if leaf and "@" in leaf:
            src[addr] = (leaf, path)
    dem = undname_ok(sorted({l for l, _ in src.values()}))
    bad = {a: v for a, v in src.items() if dem.get(v[0]) is None}
    pdb = pdb_symbols()
    rows, completions = [], {}
    for addr in sorted(bad):
        leaf, path = bad[addr]
        name = cat.get(addr, "")
        r = dict(address=addr, catalog_name=name, leaf=leaf, source=str(path.relative_to(ROOT)), candidates=0,
                 completed="", pdb_name="", verdict="")
        if not leaf.startswith("?"):
            r["verdict"] = "c-decorated"; rows.append(r); continue
        cands = starting_with(pdb, leaf)
        r["candidates"] = len(cands)
        if len(cands) > 1:
            r["verdict"] = "ambiguous"; r["completed"] = " ".join(cands[:6]); rows.append(r); continue
        if not cands:
            r["verdict"] = "no-match"; rows.append(r); continue
        r["completed"] = cands[0]
        if "?A0x" in cands[0]:
            r["verdict"] = "anon-namespace"; rows.append(r); continue
        completions[addr] = cands[0]
        rows.append(r)
    full = undname_ok(sorted(set(completions.values())), ("--no-access-specifier", "--no-calling-convention",
                                                         "--no-return-type", "--no-member-type"))
    for r in rows:
        if r["verdict"]:
            continue
        d = full.get(r["completed"])
        q = S.qualified(d) if d else None
        r["pdb_name"] = q or (d or "")
        if any(p in r["catalog_name"] for p in skip_patterns):
            r["verdict"] = "skip-respell"
        elif exclude and r["address"] in exclude:
            r["verdict"] = "excluded"
        elif only is not None and r["address"] not in only:
            r["verdict"] = "not-selected"
        elif q and S.norm(q) == S.norm(r["catalog_name"]):
            r["verdict"] = "complete"
        elif q and refines(r["catalog_name"], q):
            r["verdict"] = "complete" if allow_refine else "refine"
            r["refined"] = True
        else:
            r["verdict"] = "conflict"
    return rows


def apply(rows):
    done = 0
    for r in rows:
        if r["verdict"] != "complete":
            continue
        p = ROOT / r["source"]
        raw = p.read_bytes()
        text = raw.decode("utf-8")
        m = LEAF_RE.search(text[:6000])
        if not m or m.group(2) != r["leaf"]:
            print(f"  {r['address']}: Leaf line changed under us, skipped", file=sys.stderr); continue
        text = text[: m.start(2)] + r["completed"] + text[m.end(2):]
        p.write_bytes(text.encode("utf-8"))
        done += 1
    return done


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--report", help="TSV of every row whose Leaf undname rejects, with its verdict")
    ap.add_argument("--conflicts", help="TSV of completions whose PDB name differs from the catalog name")
    ap.add_argument("--plan", help="TSV of the completions (address, source, old leaf, new leaf)")
    ap.add_argument("--only", help="file of addresses: complete only these")
    ap.add_argument("--exclude", help="file of addresses never to complete")
    ap.add_argument("--allow-refine", action="store_true",
                    help="also complete rows whose PDB name only adds template arguments / outer scopes to the catalog name")
    a = ap.parse_args()
    if not a.dry_run and not a.only:
        ap.error("applying needs --only <addresses proven by a recheck> (see the module docstring); use --dry-run --plan first")
    rows = classify(exclude=read_addrs(a.exclude), only=read_addrs(a.only), allow_refine=a.allow_refine)
    c = collections.Counter(r["verdict"] for r in rows)
    print(f"Leaf lines llvm-undname rejects: {len(rows)} rows, {len({r['leaf'] for r in rows})} distinct leaves")
    for k in ("complete", "refine", "conflict", "ambiguous", "no-match", "c-decorated", "anon-namespace", "skip-respell",
              "excluded", "not-selected"):
        if c[k]:
            print(f"  {k:15s} {c[k]:6d} rows, {len({r['leaf'] for r in rows if r['verdict'] == k}):5d} distinct leaves")
    cols = ["address", "verdict", "candidates", "leaf", "completed", "catalog_name", "pdb_name", "source"]
    if a.report:
        with open(a.report, "w", encoding="utf-8", newline="") as f:
            w = csv.DictWriter(f, fieldnames=cols, delimiter="\t", lineterminator="\n", extrasaction="ignore")
            w.writeheader(); w.writerows(rows)
    if a.conflicts:
        with open(a.conflicts, "w", encoding="utf-8", newline="") as f:
            w = csv.writer(f, delimiter="\t", lineterminator="\n")
            w.writerow(["address", "catalog_name", "pdb_demangled_name", "evidence"])
            shared = collections.Counter(r["leaf"] for r in rows)
            for r in rows:
                if r["verdict"] == "conflict" or r.get("refined"):
                    kind = ("refinement: the PDB name adds template arguments / outer scopes to the catalog name"
                            if r.get("refined") else "different name")
                    also = f"; {shared[r['leaf']] - 1} other landed sources carry the same Leaf" if shared[r["leaf"]] > 1 else ""
                    w.writerow([r["address"], r["catalog_name"], r["pdb_name"],
                                f"{kind}; Leaf prefix {r['leaf']} completes uniquely to the PDB symbol {r['completed']}"
                                f" (fablewin/egor name sets){also}"])
    if a.plan:
        with open(a.plan, "w", encoding="utf-8", newline="") as f:
            w = csv.writer(f, delimiter="\t", lineterminator="\n")
            w.writerow(["address", "source", "leaf", "completed", "verdict"])
            for r in rows:
                if r["verdict"] in ("complete", "refine"):
                    w.writerow([r["address"], r["source"], r["leaf"], r["completed"], r["verdict"]])
    if a.dry_run:
        return
    print(f"completed {apply(rows)} Leaf lines")


if __name__ == "__main__":
    main()
