# Retail class-model migration, wave 6 (one agent; the chunk 590 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig6/repo; cd $S/mig6/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1-5 landed (613 + 711 + 838 + 1,228 + 1,103 rows); naming wave 10 then applied the respellings wave 5 asked for
(0x00CD23B9 IsValid, 0x006E7510/30 Min/MaxWorldFrameForMessages, 0x00449970 GetMainPlayer, the ScriptedThingBase UAE slots,
the libpk lock family; records `rebuild/candidates/manual/naming/2026-10-30_wave10/`). Migration records are in
`rebuild/candidates/manual/headers/2026-10-*_retail_models_wave{1..5}/` (HEADERS.md, LESSONS.md, scripts/; wave 5's NEXT.md
is the plan you execute, minus its item 1 which wave 10 did). Full rules: `$S/mig1/BRIEF.md` (your worktree is `$S/mig6/repo`;
scratch `$S/mig6/work/`; hand-back in `$S/mig6/`). Copy wave 5's `scripts/` into `$S/mig6/scripts/` and extend them.
Regrading runs ~50 rows/min when the lane is quiet; budget ~1,200 edited rows.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS.

## Order of work
1. **Re-run the family dry runs on the current tree first** (wave 10 respelled 543 rows; many previously blocked CGSI /
   GetMainPlayer / GRO rows should now take the headers): `mig_cgsi_m.py`, `mig_gro.py`, the GetMainPlayer accessor rows
   (wave 10 DEFERRED: 14 rows on the method-less generated `engine/CPlayerManager.h` -- add the PDB methods to that header
   by hand with evidence, the generator skips hand-corrected headers), the 4 reverted `GetPlayer` rows.
2. **CGSI remainder** per wave 5 NEXT items 2-5: donor-alias rows (41; drop the `#define CGameScriptInterface
   CGameScriptInterfaceDonor` trio first), GetTextIDFromSymbol 0x006E77F0 (38 rows; prove the name from callers, the string
   table it reads and FableWin's inline expansions, then add it to the header), CGSIB rows on the CScriptBase chain (24,
   with item 4), float setters (8: the PDB slot takes float, the row stores a long).
3. **GRO mixed inlining (~106 rows):** find what decides VC7.1's per-site inlining of the implicit ScriptedThingBase ctor/dtor
   (EH state? caller size? auto_inline?). The 50 rows calling 0x007E72A0 explicitly are the test set. Do not change the GRO
   headers for this (wave 5 LESSONS item 2); record the rule, then migrate what the rule allows. Time-box ~1.5 h.
4. **The NScript bulk** (CScriptBase 1,704 / CScriptThing 2,503 / CEntityScriptBase 1,122 / CLevelScriptBase 951 / CTimer 200
   local models) per wave 1 NEXT (a)-(d): CScriptBase + CLevelScriptBase first (script every canonical shape, mcheck), then
   CEntityScriptBase + CScriptThing (the CScriptThing copy rule: out-of-line in the header, a row-local InlineCopy tag at the
   inline site; chunk 583 LESSONS: wrappers after the header need `__forceinline`), then CIsThingConscious (48) and the CGSI
   `CScriptThing Hero` / timer-map members that follow from it.
5. **Wave 4 leftovers** as budget allows: PDCB 67 (7 fixable), BC 21, DCB clusters, the 16 SFB hand edits.
6. **Scope only (NEXT.md, no edits):** the engine value-class headers chunks 585-589 keep asking for -- C3DVector,
   C3DBoundingBox, CMatrix3x4, CRightHandedSet, CArray<T>, CCountedPointer<T>, CCharString/CBasicString, CEngineScene*,
   C3DGroup2/C3DQuad2 -- count the landed rows modelling each locally and propose header shapes with evidence sources. Also
   count landed rows carrying `__declspec(naked)` / `__asm` without a `// Retail-asm:` line (27 at wave 6 start) and list them.

## Rules (unchanged; waves 3-5 LESSONS are binding)
- Def-chain copy members stay out-of-line declarations; the /O2 expanders and CBaseClass.cpp TU rows stay local by design.
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm.
- Regrade in batches of ~300-400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig6/work/lv<n>`, delete
  each lv after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert rows that stop matching; log residue.
  Check final files against the last regraded source (payload_check.py).
- Bindings: one retail address = one name; update `reloc_bindings.tsv` only when a rewrite changes a row's relocation
  symbols (list them in bind_add.tsv); `bindings_multi_check.py` must print 0. NAME_FIXES respellings go to NAME_FIXES.tsv
  for the coordinator, not into the rows, unless the row's own leaf changes with its header.
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig6/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md, NEXT.md (wave 7 counts, the engine value-class header scope, the asm_bake list),
NAME_FIXES.tsv, LESSONS.md (the GRO inlining rule), bind_add.tsv, logs/, scripts/. Final message: headers added/changed,
rows per family, recheck totals, reverted rows, bindings changes, the GRO rule, open questions. Leave no background job running.
