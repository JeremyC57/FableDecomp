# Chunk 523 notes (agent523)

20 rows: 15 RELOCATION_MATCH (src/ + test/, all PASS under g++ and VC7.1/Wine), 5 parked in differ/, 0 retail_asm.
vland dry run: 15 wins, no RELOC_CONFLICT / RELOC_DUPLICATE / RELOC_IDENTITY (only SHARED_HEADER_AVAILABLE and RELOC_NAME
label warnings, listed below).

## Proven names (two sources each)

Rows:
- 0x00679560 `??0COpinionReactionManagerDef@@QAE@XZ`: the vtable it stores is 0x0125B7DC (RTTI COpinionReactionManagerDef, its slots 18/19/20 are landed), and the PDB has the default ctor (Ego_r donor 0x006AB9C0, FableWin 0x0183DA38).
- 0x006C3CA0 `?AddCarriedThingsOfThingToFadeList@CTCFadeOutAndIn@@AAEXABVCThing@@@Z`: the Ego_r donor order of the CTCFadeOutAndIn TU puts it between the ctor 0x006C3C30 and AddThingToFade 0x006C3DF0, and the landed UpdateFadingThings binds both of its call sites to it.
- 0x006C9350 `?TransferIn@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@JPAVCOpinionDeed@@@std@@@Z` (parked): its only caller is the landed TransferableVector hook 0x006C9310 (vtable 0x0125E9DC slot 3); the symbol is in both PDBs (Ego_r donor 0x00700120).
- 0x006C9900 `?TransferIn@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV?$CIntelligentPointer@VCThing@@@@@Z`: Ego_r donor 0x006FF800 sits after TransferBinaryIn, whose retail twin is the landed 0x006C8780; its callers 0x006C9773 and 0x006C9B59 are in the same TU.
- 0x006D72D0 `?GetActiveComboMultiplierData@CTCHeroExperience@@ABEABVCComboMultiplierData@@XZ`: Ego_r donor 0x00588900 (0x100 bytes), it reads the PDB members, and its seven callers are CTCHeroExperience members.
- 0x006E89B0 `?GetAttackerPlacementVector@CCombatWheel@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@0J@Z` (parked): the PDB signature (const, two refs, a long, a C3DVector returned through a hidden pointer) exactly fits the `ret 0x10` frame. Donor order puts it next to GetRingRadius 0x006E8850, GetAttackerProng 0x006E87D0 and GetAttackerProngVector 0x006E8810. Its one caller, 0x006EA855, passes `info+0x40` (DefenderInfo.CombatWheel).
- 0x006ED030 `?UpdateAttackingCombatantsList@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z`: only two CCombatManager members take (CCombatantInfo&, bool), this one and SetupDefenderPlacementWheel (Ego_r donors 0x00716990 and 0x007167A0). This body calls 0x006ECEA0 with its own (info, bool), so it is the list update and 0x006ECEA0 is the wheel setup.
- 0x006FC7A0 `?_M_fill_insert@?$vector@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@...` and 0x006FC950 `??$_M_range_insert@PAU?$pair@...`: STLport 4.6 line for line, and the element type is proven by the landed neighbours and callees (0x006FC6A0, 0x006FBAE0, 0x006FB5C0).
- 0x00711230 `??$__merge_without_buffer@PAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@HVCAnimSetItPriorityLessThan@@@std@@YIX...`: STLport `_algo.c` verbatim; its caller, the landed __inplace_stable_sort 0x00711640, names the argument order.
- 0x00723110 `?_M_insert_overflow@?$vector@VCGenerationPos@CTCSmokeGenerator@@...@std@@IAEXPAVCGenerationPos@CTCSmokeGenerator@@ABV34@ABU__false_type@2@I_N@Z` (parked): STLport verbatim; its caller is the landed push_back 0x00723270, and the element is Ego_r CTCSmokeGenerator::CGenerationPos.
- 0x00723E50 `?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z` (parked): vtable 0x01265F84 slot 77. It is the only CTCPhysicsStandard virtual taking (const C3DVector&) and returning void, and the body writes RHSet.Up/Forward.
- 0x0072B940 `?CalculateWeaponState@CTCSpecialAbilities@@SI?AW4EWEAPON_STATE@1@AAVCThingCreatureBase@@@Z`: Ego_r/FableWin donor 0x00742E70; the landed SetWeaponState callers take the same enum.
- 0x007653B0 `?insert@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEPAVC3DVector@@PAV3@ABV3@@Z`: STLport verbatim; its callees are already bound for vector<C3DVector>.
- 0x00766920 `?GetAugmentations@CTCObjectAugmentations@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z`: Ego_r donor 0x007756A0, the PDB member Augmentations is at +0x14, and the callers fetch interface 0xBF.
- 0x0076CFE0 `?CalculateEnemyTC@CTCEnemy@@CI_NABVCThing@@PAPBV1@AAPBV2@_N@Z`: Ego_r donor 0x0077B080; the signature matches the register and stack use exactly.
- 0x0076D2F0 `?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z` (parked): Ego_r donor 0x0077B430; it calls CalculateEnemyTC twice and itself recursively.
- 0x0078AE30 `??0CCreatureAction_HeroLoadRangedWeapon@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z`: it stores vtable 0x0126B9D4 (RTTI), PDB donor 0x00791DF0, and loads the literal "PROJECTILE_WEAPON_LOAD".
- 0x007B47B0 `?TransferIn@?$CPersistTraits@VCCrimeWaitingForBribe@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCrimeWaitingForBribe@@@Z`: its only caller is the landed TransferableVector hook 0x007B4770; the symbol is in both PDBs.
- 0x007C0E80 `?CreateShot@NThingShot@@YIPAVCThing@@AAV2@PAV2@1PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@J@Z`: Ego_r donor 0x007BF690; 2 register + 7 stack arguments gives exactly `ret 1Ch`, and it forwards to the matching Init overload.

Callees named in this chunk:
- 0x006ECEA0 `?SetupDefenderPlacementWheel@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z`: PDB signature, and UpdateAttackingCombatantsList passes it (info, bool).
- 0x006EA0C0 `?UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z`: PDB (CCombatantInfo&) set. The body compares PAttackPattern (+0x64) with the creature type def's AttackPatterns and calls ResetDefenderAttackPattern 0x006E8C30.
- 0x006E8C30 `?ResetDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@V?$CDefPointer@$$CBVCAttackPatternDef@@@@@Z`: `ret 8` fits (info&, CDefPointer by value). It sets PAttackPattern +0x64, AttackPatternIndex +0x68, FrameForNextInstruction +0x6C and resets each ring's rotation. Not used in src.
- 0x006E86A0 `?UpdateDefenderAttackInstruction@CCombatManager@@AAEXAAUCCombatantInfo@@@Z` and 0x006EABB0 `?UpdateAttackingCombatantSequenceResets@CCombatManager@@AAEXAAUCCombatantInfo@@@Z`: both are already landed under these names. The PDB takes the info by reference; the landed 0x006EABB0 models it as a pointer, but the call bytes are the same.
- 0x006F0250 `?_M_find@?$_Rb_tree@_KU?$pair@$$CB_KUCCombatantInfo@@@std@@U?$_Select1st@...@2@U?$less@_K@2@V?$allocator@...@2@@std@@ABEPAU?$_Rb_tree_node@...@2@AB_K@Z`: CCombatManager::Combatants is the PDB map<unsigned __int64, CCombatantInfo>, and STLport 4.6 makes `find` an inline wrapper over private `_M_find` (spelling follows the landed `_Rb_tree::_M_find` precedent, `ABE`).
- 0x006F2680 `?_M_insert_overflow@?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@IAEXPAPAVCThingCreatureBase@@ABQAV3@ABU__true_type@2@I_N@Z`: STLport push_back of the local attacker vector.
- 0x0085FB30 `?IsAbleToBeAttacked@NCreatureMode@@YI_NABVCThing@@@Z`, 0x00662410 `?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ` (GetCombatManager().IsAttacker(*this)), 0x00662650 `?FinishCurrentAction@CThingCreatureBase@@QAEXXZ` (the action's slot-4 FinishAction under the aborting flag): PDB symbols plus their bodies.
- 0x00666D10 `?GetCombatTypeDef@CThingCreatureBase@@QBEABVCCombatTypeDef@@XZ`: the PDB returns a const reference; the landed spellings return pointers.
- 0x006E87D0 `?GetAttackerProng@CCombatRing@CDefendingCombatantInfo@@QBEPBVCProng@2@ABVCThingCreatureBase@@@Z` (the const and non-const PDB twins are one ICF body); 0x006E7FD0 `?GetProngVector@CProng@CDefendingCombatantInfo@@QBE?AVC3DVector@@M@Z`; 0x006E8850 `?GetRingRadius@CCombatWheel@CDefendingCombatantInfo@@QBEMJ@Z` (it sums RingThickness for rings 0..index); 0x006E8810 `?GetAttackerProngVector@CCombatRing@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z`. All four come from PDB donor order plus their bodies.
- CCombatSequenceBase vtable 0x01278FEC: slot 1 OnCreate, 2 GetName, 3 GetCombatSequenceType, 4 GetCombatSequenceInterruptionType, 5 IsInterruptable, 6 IsInterruptableDueToZoneChange, 7 IsMeleeSequence, 8 IsBreakIntoMeleeSequence, 9 IsBoastSequence, 10 IsOuterRingSequence, 11 IsAllowedOnOuterRing, 15 GetZone. Evidence: each default 0x006F06E0..0x006F0740 tests slot 3, 4 or 15 exactly as its PDB name says, and in the derived CCombatSequence_BanditBoast 0x012793AC slot 2 builds a CCharString and slot 3 returns 3 (boast).
- CCreatureActionBase vtable 0x01259D24: slot 15 IsStrikeAction (CCombatActionBase overrides it, 0x0062EA30 = count > 0), slot 27 IsActionReadyForInterruption (bound at 0x00856170).
- CIVCountedPointeeBase: IVRefCount at +0x04, `Delete() const` at virtual slot 1 (0x0042D930), AddRef/Release in the FableWin PDB. These replace the descriptive "m_lockCount / DeleteThis / Lock / Unlock" of my earlier draft (007C0E80, 00766920).
- CResource virtual slots 2..9 (UnloadResource .. ResetDebugMemoryInUse) and CDefClassBase slot 10 GetInstantiationName: CDefClassBase vtable 0x01230BCC, labels consistent with the PDB.
- 0x006B9460 `?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z`; 0x0076C7D0 `?IsEnemyOfFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z`; 0x0076C7F0 `?IsAlliedWithFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z`; 0x0076CCE0 `?IsEnemyOf@CTCEnemy@@ABE_NABVCThing@@@Z` (private).
- 0x006D4D70 `?IsFollower@CTCFollowed@@QBE_NPBVCThing@@@Z` (it walks Followers +0x0C; the AIFollowers twin is 0x006D4DB0).
- 0x006924B0 `?GetRandomAnimFromName@CCreatureActionBase@@KIABVCAnimationEntry@@ABVCThingCreatureBase@@ABVCCharString@@@Z` (0x006924A0 is GetAnimFromName).
- 0x0078B230 `??$GetDef@VCThingShotDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingShotDef@@@@@Z` (FableWin.pdb), 0x009AD9E0 GetPDefFromGlobalIndex, 0x007C0690 NThingShot::CTCShotDefinitionBase::Init, 0x005DA750 GetCreatedShotDefGlobalIndex, 0x004C9D60 CThing::AddTC: PDB symbols plus their call sites in CreateShot.
- 0x00487DC0 CPlayer::GetPControlledCreature const and 0x00487DD0 PeekPControlledCreature const; 0x00449960 / 0x00449970 GetMainPlayer const / non-const.
- 0x00723930 `?UpdateOldRHSet@CTCPhysicsStandard@@MAEXXZ` (slot 95) and 0x00723B30 `?GetFacingAngleXY@CTCPhysicsStandard@@UBEMXZ` (slot 63).

## Probable names
- 0x008B0750 `?IsNextActionInSequenceStrikeAction@CCombatSequenceBase@@QBE_NXZ`. The PDB has two non-virtual bool CCombatSequenceBase queries, IsZoneValid and IsNextActionInSequenceStrikeAction; IsZoneValid is 0x008B0610 (landed, called by 0x006EABB0). This body tests CombatActions[CurrentActionIndex] and returns its action's virtual slot 16, which the labels call IsPartOfComboChain, not IsStrikeAction (slot 15). Identity is by elimination; check at landing.
- 0x006E8600 CCombatManager::IncrementDefenderAttackPattern(CCombatantInfo&): called by UpdateDefenderAttackInstruction when an idle entry's wait expires.
- 0x00723B30 GetFacingAngleXY: atan2 of Forward.X / Forward.Y.

## Descriptive names (no retail symbol found)
- 0x01375731 `g_bFollowersEnemyProxyEnabled` (bool switch read by CalculateEnemyTC).
- 0x0137573F `g_bShotsEnabled` (bool switch read by CreateShot).
- CTCSpecialAbilities::EWEAPON_STATE enumerators UNARMED = 0 / ARMED = 1 / BOW = 2 (the enum is in the PDB; the enumerator spellings are descriptive).
- AnglesToDirection (parked 0x00723E50 only): an inline helper with no retail symbol; it carries the sin/cos form.
- 0x013CE558 `g_InvSqrtMantissaTable` (the fast inverse-sqrt mantissa table, parked 0x00723E50 only).
- Parameter names `force` (UpdateAttackingCombatantsList / SetupDefenderPlacementWheel) and the `GetPreferredCombatDistanceForAttacker` argument order (attacker, defender) are unproven.

## Label corrections (manifest -> evidence)
- 0x00679560 `_global::CResource_Constructor` -> COpinionReactionManagerDef::COpinionReactionManagerDef()
- 0x006C3CA0 `COptimisedPrimitive_RemapAndFilter` -> CTCFadeOutAndIn::AddCarriedThingsOfThingToFadeList
- 0x006C9350 `CPersistTraits<CAtmosBankEntry>::TransferIn` -> CPersistTraits<std::pair<long, COpinionDeed*> >::TransferIn
- 0x006D72D0 `BSTInterval_FindNode` -> CTCHeroExperience::GetActiveComboMultiplierData
- 0x006DA200 `BSTInterval_FindSuccessor` -> std::_Rb_tree_iterator<pair<const long, CComboMultiplierData>, _Const_traits<..> >::operator++(int)
- 0x006E89B0 `CDefendingCombatantInfo_ComputeAttackVector` -> CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector
- 0x006E8850 `Array_SumFloatsStride` -> CDefendingCombatantInfo::CCombatWheel::GetRingRadius
- 0x006EA0C0 `CDefendingCombatantInfo::InitializeDefenderFromCreature` -> CCombatManager::UpdateDefenderAttackPattern
- 0x006ECEA0 `CDefendingCombatantInfo::UpdateCombatState` -> CCombatManager::SetupDefenderPlacementWheel
- 0x006ED030 `CCombatManager_UpdateDefenderState` -> CCombatManager::UpdateAttackingCombatantsList
- 0x006F0250 `RedblackTree_FindLE` -> STLport _Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo>, ..>::_M_find
- 0x006F2680 `std::vector<uint>::_Ufill` -> std::vector<CThingCreatureBase*>::_M_insert_overflow
- 0x006FC7A0 `AnimEventVector_Insert` -> STLport vector<pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > >::_M_fill_insert
- 0x006FC950 `AnimEventVector_ReallocateInsert` -> the same vector's _M_range_insert<pointer>
- 0x00711230 `AdaptiveSortMerge_AnimSet` -> std::__merge_without_buffer
- 0x00723110 `std::std::vector_ReserveAndCopy` -> std::vector<CTCSmokeGenerator::CGenerationPos>::_M_insert_overflow
- 0x00723E50 `CCharacter_UpdateOrientation` -> CTCPhysicsStandard::SetRHSetAsFlushOnNormal
- 0x0072B940 `_global::CalculateWeaponState` -> CTCSpecialAbilities::CalculateWeaponState (static member)
- 0x007653B0 `std::std::vector_InsertBefore` -> std::vector<C3DVector>::insert(iterator, const C3DVector&)
- 0x00766920 `CParticleMorphs_InitFromDef` -> CTCObjectAugmentations::GetAugmentations
- 0x0076CFE0 / 0x0076D2F0 `_global::CalculateEnemyTC` / `_global::AreThingsEnemies` -> CTCEnemy:: members (private static / public static)
- 0x007B47B0 `CPersistTraits<CAtmosBankEntry>::TransferIn` -> CPersistTraits<CCrimeWaitingForBribe>::TransferIn
- 0x007C0E80 `_global::CreateShot` -> NThingShot::CreateShot (the thing-taking overload)
- 0x00662410 two labels (IsRegisteredCombatant / IsInMeleeAsAttacker) -> CThingCreatureBase::IsInMeleeAsAttacker
- 0x00487DC0 `CPlayer::GetHeroCreature` -> CPlayer::GetPControlledCreature
- 0x009BA5B0 `CStringParser::ReadInt64` -> CStringParser::ReadAsUHUGE
- 0x005DA7D0 `NInventory::CCategory::GetCategoryIdentifier` -> CTCWeapon::GetWeaponType. This one may be an ICF fold of two trivial getters, so keep both if the PDB proves the other.
- 0x0076C7D0 manifest `IsAlliedWithFaction` -> CTCEnemy::IsEnemyOfFaction (0x0076C7F0 is IsAlliedWithFaction)
- 0x006D4D70 manifest `IsAIFollower` -> CTCFollowed::IsFollower (IsAIFollower is 0x006D4DB0)

## Header debt
- engine/CThingCreatureBase.h: +0x60 is `long q` ("retail-only long"), but it is CThing::PhysicsTC (CTCPhysicsBase*, Ego_r +0x70 less 0x10 of STLport/CVectorMap savings); +0x18 is CThing::UniqueID (unsigned __int64); +0x5C PTCDriver, +0x64 PTCGraphicAppearance. 0x006E89B0 and 0x006ED030 model CThing / CThingCreatureBase locally (parameters, not `this`).
- engine/CCreatureActionBase.h and engine/CCreatureAction_HeroLoadRangedWeapon.h are flat pad layouts without the vtable. 0x006ED030 declares the 28 real virtual slots locally; 0x0078AE30 includes its header under an alias pinned by sizeof.
- engine/CTCPhysicsStandard.h: 0x00723E50 (parked) includes it under an alias and declares 96 virtual slots of which 93 are v-numbered placeholders (only 63 GetFacingAngleXY, 77 itself and 95 UpdateOldRHSet are named; vtable 0x01265F84 labels cover few of the rest). Fill in the real slot names before that row lands.
- CIVCountedPointeeBase / CDefinitionManager / CTCBase / CTCWeapon / CPlayer / CPlayerManager / CStringParser / CWorld / CResource are modelled locally (non-`this` helper classes, vland SHARED_HEADER_AVAILABLE warnings); retype at landing if strict mode needs it.
- CCombatManager, CCombatantInfo, CAttackingCombatantInfo, CDefendingCombatantInfo (and nested CCombatWheel/CCombatRing/CProng) and CCombatSequenceBase have no engine headers; the local models carry the Ego_r member names at retail offsets.
