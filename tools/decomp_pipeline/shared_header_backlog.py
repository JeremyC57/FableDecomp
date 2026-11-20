#!/usr/bin/env python3
"""Shared-header backlog: landed rows that define a local class/struct with the name of a shared header.

Uses verify_and_land.shared_header_redefinitions (the same test as the lander's SHARED_HEADER_AVAILABLE warning) over every landed
source in rebuild/build_candidates.ps1, with rebuild/include as the header root. The match is by name only, so the counts are an upper
bound: a local struct can share a header's name without modelling the same class.

Usage (repo root): python3 tools/decomp_pipeline/shared_header_backlog.py [out_dir]
Writes <out_dir>/local_models.tsv (address -> classes) and <out_dir>/by_class.tsv (class -> rows), prints a summary.
With --count, prints only the total row count (for a before/after ratchet check in a landing).
"""
import collections
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "decomp_pipeline"))
import verify_and_land as v  # noqa: E402


def catalog():
    bc = (ROOT / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8-sig")
    ent = {}
    for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", bc, re.S):
        b = m.group(1)
        a = re.search(r"Address = '([0-9a-fA-F]{8})'", b)
        if a:
            ent[a.group(1).lower()] = dict(re.findall(r"(\w+) = '([^']*)'", b))
    return ent


def scan(addresses=None):
    roots = [ROOT / "rebuild" / "include"]
    rows = {}
    for a, e in catalog().items():
        if addresses is not None and a not in addresses:
            continue
        p = ROOT / "rebuild" / "src" / "compiled" / e.get("Source", "")
        if not e.get("Source") or not p.exists():
            continue
        found = [sn for sn, _ in v.shared_header_redefinitions(p.read_text(errors="replace"), roots)]
        if found:
            rows[a] = found
    return rows


def main():
    args = [x for x in sys.argv[1:] if not x.startswith("--")]
    rows = scan()
    if "--count" in sys.argv:
        print(len(rows))
        return
    by = collections.defaultdict(list)
    for a, cs in rows.items():
        for c in cs:
            by[c].append(a)
    out = Path(args[0]) if args else None
    if out:
        out.mkdir(parents=True, exist_ok=True)
        with open(out / "local_models.tsv", "w") as f:
            f.write("address\tclasses\n")
            for a in sorted(rows):
                f.write(a + "\t" + " ".join(rows[a]) + "\n")
        with open(out / "by_class.tsv", "w") as f:
            f.write("class\trows\taddresses\n")
            for c, aa in sorted(by.items(), key=lambda x: -len(x[1])):
                f.write(f"{c}\t{len(aa)}\t{' '.join(sorted(aa))}\n")
    print(f"rows with a local model of a header class: {len(rows)}; classes: {len(by)}; "
          f"row-class pairs: {sum(len(x) for x in rows.values())}")
    for c, aa in sorted(by.items(), key=lambda x: -len(x[1]))[:20]:
        print(f"{len(aa):6} {c}")


if __name__ == "__main__":
    main()
