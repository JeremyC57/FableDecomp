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
1. Parked-row retry sweep COMPLETE 2026-10-31: wave 1 landed 114 of 237 (`batches/2026-10-30_parked_retry1/`), wave 2 landed 169 of 2,121 over
   sections 1, 1b, 2, 2b, 2c, 3, 4, 4b, 5, 5b, 6, 6b (`batches/2026-10-3*_parked_retry2_sec*/`; each README has the counts). Remaining pending
   differ/ rows are the chunk pool (item 2). Open from the sweep: sec5b's six SSE rows in `pending/2026-10-31_parked_retry2_sec5b/retail_asm_review/`
   now have a landed precedent (sec6b's five kernels, 0x00BF7DF0 form) -- owner call to land them the same way; name_review rows (sec3 x3, sec2b x4, sec6 00f3fb60).
2. Chunks 596-604 landed (596-599: 36 of 40; 600: 6 of 10; 601: 5 of 10 + 1 held for naming; 602: 7 of 10, the libpk pool copy is
   `CHandleBasedMemoryPool_012b4098`, GOTCHAS 2026-10-31; 603: 8 of 8, the last engine-remainder rows; 604: 4 of 10, the first chunk
   cut from the sweep's parked rows; 605: 2 of 10; 606: 1 of 10; 607: 1 of 10; 609: 2 of 10). Chunks now come from the sweep's parked rows: `$S/w30/pick.py` ranks them (closest residue,
   shortest first) into `$S/w30/ranked.tsv`, `$S/w30/cutp.sh N addrs` cuts a chunk (agent dir `$S/w22/agentN`, CHUNK_BRIEF.md,
   `rebuild/candidates/manual/chunk_history.tsv`). Running: 610 (`$S/w22/agent610`); 608 reported, landing. Cut and queued, not launched: 611.
   Land each as it reports (`coordinator/land_chunk.sh N`, `records.py N`); re-run pick.py before cutting 609+.
3. Migration wave 10 per `headers/2026-10-30_retail_models_wave9/NEXT.md` (item 0: the CDefPointer(ENullDef) ctor is in the header since 2026-10-31 — migrate the 76 local `enum ENullDef` models; then the 1,317 proven CP rows).
4. Naming wave 12: queue rows 594-599, libsig/libsig2/libsig3, mig9, retry1, sec1-4 (~180 rows); the duplicate per-TU static / unfolded-COMDAT convention (owner decision open: ilog2 x3, SmoothFadeValue x3, CDefString x2, IS_THING_ALIVE<CIntelligentPointer<const CThing>> x2, LowerBound 0x004365B0/0x0040F020); three byte-matching sec3 bodies in `pending/2026-10-30_parked_retry2_sec3/name_review/` land once named; 0x00CB0C10 CompatibleTimeSetEvent; EAX GUID linkage; CDspGain::Apply two spellings; ScaleAccumulate16Ramp x3 (different bodies, distinct names).
5. Codex parked-50 pass out (2026-10-31, `pending/2026-10-31_codex_parked50/`): 50 rows reserved until 2026-11-03; regrade the returned zip with vland.py like ChatGPT pass 9. LZO 0x00C069D0 landed as toolchain_library from the vendor .asm (owner decision 2026-10-31; `LIBRARY_SIGNATURES.md` "Third-party assembled objects"); 0x00BE8920 is a pre-existing lane DIFFER (old Windows row) to recheck.
6. Older open work: `docs/ROADMAP.md`.
