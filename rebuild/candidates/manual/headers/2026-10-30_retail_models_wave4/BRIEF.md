# Retail class-model migration, wave 4 (one agent; the chunk 583 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig4/repo; cd $S/mig4/repo` (env.sh resets FABLE_ROOT: export it after).

Waves 1-3 landed (08436554e, 96aef05db, 6b8099482: 613 + 711 + 838 rows). Records: `rebuild/candidates/manual/headers/
2026-10-29_retail_models_wave1/`, `2026-10-30_retail_models_wave2/`, `2026-10-30_retail_models_wave3/` (HEADERS.md,
LESSONS.md, scripts/; wave 3's NEXT.md is the plan you execute). Full rules: `$S/mig1/BRIEF.md` (your worktree is
`$S/mig4/repo`, detached HEAD 6b8099482; scratch `$S/mig4/work/`; hand-back in `$S/mig4/`). Copy wave 3's `scripts/` into
`$S/mig4/scripts/` and extend them. Regrading costs about 6 rows/min on the shared lane; budget ~800 edited rows.

## Goal
Owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS.

## Order of work
1. **CGSI interface-only:** `mig_cgsi.py --rename` (693 dry-run ok; slot-7 Update/YieldFrame/Pump -> NewScriptFrame; named
   slots the row never calls are fillers). Check every named slot the row DOES call against the header spelling before
   dropping the local block (const-ness and overloads change the call only if a spelling differs).
2. **BC remainder:** the 64 whitespace-only dtor rows, `mig_bc_copy.py` (169), the 146 inline-ctor rows via `bc_ctor_rule`
   (vtable store overwritten by the derived store = implicit copy `: CBaseClass(other)`; a call = the out-of-line ctor
   0x0099A2D0), then the ~233 v00/v000 stand-in shapes (extend the regex).
3. **PDCB remainder (108):** start with wave 2's 12 reverts (drop derived `_body` pads over the base; the two `typedef long
   CDefString` rows; `add_def_overrides.py` on abstract-derived SRC_FAILs); the 70 typed-member rows via
   `FABLE_PDCB_TYPED_MEMBERS` with the row's own CVectorMap.
4. **DCB clusters** as budget allows: 83101c63 (18), 46f7e374 (15), 662692ff (14), 012a455e (24) / fe4a0baa (7),
   Destroy/RefCount stand-ins, the 35 local-CResource rows, the 16 SFB hand edits.
5. **New headers, if any row family needs them and the evidence is there** (chunk 582 flagged these retail-vtable classes
   still modelled locally): CBaseObject 0x0129A7D8, CScriptGameResourceObjectBase 0x0126008C,
   CScriptGameResourceObjectScriptedThingBase 0x0127094C, CScriptGameResourceObjectMovieBase 0x01260EF4, NScript::
   CIsThingConscious 0x012C2FE8, and a shared CSpawnedFunc<T> / CSpawnedFunc1<T,P> template (every spawning row defines it
   locally). Prove chain, size, every slot (name + PDB signature + retail address) and 5 rows each before migrating.

## Rules (unchanged; wave 3 LESSONS item 1 is now binding)
- Def-chain copy members (CDefClassBase / CParentDefClassBase operator=, copy ctor) stay out-of-line declarations; the five
  /O2 expanders keep their row-local __forceinline chain. The CScriptThing copy: out-of-line in the header, a row-local
  InlineCopy tag at the inline site.
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm.
- Regrade in batches of ~400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig4/work/lv<n>`, delete each
  lv after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert rows that stop matching; log residue.
- Bindings: one retail address = one name; update `reloc_bindings.tsv` only when a rewrite changes a row's relocation
  symbols; `bindings_multi_check.py` must print 0. Leave 0x0143E8F8 and NAME_FIXES respellings to the coordinator.
- Header comments cite retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig4/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md, NEXT.md (wave 5 counts and plan), NAME_FIXES.tsv, LESSONS.md, logs/, scripts/. Final
message: headers added/changed, rows per family, recheck totals, reverted rows, bindings changes, open questions. Leave no
background job running.
