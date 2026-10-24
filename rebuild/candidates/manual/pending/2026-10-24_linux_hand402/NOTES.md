# Hand chunk 402 (CCombatActionBase .. CCreatureInteraction): 37 of 40 byte-exact

All 37 matched sources grade MATCH (2) or RELOCATION_MATCH (35) under the RTM lane (`gall.sh` -> `grades.txt`). Every leaf
names exactly one defined symbol in its object (`objdump -t`). All 37 behaviour tests print `<addr>_TEST PASS` under g++
(`tall.sh`) and under VC7.1 in Wine (`tall.py` -> `tall_vc.txt`).

## What matched (37)

- **NCreatureMode predicates and mode helpers (24 of 27)**, all `__fastcall` free functions in one TU, each with the full
  Ego_r.pdb symbol on its Mangled line. They share one local CThing / CThingCreatureBase model (generator `probe/gen.py` + `probe/rows.py`):
  - IsRolling, IsToDeflectAllHits, IsCarryingTooHeavyWeapon, IsKnockedDown, IsStabbableByHero, IsIncapacitated, IsUnderground,
    IsAbleToUseConversationAnimations, IsLookTurningOverridenByParent, IsLyingDown, IsSitting, IsFaceHitterOnHit, IsAbleToMove.
  - IsAvailableForHeroSprint, IsAvailableForHeroRun, RemoveMovementBlockingModes, RemoveAnimationBlockingModes,
    GetOldTargetFlagsAndReplaceWith, RevertOldTargetFlags, IsLightningable, IsAbleToBlock, IsAbleToUseTurningAnimations,
    IsPhysicsPushableByHero, IsPhysicsPushableBySelfAvoidance.
  - Engine/CThing.h is not retail-accurate, and engine/CThingCreatureBase.h disagrees with retail (PDB +0x100 is
    ShotAccuracyPercentage; retail +0x100 is PCurrentAction), so CThing is a local model. Each source says so.
  - Mode numbers come from each CMode*::vfunc_1 body.
- **NCreatureMode animation helpers (4):**
  - GetStunnedEffectCreationPos 0x0085EE50: head position + 0.75 Z.
  - GetAnimationFromCreature 0x008618B0.
  - GetMovementSpeedFromAnimation, both overloads: the `long` one 0x00861950 and the `const CCharString&` one 0x00861C90, which
    inlines GetAnimationFromCreature and the CAnimationEntry overload.
  - The anonymous-namespace AddCombinationIdles 0x008619C0.
- **Combat (3):**
  - CCombatActionBase::GetPCombatActionFromEnum 0x00857430. It extends the landed CCreatureActionBase::GetPActionFromEnum
    (0x00692E50) and uses the same registry, 0x013BA8C8.
  - CCombatActionBase::CStrikeEvent's non-virtual scalar deleting destructor 0x0085AE90.
  - The CFilter_Bind4<IsNot, CombatIsStrikeToStrike, CombatOverlappingSphere, CombatCanCollideOrInteractWithThing>::operator()
    <CThingGameObject> instance 0x0085B160. CombatOverlappingSphere is inlined, using interface 0x7F CTCHitLocations::GetBoundingSphere.
- **STLport helpers (3):**
  - fill<CTextureManagerPoolInfo*> 0x0085D680 (MATCH).
  - __uninitialized_fill_n<CTextureManagerPoolInfo*, unsigned int> 0x0085D7A0 (MATCH). It needs the memberwise copy
    constructor and UseMipmaps as a dword.
  - vector<NThingMap::CThingMapCell>::operator= 0x0085DFD0.
- **CCreatureInteraction (2):** the default constructor 0x00863170 and AddParticipant 0x008631D0, over a 4-byte STLport list
  model (sentinel pointer).

## Parked for Fable 5.1 (3)

- 0085fa00 NCreatureMode::IsToEvadeAllHits DIFFER(154v154): only the two return tails after the IsModeActive(0x41) test are swapped: retail lays out `mov al,1` before `xor al,al` (`je false`), the lane emits false first (`jne true`) | tried: `||` chain with a named mode manager, out-parameter GetInterface `&&` chain, two separate ifs, optimize("s"), the QFE-4035 compiler | next: a same-TU IsModeActive stand-in (the 0x0085B160 slot lever) or a named `bool` result tail; else lane gap: return-tail block order
- 0085fb30 NCreatureMode::IsAbleToBeAttacked DIFFER(135v135): only the two return tails after the 0x45/0x51 mode compares are swapped: retail lays out `xor al,al` before `mov al,1` (`jne true`), the lane emits true first | tried: `||`, a two-case switch, two separate ifs, optimize("s"), the QFE-4035 compiler | next: the value-return tail `return (mode == 0x45 || mode == 0x51) ? false : true;` (the 0x0085FBC0 negation lever); else lane gap: return-tail block order
- 0085fbc0 NCreatureMode::IsAbleToBeEngagedInCombat DIFFER(142v142): retail places the shared `xor al,al` false block inline after the first (+0x6C bit 1 / +0xBC bit 5) test and the later checks jump back to it; the lane merges it after the mode compares (the `? false : true` dragon-name negation already matches) | tried: separate ifs, a two-case switch, the first two checks merged with `||`, optimize("s"), the QFE-4035 compiler | next: two separate identical early `return false;` statements for the physical and the underground tests (the 0x00857430 lever: separate early returns keep the merged block on the fall-through); else lane gap: tail-merge block placement

## Skipped

None. No row holds two functions, none ends in a noreturn call, and every row's bytes end exactly at the function's
`ret` (trailing int3 padding only).

## Label corrections

Rows (catalog label -> real name; evidence in each source header):
- 0x0085F310 "IsKnockedDown" -> NCreatureMode::IsSitting (modes 8/9/0xA/0xB = SitChair/SitBench/SitFloor/SitTavernStool).
- 0x0085F6A0 "RemoveInterfaceMode_impl" -> NCreatureMode::RemoveAnimationBlockingModes.
- 0x0085EE50 "GetHeadPosWithOffset" -> NCreatureMode::GetStunnedEffectCreationPos.
- 0x00861950 "GetMovementSpeedModifier" -> NCreatureMode::GetMovementSpeedFromAnimation(long, const CThingCreatureBase&, const CMeshDataBank&).
- 0x00863170 "LinkedList_Init" -> CCreatureInteraction::CCreatureInteraction().
- 0x008631D0 "CreatureInteraction_InsertAfter" -> CCreatureInteraction::AddParticipant(CThingCreatureBase&).
- 0x0085DFD0 "Vector_Reallocate" -> std::vector<NThingMap::CThingMapCell>::operator=.
  - Evidence: its callers are the landed fill/copy/copy_backward over 12-byte elements, used by the
    vector<vector<CThingMapCell> > bodies next to the CThingMap deleting destructor 0x0085ECF0.
  - Ego_r.pdb CThingMap::Cells has that type.
- 0x0085D7A0 "fill<CTextureManagerPoolInfo*,...>" -> std::__uninitialized_fill_n<CTextureManagerPoolInfo*, unsigned int, ...>.
- 0x0085AE90 "CParticleMorphs_Destroy" -> CCombatActionBase::CStrikeEvent::`scalar deleting destructor'.
  - Evidence: the destroyed members match Ego_r.pdb CStrikeEvent: HelperPointName +4, PWeapon +0x14, StruckThingList +0x1C.
  - No callers or references.
- 0x0085B160 "CThingFilter_CombatStrikeToStrike_Apply" -> CFilter_Bind4<...>::operator()<CThingGameObject> (descriptive label;
  the filter set is the Ego_r.pdb GetAllThingsInRoughArea/GetAllThings instantiation).

Callees (manifest label -> real name):
- 0x00A242C0 "CTCInventoryAbilities::GetAbilityLevel" -> CMeshDataBank::PeekAnimation.
- 0x007EA270 "CObservable::AddExclusiveObserver" -> CTCTargeted::SetTargetingFlags.
- 0x0073A6E0 "CPixelShader::CountReferences" -> CTCCreatureModeManager::GetCurrentMode.
- 0x0073A8A0 "CPlayer::HasMode" -> CTCCreatureModeManager::IsModeActive.
- 0x0099EAE0 "_Cons_val..." -> CCharString::~CCharString.
- 0x0085DCA0 "Vector_CopyAndReallocate" -> vector<CThingMapCell>::_M_allocate_and_copy.
- 0x0085D820 "_Uninit_copy<...unsigned int...>" -> STLport `__fastcall` __uninitialized_copy(first, last, result, const __false_type&).
- 0x0085AA90 "CVillageTaskCarryCrate::AssignCreatureToTask" -> CCombatActionBase::SetTarget(CThing&).
- 0x0070C050 "CInitRumble::CInitRumble" -> CAnimInit::Init(uchar, ulong, long, EAnimUpdateMode, bool, ulong, long).
  - The 7 arguments match Ego_r.pdb, and CAnimInit is 0x2C bytes.
- 0x005DC1F0 "CCombatActionBase::GetAnimationTransitionInTime" -> CAnimationEntry::GetDelayTime.
  - Its neighbours are 0x005DC1D0 GetTransitionTime and 0x005DC210 GetFlags, in Ego_r.pdb order.
  - They read the components of 0x005E0280, 0x005E0300 and 0x005E0380.
- The landed 0x008350D0 note says CAnimFilterAnyGroup's vtable (0x0127261C) is "slot 0 destructor, 2 predicate". That is
  backwards: slot 2 (0x00834CD0) is the deleting destructor, and slot 0 (0x00834CC0) is the predicate.
