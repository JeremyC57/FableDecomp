# Chunk 525 -- notes (agent525, 2026-10-28)

19 of 20 rows match under VC7.1 RTM 13.10.3077 (18 RELOCATION_MATCH + 1 MATCH). 0x0084E420 is parked as DIFFER(357v357) after
4 grades; the residue is the last 8 instructions. No retail_asm rows and no SKIP rows. Tests pass under g++ and VC7.1/Wine.
The vland dry run gives 19 wins, with no RELOC_CONFLICT or RELOC_DUPLICATE. It warns RELOC_NAME twice, and NAME_FIXES.tsv covers
both (0x006925C0, 0x00692910).

Sources are built from `parts/` by `mkall.sh`:
- shared models: `common.inc`, `defpointer.inc`, `tcbase.inc`, `carrying.inc`, `charstring.inc`, `combat.inc`, `unsheathe.inc`;
- each row's own part: `parts/<addr>.h` holds the comment header, `parts/<addr>.cpp` the classes and body.

`src/008ab5d0.cpp` stands alone. Earlier records were only chunk_history.tsv (chunk 189), with no sources.

## Proven (two sources each)

The row functions:

- 0x00847470 `?PerformAction@CCreatureAction_PerformExpressionExtended@@UAEXXZ`: RTTI vtable slot + Ego_r/FableWin.
- 0x00847990 `?PerformAction@CCreatureAction_LearnExpression@@UAEXXZ`: RTTI vtable slot + Ego_r/FableWin.
- 0x00847BE0 `?PerformAction@CCreatureAction_Drink@@UAEXXZ`: RTTI vtable slot + Ego_r/FableWin.
- 0x00848850 `?PerformAction@CCreatureAction_PutGoodsDown@@UAEXXZ`: RTTI vtable slot + Ego_r/FableWin.
- 0x00848F10 `?PerformAction@CCreatureAction_GetOutOfBed@@UAEXXZ`: RTTI vtable 0x01276B0C slot 3 + Ego_r/FableWin.
- 0x00849D90 `?PerformAction@CCreatureAction_GetItemOutOfPocket@@UAEXXZ`: RTTI vtable slot + Ego_r/FableWin.
- 0x00849E80 `?PerformAction@CCreatureAction_PutItemIntoPocket@@UAEXXZ`. Evidence:
  - It is a byte twin of 0x00849D90 with zero references and no RTTI slot.
  - The donor order matches: Ego_r 0x00525190 -> 0x005252A0 (0x110 each) and FableWin 0x01DE0F30 -> 0x01DE1070 (0x140 each).
  - It sits next to its twin in retail and has the same ItemDefIndex layout.
  - The catalog row says GetItemOutOfPocket; that is wrong.
- 0x0084A0A0 `?PerformAction@CCreatureAction_PutDownThingAtTable@@UAEXXZ`: RTTI vtable slot + Ego_r/FableWin.
- 0x0084E420 `??0CBase@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z`:
  - it stores vtable 0x0124DD74, whose RTTI is `.?AVCBase@NCameraModes@@`;
  - this is the only CBase ctor in Ego_r (0x008461C0) and FableWin (0x02882C50);
  - `ret 8` fits its two arguments.
- 0x0087CA30 `?Initialise@CCreatureAction_GeneralInterruptedAttack@@UAEXXZ`: vtable slot 12 + Ego_r/FableWin.
- 0x0087CDF0 `??0CCreatureAction_InterruptableMidAttackAutoTurn@@QAE@AAVCThingCreatureBase@@AAVCThing@@PBV2@ABVC3DVector@@ABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z`:
  stores vtable 0x012778CC + the PDB ctor that calls the CThing& base.
- 0x0087CEC0 `??0CCreatureAction_InterruptableMidAttackAutoTurn@@QAE@AAVCThingCreatureBase@@PBVCThing@@ABVC3DVector@@ABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z`:
  the four-argument PDB ctor, which calls the creature-only base.
- 0x008AB5D0 `??$__find@PAVCScriptThing@@V1@@std@@YIPAVCScriptThing@@PAV1@0ABV1@ABUrandom_access_iterator_tag@0@@Z`. Evidence:
  - It graded a full MATCH from the STLport 4.x `__find` text.
  - The element is a 12-byte CScriptThing. Its `operator==` (Ego_r `??8CScriptThing@@QBE_NABV0@@Z`) calls virtual slot 78, IsEqualTo.
  - The landed caller 0x008A80F0 already names it `__find<CScriptThing*, CScriptThing>`.
  - The catalog label "std::lower_bound_with_vtable" is wrong.
- 0x008B8260 `??0CCreatureAction_ProjectileCrouch@@QAE@AAVCThingCreatureBase@@0_N@Z`: vtable 0x0127AC94 + Ego_r 0x008F0FE0.
- 0x008B8310 `??0CCreatureAction_ProjectileCrouch@@QAE@AAVCThingCreatureBase@@0ABVC3DVector@@@Z`: vtable 0x0127AC94 + Ego_r 0x008F1090.
- 0x008B9FF0 `?Initialise@CCreatureAction_ForcePushInto@@UAEXXZ`: RTTI vtable slot + PDBs.
- 0x008BA140 `?FinishAction@CCreatureAction_FinishWillForceSpell@@UAEXXZ`: RTTI vtable slot + PDBs.
- 0x008BA6B0 `?FinishAction@CCreatureAction_ReleaseWillEnflameSpell@@UAEXXZ`: RTTI vtable slot + PDBs.
- 0x008BAA90 `?FinishAction@CCreatureAction_ReleaseWillLightningSpell@@UAEXXZ`: RTTI vtable slot + PDBs.
- 0x008BF740 `?Initialise@CCreatureAction_GenericAimProjectileWeapon@@UAEXXZ`: RTTI vtable slot + PDBs.

Callees, all PDB spellings that also fit the retail call shape. NAME_FIXES.tsv carries the wrong spellings seen.

- 0x006925A0 / 0x006925B0 / 0x006925C0 / 0x006925D0: CCreatureActionBase HasPerformedAction / GetTotalTimeWorldFrames /
  GetFramesRunning / GetFramesRemaining, all `QBE`. The Ego_r and FableWin run order and sizes match retail, and so do the bodies.
- 0x00692910 `?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ`:
  - Ego_r has SetCreature, then PeekDefinitionManager / PeekPlayer / GetPlayer / GetWorld / PeekWorld (0x10 each), then GetTotalTimeSeconds.
  - Retail has the same run: 0x00692910..0x00692950, then 0x00692960 GetTotalTimeSeconds.
- 0x00857F30 / 0x00858030: the CCombatActionBase ctors (CThingCreatureBase&, CThing&, long, const CSoundPair&) and
  (CThingCreatureBase&, long, const CSoundPair&).
- 0x008561A0 `?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z`: protected static
  `__fastcall`; the hidden return pointer is in ecx.
- 0x00857520 `?Initialise@CCombatActionBase@@UAEXXZ`, 0x00859A20 `?FinishAction@CCombatActionBase@@MAEXXZ`,
  0x008574D0 `?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z`.
- 0x006A1440 GetThingInPrimarySlot const, 0x006A1E00 RemoveThingFromPrimarySlot(bool) -> CThing*,
  0x006A1F80 PutThingInCarrySlot(CThing&, long, bool) (CTCCarrying).
- 0x0073ABA0 `?RemoveMode@CTCCreatureModeManager@@QAEXK@Z`, 0x0073ACC0 AddMode(CCountedPointer<CModeBase>),
  0x00718A00 `CCountedPointer<CTCCreatureModeManager::CModeBase>(CModeBase*)`.
- 0x008BD7F0 `??0CModeChargeForcePush@NCreatureMode@@QAE@AAVCThing@@_N@Z` (vtable 0x01268328, 0x78 bytes).
- 0x00703210 CThingObject::Create (the settled spelling), 0x00705CC0 CTCCarryable::GetCarrySlotActive const.
- 0x0070B460 CTCAnimationComplex::GetAnimation, 0x0070D8C0 CAnimateThing::RemoveCombinationAnimsOfType(long).
- 0x0069F0F0 / 0x005CC091: the CCreatureAction_UnsheatheItemFromInventory ctor and virtual dtor.
- 0x006644F0 `?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z`.
- 0x005DAA40 `?IsProjectileWeapon@CTCWeapon@@QBE_NXZ`, 0x005DA4D0 `?SetAiming@CTCWeapon@@QAEXXZ`.
- 0x0057C4F1 `?ApplyBonusItemEffects@CTCHeroStats@@QAEXABVCBonusItemDef@@@Z`.
- 0x007E1D30 `??$GetSubDef@VCBonusItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCBonusItemDef@@@@@Z`.
- 0x008113C0 `?AddEvent@CTCEntityEvents@@QAEXPAVCEventBase@NEntityEvents@@@Z`, 0x0084B010 the CEventExpressionPerformedTo ctor.
- 0x006AC430 `?LearnExpression@CThingPlayerCreature@@QAE_NABVCCharString@@J_N@Z`.
- 0x004C7CA0 `?GetJoystickDeviceNumber@CThing@@QBEJXZ`.
- 0x008F7950 `?CreateDustEffectForCreature@NCombatActionHelpers@@YIXAAVCThingCreatureBase@@ABVCWorldMap@@ABVCGameDefinitionManager@@ABVC3DVector@@PAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z`.
- 0x0099EBF0 / 0x0099EAE0: the CCharString ctor (const char*, long) and dtor. 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10:
  CBaseIntelligentPointer.

Virtual slots, used by name only (no address in a src/ file):

- CThingCreatureBase::IsPlayerCharacter() const is slot 77 (+0x134). FableWin emits the inline virtuals in this order:
  - CThingPlayerCreature: IsAffectedByDamage / IsControlled / GetMaxJoggingSpeed / IsToUnsheatheItemsAfterCutscenes / IsPlayerCharacter;
  - CThingAICreature: IsAffectedByDamage / FeelsGravity / IsToUnsheatheItemsAfterCutscenes / IsPlayerCharacter / IsControlled.

  These map one to one onto the retail stub order:
  - player vtable 0x012457FC: 0x0052ABA0 (slot 47), 0x0052ABB0 (76), 0x0052ABC0 (80; `fld [ecx+0x194] ; ret 4` = GetMaxJoggingSpeed), 0x0052ABD0 (88), 0x0052ABE0 (77);
  - AI vtable 0x0127293C: 0x00835910 (47), 0x00835930 (88), 0x00835940 (77), 0x00835950 (76).

  The return values agree: player `mov al,1`, AI `xor al,al`.
- CTCPhysicsBase::GetFacingVector() const is slot 70 (+0x118) and GetFacingVector(float) const is slot 69. The evidence:
  - Ego_r emits 0x006EE000 `()` before 0x006EE020 `(float)`;
  - retail slot 70 = 0x006B1410 and slot 69 = 0x006B1430, the same order;
  - overloads take slots in reverse declaration order.
- CJoystick::AddRumble(long, const CJoystick::CInitRumble&, bool) is slot 17 (+0x44). Ego_r has
  `?AddRumble@CJoystickDX@@UAEXJABVCInitRumble@CJoystick@@_N@Z`, and CInitRumble (MaxLevel, Smoothness, AttackTime, DecayTime,
  Cumulative) is 20 bytes. The landed 0x008E6500 calls slot 17 with the same 20-byte block.
- CScriptThing::IsEqualTo(const CScriptThing&) const is slot 78 (landed rows agree; 0x004AA8A0).

## Probable

- 0x00692C10 `?GetWorldMap@CCreatureActionBase@@IAEAAVCWorldMap@@XZ`.
  - Ego_r has both overloads (const 0x00519580, non-const 0x005195A0, 0x20 each); retail has one 0x20 body in that place (an ICF fold).
  - 0x0087CA30 is a non-const member, so it binds the non-const overload.
  - Const callers bind `IBEABV` to the same address. See Review at landing in LESSONS.md.
- 0x013B8388 `g_pInputManager` (CInputManager*). This is the settled name in work/SETTLED_NAMES and the majority landed spelling.
  Two landed rows say `g_Loader`.
  - PJoysticks (`vector<CCountedPointer<CJoystick> >`) is at retail +0x2C.
  - The Ego_r CInputManager offsets map with 12-byte vectors: ProcessedEvents, EventStore, NoStoredEvents, NoEventScansRunning,
    PMouse, PKeyboard, PJoysticks.
- 0x013CA828 `g_DefStringTable`: the majority landed spelling.
- 0x009ADA10 `?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z`: the protected non-const
  overload; hand523 has the const one at 0x009AD9E0.

## Descriptive

- `g_PutGoodsDownZOffset_013baf54` (0x013BAF54, .bss float, never written). It is used by 0x00848850 only.
- Placeholder virtual slots in the models: `ThingVSlotNN`, `PhysicsVSlotNN`, `JoystickVSlotNN` and `ScriptThingVSlotNN`. These
  only fill a vtable to a known slot and are never called or emitted.

## Label corrections (catalog / manifest)

| Address | Catalog label | Correct name |
|---|---|---|
| 0x0084E420 | CActionParam_Constructor | NCameraModes::CBase::CBase |
| 0x008AB5D0 | std::lower_bound_with_vtable | std::__find<CScriptThing*, CScriptThing> |
| 0x00849E80 | CCreatureAction_GetItemOutOfPocket PerformAction | CCreatureAction_PutItemIntoPocket::PerformAction |
| 0x006925A0 | GetCanItemBeQuickAssigned | CCreatureActionBase::HasPerformedAction |
| 0x006925B0 | DrawGetEnvironment | CCreatureActionBase::GetTotalTimeWorldFrames |
| 0x006925C0 | DrawGetEnvironment / GetMember_dword0x1c | CCreatureActionBase::GetFramesRunning |
| 0x00692910 | _Dest_val<...CMusicManager...> | CCreatureActionBase::PeekDefinitionManager |
| 0x00692920 | _Dest_val<...> | CCreatureActionBase::PeekPlayer (probable, order + size) |
| 0x00692930 | _Dest_val<...> | CCreatureActionBase::GetPlayer (probable, order + size) |
| 0x00692940 | _Dest_val<...> | CCreatureActionBase::GetWorld (probable, order + size) |
| 0x00692950 | _Dest_val<...> | CCreatureActionBase::PeekWorld (probable, order + size) |
| 0x00705CC0 | GetBanter | CTCCarryable::GetCarrySlotActive |
| 0x0070B460 | DrawGetEnvironment | CTCAnimationComplex::GetAnimation |
| 0x0070D8C0 | AddChild | CAnimateThing::RemoveCombinationAnimsOfType |
| 0x0052ABE0 / 0x00835940 | vfunc_77 | CThingPlayerCreature / CThingAICreature ::IsPlayerCharacter |
| 0x0052ABB0 / 0x00835950 | vfunc_76 | ::IsControlled |
| 0x0052ABD0 / 0x00835930 | vfunc_88 | ::IsToUnsheatheItemsAfterCutscenes |
| 0x0052ABA0 / 0x00835910 | vfunc_47 | ::IsAffectedByDamage |
| 0x0052ABC0 | vfunc_80 | CThingPlayerCreature::GetMaxJoggingSpeed(const C3DVector&) const |
| 0x006B1410 / 0x006B1430 | CTCPhysicsBase::vfunc_70 / vfunc_69 | CTCPhysicsBase::GetFacingVector() const / GetFacingVector(float) const |

## Layout facts (retail)

- CCreatureActionBase is 0xA8 (Ego_r 0xB4). Members: PCreature +0x08, FramesRunning +0x1C, PItemRemoved +0x2C, PerformedAction +0x62.
- CCombatActionBase is 0x114 (Ego_r 0x12C). Members: PTarget +0xA8, PCameraPoint +0xB0, PMeleeAbilityDef +0xB8.
- CThing members: UsingComponents +0x20, Components +0x44, PhysicsTC +0x60, PBaseDef +0x70, PlayerNumber +0x90, Dead +0x91 bit 0.
- C3DVector has alignment 1. Ego_r CCreatureAction_ProjectileCrouch puts TurnToFaceTarget at +300, FacePos at +301, size 316,
  so retail has +0x114 / +0x115.
- NCameraModes::CBase is 0xA4 retail (Ego_r 0xC8). CPhysicsState is 0x30 (STLport lists are 4 bytes).

## Header debt

- The shared engine headers for CCreatureActionBase, CCombatActionBase, CTCBase/CTCPhysicsBase, CThing, CIVCountedPointeeBase,
  CTCWeapon, CTCHeroStats, CTCEntityEvents, CJoystick and CDefinitionManager are flat (base flattened into pads), so every row
  uses local models. vland prints SHARED_HEADER_AVAILABLE warnings for them. A header regen with real bases and retail offsets
  is needed before strict mode.
- The CThing vtable model is placeholders 1-76 plus IsPlayerCharacter at 77. The real slot names are not modelled.
