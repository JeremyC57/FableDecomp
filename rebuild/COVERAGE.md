# Full-decompilation coverage dashboard

Generated: `2026-10-31T08:59:53+00:00`

## Landed reconstruction (live)

From `rebuild/oracles/auto-re-candidates.tsv` and `rebuild/build_candidates.ps1`, which the landing tool writes for every row that grades MATCH or RELOCATION_MATCH against retail and passes its behaviour test (`tools/landed_metrics.py`). The catalog does not split the two grades.

| Measure | Landed | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Original catalog functions landed, genuine C++ | 45,258 | 49,568 | 91.305% | Byte-matching C++ (no hand-written asm bake; `retail_asm` and `toolchain_library` rows included) for the original analysis catalog |
| All known functions landed, genuine C++ | 52,613 | 57,579 | 91.375% | Adds the gap-scan and boundary functions found later |
| Retail function bytes reproduced, genuine C++ | 5,221,950 | 10,257,468 | 50.909% | Landed row lengths over landed + unlanded (unlanded sized to the next function start, capped at 64 KB) |
| Retail `.text` bytes reproduced, genuine C++ | 5,221,950 | 14,858,408 | 35.145% | Against the whole `.text` section (includes padding and inline data) |
| Landed rows using retail inline asm (grade `retail_asm`) | 166 | 52,658 | — | Counted as genuine: retail itself ran the asm (owner rule 2026-10-24) |
| Landed rows that are toolchain/vendor library bytes (grade `toolchain_library`) | 441 | 52,658 | — | Counted as genuine: the bytes are a VC7.1 library member's (msvcrt.lib CRT object or linker import thunk) and the build links that library (owner rule 2026-10-30; `docs/pipeline/LIBRARY_SIGNATURES.md`) |
| Landed rows that are VC7.1 compiles of vendor CRT source (`// Library:` line) | 8 | 52,658 | — | Genuine compiles whose bytes equal the library object retail linked (crtexew, atonexit, initsect, fp8, ehvecdtr); vendor code, not Fable logic |
| Landed rows that are hand-written asm bakes | 13 | 52,658 | — | Byte-exact oracles, **not** reconstruction; excluded above (`toolchain_library` rows are not asm bakes) |

## Analysis and pipeline snapshot (`rebuild/manifest/status.json`, 2026-10-20)

Written by `tools/bootstrap_rebuild_tree.py` (the Windows pipeline). The Linux lane does not refresh it, so the compile, parity and match rows below stop at that date; use the live table above for landing progress.

| Stage | Complete | Denominator | Coverage | Meaning |
|---|---:|---:|---:|---|
| Clean Ghidra pseudo-C (measured sweep) | 49,081 | 49,082 | 99.998% | Decompiler completed; not necessarily compilable C++ |
| Mechanically named | 49,568 | 49,568 | 100.000% | No default `FUN_*` labels |
| Accepted naming quality | 49,177 | 49,568 | 99.211% | Excludes current low-confidence/address-suffixed review tail |
| Usable reconstruction/navigation names | 49,525 | 49,568 | 99.913% | Includes explicitly classified compiler/template family labels; does not claim original symbols |
| Calling convention known | 38,537 | 49,568 | 77.746% | Required for ABI-correct declarations |
| Complete non-undefined prototype | 34,288 | 49,568 | 69.174% | Known CC, return, and parameter types |
| Auto-RE candidate source | 1,066 | 49,568 | 2.151% | Generated/reviewed candidate; not compile-verified |
| Auto-RE checker PASS | 1,053 | 49,568 | 2.124% | Structural checker grade only |
| Auto-RE source integrity | 1,066 | 1,066 | 100.000% | Balanced, non-empty source snapshots |
| Host C++20 syntax pass | 575 | 1,066 | 53.940% | Strict 32-bit translation unit with real declarations |
| Candidate signature audit clean | 542 | 1,066 | 50.844% | Definition agrees with manifest owner/name/shape and has unique linkage |
| VC7.1 compiled candidate | 18,604 | 49,568 | 37.532% | Compiles to an object; behavioral parity not yet claimed |
| Behavior-tested candidate | 18,604 | 49,568 | 37.532% | Local branch/layout oracle passes; retail parity not yet claimed |
| Candidate retail `.text` match | 8,066 | 49,568 | 16.273% | Generated-candidate track exact matches awaiting promotion |
| Candidate relocation-masked `.text` match | 10,495 | 49,568 | 21.173% | Exact instruction bytes/layout after masking expected COFF linker fields |
| Verified functional or matching C++ | 18,579 | 49,568 | 37.482% | Compiled under VC7.1 and checked against retail |
| Byte-identical C++ | 8,082 | 49,568 | 16.305% | Exact `.text` match (includes asm bakes below) |
| Byte-identical **genuine** C++ | 7,790 | 49,568 | 15.716% | Exact match from real compiler input; no `__asm`, `naked`, `_emit` |
| Hand-written asm bakes (grade `asm_bake`) | 292 | 600 | — | Byte-exact oracles, **not** reconstruction; de-bake worklist |
| Landed sources typed onto shared engine headers | 651 | 14,431 | 4.511% | `#include "engine/<Class>.h"` with PDB member names (957 headers) |
| Generic local struct declarations remaining | 4,417 | — | — | `struct T/Sub/Owner...` throwaways in 3,278 files; retype worklist |
| Matched retail bytes (genuine C++) | 385,690 | — | — | Bytes of retail `.text` reproduced by genuine landings |
| Lifted modules | 3 | 3,433 | 0.087% | Module-level reconstruction proof |

## Interpretation

The reverse-engineering database is near complete. Landed byte-matching C++ covers 91.305% of the original catalog by function count but 50.909% by function bytes: the functions left are the large ones. Auto-RE output is deliberately not counted as reconstructed source until it compiles.

The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,568 functions. The dashboard does not silently extrapolate the missing 486 measurements.

## Current gates

- 15,280 functions still lack a complete non-undefined prototype.
- 11,031 functions still have an unknown calling convention.
- 40 names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.
- 71 of 1,066 generated auto-RE intake rows currently pass the standalone VC7.1 compatibility gate; the independently authored compiled catalog is not subtracted from this intake ledger.
- 524 original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.
- 43 behavior-tested candidates still differ structurally from the retail parity oracle.
- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.
