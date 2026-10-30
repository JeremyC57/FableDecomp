#!/usr/bin/env python3
"""Synchronize public README progress rows with canonical rebuild reports."""
from __future__ import annotations

import argparse
import csv
import json
import re
from datetime import datetime
from pathlib import Path


def percentage(value: int, total: int, digits: int = 3) -> str:
    return f"{(100.0 * value / total) if total else 0.0:.{digits}f}%"


def replace_row(text: str, label: str, value: str) -> str:
    pattern = re.compile(
        rf"(?m)^(\|\s*[^|]+\|\s*{re.escape(label)}\s*\|\s*)[^|]+(\|)$"
    )
    updated, count = pattern.subn(rf"\g<1>{value} \g<2>", text)
    if count != 1:
        raise RuntimeError(f"README row not found exactly once: {label!r}")
    return updated


def replace_snapshot_row(text: str, label: str, value: str) -> str:
    pattern = re.compile(
        rf"(?m)^(\|\s*{re.escape(label)}\s*\|\s*)[^|]+(\|)$"
    )
    updated, count = pattern.subn(rf"\g<1>{value} \g<2>", text)
    if count != 1:
        raise RuntimeError(
            f"README snapshot row not found exactly once: {label!r}"
        )
    return updated


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8-sig", newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


def replace_landed_sentence(text: str, count_pct: str, total: int, bytes_pct: str) -> str:
    """Keep the prose landed-coverage sentence in step with the dashboard table."""
    updated, count = re.subn(
        r"(Landed byte-matching C\+\+ covers \*\*)\d+(?:\.\d+)?%(\*\* of the )\d{1,3}(?:,\d{3})*"
        r"(-function catalog by count\nand \*\*)\d+(?:\.\d+)?%(\*\* of retail function bytes)",
        rf"\g<1>{count_pct}\g<2>{total:,}\g<3>{bytes_pct}\g<4>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("landed coverage sentence not found")
    return updated


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument(
        "--check",
        action="store_true",
        help="do not write; exit 1 if README numbers differ from the generated dashboard",
    )
    args = parser.parse_args()

    root = args.root.resolve()
    readme = root / "README.md"
    coverage = json.loads((root / "rebuild" / "COVERAGE.json").read_text(encoding="utf-8-sig"))
    status = json.loads(
        (root / "rebuild" / "manifest" / "status.json").read_text(
            encoding="utf-8-sig"
        )
    )
    parity = json.loads(
        (root / "rebuild" / "compile-gate" / "retail-parity.json").read_text(
            encoding="utf-8-sig"
        )
    )
    naming_triage = json.loads(
        (
            root
            / "ghidra_out"
            / "naming_stragglers"
            / "quality_triage.json"
        ).read_text(encoding="utf-8-sig")
    )
    total = int(status["catalog_functions"])
    # Landing progress comes from the live catalog (tools/landed_metrics.py, merged into COVERAGE.json as
    # landed_*); status.json / retail-parity.json are the old Windows pipeline's snapshot.
    landed = int(coverage["landed_genuine_original"])
    landed_total = int(coverage["landed_original_functions"])
    landed_bytes = int(coverage["landed_matched_bytes_genuine"])
    bytes_total = landed_bytes + int(coverage["landed_unlanded_bytes_estimate"])
    gfmain_calls = read_tsv(root / "rebuild" / "integration" / "gfmain_calls.tsv")
    gfmain_phases = read_tsv(root / "rebuild" / "integration" / "gfmain_phases.tsv")
    matching_grades = {"MATCH", "EXACT", "RELOCATION_MATCH"}
    proven_calls = sum(
        row.get("validated_grade") in matching_grades for row in gfmain_calls
    )
    current_phase = len(gfmain_phases)
    current_phase_calls = [
        row
        for row in gfmain_calls
        if row.get("phase") == str(current_phase)
    ]
    current_phase_proven = sum(
        row.get("validated_grade") in matching_grades
        for row in current_phase_calls
    )
    authored_phases = sum(
        (root / "rebuild" / "integration" / f"gfmain_phase{phase}.cpp").exists()
        for phase in range(1, len(gfmain_phases) + 1)
    )

    snapshot_values = {
        "Catalogued retail functions": f"**{total:,}**",
        "Landed byte-matching C++ (genuine)": (
            f"**{landed:,} · {percentage(landed, landed_total, 2)}**"
        ),
        "Retail function bytes reproduced": (
            f"**{landed_bytes:,} · {percentage(landed_bytes, bytes_total, 2)}**"
        ),
    }
    values = {
        "Functions catalogued": f"**{total:,}**",
        "Mechanically named (no `FUN_*`)": percentage(
            int(coverage["mechanically_named"]), total
        ),
        "Accepted naming quality": percentage(
            int(coverage["accepted_name_quality"]), total
        ),
        "Usable reconstruction/navigation names": percentage(
            int(naming_triage["usable_navigation_names"]), total
        ),
        "Calling convention known": percentage(
            int(status["calling_convention_known"]), total
        ),
        "Complete non-`undefined` prototype": percentage(
            int(status["prototype_complete"]), total
        ),
        "Landed rows (VC7.1 MATCH or RELOCATION_MATCH **and** behaviour-gated)": (
            f"**{int(coverage['landed_catalog_rows']):,}**"
        ),
        "Landed byte-matching genuine C++, original catalog": (
            f"**{landed:,}** ({percentage(landed, landed_total, 2)})"
        ),
        f"Landed byte-matching genuine C++, all {int(coverage['landed_known_functions']):,} known functions": (
            f"{int(coverage['landed_genuine_known']):,} "
            f"({percentage(int(coverage['landed_genuine_known']), int(coverage['landed_known_functions']), 2)})"
        ),
        "Retail function bytes reproduced by genuine C++": (
            f"{landed_bytes:,} ({percentage(landed_bytes, bytes_total, 2)})"
        ),
        "— landed rows using retail inline asm (grade `retail_asm`, counted as genuine)": (
            f"{int(coverage['landed_retail_asm']):,}"
        ),
        "— landed rows that are toolchain/vendor library bytes (grade `toolchain_library`, counted as genuine)": (
            f"{int(coverage.get('landed_toolchain_library', 0)):,}"
        ),
        "— landed VC7.1 compiles of vendor CRT source (`// Library:` line, counted as genuine)": (
            f"{int(coverage.get('landed_library_source', 0)):,}"
        ),
        "— landed hand-written asm bakes (grade `asm_bake`, **not** counted as reconstruction)": (
            f"{int(coverage['landed_asm_bake']):,}"
        ),
        "Landed sources typed onto shared PDB-named engine headers": (
            f"{int(coverage.get('landed_using_engine_header', 0)):,}"
        ),
        "Throwaway local `struct T {...}` declarations remaining": (
            f"{int(coverage.get('generic_struct_decls', 0)):,}"
        ),
        "Compiled sources still honestly `DIFFER`": f"{int(parity['differing']):,}",
        "Compiled rows lacking a Ghidra function-start oracle": (
            f"{int(parity['oracle_missing']):,}"
        ),
        "Generated candidates / structural checker PASS": (
            f"{int(status['agent_candidates']):,} / {int(status['agent_pass']):,}"
        ),
        "GFMain direct-call sites proven": (
            f"**{proven_calls:,} / {len(gfmain_calls):,}** "
            f"({percentage(proven_calls, len(gfmain_calls), 2)})"
        ),
        "Callable authored GFMain phases": (
            f"**{authored_phases} / {len(gfmain_phases)}** "
            f"({percentage(authored_phases, len(gfmain_phases), 2)})"
        ),
        f"Current Phase {current_phase} direct calls proven": (
            f"**{current_phase_proven} / {len(current_phase_calls)}** "
            f"({percentage(current_phase_proven, len(current_phase_calls), 2)})"
        ),
    }

    text = readme.read_text(encoding="utf-8")
    # The known-function count is part of one row's label: respell it from COVERAGE.json first, so a manifest that
    # grows (gap-scan or boundary rows, e.g. the 312 library rows of 2026-10-30) does not orphan the row.
    text, count = re.subn(
        r"(Landed byte-matching genuine C\+\+, all )\d{1,3}(?:,\d{3})*( known functions)",
        rf"\g<1>{int(coverage['landed_known_functions']):,}\g<2>",
        text,
    )
    if count != 1:
        raise RuntimeError("known-functions row label not found exactly once")
    for label, value in snapshot_values.items():
        text = replace_snapshot_row(text, label, value)
    for label, value in values.items():
        text = replace_row(text, label, value)

    generated = datetime.fromisoformat(coverage["generated_at"]).date().isoformat()
    text, count = re.subn(
        r"Counts above are from the \d{4}-\d{2}-\d{2} canonical refresh:",
        f"Counts above are from the {generated} canonical refresh:",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("canonical refresh date sentence not found")

    count_pct = percentage(landed, landed_total, 2)
    bytes_pct = percentage(landed_bytes, bytes_total, 2)
    text = replace_landed_sentence(text, count_pct, landed_total, bytes_pct)
    text, count = re.subn(
        r"(The )\d+(?:\.\d+)?%( figure is intentionally the strict,)",
        rf"\g<1>{count_pct}\g<2>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("strict denominator sentence not found")

    if args.check:
        original = readme.read_text(encoding="utf-8")
        if original != text:
            import difflib
            diff = difflib.unified_diff(
                original.splitlines(), text.splitlines(),
                "README.md (committed)", "README.md (generated)", lineterm="", n=0,
            )
            print("\n".join(list(diff)[:40]))
            print("readme_progress=DRIFT: run tools/write_decomp_dashboard.py then "
                  "tools/update_readme_progress.py and commit")
            return 1
        print(f"readme_progress=consistent catalog={total}")
        return 0
    temporary = readme.with_suffix(".md.tmp")
    temporary.write_text(text, encoding="utf-8")
    temporary.replace(readme)
    print(
        f"readme_progress=updated catalog={total} compiled={status['compiled_candidates']} "
        f"agent={status['agent_candidates']}/{status['agent_pass']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
