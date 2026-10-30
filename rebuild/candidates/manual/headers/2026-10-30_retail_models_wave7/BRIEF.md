# Retail class-model migration, wave 7 (one agent; the chunk 591 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig7/repo; cd $S/mig7/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1-6 landed (613 + 711 + 838 + 1,228 + 1,103 + 1,481 rows). Records in `rebuild/candidates/manual/headers/
2026-10-*_retail_models_wave{1..6}/`; wave 6's NEXT.md is the plan you execute and its LESSONS.md item 1 (the GRO dtor-size
inlining rule) is binding. Full rules: `$S/mig1/BRIEF.md` (your worktree is `$S/mig7/repo`, detached HEAD 7b828c0c5; scratch
`$S/mig7/work/`; hand-back in `$S/mig7/`). Copy wave 6's `scripts/` into `$S/mig7/scripts/` and extend them. Regrading runs
~50 rows/min when the lane is quiet, ~17 with a chunk agent on it; budget ~1,500 edited rows.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS.

## Order of work (wave 6 NEXT items 1-7)
1. **The 185 non-template CLevelScriptBase rows**: `: public CLevelScriptBase<T>` with the PDB master-data T (from the row's
   `CQ_XMasterData* PMasterData` or the Ego_r class key of the level script), then mig_sb.py. The 78 inline-ctor rows need
   the retail CLevelScriptBase ctor settled first: look for the two stores after `call 0x00CB8110` in the level-script ctors
   and declare it in the header accordingly (inline or out of line), with the evidence in the header comment.
2. **`CGameScriptInterface* Game` (41 + 5 rows)**: the PDB member is `CGameScriptInterfaceBase* Game`; change the type and
   cast at the call, or a typed accessor on the row's own base; mcheck decides.
3. **SB rows with a declaration-only local CBaseClassNonCopyable (42)**: drop the block, mig_sb.py.
4. **CScriptThing (2,507), then CEntityScriptBase (1,295), then CIsThingConscious (48)** per wave 1 NEXT (c): the CScriptThing
   copy rule (out-of-line in the header, a row-local InlineCopy tag at the inline site); wrappers defined after the header
   need `__forceinline` (chunk 583); an inline CScriptThing copy ctor must list `CBaseClass(other)` (chunk 584). Script every
   canonical shape, mcheck, regrade in batches. This is the wave's bulk; start it by hour 2 even if items 1-3 are unfinished.
5. **GRO residue**: MovieBase rows (85) and argument temporaries -- try the full Ego_r CCountedPointer<T> member set
   (ReleaseData, DeleteData static, ShareData, Reset: the inliner's budget counts the callee's own call sites), then
   inline_depth, then /EHsc only for the temporaries. Time-box ~1 h; record the rule or the residue.
6. **CGSI remainder** (77 slot-index rows need per-slot evidence; the 2 GetTextIDFromSymbol rows) and **wave 4 leftovers**
   (PDCB 67 with 7 fixable, BC 21, DCB clusters, 16 SFB hand edits) as budget allows.
7. **CPlayerManager retail branch** (vtable 0x01231CD0, `??1` UAE; 487 local models) if a family needs it: prove slots as
   before (RTTI, vtable, both PDBs, 5 rows) before migrating.

## Rules (unchanged; waves 3-6 LESSONS are binding)
- Def-chain copy members stay out-of-line declarations; the /O2 expanders and CBaseClass.cpp TU rows stay local by design;
  a CCountedPointer<T> model carries the real retail release dtor.
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm; do not touch the 27
  asm_bake rows listed in wave 6 `logs/asm_bake.tsv` (the owner decides those).
- Regrade in batches of ~300-400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig7/work/lv<n>`, delete
  each lv after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert rows that stop matching; log residue.
  Check final files against the last regraded source (payload_check.py). A revert of one family's edit must not drop a
  row's passing edit from another family (wave 6 lost 00ec1ef0 that way): revert per edit, not per file.
- Bindings: one retail address = one name; update `reloc_bindings.tsv` only when a rewrite changes a row's relocation
  symbols (list them in bind_add.tsv); `bindings_multi_check.py` must print 0. NAME_FIXES respellings go to NAME_FIXES.tsv.
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig7/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md, NEXT.md (wave 8 counts and plan), NAME_FIXES.tsv, LESSONS.md, bind_add.tsv, logs/,
scripts/. Final message: headers added/changed, rows per family, recheck totals, reverted rows, bindings changes, the
CLevelScriptBase ctor and MovieBase findings, open questions. Leave no background job running.
