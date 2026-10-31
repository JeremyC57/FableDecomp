# Resume here (one page) — updated 2026-10-30

**Next session: read `docs/journal/2026-10/2026-10-29_HANDOFF.md`** (lane rules, resume steps) and the 2026-10-30 lines at the
end of `docs/journal/FINDINGS_LOG.md`. Fresh container: `docs/pipeline/FRESH_SESSION_HANDOFF.md`; coordinator commands in
`tools/decomp_pipeline/linux_lane/lane_tools/coordinator/README.md`. Older one-page handoffs: `docs/journal/HANDOFF_ARCHIVE.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`), pushed. Coverage figures live in `rebuild/COVERAGE.md` (`tools/landed_metrics.py`,
  `tools/write_decomp_dashboard.py`, `tools/update_readme_progress.py`; never hand-edit): ~90.7% of the 57,579 known
  functions, 90.6% of the original 49,568 catalog; 440 rows are grade `toolchain_library` (vendor bytes, link work), 13 asm_bake.
- Retail compiler: cl 13.10.3077 (Rich header: 1,369 of 1,380 compiled objects; 5 QFE-4035 objects, possibly WinDDK GUID
  libs; 6 import descriptors; link 3077, cvtres 3052). Lane default 3077, `--qfe` 4035. VS 2003 `Vc7/lib` lives in the lane
  only (`$S/wine/vs2003lib`, `libsig/fetch_vs2003_libs.sh`); never committed.

## Owner rules in force (2026-10-28..30)
- Names right the first time (exact PDB spelling, two sources, one retail address = one mangled name; NAME_FIXES queue
  `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`). Wide text `unsigned short`. Retail calling convention wins.
- A class with a retail vtable is modelled from a shared `engine/<Class>.h`, never a row-local stand-in (migration waves
  1-9, `rebuild/candidates/manual/headers/`). Class-model migration and chunk cutting run concurrently.
- Grades: `retail_asm` only for instructions VC7.1 cannot emit; `toolchain_library` for CRT / import-thunk bytes proven by
  `libsig/lib_verify.py` (`docs/pipeline/LIBRARY_SIGNATURES.md`). Everything else is compiled C++ that byte-matches.
- At most two Opus 5.5 high-effort agents at once; "lane gap" / parked labels are opinions to retest, never a reason to skip.

## Done 2026-10-30 (details in FINDINGS_LOG)
- Chunks 585-595 (engine-range pass 9 rows): 92 of 110 landed; migration waves 7-9 (1,667 + 1,384 + 1,578 rows; new
  CLevelScriptBase/CEntityScriptBase inline ctors, CActiveEntityScript<T>, CCountedPointer.h); naming wave 11 (173 rows);
  asm re-author (9 of 27); vendor-library signature, landing and manifest-boundary passes (448 oracle rows, all landed).

## Next
1. Parked-row retry sweep: wave 1 landed 114 of 237 (`batches/2026-10-30_parked_retry1/`; the 10 MMX/SSE2 kernels landed as retail_asm by owner decision). Wave 2 runs the 2,121 untouched rows as six address sections (`$S/retry/sec1..6`, split.py + BRIEF_TEMPLATE.md), two agents at a time: sections 1 (14 of 110), 2 (6 of 353, flag sweep only), 4 (33 of 354), 3 (32 of 353) and 2b (12 of 337, 85 rows unattempted at the lane limit) are landed (`batches/2026-10-30_parked_retry2_sec{1,2,3,4,2b}/`). Landed too: 1b (20 of 244, `batches/2026-10-30_parked_retry2_sec1b/`). Landed too: 2c (2 of 85, `batches/2026-10-31_parked_retry2_sec2c/`). Landed too: 4b (2 of 8 skeletons, `batches/2026-10-31_parked_retry2_sec4b/`). Landed too: 5 (18 of 354, `batches/2026-10-31_parked_retry2_sec5/`; 39 unopened rows → 5b in `$S/retry/sec5b`). Landed too: 6 (38 of 353, `batches/2026-10-31_parked_retry2_sec6/`) and 5b (5 of 39, all retail_asm, `batches/2026-10-31_parked_retry2_sec5b/`). Running: 6b (`$S/retry/sec6b`, 99 rows: 88 unreached from 6 + 11 unopened from 5b). After 6b reports, the sweep is complete; remaining pending differ/ rows return to the regular chunk pool. Land each section as it reports (procedure: the sec4 landing commands in the session transcript / README of each batch).
2. Chunks 596-599 landed (36 of 40). Chunks 600 (6 of 10), 601 (5 of 10 + 1 held for naming) and 602 (7 of 10; the libpk pool copy is now `CHandleBasedMemoryPool_012b4098`, GOTCHAS 2026-10-31) landed; 603 (the last 8 engine-remainder rows) is running in `$S/w22/agent603`; after it the pass 9 engine remainder is exhausted and chunks come from the sweep's pending differ rows.
3. Migration wave 10 per `headers/2026-10-30_retail_models_wave9/NEXT.md` (item 0: the CDefPointer(ENullDef) ctor is in the header since 2026-10-31 — migrate the 76 local `enum ENullDef` models; then the 1,317 proven CP rows).
4. Naming wave 12: queue rows 594-599, libsig/libsig2/libsig3, mig9, retry1, sec1-4 (~180 rows); the duplicate per-TU static / unfolded-COMDAT convention (owner decision open: ilog2 x3, SmoothFadeValue x3, CDefString x2, IS_THING_ALIVE<CIntelligentPointer<const CThing>> x2, LowerBound 0x004365B0/0x0040F020); three byte-matching sec3 bodies in `pending/2026-10-30_parked_retry2_sec3/name_review/` land once named; 0x00CB0C10 CompatibleTimeSetEvent; EAX GUID linkage; CDspGain::Apply two spellings; ScaleAccumulate16Ramp x3 (different bodies, distinct names).
5. Codex parked-50 pass out (2026-10-31, `pending/2026-10-31_codex_parked50/`): 50 rows reserved until 2026-11-03; regrade the returned zip with vland.py like ChatGPT pass 9. LZO 0x00C069D0 landed as toolchain_library from the vendor .asm (owner decision 2026-10-31; `LIBRARY_SIGNATURES.md` "Third-party assembled objects"); 0x00BE8920 is a pre-existing lane DIFFER (old Windows row) to recheck.
6. Older open work: `docs/ROADMAP.md`.
