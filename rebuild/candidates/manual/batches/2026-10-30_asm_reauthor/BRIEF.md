# asm_bake re-author pass (one agent; migration wave 7 shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; cd /home/user/FableDecomp`. Work only in `$S/asmre/`. Never run git, never write under
/home/user/FableDecomp, never kill processes; `timeout` on every compile; never repeat a failing command more than twice.

## Why
27 landed rows (`$S/asmre/rows.tsv`; the list came from migration wave 6, `rebuild/candidates/manual/headers/
2026-10-30_retail_models_wave6/logs/asm_bake.tsv`) are grade `asm_bake`: whole function bodies transcribed as
`__declspec(naked)` / `__asm`. CLAUDE.md: that grade is an oracle, never reconstruction. The owner wants them re-authored as
C++ that VC7.1 compiles to the retail bytes. Two groups (column `group` in rows.tsv):
- **`reauthor` (22 rows):** ordinary C++ in retail (getters, operator+, a 150-byte GiveHeroMorality, bank-file openers, a
  404-line console-variable initialiser...). Write the C++. The current asm file is your byte oracle AND a map of what the
  function does; read it, then write C++ from scratch -- do not keep any `__asm`.
- **`crt_or_cpuid` (5 rows):** 0x00A5B850 runs `cpuid` (no VC7.1 intrinsic), 0x0040138C / 0x004013C7 are the CRT
  `__SEH_prolog` / `__SEH_epilog`, 0x00BFE9F9 `__security_check_cookie`, 0x00401284 is an import-thunk cluster. For these,
  do NOT re-author: write the smallest C++ body with the `__asm` block retail needs and a `// Retail-asm: <retail addr>
  <instruction>` line naming the instruction VC7.1 cannot emit (cpuid; `mov fs:[0],...` SEH frame install; for the cookie
  check and the thunks say which instruction, or report that the row should come from the CRT library, not reconstruction).
  `python3 -c "import sys; sys.path.insert(0,'tools/decomp_pipeline'); from crawl import purity; print(purity.classify(open(f).read()))"`
  must print `retail_asm`. No `naked`, no `_emit`, no `ret` inside the asm.

## Method (per row)
1. Read the landed source (path in rows.tsv) and its test; read the retail bytes: `$S/asmre/oracle.tsv` has address/name/
   length/bytes for 21 rows; for the 6 with no catalog length (0099bf30, 0099be70, 009a8840, 009a8170, 00413c50, 00401284)
   take the length from the landed file's comments or from `python3 $S/w29/extent.py` style disassembly of `$FABLE_EXE`
   (file offset = VA - 0x400000; trim trailing 0xCC) and write your own oracle line.
2. Prove the class/name as the chunk agents do (`tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`;
   PDB name lists `ghidra_out/egor_pdb_names.tsv` / `fablewin_pdb_names.tsv`; headers `rebuild/include/engine/<Class>.h`
   with the retail opt-ins FABLE_RETAIL_DEF_CLASS_BASE / FABLE_RETAIL_SUSPENDABLE_PROCESS / FABLE_RETAIL_NSCRIPT /
   FABLE_RETAIL_BASE_OBJECT; never a local stand-in for a class with a retail vtable). Keep the landed name unless you prove
   it wrong (then NAME_FIXES.tsv).
3. Write C++, grade with `bash $S/gq.sh` / `$S/tools/q.py` (as the chunk agents; read `$S/w22/agent592/CHUNK_BRIEF.md` and
   `PROMPT.md` for the lane tools, flags `/O2 /Oy /W3` + per-row extras, `leaf`), up to 6 grades per row. Lessons for the
   shapes these rows hit are in `docs/pipeline/GOTCHAS.md` (last ~120 lines: CWideString/CCharString copy rules, same-TU
   statics, `fistp` as retail_asm, per-site dtor inlining, implicit copy ctors) and the hand585-592 batch LESSONS.md files.
4. On a match, write a test that embeds the source and prints exactly `<addr>_TEST PASS` (keep the landed test's
   behaviour checks where they still apply); dry-run `vland.py` (`LANDVERIFY_WORK=$S/asmre/lv`) without `--land`: MATCH or
   RELOCATION_MATCH + PASS, no RELOC_IDENTITY. The row's bindings should keep the same targets with proven names.
5. Rows you cannot match: keep the asm file unchanged (it stays an oracle), record the residue.

## Hand-back (`$S/asmre/`)
`src/<addr>.cpp`, `test/<addr>.cpp`, `payload.json` (`{"result":{"authored":[{address,module,name,source_cpp,test_cpp,
pass_pattern,leaf,flags?}]}}`; retail_asm rows in a separate `retail_asm_payload.json`), `oracle_final.tsv`, STATUS.tsv
(addr, status, grades, best residue), NOTES.md, LESSONS.md, NAME_FIXES.tsv. Final message: matched / retail_asm / unmatched
counts per row with residues, names changed, lessons. Leave no background job running; delete lv dirs.
