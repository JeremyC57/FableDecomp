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
1. Land the parked-row retry sweep (`$S/retry1/`, 2,362 parked/skipped rows retested with the current levers) and run wave 2.
2. Chunk 596 (queued) then 597+ from `pending/2026-10-29_chatgpt_pass9/UNTOUCHED_RESERVED.tsv` engine remainder (68 rows left).
3. Migration wave 10 per `headers/2026-10-30_retail_models_wave9/NEXT.md` (1,317 proven CP rows first).
4. Naming wave 12: queue rows 594, 595, libsig, libsig2, libsig3, mig9; 0x00CB0C10 CompatibleTimeSetEvent; EAX GUID linkage.
5. Older open work: `docs/ROADMAP.md`.
