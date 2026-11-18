# 2026-11-17 landing nf83: name-fix round 7, NF83 (43 rows: the nf79 and nf81 follow-ups, q11010..q11043)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF83 took the 32 entries queued at the nf79 and nf81 landings (q11010..q11043 without q11015 / q11016, which wait for a coordinator
ruling). Brief, list and records: `naming/2026-11-17_agentNF83/`.

## Rows (STATUS.tsv, 88): 43 ported, 13 not-a-binder, 32 blocked
- **Ported, among them:** the STLport deque helpers of `deque<NUISystem::CComponent *>` (0x0059AC41 `_M_decrement`, 0x0059A8F3 `_M_subtract`
  (was catalogued `NThingMap::CThingMapManager::GetMemoryFootprint`), 0x0059AD35 `_M_pop_back_aux`, 0x0059B61C `push_back` (was catalogued
  `CCreatureAction_FireMissileWeapon::FrameUpdate`), 0x0059AF53 `operator--`); the EDamageAttribute tree destructor 0x00451AC6 and the map `clear`
  rows 0x00869900 / 0x006DC970; the pair scalar deleting destructor 0x004E3625; NCreatureMode::CModeSitBase FrameUpdate / RemoveModeWithAction;
  the Bandit campfire Idle / Eat / Drink; CThingCreatureBase::IsFreeToPerformAction; CEngineSubPrimitiveBase `operator delete` 0x00BEB550 (was
  `_global::Nop_ret0_00beb550`).
- **Blocked (32; STATUS gives each reason):** q11021 / q11022 (CIntelligentPointer's T* constructor with T incomplete, plus 7 STL helper rows),
  q11036 (five `??_G` binders of 0x00BEB550), q11041..q11043 (NF79's blockers stand), q11017 (0x009FE380 DIFFER 181 v 181, one register swap),
  q11014 (0x0059A238's port onto CFrontEndManager.h). Several were blocked after a probe only (STATUS says which).

## Entries (APPLIED.tsv, 32): 14 done, 1 done-already (q11032, a Ghidra label fix only), 17 blocked

## Rows and checks
- **Section 1: 24 self renames (34 file moves).** Coordinator corrections: 0x00869900's module takes its siblings' spelling
  (`map<EDamageAttribute,float>`); 0x0059A238 keeps the module `NUISystem::CFrontEndManager`; 0x00BEB550 is spelled `operator_delete` like
  its sibling 0x00BEB540 `operator_new`.
- **0x0059A238: an interim leaf.** engine/CTBaseSingleton.h now includes engine/CCountedPointer.h (the q11038 respell), so the row's
  CCountedPointer parameter is no longer NUISystem-scoped. Its leaf is closer to the PDB but still not the PDB's `UAE ... AAV`; its catalog
  name is unchanged and q11014 stays open.
- **One-source name:** 0x00860130 `CBrainOutput::SetAsPerformAction` rests on Ego_d.pdb alone (the old name was a free-function stand-in);
  queued for a second source.
- **Name collision (queued for a ruling):** 0x00451AC6 now defines the EDamageAttribute `~_Rb_tree`, which the landed 0x006DC020 (NF81, named
  descriptively, no caller) also defines. One of them needs another name; 0x00451AC6 has the callers.
- **Headers (opt-ins in 6 files):** engine/CTCNoiseDef.h (`FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE`, 1 assert pinned), engine/CDefClassBase.h,
  engine/CThingCreatureBase.h (3), engine/NCreatureMode_CModeSitFloor.h (2; 3 asserts pinned), stlport/fable_deque.h (2), engine/CPlayer.h. NF83's
  base copies were identical to the repo; `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65. Details:
  `naming/2026-11-17_agentNF83/hdr/README.md`.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 43 rows graded from the repo after landing: 39 RELOCATION_MATCH and 4 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 149 binding lines and added 116; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 12)
NF83's NAME_FIXES.tsv (landed rows outside its slice that still declare stand-in forms of 0x00662210, 0x00668D90, 0x0059A8F3, 0x0059B61C,
0x0085FF10, 0x00BEB550, 0x01436E9C, 0x008D6C80 and 0x0059B576; found by reading the source, not graded), plus the 0x006DC020 collision and
the second source for 0x00860130. A second, unchanged copy of the 0x0059A238 row (`CFrontEndManager_Action_0059a238_match.cpp`) is noted in
NOTES.

## Rule slips
- Two scratch files were written under /tmp outside NF83's directory (both removed).
- One grade on 0x00869900 was spent on a misspelt leaf; 0x009FE380 got 4 failed forms, one past the limit.
- Several rows were blocked after a probe only; some entries were handled out of order.
