#!/usr/bin/env python3
"""Cross rebuild/oracles/library_signatures.tsv with the landed catalog and the unlanded pool.

    python3 tools/decomp_pipeline/libsig/libsig_cross.py [--oracle rebuild/oracles/library_signatures.tsv] [--out FILE.tsv]

Buckets (from the oracle's `catalog` column, recomputed here against the live catalog):
  landed    a landed row (rebuild/oracles/auto-re-candidates.tsv): vendor bytes the project reconstructed or
            landed as asm. Each is listed with its catalog Source, its purity class (crawl/purity.py) and its
            catalog Grade, so a reviewer can tell a toolchain_library stub from a C++ compile of CRT source
            (still byte-exact, still genuine) from a C++ model of a linker thunk (a stand-in body for bytes no
            compiler emitted).
  unlanded  a manifest function (rebuild/manifest/functions.tsv) not landed: link work, not reconstruction.
  outside   not a manifest function start.
"""
from __future__ import annotations

import argparse
import csv
import re
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
sys.path.insert(0, str(HERE))
from crawl import purity  # noqa: E402
import libsig_scan as LS  # noqa: E402  (ROOT, catalog loader)

ROOT = LS.ROOT


def catalog_entries():
    text = (ROOT / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8", errors="ignore")
    out = {}
    for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
        kv = dict(re.findall(r"(\w+) = '([^']*)'", m.group(1)))
        if "Address" in kv:
            out[kv["Address"].lower()] = kv
    return out


def verdict(row, kind, grade):
    if kind == "toolchain_library":
        return "toolchain_library stub (linked library bytes, proof line)"
    if kind in ("naked_asm", "emit_bake"):
        return "hand-written asm of library bytes (re-grade to toolchain_library)"
    if row["kind"] == "import_thunk":
        return "C++ stand-in compiling to a linker import thunk (no source exists; candidate for toolchain_library)"
    return "VC7.1 compile reproducing the CRT object's bytes (legitimate compile of CRT source; library bytes)"


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--oracle", default=str(ROOT / "rebuild" / "oracles" / "library_signatures.tsv"))
    ap.add_argument("--out", default=None)
    a = ap.parse_args(argv)
    landed, known, parked = LS.load_catalog(ROOT)
    cat = catalog_entries()
    rows = list(csv.DictReader(open(a.oracle, encoding="utf-8"), delimiter="\t"))
    out = []
    for r in rows:
        va = int(r["address"], 16)
        bucket = LS.catalog_bucket(va, landed, known)
        e = cat.get(r["address"].lower(), {})
        src = ROOT / "rebuild" / "src" / "compiled" / e["Source"] if e.get("Source") else None
        kind = purity.classify(src.read_text(encoding="utf-8", errors="ignore")) if src and src.exists() else ""
        out.append(dict(address=r["address"], bucket=bucket, kind=r["kind"], symbol=r["symbol"], lib=r["lib"],
                        member=r["member"], length=r["length"], confidence=r["confidence"],
                        catalog_name=landed.get(va, (known.get(va, ""),))[0], parked="yes" if va in parked else "",
                        source=e.get("Source", ""), purity=kind, grade=e.get("Grade", ""),
                        verdict=verdict(r, kind, e.get("Grade", "")) if bucket == "landed" else ""))
    c = Counter((o["bucket"], o["kind"]) for o in out)
    print(f"library signature hits: {len(out)}")
    for b in ("landed", "unlanded", "outside"):
        print(f"  {b:9} {sum(v for (bb, _), v in c.items() if bb == b):4}  "
              + ", ".join(f"{k} {v}" for (bb, k), v in sorted(c.items()) if bb == b))
    print("landed rows:")
    for o in out:
        if o["bucket"] == "landed":
            print(f"  {o['address']} {o['kind']:12} {o['symbol']:40} {o['purity'] or '-':17} {o['grade'] or '-':17} "
                  f"{o['source']} :: {o['verdict']}")
    if a.out:
        with open(a.out, "w", encoding="utf-8", newline="") as f:
            w = csv.DictWriter(f, fieldnames=list(out[0]), delimiter="\t", lineterminator="\n")
            w.writeheader()
            w.writerows(out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
