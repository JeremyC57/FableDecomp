#!/usr/bin/env python3
"""Teach the manifest the function starts recovered by the boundary job.

The boundary job (`rebuild/manifest/boundary-starts.tsv`) re-cut catalog rows that held several
functions, a truncated body or a shifted start. Its recovered starts are real functions (prologue,
callers or RTTI/PDB evidence per row), but `verify_and_land` rejects any candidate without an
authoritative manifest start, so they land as OUTSIDE_MANIFEST until they are listed here.

Like `manifest_add_gaps.py`, this appends them as first-class rows, marked
`agent_verdict=discovered`, `agent_source=boundary`, so they stay distinguishable from
Ghidra-derived starts. Signature fields are left conservative. Addresses already in the
manifest are skipped; the file stays sorted by address.

Usage: python manifest_add_boundaries.py [--write]
"""
import csv
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
MANIFEST = ROOT / "rebuild/manifest/functions.tsv"
STARTS = ROOT / "rebuild/manifest/boundary-starts.tsv"
WRITE = "--write" in sys.argv

lines = MANIFEST.read_text(encoding="utf-8").splitlines()
header, rows = lines[0], lines[1:]
cols = header.split("\t")
have = {r.split("\t", 1)[0].lower() for r in rows}

new = []
with STARTS.open(encoding="utf-8") as f:
    for s in csv.DictReader(f, delimiter="\t"):
        a = s["address"].lower()
        if a in have:
            continue
        rec = dict.fromkeys(cols, "")
        rec.update(address=a, name=s["name"], module=s["module"], calling_convention="unknown",
                   return_type="undefined", parameter_count="0", prototype_complete="0",
                   agent_verdict="discovered", agent_source="boundary")
        new.append("\t".join(rec[c] for c in cols))
        have.add(a)

print(f"{len(new)} boundary starts to add ({len(rows)} manifest rows)")
if WRITE and new:
    allrows = sorted(rows + new, key=lambda r: int(r.split("\t", 1)[0], 16))
    MANIFEST.write_text("\n".join([header] + allrows) + "\n", encoding="utf-8")
    print("written")
elif not WRITE:
    print("(dry run; pass --write)")
