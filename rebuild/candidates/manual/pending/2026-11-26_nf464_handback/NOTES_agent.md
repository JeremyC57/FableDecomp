# NF464 (2026-11-26): GetPreferredPos unit (bQ + T3 nf287), WatchFor* callbacks, EnumerateScene binder

Scratch: `$S/w22/agentNF464`. Tools: NF463's (`tools/`, paths and lock owner moved to NF464) plus `objproof_keyed.py` (objproof that pairs
sections by their defining external symbol, so an added COMDAT is named instead of shifting the zip; it gives ALL_EQ on bQ like
objproof_map) and `wfedit.py` (the WatchFor* parameter respell). Header overlay: `hdr/engine/CEngineScene.h` (bE only; bE carries its own copy).
HEAD 12e00c9de9 at the start, eec399a1ff at the end (read from .git/refs; all gates ran at 12e00c9de9; `find rebuild/include rebuild/oracles -newer` the first vland log is empty, so the move touched none of their inputs); every orig/ file (sources, tests, hdr_CEngineScene.h) still equals the
working tree at the end. No git; nothing written under the repo; repo oracle md5s identical before and after every sandbox run (evidence/sandbox_all.txt).

## Batches (NF463 layout: rows/, orig/ = exact HEAD copies incl. tests and hdr_*.h, hdr/engine/, head/payload.json, RESULTS, SELF_SYMBOLS,
## UNITS, CHECKS, TABLE_CHANGES, HEADERS, renames.tsv, evidence/)
| batch | rows | verdict | what |
|---|---:|---|---|
| bQ_getpreferredpos_unit | 1 | ACCEPT | queue 13787. NF463's bQ (0x006EA770 callee ComputeAttackVector -> GetAttackerPlacementVector), re-gated at HEAD 12e00c9de9. The T3_PORT_QUEUE line it was held on (nf287 0x006ea770: the `GetPreferredPos@CCombatPositionMethods` stand-in in binder 0x0093DCD0) has no stand-in left and its binding is already PDB (0x0093DCD0 via NF443's callee-only rebind, 2026-11-25: a local CCombatManager declaration + reinterpret_cast, not engine/CCombatManager.h; RULINGS r464d): the 8 retail caller rows of 0x006EA770 (0091e050 0093d180 0093dcd0 0093e090 0095bcf0 0096e700 0096e830 0096ec10) all bind the PDB `?GetPreferredPos@CCombatManager@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z` in their VC7.1 HEAD objects (evidence/t3_nf287_census.tsv). So the unit is bQ plus a T3 release on r464d (T3_RESOLVED.tsv: `--release-t3 nf287` for 0x006ea770 only). TABLE 1 replace. |
| bW_watchfor | 5 | ACCEPT x5 (informational r464c) | queue 13807-13809. The binders 0x00D16DC0, 0x00E38F80, 0x00F083A0, 0x00F08A30, 0x00F08C30 change only the callback's by-value parameter type CScriptThingRuntime -> CScriptThing (engine/CScriptThing.h flat view, class-keyed V; already included by 3 of them, added to 0x00E38F80 / 0x00F083A0) in the row-local script class and the member-pointer typedef. No CSpawnedFunc1 port is needed: the functor stays the row-local model, and its stand-in ctor symbol (??0CSpawnedFuncBounty / Phase3 / EnemyDeath / Ambush) renames with its member-pointer parameter (renames.tsv). 0x00D16DC0 also respells WatchForDecapitation 0x00D16FD0 (same typedef; Ego_r + FableWin). TABLE 3 replace + 1 delete (WatchForEnemyDeath: the PDB spelling is already bound, witness 0x00F09420). |
| bE_enumeratescene | 1 | HOLD r464a | queue 13800, binder 0x00B321B0 only. engine/CEngineScene.h gains the add-only opt-in `FABLE_RETAIL_CENGINESCENE_ENUMERATESCENE` (declaration of `EnumerateScene(class CEngineSceneEnumerator&)`, public; g++ -E -P HEAD v new IDENTICAL under 3 macro sets; the only assert is `_AT(26)`). The row drops its local CEngineScene / CBaseClassNonCopyable models for the shared headers and derives the stats enumerator from a local `class CEngineSceneEnumerator : public CBaseClassNonCopyable {}` (no header; r462a). All gates pass except one added pick-any COMDAT `??1CEngineSceneEnumerator@@UAE@XZ` (inline implicit dtor of the real base; 0x00BACA20 emits the same COMDAT at HEAD; no retail address). TABLE 1 replace (apply only with the batch). |

## Gates
- vland --recheck-landed HEAD v new at one LANDVERIFY_WORK path (runpair.sh; runpair_hdr.sh with the overlay for bE): grade unchanged
  (RELOCATION_MATCH x7), tests PASS both sides. Tests unchanged (self-contained g++ models; no test names the respelt symbols).
- WARN: bQ 11 -> 10 (RELOC_NAME ComputeAttackVector gone); bW identical per row (3/4/5/5/5); bE 7 -> 5 lines (SHARED_HEADER_AVAILABLE
  CBaseClassNonCopyable and CEngineScene gone; the leaf / STANDIN_LEAF `CollectPrimitiveStatsForArea` pair is HEAD state and unchanged:
  the catalog name differs from the source's `CollectPrimitiveStats`; not respelt here).
- objproof on vland's own objects: bQ ALL_EQ (objproof_map and objproof_keyed); bW ALL_EQ x5 (objproof_map and keyed, under renames.tsv); bE EQ_EXCEPT
  +COMDAT ??1CEngineSceneEnumerator (6 sections paired equal). .drectve byte-identical everywhere (no new /DEFAULTLIB).
- link HEAD v new per unit (lk2.py with HEAD objects of the callee / sibling rows):
  - bQ: dup 0 -> 0, unresolved 11 -> 10.
  - bW (5 rows + selves 00d17090 00e39140 00f08590 + PDB binders 00f08e90 00f09420 00f09530): dup 1 -> 1, the HEAD duplicate stand-in
    ??0CSpawnedFuncAmbush (0x00F08A30 + 0x00F08C30), renamed together; with 0x00F09640 (which still defines the old ??0CSpawnedFuncAmbush) 2 -> 1; unresolved 403 -> 400. The one "new" unresolved is
    WatchForDecapitation under its PDB spelling: 0x00D16FD0 has no catalog row, so it is unresolved under either spelling (same address;
    strictly a set change, stated in the row's RESULTS reason and r464c).
  - bE: dup 1 -> 1 (HEAD LNK4006 ??0CEngineSceneEnumerator, 0x00BACA20 v 0x00B31D80), unresolved 7 -> 6.
- sandbox (all three batches cumulatively, fresh repo copies): apply_table --dry 0 misses, applied, multi-target 1 -> 1; clash 0;
  descriptive_check 2,030 lines 0 FAIL.
- No __asm / volatile. No new duplicate definitions (grep: no other source defines the respelt names; the WatchFor* selves define them).
- Names: all Ego_r + FableWin (GetAttackerPlacementVector, GetPreferredPos, WatchForLeaderKilled / Decapitation / AssassinDead / EnemyDeath,
  EnumerateScene); CEngineSceneEnumerator = RTTI .?AVCEngineSceneEnumerator@@ + FableWin ??0 / ??1 / ??_E. No BEST_GUESS names.

## Findings
- **0x00B320A0 (r464b):** the inline-built ResetVisibilityRatio enumerator on the real chain keeps retail bytes (the base vptr store is dead-stored)
  but adds ??0 / ??_7 / ??_G / ??1 CEngineSceneEnumerator COMDATs; ??0 sits beside the strong landed 0x00B31D80 (new duplicate). Probe only
  (bE/evidence/probe_00b320a0*). 0x00B36C20 #includes this source, so it belongs to the unit.
- **NAME_FIXES.tsv:** 0x00BACA20's PDB ctor spelling (map<unsigned long,unsigned long>&, EEnginePrimitiveStatIndex) is what the self defines;
  its only caller 0x00B321B0 binds the stand-in `PAUCPrimitiveStatMap ... JK` spelling. Needs the STLport map type in 0x00B321B0 (watch .drectve).
- **Same pattern as bW, not queued:** reloc_bindings still holds CScriptThingRuntime callback spellings for WatchForHostage1Killed / Hostage2Killed
  (0x00D16920 / 0x00D16D00), WatchForBanditDead 0x00E1F520, WatchForContactDeath / HeroAttackOnContact (0x00F09830 / 0x00F09950),
  WatchForAttack 0x00E9DC80, WatchForDeputy2Killed 0x00D16AF0 (also no NScript), the CQ_AcolyteParentScript pair; `wfedit.py` does each in minutes.

## Resume (smallest first)
1. Rulings r464d (T3 release for bQ), r464c (bW), r464a (lands bE as built) and r464b (0x00B320A0 + 0x00B36C20), then close queue line 13800.
2. The bW-pattern lines above (one binder each; check each self row's spelling first).
3. NF460 list items unchanged: 0x0099A2D0 family (3 rows), 0x0076C830 for 0x00686870, 0x00686830 (8 rows), 0x004ABE90, the self-change lines
   (0x009FC210, 0x0064CC65, 0x0097DA40, 0x009D6490), then the large families; NF463's r463a / r463b first for 0x00861E00.

## Locks (NF464)
Changed rows (7): 006ea770 (taken over from COORD_HOLD_NF463), 00d16dc0 00e38f80 00f083a0 00f08a30 00f08c30, 00b321b0.
Held unchanged (unit members / selves): 00d17090 00e39140 00f08590 00b93290 00b320a0 00b36c20. Not taken: 006e89b0 (NF463, unchanged callee),
0093dcd0 and the other 7 GetPreferredPos callers (read only).
