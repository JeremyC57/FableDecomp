# Naming pass wave 33 (2026-11-03): 840 landed `CVtable_<addr>::`scalar deleting destructor'` rows respelled as the compiler's own ??_G -- entity scripts, script classes, CSpawnedFunc<T>; the CCountedPointer / CIVCountedPointer release family deferred

Inputs: the coordinator's brief (PLAN.md §4 item 2: the mechanical placeholder families on landed rows). One Opus 5.5 agent, overlay generated in
`$S/wave33` (scripts/ here) and **applied to the working tree, uncommitted**: 840 sources rewritten under their new file names, 840 tests moved
(content unchanged), the 840 `build_candidates.ps1` blocks' Source / TestSource / Module updated. Not touched: the catalog, the manifest,
reloc_bindings.tsv, any shared header, any `// Leaf:` line outside these 840 rows, the PRIORITY rows of NAME_FIXES_QUEUE.tsv. The coordinator applies
LABELS (`ghidra_out/labels_naming_wave33_2026-11-03.tsv`, 841 rows = 840 + the callee 0x00C10940) and BINDINGS (5 ADD).

## The premise that did not hold
None of the 1,509 family (a) rows has an RTTI vtable: the 1,272 `CVtable_<addr>` rows sit in no-RTTI NScript / libpk TUs (that is why earlier RTTI
waves left them), and the 237 `ScalarDeletingDtor_<addr>` rows have no vtable or data reference at all (non-virtual QAE ??_G). The vtables were scanned
straight from Fable.exe (scripts/vtscan.py: 2,666 RTTI vtables; scripts/famA.py: refs of every row). The class therefore comes from **the landed,
PDB-named row that stores the vtable last** (scripts/textrefs.py finds every imm32 use in .text, scripts/laststore.py checks it is the final
`mov [reg], vtable` at [this]), and the second source is the PDB name set (FableWin ??_E / ??1 / ??_G, Ego_r where listed) of that same class.
Family (b) bodies name no T at all (D6).

## Families (`runs/<fam>.rows.txt`; generators scripts/gen_<fam>.py; DECISIONS_W33 D1-D4)
- **esb (557)** -- entity-script ??_G (`push esi; call ~CEntityScriptBase 0x00F35B40; delete`): vtable stored last by the landed
  `?Alloc@<Entity>@<Script>@NScript@@SI..` row; source = that Alloc row's TU with the entity's user-declared dtor dropped (D1). 59 of them replace the
  Alloc's row-local CCountedPointer stand-in with engine/CCountedPointer.h (D4).
- **esbm (32)** -- the same with member destructors (CScriptThing, CTimer, CCharString, CScriptGameResourceObjectScriptedThingBase,
  vector<CScriptThing>) as the Alloc models declare them.
- **ctor (90)** -- vtable stored last by a landed PDB-named constructor: 83 `NScript::<Script>` level scripts, CScriptBase, the five
  `CLevelScriptBase<T>` instantiations, CCardPositionsDef (copy-ctor storer). Source = the ctor row's TU + `#define FABLE_RETAIL_CLEVELSCRIPTBASE` (D2).
- **sf (161)** -- `CSpawnedFunc<NScript::<Script>>` ??_G: every landed storer is a method of T whose source writes `new CSpawnedFunc<T>`; MakeOne emitter
  as the wave 29 sfg precedent (D3).

## Results
- Final recheck from the working tree (`lane_tools/recheck.py` + `vland.py --recheck-landed`, 4 parallel splits; logs/recheck_final.log): **840 rows,
  838 RELOCATION_MATCH + 2 RELOCATION_MATCH[/EHsc] (0x00C109B0 and 0x00E54290: their storers' TUs need /EHsc, so this wave added `CompilerFlags = '/O2 /Oy /W3 /EHsc'` to those two ps1 blocks -- the CCardPositionsDef ctor row 0x00C108E0 carries the same -- and regraded them with the flag read from ps1: RELOCATION_MATCH + PASS, logs/recheck_ehsc_flags.log), all PASS**; no RELOC_NAME / leaf / conflict warning. 16
  SHARED_HEADER_AVAILABLE warnings on 10 ctor / esbm rows (CVectorMap, CRuntimeScriptedEntityPersist, CScriptGameResourceObjectScriptedThingBase,
  CCountedPointer, CBaseClassNonCopyable) are inherited verbatim from their storer rows' sources.
- Counts: respelled 840 (esb 557, esbm 32, ctor 90, sf 161; all from the CVtable_ family, none from `_global::ScalarDeletingDtor_`); already correct 0;
  deferred 669 of family (a) + 1,336 of family (b) (DEFERRED.tsv, one row each with the reason).
- Family (a) deferred: 120 CActiveEntityScript<T> (HOLD, DECISIONS_W29_RESOLVED D1), 237 noref (one source only, D7), 199 libpk / GameCODA / Vorbis
  (no RTTI, no PDB, D7), 61 ctor (24 graded DIFFER / RELOC_NAME -- mostly the storer's model lacks a member destructor, ours 30 v retail 40 bytes -- and
  37 with no unique / complete PDB-named ctor storer or the storer misnamed, D5), 9 esb + 6 esbm (no unique Alloc storer), 4 sf (no landed storer),
  33 not reached (no unique named storer).
- RENAMES.tsv 840 (old CLandscapeBackgroundPatch_* / CEngineSubPrimitiveText_* stand-in files -> `<Class>_scalardeletingdestructor_<addr>.cpp`).
- BINDINGS.tsv: ADD 5 / RETIRE 0 / KEEP 0 / CONFLICT 0 (the old free __fastcall stand-ins were never in reloc_bindings.tsv); 
  `bindings_multi_check.py` on the after-table (logs/bindings_multi_check.log): 0. The 5 ADDs bind the out-of-line dtors the new ??_Gs call
  (CQ_AmbushTradersScript 0x00CFA340, CV_AssassinAttacksScript 0x00E398A0, CV_BeardyBaldyScript 0x00E542B0, CV_FishingCompetitionScript 0x00E81F30 --
  all catalogued so -- and ??1CCardPositionsDef 0x00C10940, whose catalog name is a placeholder and whose existing binder is @FableBaseDtor_c10a10@4:
  NAME_FIXES.tsv).
- DECISIONS_W33.tsv: D1-D7, N1 (a concurrent writer in the same working tree: commit the waves separately).

## Regenerate (from the repo root, S exported, $S/wine/env.sh sourced; W=$S/wave33)
`vtscan.py $W/vtslots.tsv`; `famA.py $W/vtslots.tsv $W/famA.tsv`; `ctx.py $W/famA.tsv $W/famA_ctx.tsv`; `textrefs.py $W/famA_ctx.tsv $W/famA_tr.tsv`
(= runs/famA_storers.tsv); `gen_esb.py $W/esb` + `fix_cp.py $W/esb <59 rows>`; `ESB_MEMBERS=1 gen_esb.py $W/esbm`; `scan_ctor.py`; `gen_ctor.py $W/ctorB A`;
`gen_sf.py $W/sf`; `recheck_fam.sh <famdir> N`; `passlist.py`; `apply.py <pass.tsv>`; `mkbind.py`; `final_recheck.sh`; `mkdeferred.py`.
