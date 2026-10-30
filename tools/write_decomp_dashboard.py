#!/usr/bin/env python3
"""Render the multi-stage full-decompilation dashboard from rebuild status."""
from __future__ import annotations

import argparse
import json
import sys
from datetime import datetime, timezone
from pathlib import Path


def pct(value: int, total: int) -> str:
    return f"{(100.0 * value / total):.3f}%" if total else "n/a"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    root = args.root.resolve()
    rebuild = root / "rebuild"
    status_path = rebuild / "manifest" / "status.json"
    status = json.loads(status_path.read_text(encoding="utf-8"))
    gate_summary_path = rebuild / "compile-gate" / "summary.json"
    gate = json.loads(gate_summary_path.read_text(encoding="utf-8")) if gate_summary_path.exists() else {}
    signature_path = rebuild / "compile-gate" / "signature-audit.json"
    signature = json.loads(signature_path.read_text(encoding="utf-8")) if signature_path.exists() else {}
    naming_triage_path = root / "ghidra_out" / "naming_stragglers" / "quality_triage.json"
    naming_triage = json.loads(naming_triage_path.read_text(encoding="utf-8")) if naming_triage_path.exists() else {}
    header_metrics_path = rebuild / "manifest" / "header_metrics.json"
    hm = json.loads(header_metrics_path.read_text(encoding="utf-8")) if header_metrics_path.exists() else {}
    # Live landing counts: auto-re-candidates.tsv + build_candidates.ps1 are what the landing tool writes.
    # status.json (above) is the old Windows pipeline's snapshot and does not see Linux-lane landings.
    sys.path.insert(0, str(root / "tools"))
    import landed_metrics
    landed_metrics.main()
    lm = json.loads((rebuild / "manifest" / "landed_metrics.json").read_text(encoding="utf-8"))
    import subprocess
    status_date = subprocess.run(["git", "-C", str(root), "log", "-1", "--format=%cs", "--", str(status_path)],
                                 capture_output=True, text=True).stdout.strip() or "undated"
    total = status["catalog_functions"]
    accepted_names = total - status["naming_quality_review"]
    verified = (
        status["lift_matching"]
        + status["lift_functional"]
        + status.get("candidate_retail_matches", 0)
        + status.get("candidate_relocation_matches", 0)
    )
    byte_identical = status["lift_matching"] + status.get("candidate_retail_matches", 0)
    # Hand-written __asm / _emit bodies are byte-exact oracles, not reconstruction. Report them
    # on their own line and subtract them from the genuine C++ figure (grade `asm_bake`).
    asm_bake_matching = hm.get("asm_bake_matching", 0)
    byte_identical_genuine = byte_identical - asm_bake_matching
    structurally_different = (
        status.get("behavior_tested_candidates", 0)
        - status.get("candidate_retail_matches", 0)
        - status.get("candidate_relocation_matches", 0)
    )
    timestamp = datetime.now().astimezone().isoformat(timespec="seconds")
    dashboard = {
        "generated_at": timestamp,
        **status,
        "mechanically_named": total,
        "accepted_name_quality": accepted_names,
        "usable_navigation_names": naming_triage.get(
            "usable_navigation_names", accepted_names
        ),
        "verified_lifted": verified,
        "byte_identical": byte_identical,
        "byte_identical_genuine": byte_identical_genuine,
        "asm_bake_matching": asm_bake_matching,
        "asm_bake_files": hm.get("naked_asm", 0) + hm.get("emit_bake", 0),
        "landed_using_engine_header": hm.get("includes_engine_header", 0),
        "generic_struct_decls": hm.get("generic_struct_decls", 0),
        "matched_bytes_genuine": hm.get("matched_bytes_genuine", 0),
        "engine_headers_generated": hm.get("engine_headers_generated", 0),
        "candidate_integrity_pass": gate.get("integrity_pass", 0),
        "candidate_host_syntax_pass": gate.get("host_cpp20_syntax_pass", 0),
        **{f"landed_{k}" if not k.startswith("landed_") else k: v for k, v in lm.items()},
    }
    json_temp = rebuild / "COVERAGE.json.tmp"
    json_temp.write_text(json.dumps(dashboard, indent=2) + "\n", encoding="utf-8")
    json_temp.replace(rebuild / "COVERAGE.json")

    lines = [
        "# Full-decompilation coverage dashboard",
        "",
        f"Generated: `{timestamp}`",
        "",
        "## Landed reconstruction (live)",
        "",
        "From `rebuild/oracles/auto-re-candidates.tsv` and `rebuild/build_candidates.ps1`, which the landing tool writes "
        "for every row that grades MATCH or RELOCATION_MATCH against retail and passes its behaviour test "
        "(`tools/landed_metrics.py`). The catalog does not split the two grades.",
        "",
        "| Measure | Landed | Denominator | Coverage | Meaning |",
        "|---|---:|---:|---:|---|",
        f"| Original catalog functions landed, genuine C++ | {lm['landed_genuine_original']:,} | {lm['original_functions']:,} | {pct(lm['landed_genuine_original'], lm['original_functions'])} | Byte-matching C++ (no hand-written asm bake; `retail_asm` and `toolchain_library` rows included) for the original analysis catalog |",
        f"| All known functions landed, genuine C++ | {lm['landed_genuine_known']:,} | {lm['known_functions']:,} | {pct(lm['landed_genuine_known'], lm['known_functions'])} | Adds the gap-scan and boundary functions found later |",
        f"| Retail function bytes reproduced, genuine C++ | {lm['matched_bytes_genuine']:,} | {lm['matched_bytes_genuine'] + lm['unlanded_bytes_estimate']:,} | {pct(lm['matched_bytes_genuine'], lm['matched_bytes_genuine'] + lm['unlanded_bytes_estimate'])} | Landed row lengths over landed + unlanded (unlanded sized to the next function start, capped at 64 KB) |",
        f"| Retail `.text` bytes reproduced, genuine C++ | {lm['matched_bytes_genuine']:,} | {lm['text_bytes']:,} | {pct(lm['matched_bytes_genuine'], lm['text_bytes'])} | Against the whole `.text` section (includes padding and inline data) |",
        f"| Landed rows using retail inline asm (grade `retail_asm`) | {lm['retail_asm']:,} | {lm['catalog_rows']:,} | — | Counted as genuine: retail itself ran the asm (owner rule 2026-10-24) |",
        f"| Landed rows that are toolchain/vendor library bytes (grade `toolchain_library`) | {lm.get('toolchain_library', 0):,} | {lm['catalog_rows']:,} | — | Counted as genuine: the bytes are a VC7.1 library member's (msvcrt.lib CRT object or linker import thunk) and the build links that library (owner rule 2026-10-30; `docs/pipeline/LIBRARY_SIGNATURES.md`) |",
        f"| Landed rows that are VC7.1 compiles of vendor CRT source (`// Library:` line) | {lm.get('library_source', 0):,} | {lm['catalog_rows']:,} | — | Genuine compiles whose bytes equal the library object retail linked (crtexew, atonexit, initsect, fp8, ehvecdtr); vendor code, not Fable logic |",
        f"| Landed rows that are hand-written asm bakes | {lm['asm_bake']:,} | {lm['catalog_rows']:,} | — | Byte-exact oracles, **not** reconstruction; excluded above (`toolchain_library` rows are not asm bakes) |",
        "",
        f"## Analysis and pipeline snapshot (`rebuild/manifest/status.json`, {status_date})",
        "",
        "Written by `tools/bootstrap_rebuild_tree.py` (the Windows pipeline). The Linux lane does not refresh it, so the "
        "compile, parity and match rows below stop at that date; use the live table above for landing progress.",
        "",
        "| Stage | Complete | Denominator | Coverage | Meaning |",
        "|---|---:|---:|---:|---|",
        f"| Clean Ghidra pseudo-C (measured sweep) | {status['clean_decompile_pass']:,} | {status['clean_decompile_measured']:,} | {pct(status['clean_decompile_pass'], status['clean_decompile_measured'])} | Decompiler completed; not necessarily compilable C++ |",
        f"| Mechanically named | {total:,} | {total:,} | 100.000% | No default `FUN_*` labels |",
        f"| Accepted naming quality | {accepted_names:,} | {total:,} | {pct(accepted_names, total)} | Excludes current low-confidence/address-suffixed review tail |",
        f"| Usable reconstruction/navigation names | {naming_triage.get('usable_navigation_names', accepted_names):,} | {total:,} | {pct(naming_triage.get('usable_navigation_names', accepted_names), total)} | Includes explicitly classified compiler/template family labels; does not claim original symbols |",
        f"| Calling convention known | {status['calling_convention_known']:,} | {total:,} | {pct(status['calling_convention_known'], total)} | Required for ABI-correct declarations |",
        f"| Complete non-undefined prototype | {status['prototype_complete']:,} | {total:,} | {pct(status['prototype_complete'], total)} | Known CC, return, and parameter types |",
        f"| Auto-RE candidate source | {status['agent_candidates']:,} | {total:,} | {pct(status['agent_candidates'], total)} | Generated/reviewed candidate; not compile-verified |",
        f"| Auto-RE checker PASS | {status['agent_pass']:,} | {total:,} | {pct(status['agent_pass'], total)} | Structural checker grade only |",
        f"| Auto-RE source integrity | {gate.get('integrity_pass', 0):,} | {gate.get('candidates', status['agent_candidates']):,} | {pct(gate.get('integrity_pass', 0), gate.get('candidates', status['agent_candidates']))} | Balanced, non-empty source snapshots |",
        f"| Host C++20 syntax pass | {gate.get('host_cpp20_syntax_pass', 0):,} | {gate.get('candidates', status['agent_candidates']):,} | {pct(gate.get('host_cpp20_syntax_pass', 0), gate.get('candidates', status['agent_candidates']))} | Strict 32-bit translation unit with real declarations |",
        f"| Candidate signature audit clean | {signature.get('pass', 0):,} | {signature.get('candidates', status['agent_candidates']):,} | {pct(signature.get('pass', 0), signature.get('candidates', status['agent_candidates']))} | Definition agrees with manifest owner/name/shape and has unique linkage |",
        f"| VC7.1 compiled candidate | {status.get('compiled_candidates', 0):,} | {total:,} | {pct(status.get('compiled_candidates', 0), total)} | Compiles to an object; behavioral parity not yet claimed |",
        f"| Behavior-tested candidate | {status.get('behavior_tested_candidates', 0):,} | {total:,} | {pct(status.get('behavior_tested_candidates', 0), total)} | Local branch/layout oracle passes; retail parity not yet claimed |",
        f"| Candidate retail `.text` match | {status.get('candidate_retail_matches', 0):,} | {total:,} | {pct(status.get('candidate_retail_matches', 0), total)} | Generated-candidate track exact matches awaiting promotion |",
        f"| Candidate relocation-masked `.text` match | {status.get('candidate_relocation_matches', 0):,} | {total:,} | {pct(status.get('candidate_relocation_matches', 0), total)} | Exact instruction bytes/layout after masking expected COFF linker fields |",
        f"| Verified functional or matching C++ | {verified:,} | {total:,} | {pct(verified, total)} | Compiled under VC7.1 and checked against retail |",
        f"| Byte-identical C++ | {byte_identical:,} | {total:,} | {pct(byte_identical, total)} | Exact `.text` match (includes asm bakes below) |",
        f"| Byte-identical **genuine** C++ | {byte_identical_genuine:,} | {total:,} | {pct(byte_identical_genuine, total)} | Exact match from real compiler input; no `__asm`, `naked`, `_emit` |",
        f"| Hand-written asm bakes (grade `asm_bake`) | {asm_bake_matching:,} | {hm.get('naked_asm', 0) + hm.get('emit_bake', 0):,} | — | Byte-exact oracles, **not** reconstruction; de-bake worklist |",
        f"| Landed sources typed onto shared engine headers | {hm.get('includes_engine_header', 0):,} | {hm.get('genuine', 0):,} | {pct(hm.get('includes_engine_header', 0), hm.get('genuine', 0))} | `#include \"engine/<Class>.h\"` with PDB member names ({hm.get('engine_headers_generated', 0):,} headers) |",
        f"| Generic local struct declarations remaining | {hm.get('generic_struct_decls', 0):,} | — | — | `struct T/Sub/Owner...` throwaways in {hm.get('files_with_generic_struct', 0):,} files; retype worklist |",
        f"| Matched retail bytes (genuine C++) | {hm.get('matched_bytes_genuine', 0):,} | — | — | Bytes of retail `.text` reproduced by genuine landings |",
        f"| Lifted modules | {status['lift_modules']:,} | {status['modules']:,} | {pct(status['lift_modules'], status['modules'])} | Module-level reconstruction proof |",
        "",
        "## Interpretation",
        "",
        f"The reverse-engineering database is near complete. Landed byte-matching C++ covers {pct(lm['landed_genuine_original'], lm['original_functions'])} of the original catalog by function count but {pct(lm['matched_bytes_genuine'], lm['matched_bytes_genuine'] + lm['unlanded_bytes_estimate'])} by function bytes: the functions left are the large ones. Auto-RE output is deliberately not counted as reconstructed source until it compiles.",
        "",
        f"The clean-decompile sweep predates later function discovery, so its denominator is {status['clean_decompile_measured']:,} while the current catalog contains {total:,} functions. The dashboard does not silently extrapolate the missing {total - status['clean_decompile_measured']:,} measurements.",
        "",
        "## Current gates",
        "",
        f"- {total - status['prototype_complete']:,} functions still lack a complete non-undefined prototype.",
        f"- {total - status['calling_convention_known']:,} functions still have an unknown calling convention.",
        f"- {naming_triage.get('remaining_semantic_review', status['naming_quality_review']):,} names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.",
        (
            f"- {gate.get('vc71_compatible', 0):,} of "
            f"{gate.get('candidates', status['agent_candidates']):,} generated auto-RE intake rows "
            "currently pass the standalone VC7.1 compatibility gate; the independently authored "
            "compiled catalog is not subtracted from this intake ledger."
        ),
        f"- {signature.get('review', 0):,} original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.",
        f"- {structurally_different:,} behavior-tested candidates still differ structurally from the retail parity oracle.",
    ]
    if status.get("vc71_sdk_probe_pass"):
        lines.append("- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.")
    else:
        lines.append("- Real Win32/DirectX engine translation units still need a passing VC7.1 Platform SDK probe.")
    md_temp = rebuild / "COVERAGE.md.tmp"
    md_temp.write_text("\n".join(lines) + "\n", encoding="utf-8")
    md_temp.replace(rebuild / "COVERAGE.md")
    print(f"wrote {rebuild / 'COVERAGE.md'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
