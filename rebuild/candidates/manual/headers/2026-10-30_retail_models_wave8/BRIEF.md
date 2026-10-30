# Retail class-model migration, wave 8 (one agent; an asm_bake re-author agent / chunk agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig8/repo; cd $S/mig8/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1-7 landed (613 + 711 + 838 + 1,228 + 1,103 + 1,481 + 1,667 rows). Records in `rebuild/candidates/manual/headers/
2026-10-*_retail_models_wave{1..7}/`; wave 7's NEXT.md is the plan you execute and waves 3-7 LESSONS are binding (wave 7
LESSONS 3-5, 7-8: slot-by-index checks, the inline implicit CScriptThing trio, the abstract entity classes, per-family
reverts from a pre-image, mstat_fam.py). Full rules: `$S/mig1/BRIEF.md` (your worktree is `$S/mig8/repo`, detached HEAD c8e04967c, the wave 7 landing; scratch `$S/mig8/work/`; hand-back in `$S/mig8/`). Copy wave 7's `scripts/` into
`$S/mig8/scripts/` and extend them. Regrading runs ~50 rows/min when the lane is quiet, ~17 with another agent on it;
budget ~1,500 edited rows.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS.

## Order of work (wave 7 NEXT items)
1. **ESB second pass (~100 rows):** mig_esb.py learns the two skipped shapes: a block inside an `EntityScriptBinding_<addr>`
   namespace (move the include above the namespace; the 34 CreateActiveEntityScript rows rename the invented `Start` slot
   to `Init`), and derived classes that model the base's vptr / Game / Me words themselves (drop those words with the base).
   Then ESB follows every CScriptThing batch (354 rows wait on their local CScriptThing).
2. **CScriptThing shapes (1,045 left), largest first:** the PImp stand-ins (~175 + ~95: header PImp is
   `CCountedPointer<CScriptThing>`; rows reading `PImp.Data->X` through a stand-in pointee need a cast or a CScriptThingImp
   model with evidence), the inline wrappers (~140: define the real slot bodies after the include where retail expands them;
   the invented non-virtual GetThing helper gets its retail name), the `class CScriptThing : public CBase` rows (89:
   respell CBase to engine/CBaseClass.h, then mig_st), named-slot-at-other-index (81: check each signature), `dtor not
   slot 0` (46), flattened `__vftable` (20), the 9 std-helper rows (`--implicit`). The 120 CActiveEntityScript<T>::
   SuspendableProcess rows need a CActiveEntityScript<T> model on CSpawnedFuncBase.h: prove it (RTTI, vtable, both PDBs,
   5 rows) before migrating.
3. **Master-data T respellings (byte-neutral):** 00eee800 / 00eeec00 -> CEmptyData; 00efc300 / 00efc260 ->
   CDanyalTestScriptsData; 00d80ba0 -> CQ_SunnyvaleMasterData; 00eea020 stays (registered with no master data: say so in
   the row comment). Evidence wave 7 HEADERS.md, scripts/regscripts.py + regmap.py.
4. **CIsThingConscious / CIsThingAlive / CPredicateBase (40 / 235 / 219):** extend mig_itc.py to the inline ctor/dtor forms
   (define after the include) and the filler-slot CPredicateBase forms; declared out-of-line copy ctors that retail has no
   copy of become the header's implicit one.
5. **The 5 out-of-line CLevelScriptBase<T> ctor rows 0x00CDD160..0x00CDD220:** try the header plus an explicit
   instantiation `template CLevelScriptBase<T>::CLevelScriptBase(CScriptDataBase*, CGameScriptInterfaceBase&);` (the ctor
   is inline now); record the result either way.
6. **CGSI remainder (220; 77 slot-index rows need per-slot overload evidence), wave 4 leftovers, CTimer (201),
   CPlayerManager retail branch (487; vtable 0x01231CD0, prove slots as before)** as budget allows.
7. **MovieBase (85), time-box ~1 h:** wave 7 LESSONS 6 lists what is excluded. Compare the ctor sides (retail inlines the
   MovieBase ctor, vtable 0x01260EF4, PImp zeroed) and look for a MovieBase-specific member around Ego_r's out-of-line
   `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` 0x007E2EA0 that wraps the release call. Record the rule or the residue.

## Rules (unchanged)
- Def-chain copy members stay out-of-line declarations; the /O2 expanders and CBaseClass.cpp TU rows stay local by design;
  a CCountedPointer<T> model carries the real retail release dtor.
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm; do not touch the
  asm_bake rows listed in wave 6 `logs/asm_bake.tsv` (another agent is re-authoring them).
- Regrade in batches of ~300-400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig8/work/lv<n>`, delete
  each lv after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert per family edit from its pre-image.
  Check final files against the last regraded source (payload_check.py); per-row status via mstat_fam.py.
- Bindings: one retail address = one name; update `reloc_bindings.tsv` only when a rewrite changes a row's relocation
  symbols (list them in bind_add.tsv); `bindings_multi_check.py` must print 0. NAME_FIXES respellings go to NAME_FIXES.tsv.
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig8/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md, NEXT.md (wave 9 counts and plan), NAME_FIXES.tsv, LESSONS.md, bind_add.tsv, logs/,
scripts/. Final message: headers added/changed, rows per family, recheck totals, reverted rows, bindings changes, the
CActiveEntityScript<T> and MovieBase findings, open questions. Leave no background job running.
