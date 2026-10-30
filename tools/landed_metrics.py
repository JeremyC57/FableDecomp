#!/usr/bin/env python3
"""Live landing metrics -> rebuild/manifest/landed_metrics.json.

The landing tool (tools/decomp_pipeline/verify_and_land.py, also run by the Linux lane's vland.py) appends
each verified win -- grade MATCH or RELOCATION_MATCH, behaviour test PASS -- to
rebuild/oracles/auto-re-candidates.tsv and rebuild/build_candidates.ps1. Those two files are the record of what
has landed. rebuild/manifest/status.json is written only by tools/bootstrap_rebuild_tree.py (the old Windows
pipeline) and has not tracked landings since 2026-09-07, so the dashboard takes its landing counts from here.

Counts:
  catalog_rows            rows in auto-re-candidates.tsv
  known_functions         rows in rebuild/manifest/functions.tsv (catalog + gap-scan + boundary rows)
  original_functions      the original analysis catalog (agent_source not gapscan / boundary)
  landed_known / landed_original   catalog rows inside each denominator
  genuine / retail_asm / toolchain_library / asm_bake  purity class of each landed row's source (crawl/purity.py);
                          retail_asm and toolchain_library (VC7.1 library bytes, docs/pipeline/LIBRARY_SIGNATURES.md)
                          count with the genuine rows
  library_source          genuine rows whose source is a VC7.1 compile of vendor (CRT) source that byte-matches the
                          library object retail linked: they carry `// Library: <lib>(<member>) <symbol>`
  landed_genuine_known / landed_genuine_original
  matched_bytes / matched_bytes_genuine   retail bytes covered by landed rows (catalog row lengths)
  unlanded_bytes_estimate  known functions not landed, each sized as the gap to the next known function start, capped at 64 KB (unlanded_gap_cap; unlanded_gaps_capped counts the caps)
                          (includes alignment padding and any data between functions, so it is an upper bound)
  text_bytes              retail .text VirtualSize (rebuild/manifest/text_section.json)
The catalog does not record MATCH vs RELOCATION_MATCH per row, so these counts are the two grades together.
"""
from __future__ import annotations

import csv
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "decomp_pipeline"))
from crawl import purity  # noqa: E402

REBUILD = ROOT / "rebuild"


def addr(s: str) -> str:
    return s.strip().lower().replace("0x", "").zfill(8)


def main() -> int:
    catalog = {}
    with open(REBUILD / "oracles" / "auto-re-candidates.tsv", encoding="utf-8-sig") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            try:
                catalog[addr(r["address"])] = int(r["length"])
            except (KeyError, ValueError):
                pass
    sources = {}
    text = (REBUILD / "build_candidates.ps1").read_text(encoding="utf-8", errors="ignore")
    for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
        a = re.search(r"Address = '([0-9a-fA-F]{8})'", m.group(1))
        s = re.search(r"Source = '([^']*)'", m.group(1))
        if a and s:
            sources[addr(a.group(1))] = s.group(1)
    known, original = set(), set()
    starts = []
    with open(REBUILD / "manifest" / "functions.tsv", encoding="utf-8-sig") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            a = addr(r["address"])
            known.add(a)
            starts.append(int(a, 16))
            if r.get("agent_source", "") not in ("gapscan", "boundary"):
                original.add(a)
    kinds = {}
    library_source = set()
    missing = 0
    for a in catalog:
        rel = sources.get(a)
        p = REBUILD / "src" / "compiled" / rel if rel else None
        if p is None or not p.exists():
            alt = list((REBUILD / "src" / "compiled").glob(f"{a[:2]}/{a[2:4]}/*_{a}.cpp")) if rel is None else []
            p = alt[0] if alt else None
        if p is None or not p.exists():
            missing += 1
            kinds[a] = "missing_source"
            continue
        text_a = p.read_text(encoding="utf-8", errors="ignore")
        kinds[a] = purity.classify(text_a)
        if kinds[a] == "genuine" and re.search(r"^// Library: \S+\(.+\) \S+", text_a, re.M):
            library_source.add(a)
    genuine = {a for a, k in kinds.items() if purity.is_genuine_kind(k)}
    out = {
        "catalog_rows": len(catalog),
        "known_functions": len(known),
        "original_functions": len(original),
        "landed_known": len(catalog.keys() & known),
        "landed_original": len(catalog.keys() & original),
        "landed_genuine_known": len(genuine & known),
        "landed_genuine_original": len(genuine & original),
        "genuine": sum(1 for k in kinds.values() if k == "genuine"),
        "retail_asm": sum(1 for k in kinds.values() if k == "retail_asm"),
        "toolchain_library": sum(1 for k in kinds.values() if k == "toolchain_library"),
        "asm_bake": sum(1 for k in kinds.values() if k in ("naked_asm", "emit_bake")),
        "library_source": len(library_source),
        "missing_source": missing,
        "matched_bytes": sum(catalog.values()),
        "matched_bytes_genuine": sum(n for a, n in catalog.items() if a in genuine),
    }
    starts.sort()
    ts = json.loads((REBUILD / "manifest" / "text_section.json").read_text(encoding="utf-8"))
    text_end = int(ts["virtual_address"], 16) + ts["size"]
    # Each unlanded function is sized as the gap to the next known start, capped at 64 KB: an unlanded start
    # followed by a long run of unknown bytes (0x00F3FD50 -> 0x01210A00 before 2026-10-30 was 2.95 MB) is not one
    # function, so an uncapped gap inflates the estimate; real retail functions stay well under the cap.
    gap_cap = 0x10000
    left = 0
    capped = 0
    for i, va in enumerate(starts):
        a = f"{va:08x}"
        if a in catalog:
            continue
        nxt = starts[i + 1] if i + 1 < len(starts) else text_end
        gap = max(0, min(nxt, text_end) - va)
        if gap_cap and gap > gap_cap:
            gap = gap_cap
            capped += 1
        left += gap
    out["unlanded_bytes_estimate"] = left
    out["unlanded_gap_cap"] = gap_cap
    out["unlanded_gaps_capped"] = capped
    out["text_bytes"] = ts["size"]
    dest = REBUILD / "manifest" / "landed_metrics.json"
    dest.write_text(json.dumps(out, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(out, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
