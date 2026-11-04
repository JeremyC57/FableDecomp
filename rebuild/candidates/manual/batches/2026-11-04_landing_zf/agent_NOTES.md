# Chunk 770 notes

23 rows: 12 matched (1 MATCH, 11 RELOCATION_MATCH), 3 parked DIFFER, 8 not attempted (identified). The READ-ONLY vland run
(`vland_result.txt`) reports WINS: 12, every test PASS and no rejections.

## Results

| addr | symbol | grade | grades | model |
|---|---|---|---|---|
| 005239c0 | ?RuntimeSaveEntities@CThingManager@@QAEXJABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@_N@Z | RELOCATION_MATCH | 2 | landed sibling RuntimeSaveGameHero 0x00523B40 TU model, verbatim |
| 00543b30 | ??_ECIconText@NUISystem@@W3AEPAXI@Z | RELOCATION_MATCH | 1 | landed CIconText ctor TU 0x00542330, verbatim |
| 00553fd0 | ??_ECTable@NUISystem@@W3AEPAXI@Z | RELOCATION_MATCH | 2 | new engine/NUISystem_CTable.h (a1 PPFAIL: CDefString.h needs the CUIDef macro set) |
| 005bf969 | ?RemoveAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ | RELOCATION_MATCH | 1 | FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED |
| 006cb610 | ?IsPosNavLineOfSightToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0@Z | MATCH | 1 | FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT |
| 0079f960 | ?SetAICasting@CTCWillLightning@@QAEXJ@Z | RELOCATION_MATCH | 1 | FABLE_RETAIL_CTCWILLLIGHTNING_AICASTING (+ CWILLBUTTONHELPERTIMER_SETAICASTING) |
| 007beff0 | ?GetPWeapon@CTCShotDefinitionBase@NThingShot@@QBEPAVCThing@@XZ | RELOCATION_MATCH | 1 | new engine/NThingShot_CTCShotDefinitionBase.h |
| 00ab6a90 | ??1CJoystickDX@@UAE@XZ | RELOCATION_MATCH | 1 | FABLE_RETAIL_CJOYSTICKDX / FABLE_RETAIL_CJOYSTICK |
| 00b39f70 | ??1CEnginePrimitiveAttachmentManager@@UAE@XZ | RELOCATION_MATCH | 1 | FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST (catalog said ~CTCInventoryStats) |
| 00c44e80 | ?Update@CAmbientEmitter@@QAEXXZ (descriptive, libpk) | RELOCATION_MATCH | 1 | local libpk model, as the landed libpk siblings |
| 00c45980 | ?UpdateLevel@CAmbientCube@@QAEXM@Z (descriptive, libpk) | RELOCATION_MATCH | 1 | local libpk model, as the landed GetLevel 0x00C45940 |
| 00ef1690 | ?Main@CWhispie@CPersonalScript_EmptyScript@NScript@@EAEXXZ | RELOCATION_MATCH | 4 | matched at a2; a3/a4 moved onto shared headers (CEntityScriptBase, CIsThingAlive, CPersonalScript_EmptyScript), still RELOCATION_MATCH, no vland warnings |
| 00e0ca50 | ?Main@CUndeadRisingFirefly@CQ_UndeadRisingScript@NScript@@EAEXXZ | PARKED DIFFER(1276v1223) | 11 | best dl=32 |
| 00e16e00 | ?Main@CWB_Guard1@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ | PARKED DIFFER(1120v1064) | 10 | best dl=22 |
| 00e17320 | ?Main@CWB_Guard2@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ | PARKED DIFFER(1120v1064) | 1 | byte twin of 00e16e00 |

Not attempted (identity in STATUS.tsv / LABELS.tsv): 00e11670 CWaspVictim::Main, 00e137b0 CQ_WaspBossScript::Outro,
00e1f7e0 CTraderToEscort::Init, 00e62d60 CV_ChickenKickingScript::CreateSpectators, 00ee8390 CCS_OakValeRevisitedScript::OakVale
(PROPOSAL), 0078c900 CTCBerserk::InitAbility, 00828710 CRegionConnectionGraph::InitialiseGraph, 00785160 (unidentified; bare
fistp ceil/floor sites, so a retail_asm candidate under the C++-first rule).

## Header overlay (hdr/engine/)
All new sub-macros are opt-in. hdrcheck2 gives default=same on every changed header, and every pre-existing macro is same.
- NInventory_CTCInventoryBase.h: FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED. It adds NInventory::CItem and types
  `std::vector<CItem> ConfiscatedItems` plus `virtual void RemoveAllConfiscatedItems()`. The original byte-array line is kept
  verbatim under `#else`.
- CTCCreatureNavigation.h: FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT, nested in FABLE_RETAIL_CTCCREATURENAVIGATION.
- CWillButtonHelperTimer.h: FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETAICASTING, an inline `SetAICasting()`.
- CTCWillLightning.h: FABLE_RETAIL_CTCWILLLIGHTNING_AICASTING. It types `CWillButtonHelperTimer m_tTimer` (+0x2c) and declares
  `SetAICasting(long)` and private `SetLevel(long)`.
- CJoystick.h: FABLE_RETAIL_CJOYSTICK, a virtual dtor in place of `void* __vftable`.
- CJoystickDX.h: FABLE_RETAIL_CJOYSTICKDX, an alternative definition with IDirectInputDevice8W COM slots 0..8 and members
  +0xd18..+0xd40.
- CEnginePrimitiveAttachmentManager.h: FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST. It adds
  CEngineInternalAttachedPrimitiveInfo, a virtual dtor, and `std::list<...> AttachedPrimitives` at +0x10.
- New: NUISystem_CTable.h (sizeof 0x170) and NThingShot_CTCShotDefinitionBase.h (PWeapon +0x30).
- SIBLINGS.tsv lists the 30 landed includers of the changed headers. None defines a new macro, so their preprocessed text is
  unchanged (hdrcheck2 default=same).

## Header debt
- 005239c0 still has SHARED_HEADER_AVAILABLE warnings for CThing, CPlayer, CMainGameComponent and CThingManager. It is a verbatim
  copy of the landed RuntimeSaveGameHero 0x00523B40 TU model, which has a local STLport vector and same-TU noinline saver
  stand-ins. Moving both rows to shared headers together is one job.
- CJoystick, CJoystickDX and CTCWillLightning are flat generated headers with retail sub-macro islands. CIconText reuses the
  landed local ctor model. The libpk rows (CAmbientEmitter, CAmbientCube) use local models: the libpk classes have no PDB types.

## Callee names
- CThingManager::SaveEntitiesBinary 0x00522160 and SaveEntitiesToScript 0x00521EB0.
- CEntityRuntimePersistence::SetGamePersistentSaveData 0x00523860, SetGamePersistentScript 0x005237F0 and
  SetLevelPersistentScript 0x00523790.
- CTCWillLightning::SetLevel 0x0079F810 (private, AAE).
- _List_base<CEngineInternalAttachedPrimitiveInfo>::clear 0x00B3A760.
- CRampedParam::Update 0x00C0B7B0.
- CAmbientEmitter::UpdateVolume 0x00C44DC0 (PROPOSAL) and CAmbientSourceDef::IsFinished 0x00C384E0 (PROPOSAL).
- NScript: CIsThingAlive dtor 0x00E241B0, CEntityScriptBase::SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30.
- GameCalls slots 65..69 (IsPlayerHolding*Button and IsHeroInProjectileWeaponMode), 112 AddScreenMessage, 506
  EntitySheatheWeapons.

## Descriptive names (no PDB symbol)
- CAmbientEmitter::Update 0x00C44E80, CAmbientCube::UpdateLevel 0x00C45980, CAmbientEmitter::UpdateVolume 0x00C44DC0 and
  CAmbientSourceDef::IsFinished 0x00C384E0 (libpk).
- g_bRuntimeSaveBinary 0x0137548C: initialised true and read only by 0x005239C0.

## Parked residues and next ideas
- 00e0ca50 CUndeadRisingFirefly::Main, 11 grades, best dl=32 (a10: break-guarded loop body plus `__forceinline CTimer::operator=`).
  Retail calls ~CScriptThing 0x004AA840 out of line for the loop's CreateCreature temporary and shares one frame slot between the
  two CreateCreature temporaries; ours expands that dtor. Next ideas: a loop or guard shape that lifts operator= above the inline
  threshold without the temporary's dtor, and the Aeon LUAGameflow port for the original statement order.
- 00e16e00 CWB_Guard1::Main, 10 grades, best dl=22 (a1). At entry, retail expands the CIsThingConscious predicate dtor (stores the
  vtable) but calls the member ~CScriptThing out of line, while ~CScriptThing is expanded at other sites; ours expands it at entry
  too. Tried:
  - scoped vs temporary predicate;
  - the header predicate with a virtual dtor (the whole dtor goes out of line, dl=67);
  - a `__forceinline` dtor;
  - a late inline ~CScriptThing;
  - an out-of-line ~CScriptThing (dl=150);
  - an implicit predicate dtor (dl=52).
  Next idea: find the retail mechanism for a nested out-of-line call inside an expanded inline dtor (the INLINE_DEPTH_REVISIT.md
  family; `the inline-depth pragma` is forbidden).
- 00e17320 CWB_Guard2::Main is a byte twin of Guard1 (vtable 0x012E0B7C, "WB_Guard2Pos" 0x012E07A8). The Guard1 fix applies here
  as is.

## Helpers / asm
None used. No row calls an asm helper, so there is no C++-attempts table.

## GOTCHAS (one line each)
- NScript Main: a second CreateCreature `CScriptThing::operator=` is expanded inline when the loop body after the timer wait is
  `if (...) break;`, and stays out of line when it is `if (...) { ... }`.
- NScript predicate: a header predicate with a virtual dtor is called out of line at entry (0x00E241B0 style). An inline
  non-virtual dtor expands its member ~CScriptThing too, so retail's "expanded dtor with an out-of-line member dtor" pattern is
  not reached by either.
- NScript AddScreenMessage: retail builds the CCharString before loading Game. Use a named local in a block,
  `{ CCharString m("..."); Game->AddScreenMessage(m, ...); }`; passing a temporary loads Game first.
- hdrcheck2: splitting an existing declaration line inside a new `#ifdef` gives default=DIFF. Keep the original line verbatim
  under `#else`.
- vland needs `$S/wine/env.sh` sourced (FABLE_ROOT). Without it, it looks for `D:\Documents\FableTLC/rebuild/manifest/functions.tsv`.
- `vland --recheck-landed` only regrades payload rows that are already landed. It does not recheck the landed includers of a
  changed header.
