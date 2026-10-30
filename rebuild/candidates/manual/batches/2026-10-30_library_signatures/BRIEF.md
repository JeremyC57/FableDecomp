# Vendor-library signature pass (one agent; a migration agent and a chunk agent share the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/libsig/repo; cd $S/libsig/repo` (env.sh resets FABLE_ROOT: export it after).
Retail exe: `$FABLE_EXE` (PE32, ImageBase 0x400000; file offset = VA - 0x400000 for .text; check section headers for .rdata/.data).
Your private worktree is `$S/libsig/repo` (detached HEAD 7a56ee86e); scratch `$S/libsig/work/`; hand-back in `$S/libsig/`.
Never run a git command that changes state; never write under /home/user/FableDecomp; never kill processes; `timeout` on
every command that could hang; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.

## Why (owner, 2026-10-30)
Retail Fable.exe contains bytes that were never compiled from Fable source: the VC7.1 CRT objects the linker pulled from
`msvcrt.lib` (e.g. `sehprolg.obj` -> `__SEH_prolog` 0x0040138C / `__SEH_epilog` 0x004013C7; the /GS cookie check
`__security_check_cookie` 0x00BFE9F9), the import thunks the linker synthesised from import libraries (`jmp [__imp__exit]`
0x00401284), and any other prebuilt vendor library. The owner's rule: identify, isolate and signature those so the project
links the same libraries instead of reconstructing them, and so the dashboard separates vendor bytes from Fable logic.
Four such rows are currently landed as hand-written asm (`Grade = 'asm_bake'`): 0040138c, 004013c7, 00bfe9f9, 00401284
(files: rebuild/src/compiled/00/40/Global_SEHProlog_0040138c.cpp, Global_SEHEpilog_004013c7.cpp, Global_Exit_00401284.cpp,
rebuild/src/compiled/00/bf/global_securitycheckcookie_00bfe9f9.cpp; catalog rebuild/build_candidates.ps1; see
rebuild/candidates/manual/batches/2026-10-30_asm_reauthor/README.md + STATUS.tsv for what was tried).

## Libraries available (the era toolchain, the same the retail Rich header names)
`$S/wine/vc71rtm/lib/*.lib` (RTM 13.10.3077: msvcrt.lib, msvcprt.lib, libc*.lib, libcp*.lib, msvcirt.lib, oldnames.lib,
bufferoverflowu.lib, delayimp.lib, strsafe.lib, kernel32/user32/uuid import libs) and `$S/wine/vc71/lib/*.lib` (QFE 4035).
Retail links the DLL CRT (msvcr71.dll / msvcp71.dll imports), so msvcrt.lib / msvcprt.lib are the primary sources; libc*.lib
are static variants (same objects for many members: record which lib each hit came from, prefer msvcrt.lib when equal).
No DirectX SDK libraries are on disk; if the exe's import table or strings show D3DX / other static vendor libs, scope them
(name, likely SDK release) in the report without bytes.

## Method
1. **Parse the COFF archives in Python** (`!<arch>` format; first/second linker members, longnames member, then object
   members; import-library members are the short-import format `00 00 FF FF`). For each object: sections, symbols,
   relocations. Collect every code section (`.text`, `.text$mn`, COMDAT sections per function) as (lib, member, symbol(s),
   bytes, relocation offsets+types+target symbol). Write the parser as `tools/decomp_pipeline/libsig/coff_lib.py` in the
   worktree (stdlib only, Python 3).
2. **Signature-match against the exe:** for each library code section of >= 8 bytes, mask the relocation bytes and search
   `.text` of the exe for the pattern (byte-exact outside relocations). Also match import thunks: every `FF 25 xx xx xx xx`
   in .text whose operand is an IAT slot, named from the import directory (`__imp__exit` etc.). Resolve each masked
   relocation to the retail target and check it is consistent (a `__imp__` or another matched library symbol or a known
   catalog address). Tool: `tools/decomp_pipeline/libsig/libsig_scan.py` -> `rebuild/oracles/library_signatures.tsv`
   (address, length, lib, member, symbol, kind = coff_section | import_thunk, relocation targets, confidence). Dedupe:
   one retail address = one library symbol; when two libs give the same bytes, record both members in a notes column.
3. **Cross with the catalog:** `rebuild/oracles/auto-re-candidates.tsv` (col 1 address, col 2 name) is the landed set;
   `rebuild/COVERAGE.md` / `tools/write_decomp_dashboard.py` show how the totals are computed; the unlanded pool is in
   `rebuild/candidates/manual/pending/`. Report: how many signature hits are (a) landed rows (list them: these are vendor
   bytes we wrongly reconstructed or landed as asm; the landed C++ may still be a legitimate compile of CRT source -- say
   which), (b) unlanded catalog functions (link work, not reconstruction), (c) outside the catalog.
4. **Land the four rows under a new grade `toolchain_library`:**
   - `tools/decomp_pipeline/crawl/purity.py` `classify()` returns `toolchain_library` for a file whose header carries a line
     `// Toolchain-library: <lib>(<member>) <symbol> -- <n> bytes verified against Fable.exe <addr>` and that contains NO
     function body / no `__asm` / no `naked` / no `_emit` (the stub is documentation plus the proof reference). Keep all
     existing classifications unchanged; add the new one before the asm checks.
   - Replace each of the four files with such a stub (the import thunk cites the import library member and the IAT slot);
     delete nothing else. Catalog: change `Grade = 'asm_bake'` to `Grade = 'toolchain_library'` on those four entries.
   - `tools/decomp_pipeline/libsig/lib_verify.py <addr>`: re-extracts the member, masks relocations, compares with the exe,
     prints MATCH/MISMATCH; the stub's proof line must be reproducible with it. Run it on all four (expect MATCH).
   - `tools/write_decomp_dashboard.py` (and anything it feeds, e.g. tools/update_readme_progress.py, rebuild/COVERAGE.*):
     add a line "Landed rows that are toolchain/vendor library bytes (grade `toolchain_library`)", counted as genuine like
     `retail_asm` (owner rule: the bytes are the toolchain's, the project links them), and make the asm-bake line exclude
     them. Run both tools; check the numbers move by exactly 4.
   - `tools/decomp_pipeline/verify_and_land.py` / `linux_lane/lane_tools/vland.py`: make sure `--recheck-landed` on a
     `toolchain_library` row calls lib_verify.py instead of compiling (or skips with a clear LIBRARY line); do not weaken
     any other path. Test: recheck.py + vland.py --recheck-landed on the four addresses (LANDVERIFY_WORK=$S/libsig/work/lv).
   - Document the grade in CLAUDE.md's "Byte purity" bullet (one sentence) and docs/pipeline/GOTCHAS.md (one line), and
     write `docs/pipeline/LIBRARY_SIGNATURES.md` (how the scan works, how to rerun, what it found, the D3DX/other scope).
5. **Do not** edit any landed row other than the four (report the others in the hand-back for the coordinator).

## Hand-back (`$S/libsig/`)
REPORT.md (counts per the three buckets with lists; the four rows' verification; libraries scoped but not on disk),
SIGNATURES.tsv (copy of the oracle), LESSONS.md, logs/. All code/doc edits stay in the worktree (the coordinator commits).
Final message: hits total / landed / unlanded / outside catalog, the four rows' status, dashboard deltas, open questions.
Leave no background job running.
