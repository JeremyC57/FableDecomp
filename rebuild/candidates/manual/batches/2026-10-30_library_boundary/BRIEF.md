# Manifest boundary pass for the 312 vendor functions outside the catalog (one agent; two other agents share the lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/libsig3/repo; cd $S/libsig3/repo` (env.sh resets FABLE_ROOT: export it
after; it exports FABLE_EXE and FABLE_VS2003_LIB=$S/wine/vs2003lib). Private worktree `$S/libsig3/repo` (detached HEAD 3e83ccc7f);
scratch `$S/libsig3/work/`; hand-back in `$S/libsig3/`. Never run a git command that changes state; never write under
/home/user/FableDecomp; never kill processes; `timeout` on every command that could hang; never repeat a failing command more
than twice; no recursive find over `/` or the scratchpad; no `__asm` / `naked` / `_emit` in anything you write.

## Context
The vendor-library signature pass and landing pass (rebuild/candidates/manual/batches/2026-10-30_library_signatures/ and
2026-10-30_library_landing/, docs/pipeline/LIBRARY_SIGNATURES.md, tools/decomp_pipeline/libsig/, oracle
rebuild/oracles/library_signatures.tsv) proved 448 retail functions are toolchain bytes and landed 128 of them as grade
`toolchain_library` (study the landed stubs, e.g. rebuild/src/compiled/00/40/Global_SEHProlog_0040138c.cpp and the
`global_<export>_<addr>.cpp` thunk stubs from the landing pass, their build_candidates.ps1 entries, which carry
`Grade = 'toolchain_library'` and are not compiled, and their rebuild/oracles/auto-re-candidates.tsv rows). 312 proven rows
are outside the function manifest (`rebuild/manifest/functions.tsv`, 57268 lines): the landing pass wrote proposed names and paths
in `2026-10-30_library_landing/OUTSIDE_CATALOG.tsv` (307 import thunks: kernel32 120, user32 75, MSVCR71 37, winmm 22, wsock32
20, advapi32 9, ole32 7, gdi32 5, imm32 4, MSVCP71 3, shell32 2, oleaut32 2, d3dx9_25 1; 5 CRT functions: _check_managed_app
00401011, the type_info copy ctor 00bfe960, ??_Etype_info 00bfe96b, ArrayUnwindFilter 00bfee68, ___security_init_cookie
00bfef3e). Owner (2026-10-30): "do the 312 and fix the manifest".

## Work
1. **Manifest:** add the 312 functions to `rebuild/manifest/functions.tsv` with `agent_source = boundary` (look at how the
   existing gapscan / boundary rows fill the other columns: name, module, calling_convention, return_type, parameter_count,
   parameter_types, prototype_complete, clean_decompile, naming_quality, agent_verdict, lift_grade, lift_source,
   agent_source_path, compiled_status, behavior_test, retail_parity, compiled_source; use the library symbol's decorated
   name for the name and the DLL / lib member for the module; keep the file's column order, encoding and line endings).
   Check for any other manifest consumers that must agree (`rebuild/manifest/*.json`, tools/landed_metrics.py
   `known` / `original` sets, tools/write_decomp_dashboard.py, docs that quote the manifest count) and update them.
   Also check whether any of the 312 collide with an existing manifest row at a different address (an existing row whose
   range contains the thunk) and resolve by evidence.
2. **Land all 312 as `toolchain_library` rows** exactly like the landing pass: stub under rebuild/src/compiled/<xx>/<yy>/
   (naming consistent with the landed thunk stubs), catalog entry in build_candidates.ps1 (CRLF: open with newline=''),
   auto-re-candidates.tsv row (address, name, length, bytes). Every row: `lib_verify.py <addr>` MATCH and
   `recheck.py` + `vland.py --recheck-landed` LIBRARY_MATCH (LANDVERIFY_WORK=$S/libsig3/work/lv; log both for all 312).
   For 00bfe960 (the type_info copy ctor picked by RTTI out of 149 identical copies) the stub must state the RTTI
   evidence that selects this copy.
3. **Bindings:** landed callers already bind some of these addresses under stand-in spellings (search reloc_bindings.tsv
   by target and scan relocation pairs like the landing pass did, LESSONS 1). One retail address = one name: the library
   symbol wins; respell and recheck the callers (MATCH/RELOCATION_MATCH + PASS). If a family exceeds ~150 callers, list it
   in NAME_FIXES.tsv instead. `apply_bindings.py` format: action/symbol/target/by_row/note; `bindings_multi_check.py` must
   print 0.
4. **rich_header.py labels:** the signature pass found that prodid 0x0F is MASM 7.10, 0x5F the C compiler and 0x60 C++
   (the matched objects' @comp.id stamps), while tools/decomp_pipeline/rich_header.py labels 0x0F "Linker" and 0x5F
   "Utc1310_CPP". Fix the table with the evidence in a comment and check the docs that quote it (docs/BUILDING.md,
   docs/journal/FINDINGS_LOG.md entries are history: leave them; add one FINDINGS line in your hand-back instead).
5. Run `tools/landed_metrics.py`, `tools/write_decomp_dashboard.py`, `tools/update_readme_progress.py`; record the deltas
   (expect toolchain_library 128 -> 440, known_functions +312, catalog rows +312). Update docs/pipeline/LIBRARY_SIGNATURES.md
   (manifest coverage of the oracle is now complete) and add one docs/pipeline/GOTCHAS.md line.

## Hand-back (`$S/libsig3/`)
ROWS.tsv (addr, name, lib/member, lib_verify, recheck), MANIFEST_ADDED.tsv, BINDINGS.tsv, NAME_FIXES.tsv, logs/ (lib_verify_all.log,
recheck logs, dashboard deltas), LESSONS.md. Final message: rows added to the manifest and landed, callers respelled and
rechecked, bindings changed, dashboard deltas, anything that could not be landed and why. Leave no background job running.
