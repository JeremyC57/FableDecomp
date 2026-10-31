#!/usr/bin/env python3
"""Sync catalog / manifest function names from the `// Leaf: <mangled>` line of every landed source.

The Leaf line names the object symbol verify_and_land.py graded the row's bytes against, so it is the spelling the
reconstruction actually compiles under (naming waves respell it together with the body). The catalog
(rebuild/oracles/auto-re-candidates.tsv, column `name`) and rebuild/manifest/functions.tsv (`module` / `name`) were
written at landing time from the then-current labels and drift behind the respells. This tool demangles every mangled
Leaf with llvm-undname, derives the qualified name (no return type, access, convention or parameters; `class`/`struct`
keywords dropped; a ctor/dtor leaf loses the repeated template arguments) and rewrites the catalog where the name is
semantically different (whitespace / keyword / `_global::` differences are not churned) and sets the manifest's
`module` / `name` of every Leaf-verified row to that spelling. Rows whose Leaf is a bare
leaf (no `@`) or whose source has no Leaf line are reported, not changed. Catalog rows missing from functions.tsv are
appended (agent_source `gapscan`, agent_verdict `accepted`).

    python3 tools/sync_catalog_names.py [--dry-run] [--report PATH.tsv]

The report is a labels file (address, name, evidence) for ghidra_out/ and LabelApply.java.
"""
import argparse, csv, io, re, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"
MANIFEST = ROOT / "rebuild" / "manifest" / "functions.tsv"
PS1 = ROOT / "rebuild" / "build_candidates.ps1"
SRC = ROOT / "rebuild" / "src" / "compiled"
UNDNAME = "llvm-undname"
KEYWORDS = re.compile(r"\b(class|struct|union|enum) ")


def landed_sources():
    text = PS1.read_text(encoding="utf-8", errors="replace")
    for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'\s*\n\s*Module = '[^']*'\s*\n\s*Source = '([^']*)'", text):
        yield m.group(1).lower(), SRC / m.group(2)


def leaf_of(path):
    try:
        head = path.read_text(encoding="utf-8", errors="replace")[:6000]
    except OSError:
        return None
    m = re.search(r"^//\s*Leaf:\s*(\S+)", head, re.M)
    return m.group(1) if m else None


def demangle(symbols):
    out = subprocess.run([UNDNAME, "--no-access-specifier", "--no-calling-convention", "--no-return-type",
                          "--no-member-type"], input="\n".join(symbols) + "\n", capture_output=True, text=True).stdout
    res = {}
    for block in out.split("\n\n"):
        lines = [l for l in block.split("\n") if l.strip()]
        if len(lines) >= 2 and not lines[1].startswith("error"):
            res[lines[0]] = lines[1]
    return res


def split_top(s, sep="::"):
    parts, depth, cur, i = [], 0, "", 0
    while i < len(s):
        ch = s[i]
        if ch == "<": depth += 1
        elif ch == ">": depth -= 1
        if depth == 0 and s.startswith(sep, i):
            parts.append(cur); cur = ""; i += len(sep); continue
        cur += ch; i += 1
    parts.append(cur)
    return parts


def strip_targs(s):
    return s[: s.index("<")] if "<" in s else s


def qualified(dem):
    """Qualified function name from an undname line without return/access/convention."""
    s = dem.strip()
    depth, i = 0, 0
    while i < len(s):
        ch = s[i]
        if ch == "<": depth += 1
        elif ch == ">": depth -= 1
        elif ch == "(" and depth == 0:
            if s[:i].endswith("operator"):          # operator()
                j = s.index(")", i); i = j + 1; continue
            break
        i += 1
    head = s[:i].strip()
    if not head or head.startswith("[thunk]") or "`" in head and "dtor'" not in head and "destructor'" not in head:
        return None
    head = KEYWORDS.sub("", head).replace(" >", ">").replace("dtor'", "destructor'")
    parts = split_top(head)
    if len(parts) >= 2:
        last = parts[-1]; base = last[1:] if last.startswith("~") else last
        if base == parts[-2]:
            parts[-1] = ("~" if last.startswith("~") else "") + strip_targs(base)
    return "::".join(parts)


def norm(x):
    x = KEYWORDS.sub("", x)
    x = re.sub(r"\s+", "", x)
    return x.replace("_global::", "").replace("dtor'", "destructor'")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--report", default=None)
    a = ap.parse_args()

    leaves = {}
    no_leaf = plain = 0
    for addr, path in landed_sources():
        leaf = leaf_of(path)
        if not leaf: no_leaf += 1
        elif "@" not in leaf: plain += 1
        else: leaves[addr] = leaf
    dem = demangle(sorted(set(leaves.values())))

    with open(CATALOG, encoding="utf-8-sig", newline="") as f:
        rd = csv.DictReader(f, delimiter="\t"); cat_rows = list(rd); cat_fields = rd.fieldnames
    cat = {r["address"].lower(): r for r in cat_rows}

    changes, verified = {}, {}
    undemangled = unparsed = same = 0
    for addr, leaf in leaves.items():
        if addr not in cat: continue
        d = dem.get(leaf)
        if not d: undemangled += 1; continue
        q = qualified(d)
        if not q: unparsed += 1; continue
        verified[addr] = q
        if norm(q) == norm(cat[addr]["name"]): same += 1
        else: changes[addr] = (cat[addr]["name"], q, leaf)

    print(f"landed sources: mangled Leaf {len(leaves)}, bare leaf {plain}, no Leaf line {no_leaf}")
    print(f"demangle failed {undemangled}, unparsed {unparsed}, already consistent {same}, to rename {len(changes)}")

    with open(MANIFEST, encoding="utf-8-sig", newline="") as f:
        rd = csv.DictReader(f, delimiter="\t"); man_rows = list(rd); man_fields = rd.fieldnames
    man = {r["address"].lower(): r for r in man_rows}
    missing = [addr for addr in cat if addr not in man]
    print(f"catalog rows missing from functions.tsv: {len(missing)}")

    if a.report:
        with open(a.report, "w", encoding="utf-8", newline="") as f:
            w = csv.writer(f, delimiter="\t", lineterminator="\n")
            w.writerow(["address", "name", "evidence"])
            for addr in sorted(changes):
                old, new, leaf = changes[addr]
                w.writerow([addr, new, f"Leaf symbol of the landed source ({leaf}); catalog had {old}"])
    if a.dry_run:
        for addr in sorted(changes)[:40]:
            print(addr, "|", changes[addr][0], "=>", changes[addr][1])
        return

    for addr, (old, new, leaf) in changes.items():
        cat[addr]["name"] = new
    man_updates = 0
    for addr, q in verified.items():            # every Leaf-verified row: the manifest follows the catalog spelling
        r = man.get(addr)
        if r is None: continue
        parts = split_top(q)
        module, name = ("::".join(parts[:-1]) if len(parts) > 1 else "_global"), parts[-1]
        if (r["module"], r["name"]) != (module, name):
            r["module"], r["name"] = module, name; man_updates += 1
    for addr in missing:
        parts = split_top(cat[addr]["name"])
        row = {k: "" for k in man_fields}
        row.update(address=addr, module="::".join(parts[:-1]) if len(parts) > 1 else "_global", name=parts[-1],
                   calling_convention="unknown", return_type="undefined", parameter_count="0", prototype_complete="0",
                   agent_verdict="accepted", agent_source="gapscan")
        man_rows.append(row)
    man_rows.sort(key=lambda r: int(r["address"], 16))

    with open(CATALOG, "w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=cat_fields, delimiter="\t", lineterminator="\n"); w.writeheader(); w.writerows(cat_rows)
    with open(MANIFEST, "w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=man_fields, delimiter="\t", lineterminator="\n"); w.writeheader(); w.writerows(man_rows)
    print(f"wrote {len(changes)} catalog renames, {man_updates} manifest module/name updates, appended {len(missing)} manifest rows")


if __name__ == "__main__":
    main()
