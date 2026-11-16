# Pass23 — final owner handoff

Read this file first, then STATUS.tsv, ROW_DETAILS.md and the selected row's NAMING_EVIDENCE.md / PROBES.md. This is the current handoff for the100 reserved rows from the2026-11-15 package. Research stopped at the owner's request on2026-11-16. No git commit or push was made.

## Verified result

- **91/100 reserved rows verified (91%). Nine parked; zero untouched.** This denominator includes assigned generated EH rows, not100 independent ordinary functions and not the whole-game completion percentage.
- **57,423 unique verified bytes:**56,795 complete function-span bytes (including embedded jump tables where applicable), plus628 disjoint generated cleanup/handler bytes. Overlapping catch spans are counted once. Context helpers and EH metadata receive no extra credit. See evidence/VERIFIED_BYTE_RANGES.tsv and BYTE_LEDGER_AUDIT.json.
- All100 selected source files were graded with real MSVC7.1 RTM13.10.3077 on the same final shared-header tree. All91 accepted rows were recompiled and their operand audits rerun after the last header merge. Tag: `handoff_final`.
- All100 standalone tests passed native `/Od /W3` **without /EHsc** and64-bit GNU C++03. Six decoder fixtures need their documented `-fcheck-new` flag to preserve VC7.1's nullable global-new behavior. The first final GCC batch omitted that flag; the failures and corrected passing receipts are both retained. No source was changed to conceal those configuration failures.
- `FINAL_VERIFICATION.json`, `evidence/HANDOFF_ALL_TESTS.json` and each `history/<addr>/handoff_final/RESULT.json` identify the final files. Source/test hashes are checked. Passing behavior tests do not make the nine parked rows byte matches.

## What to apply and verify

1. Start with the original pass23 repository/include baseline, or merge into your newer tree using the old-text/new-text pairs in `hdr/PATCHES.json` and the small patches in `hdr/patches/`. The191 returned header files include new files and changed files. Never blindly overwrite a newer repository header. Default and old opt-in paths were checked; moved assert typedefs are pinned. Re-run your regression on your actual merged tree.
2. Selected production sources and self-contained tests are `rows/<addr>/<addr>.cpp` and `<addr>_test.cpp`. Required parent translation units are beside them; generated cleanups are emitted by compiling those parents, never hand-authored. Source dependencies and actual comparison starts are in ROW.json / STATUS.tsv / BOUNDS_FIX.tsv.
3. Compile with RTM13.10.3077, base `/O2 /Oy /W3` plus each source's Flags. Check the actual emitted Leaf against LABELS.tsv, mask only genuine relocations, and independently resolve every operand. Generated EH rows require the parent/EH graph checks described per row. The return label can be the parent symbol when that is what the wrapper intentionally compiles.
4. Re-run each test alone with your merged `include/` only, `/Od /W3` without EHsc. No test_include directory is required. On GNU use `-std=gnu++03 -O0 -fno-rtti -Wall -Wextra`; add `-fcheck-new` for01216762/6d/7b/86/94/9f. Supplemental exception tests and historical case counts are documented per row; the final standard native run intentionally omits EHsc.
5. Land only the91 RELOCATION_MATCH entries after your own review. Keep the nine PARKED entries as research. Exact generated children of an unmatched required parent are not credited independently.

## Remaining work — five independent problems / nine reserved rows

| Rows | Required body | Best result | What remains |
|---|---|---|---|
|007d00f0|CTCTavernGameShoveHaPenny::FrameUpdate|1016/1016|Frame0x28 versus0x2c and local-slot placement. Saved best has genuine shared string cleanup, correct array-delete binding,61 operand and10 virtual checks; five context helpers match but are uncredited.77 research/control receipts,28 conservative families; final reviews separate.|
|0089c910|CGameScriptInterface::CanThingBe_Sensed_ByOtherThing|1016-byte span;974 code bytes versus976|Only `fcompp` versus `fcomp st(1)` plus a later `fstp st(0)` remains; later branch/local targets shift2bytes. Original squared-magnitude shape fixed the prior product and load-order differences. Both revised helpers are ordinary inline with `/d2inlS100`;113 earlier source/control/review snapshots,18 conservative families, plus final handoff grade.|
|012152d0|DecodeXboxAdpcm at00c425e0|221/217|Extra receiver LEA and scratch/address-load order.76 research/control receipts,21 conservative families; correct original SDK class/header. No complete operand acceptance claimed while the body differs.|
|012117a5,012117ad|CSoundEventManager::PlayStreamed at00c12e50|741/741|One offset-store permutation:13 unequal byte positions477–490. Typed best source is selected. Eleven final constructor-chain probes did not improve it;24 conservative families total. Generated29bytes and metadata44bytes exact, zero credit until parent matches. Real constructor-chain research includes useful exact context bodies but remains unselected.|
|012139db,012139e3,012139eb,012139f6|CEventScript constructor at00c2cbd0|132/132|One owner-load permutation:10 unequal nonrelocated bytes.81 research/control/preparation receipts,20 conservative families; separate verification grades are not additional strategies. Generated48bytes and metadata60bytes exact, zero credit until parent matches. Genuine owner-factory context93bytes matches but does not fix parent and is uncredited.|

## Identity and limits

Use exact original PDB/SDK names where the evidence supports them. `NAMING_EVIDENCE.md` separates original identity from descriptive reconstruction. Original libpk names are absent from the available game PDB name sets in several sound/script families; those names remain explicitly descriptive and are not presented as recovered original spellings. Some unused virtual-prefix parameter types are conservatively opaque and not claimed proven original types. Existing owner-approved class suffixes remain qualified in the evidence. A renderer private global has its original source spelling proved but its original private linker decoration unavailable; its NAME_FIXES evidence says so.

`NAME_FIXES.tsv` consolidates406 address-based corrections; evidence includes confidence/qualification. `NAME_FIXES_NONADDRESS_NOTES.tsv` retains two type/inline-name notes which are not retail-address bindings. Do not infer that every descriptive name is a PDB name. LABELS.tsv is the exact compiled selected leaf, not a manifest guess.

## History and reproducibility

- `history/<addr>/<tag>/`: retained source snapshots, grade/test receipts, compiler logs and symbolic audits. Controls, failed preparations, verification repeats and genuine new shapes are distinguished in PROBES.md. A large receipt count is not a count of distinct strategies. Parent research is shared across its assigned cleanup rows.
- `attempt_headers/manifests/<addr>/<tag>.json` maps historical header paths to SHA256-addressed text under `attempt_headers/blobs/`. `tools/restore_attempt_headers.py` reconstructs any such overlay. This preserves all duplicate immutable header snapshots without hundreds of thousands of repeated files. An older partial-storage manifest overridden by a later complete grade is retained under `archived_manifests/`.
- Historical rejected candidates and constructor-context experiments are evidence, **not selected production**. Do not apply their headers over the final `hdr/`. The row-root source/test and root header patches are authoritative.
- Original local paths in historical receipts refer to `/tmp/work/codex_pass23_20261115`; map that prefix to this extracted archive. Toolchain/reference paths are historical, not bundled dependencies. The original game exe, compiler, Wine, PDB binaries, raw cuts, COFF objects and disassembly dumps are intentionally excluded.
- Verification/research Python scripts are retained under `verification_tools/` for inspection/adaptation. They may reference the original local toolchain; use your maintained grading pipeline for landing.
- Storage exhausted the workspace inode limit during final verification. Completed results were preserved, incomplete snapshots kept, duplicate headers compacted losslessly with CRC/SHA checks, and remaining checks resumed. The final result summaries supersede those retained preparation failures.

`SHA256SUMS` covers every archive file except itself. Use `tools/check_archive.py` after extraction. This handoff is ready for your independent verification and landing review; no online repository state was changed.

## Compact log storage
Thousands of historical compiler banners and empty preprocessing logs were byte-identical. `history/LOG_ALIASES.json` records each original path, canonical identical log and SHA256. Run `tools/restore_logs.py` to materialize those duplicates if a historical script expects them. All differing/error logs, every source, every test and every grade receipt remain ordinary files. This is lossless deduplication, not dropped attempt information. The ZIP uses standard LZMA compression; extract with Python zipfile or7-Zip.
