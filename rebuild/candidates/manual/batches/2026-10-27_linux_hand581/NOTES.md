# Chunk 581 notes (agent581)

8 of 10 rows match (all RELOCATION_MATCH, no retail_asm); 2 are parked in differ/. Tests pass under g++ and VC7.1.

## Rows, names and evidence

| Row | Status | Name (Leaf) | Evidence |
|---|---|---|---|
| 0x00EFF130 | RELOCATION_MATCH 144 | `?Main@CTonyTestScriptsScript@NScript@@UAEXXZ` | Slot 2 of quest vtable 0x012F6F90, stored by the landed ctor 0x00EFF060; referenced only from 0x012F6F98. Ego_r name set. The "Dummy" binding uses the landed CDummy::Alloc 0x00EFF3F0. |
| 0x00F34660 | RELOCATION_MATCH 455 | `?_M_fill_insert@?$vector@VCFeatDef@@...` | /100 stride; landed CFeatDef copy 0x00F2D270 and op= 0x00F2D200; landed overflow 0x00F34860; only caller is resize 0x00F34580. |
| 0x00F26570 | RELOCATION_MATCH 456 | `?_M_fill_insert@?$vector@VCArenaCreatureDef@@...` | /56 stride; landed CArenaCreatureDef copy 0x00F25DD0 and op= 0x00F25F10; only caller is 0x00F264ED, inside the bound `vector<CArenaCreatureDef>::resize` 0x00F26490 (xref.py). |
| 0x00F281E0 | DIFFER 558v481 (parked) | `?_M_fill_insert@?$vector@VCArenaWaveDef@@...` | /60 stride; implicit copy 0x00F25C60 (vtable 0x012FB2F0); op= 0x00F25F60; overflow 0x00F28400. |
| 0x00F277F0 | DIFFER 601v493 (parked) | `?_M_fill_insert@?$vector@VCHeroSoulsCreatureDef@@...` | /52 stride; copy 0x00F27060, op= 0x00F27190, fill 0x00F279E0, overflow 0x00F27A10. |
| 0x00E99260 | RELOCATION_MATCH 519 | `?WatchForLeaving@CV_HeroDuelScript@NScript@@QAEXXZ` | CSpawnedFunc Func store 0x00E98DE1 in Main 0x00E987C0, right after the "WatchForLeaving" literal; Ego_r name set. |
| 0x00E1F0C0 | RELOCATION_MATCH 525 | `?DoMission@CQR_EscortTraderScript@NScript@@QAEXXZ` | Spawned as "DoMission" (Func store 0x00E1E399) by the landed Main 0x00E1E110. Ego_r name set. Already bound so in reloc_bindings.tsv. |
| 0x00E268C0 | RELOCATION_MATCH 534 | `?Main@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UAEXXZ` | Slot 2 of quest vtable 0x012E1F08 (ctor 0x00E267E0); referenced only from 0x012E1F10. Ego_r / FableWin name sets. |
| 0x00F33EF0 | RELOCATION_MATCH 538 | `?_M_fill_insert@?$vector@VCFireHeartPatternDef@@...` | /52 stride; the landed overflow 0x00F34160 names this row as its only caller (site 0x00F340FD); `__copy_backward_ptrs` 0x00F34110 names call site 0x00F33FCD; caller is resize 0x00F33DE0. |
| 0x00EC1CD0 | RELOCATION_MATCH 540 | `?Init@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ` | Slot 2 of entity vtable 0x012F1390, stored by the landed Alloc 0x00EC1B70 (slot 1 Main 0x00EC1EF0, slot 5 OnPredicateFail 0x00EC1C20). The Ego_r name-set order (Alloc, OnPredicateFail, Init, Main) matches retail address order. |

Member and global names come from Ego_r.pdb, at Ego_r offset - 0x14 for script classes:
- TraderCreated +0x5E.
- CRandomPopulationCreature: Exit +0x1C, MyType +0x28, AttachedToVillage +0x2C.
- CV_RandomPopulationSimScript: CurrentPopulationScale +0x48, CurrentPopulationRegion +0x5C, CreaturesAlive +0x60.
- CScriptDef: RandomPopulationTraderGoldMinimum +0x724 and RandomPopulationTraderGoldVariation +0x728. These sit at Ego_r - 0x5C, the same delta as the landed AssassinThreatDistSq +0x720 / Ego_r +0x77C, the field just before them in Ego_r.
- The gold fields are read as `rand() % Variation + Minimum`.

Literal pairings:
- 0x00EC1CD0: "RandomPopulationVillager" 0x012F1374 gives type 0 plus gold; "RandomPopulationGuard" 0x012F1354 gives 1; "RandomPopulationBandit" 0x012F133C gives 2.
- 0x00E1F0C0: "LookoutPoint" goes to IsRegionLoaded (slot 12); "M_ET_TraderSpawnPoint" to GetThingWithScriptName; "CREATURE_TRADER_01" and "TraderToEscort" to CreateCreature (slot 91).

Possible error in a landed comment: the OnPredicateFail 0x00EC1C20 line 1 calls type 2 "a guard". This row's Init proves 1 = Guard and 2 = Bandit. The coordinator may want to fix that comment.

## Catalog label corrections
- 0x00F26570: `vector_insert_impl_CParticleMorphs` should be vector<CArenaCreatureDef>::_M_fill_insert.
- 0x00F281E0: `vector_insert_impl_CParentDefClassBase` should be vector<CArenaWaveDef>::_M_fill_insert.
- 0x00F277F0: `vector_insert_impl_pair_CSymbolInfo` should be vector<CHeroSoulsCreatureDef>::_M_fill_insert.
- 0x00F33EF0: `vector_insert_impl_ThingBuildingDef_map` should be vector<CFireHeartPatternDef>::_M_fill_insert.
- 0x00F34660: `vector_insert_impl_CFeatDef` was behavioural only.
- The four `sub_` script rows (0x00EFF130, 0x00E99260, 0x00E1F0C0, 0x00E268C0) and `sub_00ec1cd0` get the names above.
- The landed 0x00EC1EF0, 0x00EFF1C0 and 0x00E1F3A0 spellings are wrong; see NAME_FIXES.tsv.

## Shared headers vs local models
- **Shared retail headers (migration wave 1, FABLE_RETAIL_NSCRIPT):** every script row (0x00EFF130, 0x00E268C0, 0x00E99260, 0x00E1F0C0, 0x00EC1CD0) uses them: CScriptThing, CLevelScriptBase, CEntityScriptBinding, CEntityScriptBase, CGameScriptInterfaceBase, plus CSpawnedFuncBase through FABLE_RETAIL_SUSPENDABLE_PROCESS.
  - 0x00EFF130 was first matched on a local model, then converted to the shared headers; it still matches. The old version is in probe/00eff130_localmodel.cpp.
  - No row still uses a local NScript base model.
- **Def rows:** these use FABLE_RETAIL_DEF_CLASS_BASE + CDefClassBase.h with the wave-1 pack(1) 0x25 branch.
  - CFeatDef's members are opaque as `unsigned long _members[0x0f]`, starting at +0x28.
  - 0x00F33EF0 matches only with the wave-1 header's declared CDefClassBase::operator= 0x00431F10.

## Header debt (local stand-ins; review at landing)
- **CScriptDef (0x00EC1CD0).** This is a def class with a retail vtable. It is modelled as an opaque `class CScriptDef { unsigned char _before[0x724]; long ...Minimum; long ...Variation; }`.
  - `pGlobalScriptDef` is declared `CScriptDef*` (bound spelling `?pGlobalScriptDef@@3PAVCScriptDef@@A`). In Ego_r.pdb it is really `CDefPointer<const CScriptDef>` (dynamic initializer + atexit dtor).
  - A CDefPointer model with operator-> / operator T* changed the register allocation (537v540), so the plain-pointer precedent stays (as the landed 0x00E1FEB0 does).
- **CQ_SunnyvaleMasterData (0x00E99260).** This class has a vptr at +0 (Ego_r). It is modelled opaque, `unsigned char _before[0x62]; bool LostToThunder;`. It needs a shared header.
- **CSpawnedFunc<T> (0x00E1F0C0).** This template has a retail vtable (0x012E12F4 for this instantiation). It is the local template every landed spawning row uses (TalentlessBard 0x00ED7830). There is no shared header yet.
- **CCharString / CCountedPointer / CBasicString<char> / CCharStringData / CCPPointerInfo.** These are local in each row, per the CScriptThing.h contract (the includer defines CCountedPointer).
- **Inline CScriptThing bodies, defined out of class after the include (as the header comment allows):**
  - 0x00E1F0C0: `__forceinline` GetPos. A plain `inline` is not expanded.
  - 0x00EC1CD0: `__forceinline` ~CScriptThing and operator=.
- **Script classes.** CV_RandomPopulationSimScript, CRandomPopulationCreature, CQR_EscortTraderScript, CV_HeroDuelScript and the others are each row's own class. They are declared in place with members to the last one used.
