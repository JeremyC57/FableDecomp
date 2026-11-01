# START HERE — Claude verification and local integration

This is the current complete **Pass10,100-row** hand-back. Read this file first. It contains all selected work, original input baselines and prior histories, every1730 new compiler-grade record and candidate source, original-name evidence, best retained unmatched candidates, and the current-source test/review receipts. No online repository writes or commits were made.

## Final result

- **57/100 functions independently verified:57%.** Each has a fresh VC7.1 RTM13.10.3077 byte/relocation review, exact-name evidence, and passing GNU64+native C++03 fixtures.
- **31 PARKED** and **12 BLOCKED**. Neither category is accepted for landing.
- **95 selected source files have current passing dual-compiler fixtures.** A fixture PASS alone is not parity or a naming approval.
- **5 rows remain baseline-only** after identity research:006bd900,006e3890,00c0ffe0,00c3d030,00c73660. They have an inherited baseline compiler grade but no new compliant reconstruction. See BASELINE_ONLY_ROWS.tsv. No assigned row was completely unexamined.
-14348 verified bytes across57 accepted functions. Original manifest totals28266 bytes, but some assigned extents are defective; use function percentage as the primary progress metric.

## What to read and what to integrate

1. AUTHORIZED_SCOPE.json and input/fabledecomp_chatgpt_pass10_100/ROWS.tsv define the only100 target addresses. No previous-pass or sibling row is authorized by this handoff.
2. ROWS.tsv/STATUS.tsv give the final disposition, selected attempt, actual comparison extent, flags, hashes, test status and one-line residue. ROW_DETAILS.md includes every row's full pass10 attempt ledger. Original input attempt history remains under input/.../rows/<address>/.
3. For each address in VERIFIED_ROWS.txt, inspect rows/<address>/<address>.cpp, its self-contained test, ROW.json, NAMING_EVIDENCE.md and reviews/<address>/{RESULT.json,MANUAL_REVIEW.json or other independent review notes}. Recompile and retest in your own environment before local integration. Source/test hashes in current receipts identify exactly what passed.
4. Recheck NAME_FIXES.tsv before binding callees/globals. It consolidates181 address reports with no disagreements between the row reports. Accepted bindings have zero symbol/address conflicts in either direction. These reports do not modify your repository. Keep the correct original name even when an old binding disagrees; inspect the cited evidence rather than copying a bad label.
5. PARKED/BLOCKED rows retain the best selected compliant or diagnostic source where available, its fixture and the actual remaining difference. history/<address>/grade_NN.json and attempt_NN/ preserve every failed build, compiler log, symbols, relocation listing and raw retail/built disassembly. **Do not land historical candidates.** The nearest-size attempt can be unsafe, wrongly named, incomplete or based on a recut extent. Selected compliant code can be larger than an inherited invalid near-match.

## Important findings and unresolved limits

**GameCODA recovery:**00c35e30 is CODA_SourceSetPlayCursor,110bytes,6native bindings,1800cases/compiler. It is accepted. Original SDK header+release COFF+debug CodeView identify its public API, real class members and helpers; original IsCreated/IsEmpty composition preserves the duplicated native count test. See rows/00c35e30/SDK_NAME_RECOVERY.md. The archived SDK is2.10; retail strings identify a Fable1.5 fork. Version differences are documented and must not be blindly imported.

**SIMD row:**00c6b260 is salFrameFloatToInt, proven by original SDK symbols/types and an exact315-byte reference-object comparison. The delivered legal C++ intrinsic model is391/315, selected9 of11, and remains BLOCKED. The reference SDK object is identity evidence, not a delivered reconstructed match. The shorter overreading/changed-rounding experiments were rejected. The original file-local sal_round helper integration, stack alignment and memory-form conversions remain unresolved. See its NAMING_EVIDENCE and complete history.

**File installer:**009c3590/009c3660/009c5690 now have original protected reference entrypoints, original helper/member names and passing sequential fixtures, but remain BLOCKED. Raw PDB fields carry the cross-thread qualifier prohibited by this pass. Ordinary-field code loses required reads or changes scheduling. Do not add the forbidden qualifier to force a match. The selected wide ABI is unsigned short; old native-wchar diagnostics are history only. DirtyDisc is protected in both PDBs; this corrects the inherited DirtyDiscError guess. See evidence/FILE_INSTALLER_NAME_RECHECK.json.

**Bad extents:**explicitly review every rows_with_recut_comparison entry in FINAL_PROGRESS.json.00854b90 has an independently reviewed exact566-byte recovered body with432cases/compiler, but its assigned span is21bytes, so it is still BLOCKED and excluded from57. Other recut comparisons are not exact accepted functions either. Do not silently enlarge the scope or count diagnostic body matches as assigned-span matches.

**Last small-row sweep:**font009a4160 original DBI locals/repeated indexes, vector005c0121 alias scheduling, friction008eebe0 completed-history review and lookup0072b330 real polymorphic base restoration did not add matches. See FOCUSED_RETRY_2026-11-01.md in those row directories. Main's two optimizer-policy diagnostics also worsened00681790/005dc340. Closest selections remain unchanged.

## Verification rules and continuation

Use retail Fable.exe SHA1 39fd416ba23a91a89713ec80c789f57ddb22e2c8 and **cl13.10.3077 RTM**; base /O2 /Oy /W3, plus only each source's recorded flags. Respect exact PDB/SDK names, class key/access/const/calling convention and one-address/one-symbol identity. Original input rules are included; later direct owner instructions override older per-row budgets: distinct evidence-led attempts were unlimited. No __asm, naked, _emit or volatile substrings in authored selected code/tests; no new packing. Shared generated headers retain their existing packing. Historical input/rejected attempts can violate rules and are included only as evidence.

Tests are self-contained C++03, /GR- native, GNU64 with long8 vs native long4 accounted for. Boundary doubles and host adaptations are explicitly documented. Some models project only the accessed original object prefix; those limitations must survive integration review. SIMD and concurrent behavior limitations are especially important. Regrading masks relocation operands only after valid alignment; raw diffs are authoritative when offsets move.

The included grade/review/development scripts preserve reproduction logic but refer to the original /workspace paths and an external repository/toolchain. Adapt paths to your checkout; do not run them blindly. verify_row.py resets its manual acceptance marker when rerun: re-establish independent name/relocation review before counting a row. Never run obsolete selection scripts just to regenerate fixtures; they may restore older candidates. Use the final hash-pinned selected files.

All workers have finished; no more rows are assigned or running. Continue from the final manifests, not old session status prose. No commit or remote push is authorized by this handoff. The owner can independently approve repository integration. No compiler, Wine, game executable, SDK binary/library, git history or symlinks are included. Textual compiler output/disassembly and original-name evidence are retained. SDK source provenance/hashes remain, but its downloaded distribution and binaries are intentionally excluded.

## Integrity

SHA256SUMS covers every other packaged file. Check it after extraction (`sha256sum -c SHA256SUMS`). PACKAGING_REPORT.json records exclusions and final counts. Evidence/FINAL_HANDOFF_AUDIT.md (if present) is the pre-normalization independent audit; HANDOFF_METADATA_NORMALIZATION.json and PRE_HANDOFF_METADATA retain corrections to stale counters/hashes/status fields. Those changes did not alter selected implementation or tests.
