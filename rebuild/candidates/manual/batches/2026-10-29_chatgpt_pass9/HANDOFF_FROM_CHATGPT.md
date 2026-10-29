# Pass 9 — current complete handoff

This is the current handoff. Start here. All authoring workers and their jobs have stopped at the owner's request. No repository files were edited, no rows were landed in a repository, and no commits or pushes were made.

The reservation is exactly **2,157 addresses** in `AUTHORIZED_SCOPE.json` and `INPUT/PASS/rows.tsv`. This is not the repository's full backlog. Of those, **157 functions (7.28%)** are currently verified selections, covering **39,391 bytes**. **1,523 reserved rows have no pass9 candidate/source grade**; see `UNTOUCHED_RESERVED.tsv`. Preparatory disassembly or name searches may exist for these rows. Many are earlier Claude retries; "untouched" means not authored/graded in this pass, not never attempted historically.

Read in this order:

1. `SUMMARY.json`, `NOTES.md`, `OWNER_NAMING_RULES.md`, and the unmodified `SETTLED_NAMES_2026-10-28.tsv`.
2. Run `python3 verify_package.py` inside the extracted root (or pass the ZIP path). This checks content hashes, reservation coverage, counters, and the final compiler/test receipts. It does not itself recompile.
3. `payload.json`, `src/`, and `test/` contain **only current verified selections**. Re-run them on your own MSVC 7.1 RTM 13.10.3077 lane against your own correctly hashed retail executable before coordinator landing. `payload_retail_asm.json` is empty.
4. `ROWS.tsv` / `STATUS.tsv` inventory every reserved row exactly once. `ROW_DETAILS.md` and `SELECTED/<category>/<address>/ROW.json` preserve attempts, residue, source selection, naming status and test limits. `HISTORY/` preserves all pass9 grade records; unresolved rows include all native experiment sources and transcripts. `WORK/` preserves additional authoring, tests, research and superseded records. Never treat these forensic files as accepted payloads.
5. `NAME_RECOVERY.md`, `NAME_FIXES.tsv`, `NAME_FIX_CONFLICTS.tsv`, `LESSONS.md`, and `NEXT_ACTIONS.md` explain recoveries, corrections and remaining risks.

Every current verified source/test pair was freshly checked before packaging: exact leaf, MATCH/RELOCATION_MATCH, relocation identity, strict GNU32 C++03 execution, and actual VC7.1 execution. `FINAL_RECHECK_INDEX.json` identifies the authoritative receipt per row; `FINAL_VERIFICATION/` contains the receipts. Three nullable-allocation fixtures require GNU `-fcheck-new` to model VC7.1 semantics; their successful final receipts supersede the initial GNU invocation without that flag. No candidate source was changed for this adaptation.

**Not accepted:** 8 historical byte/test selections still lack final owner naming/model approval; 12 other selected byte matches are also held. These are outside `payload.json`. Some have exhausted four source grades. A match with a known wrong declaration is not a finished row. Do not use an ungraded correction as if it had passed a native grade.

The archive contains no Wine, compiler, executable, object files, PDBs, oracle, or symbolic links. `SUPPORT/rebuild_include/` supplies the unchanged engine include snapshot. Use your repository/toolchain and cut your own oracle from retail SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`. Original input rules/data are under `INPUT/`; newer owner rules and this closeout supersede old continuation instructions in historical files.

All 1,108 pass9 native source experiments are retained in metadata. Four grades per row were enforced across root and every worker, including failed builds and duplicate strategies. Preserve counters when continuing this pass. Earlier Claude attempts are described by original `prior_sources` / `prior_grade`; no unknown pre-pass lifetime count is invented.
