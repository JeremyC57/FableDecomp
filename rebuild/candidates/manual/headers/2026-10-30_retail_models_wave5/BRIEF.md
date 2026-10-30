# Retail class-model migration, wave 5 (one agent; the chunk 584 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig5/repo; cd $S/mig5/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1-4 landed (08436554e, 96aef05db, 6b8099482, 84edb11e4: 613 + 711 + 838 + 1,228 rows). Records in
`rebuild/candidates/manual/headers/2026-10-*_retail_models_wave{1,2,3,4}/` (HEADERS.md, LESSONS.md, scripts/; wave 4's
NEXT.md is the plan you execute). Full rules: `$S/mig1/BRIEF.md` (your worktree is `$S/mig5/repo`, detached HEAD 84edb11e4;
scratch `$S/mig5/work/`; hand-back in `$S/mig5/`). Copy wave 4's `scripts/` into `$S/mig5/scripts/` and extend them.
Regrading runs ~50 rows/min when the lane is quiet; budget ~1,200 edited rows.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS.

## Order of work
1. **New headers first (this wave's main job):** the largest families with NO shared header, flagged by chunks 582-583.
   For each: RTTI chain (`ghidra_out/rtti_bases.tsv`), vtable from the COL at vt[-1] (or the ctor's store when there is no
   RTTI), every slot named with its PDB signature and retail address (catalog, `ghidra_out/labels_*.tsv`, both PDB name
   lists), size and member offsets from retail accesses (Ego_r is a different build), compile check (VC7.1 + g++ c++98),
   then prove on 5 rewritten landed rows before migrating the family. Unproven items stay `// unproven` with the gap.
   - **CBaseObject** (vtable 0x0129A7D8; 652 local models) -- the biggest.
   - **CScriptGameResourceObjectBase** 0x0126008C (199), **CScriptGameResourceObjectScriptedThingBase** 0x0127094C (390),
     **CScriptGameResourceObjectMovieBase** 0x01260EF4 (97): one chain, so one evidence pass.
   - **NScript::CIsThingConscious** 0x012C2FE8 (49) next to the existing CIsThingAlive.h.
   - **A shared `CSpawnedFunc<T>` / `CSpawnedFunc1<T,P>` template** over engine/CSpawnedFuncBase.h (232 + 21 local copies;
     retail has one vtable per instantiation, e.g. 0x012E12F4, 0x012F55CC): the template must reproduce every landed row's
     shape (the __forceinline ctor, the SuspendableProcess override calling `(PClass->*Func)()` / `(Par1)`).
   Then migrate each family by script with mcheck, as earlier waves did.
2. **CGSI remainder** per wave 4 NEXT items 1-2: the 82 compile-failure rows (33 local vector models -> minimal STLport
   vector; 26 int-to-enum; 9 CCountedPointer struct/class clashes; the 11 CScriptThing stand-ins go with the CScriptThing
   family), then the 460 blocks with non-virtual items (survey by cluster), the 35 named-slot-at-other-index rows.
3. **PDCB (67) and BC (21) remainders** per wave 4 NEXT items 3-4 (the fixable ones; by-design locals stay and are listed).
4. If budget remains: start the NScript CScriptBase + CLevelScriptBase family per wave 1 NEXT (a)-(d).

## Rules (unchanged; waves 3-4 LESSONS are binding)
- Def-chain copy members stay out-of-line declarations; the /O2 expanders and the CBaseClass.cpp TU rows stay local by
  design. The CScriptThing copy: out-of-line in the header, a row-local InlineCopy tag at the inline site.
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm.
- Regrade in batches of ~300-400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig5/work/lv<n>`, delete
  each lv after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert rows that stop matching; log residue.
  Check final files against the last regraded source (wave 4's payload_check.py).
- Bindings: one retail address = one name; update `reloc_bindings.tsv` only when a rewrite changes a row's relocation
  symbols; `bindings_multi_check.py` must print 0. Leave 0x0143E8F8 and NAME_FIXES respellings to the coordinator.
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig5/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md (each new header: evidence per slot/member, what is unproven), NEXT.md (wave 6 counts
and plan), NAME_FIXES.tsv, LESSONS.md, logs/, scripts/. Final message: headers added (with slot counts proven), rows per
family, recheck totals, reverted rows, bindings changes, open questions. Leave no background job running.
