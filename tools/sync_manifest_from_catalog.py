#!/usr/bin/env python3
"""Bring rebuild/manifest/functions.tsv back in step with what has actually been found.

Two name authorities drift apart from the manifest:

  * landed rows: the catalog name (rebuild/oracles/auto-re-candidates.tsv, column `name`) is what the row was
    landed under and what naming waves respell (tools/sync_catalog_names.py keeps it on the source's `// Leaf:`
    where that is a full mangled symbol). The manifest's `module` / `name` were written at landing time and
    were never re-synced for rows whose Leaf is bare or a prefix (2026-11-03: 33,028 of 53,498 landed rows
    disagreed, 7,647 of them with the class repeated in both columns).
  * unlanded rows: the labels files ghidra_out/labels_*.tsv (the reproducible Ghidra DB) carry proven names
    the manifest never received before coordinator/apply_labels.py existed (2026-11-01).

This tool rewrites functions.tsv:
  1. every landed row takes its catalog name, split on the last top-level `::` into module / name
     (the same split coordinator/apply_labels.py uses; `_global` when unscoped);
  2. with --labels, every unlanded row takes the newest label for its address (labels files ordered by the
     date in their file name, then by name); labels named `1`/`2`/`3` or empty are ignored, as apply_labels does.
It appends every catalog row that is missing from the manifest (agent_source `gapscan`) and writes a labels
file of the rows it changed (address, name, evidence) so LabelApply.java can bring the Ghidra DB along.

    python3 tools/sync_manifest_from_catalog.py [--labels] [--dry-run] [--report ghidra_out/labels_<x>.tsv]

Run tools/write_decomp_dashboard.py and tools/update_readme_progress.py afterwards (never hand-edit counts).
"""
from __future__ import annotations

import argparse
import csv
import glob
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"
MANIFEST = ROOT / "rebuild" / "manifest" / "functions.tsv"
LABEL_GLOB = str(ROOT / "ghidra_out" / "labels_*.tsv")
KEYWORDS = re.compile(r"\b(class|struct|union|enum) ")


def addr(s: str) -> str:
    return s.strip().lower().replace("0x", "").zfill(8)


def split_scope(name: str) -> tuple[str, str]:
    """`Scope::Member` -> (Scope, Member) on the last `::` outside template brackets; `_global` when unscoped."""
    depth, cut, i = 0, -1, 0
    while i < len(name):
        c = name[i]
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == ":" and depth == 0 and name[i : i + 2] == "::":
            cut = i
            i += 2
            continue
        i += 1
    if cut < 0:
        return "_global", name
    return name[:cut], name[cut + 2 :]


def norm(s: str) -> str:
    return re.sub(r"\s+", "", KEYWORDS.sub("", s)).replace("_global::", "")


def qualified(module: str, name: str) -> str:
    return name if module in ("", "_global") else f"{module}::{name}"


def read_tsv(p: Path):
    txt = p.read_text(encoding="utf-8-sig")
    nl = "\r\n" if "\r\n" in txt else "\n"
    rows = [l.split("\t") for l in txt.split(nl)]
    return rows, nl


def write_tsv(p: Path, rows, nl: str) -> None:
    p.write_text(nl.join("\t".join(r) for r in rows), encoding="utf-8")


def file_date(p: str) -> str:
    m = re.search(r"(20\d\d-\d\d-\d\d)", p)
    return m.group(1) if m else "0000-00-00"


def latest_labels() -> dict[str, tuple[str, str]]:
    out: dict[str, tuple[str, str]] = {}
    for p in sorted(glob.glob(LABEL_GLOB), key=lambda q: (file_date(q), q)):
        with open(p, encoding="utf-8-sig", newline="") as f:
            rd = csv.DictReader(f, delimiter="\t")
            if not rd.fieldnames or "address" not in rd.fieldnames or "name" not in rd.fieldnames:
                continue
            for r in rd:
                a, n = (r.get("address") or "").strip(), (r.get("name") or "").strip()
                if not re.fullmatch(r"(0x)?[0-9a-fA-F]{1,8}", a) or not n or n in ("1", "2", "3"):
                    continue
                out[addr(a)] = (n, Path(p).name)
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--labels", action="store_true", help="also apply the newest label to every unlanded row")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--report", type=Path, help="labels file (address, name, evidence) of the rows changed")
    args = ap.parse_args()

    catalog: dict[str, str] = {}
    with open(CATALOG, encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            catalog[addr(r["address"])] = r["name"].strip()

    rows, nl = read_tsv(MANIFEST)
    hdr = rows[0]
    iA, iM, iN, iS = hdr.index("address"), hdr.index("module"), hdr.index("name"), hdr.index("agent_source")
    changed: list[tuple[str, str, str, str]] = []  # address, old, new, evidence
    seen = set()
    doubled = 0
    for r in rows[1:]:
        if len(r) <= max(iM, iN, iS):
            continue
        a = addr(r[iA])
        seen.add(a)
        if a in catalog:
            cn = catalog[a]
            mod, nam = split_scope(cn)
            if (r[iM], r[iN]) == (mod, nam):
                continue
            old = qualified(r[iM], r[iN])
            if r[iN] == cn and r[iM] not in ("", "_global"):
                doubled += 1
            r[iM], r[iN] = mod, nam
            changed.append((a, old, cn, "catalog name (landed row); manifest module/name resynced"))
    labels = latest_labels() if args.labels else {}
    label_changed = 0
    if labels:
        for r in rows[1:]:
            if len(r) <= max(iM, iN, iS):
                continue
            a = addr(r[iA])
            if a in catalog or a not in labels:
                continue
            n, src = labels[a]
            mod, nam = split_scope(n)
            if (r[iM], r[iN]) == (mod, nam) or norm(n) == norm(qualified(r[iM], r[iN])):
                continue
            old = qualified(r[iM], r[iN])
            r[iM], r[iN] = mod, nam
            changed.append((a, old, n, f"newest label ({src}); unlanded row"))
            label_changed += 1
    appended = 0
    for a, cn in sorted(catalog.items()):
        if a in seen:
            continue
        mod, nam = split_scope(cn)
        new = [""] * len(hdr)
        new[iA], new[iM], new[iN], new[iS] = a, mod, nam, "gapscan"
        if "agent_verdict" in hdr:
            new[hdr.index("agent_verdict")] = "accepted"
        if "calling_convention" in hdr:
            new[hdr.index("calling_convention")] = "unknown"
        if "return_type" in hdr:
            new[hdr.index("return_type")] = "undefined"
        if "parameter_count" in hdr:
            new[hdr.index("parameter_count")] = "0"
        if "prototype_complete" in hdr:
            new[hdr.index("prototype_complete")] = "0"
        if rows[-1] == [""]:
            rows.insert(len(rows) - 1, new)
        else:
            rows.append(new)
        changed.append((a, "", cn, "catalog row missing from the manifest; appended"))
        appended += 1
    if not args.dry_run:
        write_tsv(MANIFEST, rows, nl)
    if args.report:
        with open(args.report, "w", encoding="utf-8", newline="") as f:
            f.write("address\tname\tevidence\n")
            for a, old, new, ev in changed:
                f.write(f"{a}\t{new}\t{ev}{'; was ' + old if old else ''}\n")
    landed_changed = len(changed) - label_changed - appended
    print(
        f"landed rows resynced {landed_changed} (module repeated in name: {doubled}); "
        f"unlanded rows relabelled {label_changed}; catalog rows appended {appended}"
        f"{'  (dry run)' if args.dry_run else ''}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
