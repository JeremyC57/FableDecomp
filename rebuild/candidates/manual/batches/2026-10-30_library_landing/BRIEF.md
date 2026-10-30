# Vendor-library landing pass (one agent; a naming agent shares the lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/libsig2/repo; cd $S/libsig2/repo` (env.sh resets FABLE_ROOT: export it
after; it also exports FABLE_EXE and FABLE_VS2003_LIB=$S/wine/vs2003lib, the VS .NET 2003 Vc7/lib the pass fetched).
Private worktree `$S/libsig2/repo` (detached HEAD ca802b46e); scratch `$S/libsig2/work/`; hand-back in `$S/libsig2/`.
Never run a git command that changes state; never write under /home/user/FableDecomp; never kill processes; `timeout` on every
command that could hang; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad; no
`__asm` / `naked` / `_emit` in anything you write. Do not edit landed rows outside the lists below.

## Context
The signature pass (rebuild/candidates/manual/batches/2026-10-30_library_signatures/README.md, docs/pipeline/LIBRARY_SIGNATURES.md,
tools/decomp_pipeline/libsig/, oracle rebuild/oracles/library_signatures.tsv, 448 rows with relocation proof) landed 4 rows as
grade `toolchain_library` (stub format, purity.py rule, verify_and_land.py library_proof(), build_candidates.ps1 status=LIBRARY,
dashboard line: study the four landed stubs 0040138c / 004013c7 / 00bfe9f9 / 00401284 and copy their shape exactly). Owner
rule: vendor bytes are linked, not reconstructed. This pass lands the rest of what the oracle proves.

## Work
1. **Re-grade the 14 landed MSVCR71 import-thunk rows** (00401356, 004013de, 00bfe9bc, 00bfea0e, 00bfea14, 00bfea1a, 00bfeae6,
   00bfeaf2, 00bfeb1c, 00bfeb22, 00bfeb3a, 00bfed16, 00bfed40, 00bfed4c): replace each file with a toolchain_library stub (the
   import library member and IAT slot from the oracle; the DLL!export name), fix the catalog entry (Grade, PassPattern not
   needed; keep Source/TestSource paths; the test file may stay but is not built), correct the catalog name in
   rebuild/oracles/auto-re-candidates.tsv col 2 to `MSVCR71.DLL::<export>` like 00401284, and retire bindings that point an
   invented stand-in name at these addresses: for every reloc_bindings.tsv line whose target is one of the 14, decide the
   right spelling (the import symbol `__imp_` is the IAT slot, the thunk itself is `_<export>` / the decorated C name) and
   respell the callers that emit it (recheck those callers: `recheck.py` + `vland.py --recheck-landed`, LANDVERIFY_WORK=
   $S/libsig2/work/lv). `bindings_multi_check.py` must print 0. Rename files with the row name when the landed name was wrong
   (list in RENAMES.tsv: old path, new path; you may `mv` inside your worktree, the coordinator will redo it with git mv).
2. **Land the 12 parked CRT sections** (_report_failure 00bfe9c8, __chkstk 00bfea30, __ftol2 00bfea70, __allmul 00bfeb50,
   __aullrem 00bfebb0, __allrem 00bfec30, __aullshr 00bfecf0, __aulldiv 00bfee00, eh vector constructor iterator 00f3fb96,
   __alldiv 00f3fc50, __allshr 00f3fd10, __aulldvrm 00f3fd50) and **the 98 unlanded catalog import thunks** (logs/cross.tsv
   bucket b) as toolchain_library rows: stub file under rebuild/src/compiled/<xx>/<yy>/ with the project's naming
   (`global_<Name>_<addr>.cpp` for CRT, `<DLL>_<export>_<addr>.cpp` style for thunks: check how the 4 and the landed thunk
   siblings are named and stay consistent), a catalog entry in build_candidates.ps1 (CRLF: open with newline=''), an
   auto-re-candidates.tsv row (address, name, length, bytes), and the parked copies removed from the pending pool
   (rebuild/candidates/manual/pending/**: list what you remove in REMOVED.tsv; do not delete anything else). Every row:
   `lib_verify.py <addr>` MATCH and `vland.py --recheck-landed` LIBRARY_MATCH (log both).
3. **Bindings for the new rows:** landed callers already bind these addresses under some spelling (e.g. `__allmul`,
   `__ftol2`, `__chkstk`, `_strlen`...). One retail address = one name: keep the C-decorated library symbol as the name;
   where a caller spells it otherwise, respell and recheck the caller (count them; if more than ~150 callers would change,
   list them in NAME_FIXES.tsv for the naming wave instead and keep the existing binding).
4. **The 8 landed CRT-source compiles** (WinMainCRTStartup 00401067, _onexit 00401296, atexit 004012bc, the RTC walkers
   004012ce / 00401312, _setdefaultprecision 0040135c, __ArrayUnwind 00bfee7a, eh vector destructor iterator 00bfeed8): do
   not change their grade; add a `// Library: <lib>(<member>) <symbol>` provenance line under their header comment so the
   dashboard can list them as vendor source (add a `landed_library_source` count to landed_metrics.py / dashboard if cheap).
5. **Manifest boundary:** write `$S/libsig2/OUTSIDE_CATALOG.tsv` (the 312 bucket c rows with proposed names) for the
   coordinator; do not add them to the catalog in this pass.
6. Run `tools/write_decomp_dashboard.py` and `tools/update_readme_progress.py`; record the deltas (expect toolchain_library
   4 -> 128, asm_bake unchanged, genuine catalog +110).
7. Update docs/pipeline/LIBRARY_SIGNATURES.md (what is landed now, how to land a future oracle row) and one GOTCHAS line.

## Hand-back (`$S/libsig2/`)
ROWS.tsv (addr, kind, lib/member/symbol, action, lib_verify, recheck), RENAMES.tsv, REMOVED.tsv, NAME_FIXES.tsv,
OUTSIDE_CATALOG.tsv, logs/ (lib_verify_all.log, recheck logs, dashboard deltas), LESSONS.md. Final message: rows re-graded /
landed, callers respelled and rechecked, bindings changed, dashboard deltas, anything the oracle proves that could not be
landed and why. Leave no background job running.
