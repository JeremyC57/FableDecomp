# Hand chunk 89 (CTCGraphicAppearanceStaticMesh .. CTCHeroExperience): 34 of 40 byte-exact

All 34 src/ rows grade MATCH (6) or RELOCATION_MATCH (28) under the lane (grades.txt), and every behaviour test prints
`<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 + Wine (tall.py, tall_vc.txt). 6 rows parked in differ/, 0 skipped.
Pragmas: `optimize("s", on)` for the three inventory-module rows (0x00572672, 0x00572689, 0x005CD2A6); no `/flags` anywhere.
Authored in two sessions: the first agent matched the bodies in probe/, the second wrote the commented sources, tests and
reports (re-grading every final file; 0x0077D470 needed the probe's `_c` variant, 0x00B2AD90 / 0x006CA040 were switched to the
PDB signatures and still match).

## What matched (34)
- NGraphicAppearance::CTCGraphicAppearanceStaticMesh / ...StaticMeshFadeOutOnProximity / ...Sprite `??_G` (0x0077BD20,
  0x0077C2A0, 0x0077C7E0): two vtable stores (CTCBase primary + CAGraphicAppearanceComponent at +0xc), unregister from the owner's
  CTCGraphicAppearance unless the owner is dead (+0x91 bit 0), the component-base vtable, ~CTCBase, flagged delete. Shape:
  `__forceinline virtual` derived destructor + an inline constructor instantiated through a `new T(thing)` factory.
- CTCGuard: GetTimeAgoThatCrimesCanBeApologisedFor (0x007B29B0) -- a generic `Clamp(const T&, const T&, const T&)` returning a
  reference and a namespace-scope `const long` upper bound bound by reference (retail takes its .rdata address).
  IsAttackingToFineRatherThanKill (0x007B2CF0) -- inline `CThing::GetInterface(&out, 0x23)` with an UNINITIALISED out pointer
  (the bit-clear path passes the stale frame slot), `bool result = false` in bl.
- CTCHaste: GetActionSpeedMultiplier (0x007FFFB0; paused -> 0, positive global override, else member) and FrameUpdate
  (0x00800000; pause countdown + RemoveTC(0x8c) when flagged).
- CTCInventoryExperience::UpdateExternalExperienceSpendStat / ...Ability (0x00572672 / 0x00572689): getter + setter forwarders,
  `optimize("s")` re-pushes both stack arguments before the getter call; modelled on engine/CTCInventoryExperience.h via `_Methods`.
- CTCInventoryStats::CTCInventoryStats (0x005CD2A6, optimize("s")): two bases (CTCInventoryBase, NHeroInformationScreens::CBase),
  out-of-line map constructor for CreaturesKilled, zero initialisers in declaration order.
- CTCHero: GiveTutorialForCategory (0x006C9FA0; singleton forwarder), the constructor (0x006C9FE0; pack(1) CIntelligentPointer at
  the unaligned +0x1d, the 64-bit id zeroed between the two pointer constructors), Copy(const CTCBase&) (0x006CA040; member copies
  + inline counted CDefPointer operator= + CTCBase::Copy chain).
- CTCEmoteIcon ctor (0x0071AFE0; CEnginePrimitiveHandle member with inline ctor over CBaseClass), CTCHeroCentreDoorMarker ctor
  (0x0077F570), CTCCameraPointScripted ctor (0x0075CC60; only StartPos is built from zeros), CTCHeroCentre ctor (0x0077D470; the
  def pointer must be a member object with an inline null constructor), CTCFlammable ctor (0x00780820).
- CEngineSurfaceCompositionManager::ProcessLoading(bool, double) (0x00B2AD90): two bank forwards, the double re-copied per call.
- CTCHeroAttachableAppearanceModifiers: IsWearingAppearanceModifier (0x00706000, MATCH), RemoveAllAppearanceModifiers (0x00706640;
  VC6-era `erase(begin(), end())` keeps its zero-count copy loop), Copy (0x00706EC0; vector-of-vectors operator= then inline
  SetAsChanged, no base Copy), UploadChangesToGraphics (0x00707930) and FrameUpdate (0x00707AD0, same body). SetAsChanged's
  `GetWorld().GetFrame()` calls GetWorld for its object expression and the STATIC frame getter with nothing in ecx.
- CTCHeroExperience: IsAbleToBreakDownDoors (0x006D6370; `!IsChild()` = `neg ; sbb ; inc`), GetRoughExperienceLevel (0x006D64B0,
  unsigned counter + size() re-evaluated), GetSoundRadiusMultiplier / GetVisibilityMultiplier (0x006D65E0 / 0x006D6620:
  reference-returning Min over two frame slots + an iterator-object operator[] so the table pointer is re-read after the select),
  GetTrainableStatMaxLevel (0x006D6770; /72 element count), GetCostToUpgradeTrainableStat (0x006D67B0; unsigned compare),
  GetShotAccuracyPercentage (0x006D6EA0; by-value counted def copy whose release null test folds after the dereference),
  OnRegionLoad (0x006D7570; a by-value CDefPointer getter temporary: add-ref, setne, release before the branch),
  GetExperienceFallbackMultiplier (0x006D7DE0; inline pick-up multiplier), OnComboMultiplierDecrease (0x006D7E70; uninitialised
  GetInterface out pointer, tail duplicated into both arms).

## Parked for Fable 5.1 (6)
- 007b2d50 CTCGuard::GetBribeAmountWillTake DIFFER(89v89): retail `mov eax,ecx ; mov eax,[eax+4] ; xor ecx,ecx ... mov ecx,[eax+4] ; call GetPVillage`, ours `mov edx,[ecx+4] ; xor eax,eax ... mov eax,[eax+4] ; mov ecx,eax` | tried: out pointer `= 0` + inline GetInterface, the same through an inline GetVillage() helper (4 grades) | next: lane gap: register allocation of the out-pointer (ecx vs eax)
- 006d6590 CTCHeroExperience::SpendExperience(EHeroExperienceStatCategory, long) DIFFER(42v40): retail `jns` on the flags of `sub [eax],esi` vs ours `test edx,edx ; jge` after the reload | tried: natural two-statement form (best), `long&` local (46v40), `if ((v[c] -= amount) < 0)` (55v40) | next: lane gap: compiler-build flag reuse across a reload
- 006d6f60 CTCHeroExperience::SpendExperience(EHeroTrainableStatType, long) DIFFER(61v59): the same `jns` vs `test ; jge` (inlined category overload) | tried: natural form (best), `long&` local (69v59) | next: lane gap: compiler-build flag reuse (as 006d6590)
- 006d6870 CTCHeroExperience::GetMeleeStrikeAnimationSpeedMultiplier DIFFER(83v82): ours `mov eax,[edi+58h] ; fld [eax+0F4h] ; add eax,0F0h ; fld [eax]`, retail `mov edi,[edi+58h] ; fld [edi+0F4h] ; fld [edi+0F0h]` | tried: `low` local in GetValue (best), one-line GetValue (79v82) (4 grades; the other two variants were not kept) | next: read the range through the by-value copy so `this` dies early, else lane gap: register reuse
- 006d7120 CTCHeroExperience::GetAttackVariationMultiplier DIFFER(85v73): retail keeps the running max in st(0) across the IsComboSuccess call (`fcom`, `fstp st(0) ; fld`), ours spills it to a frame slot (`push ecx ; mov [esp+0Ch],3F800000h`) | tried: natural loop (1 grade) | next: lane gap: x87 value live across a call
- 006d7e10 CTCHeroExperience::GetExperienceMultiplierFallbackIndex DIFFER(88v86): retail `lea ecx,[esp+7] ; push ecx ; mov [esp+0Ch],eax ; ... lea edx,[esp+0Ch] ; push edx` (tag pushed first), ours stores the value and loads first/last before forming the tag | tried: one-expression find (3 grades) | next: multiplier/tag as named locals, else lane gap: find tag-temporary scheduling

## Skipped
None (no row covers two functions).

## Label corrections
- 0x0077C2A0 / 0x0077C7E0 "CTCGraphicAppearanceStaticMesh::`vector_deleting_destructor'" -> ??_G of
  NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity (vtables 0x0126AE04/0x0126ADEC) and
  NGraphicAppearance::CTCGraphicAppearanceSprite (0x0126AE94/0x0126AE78).
- 0x00800000 "CTCHaste::vfunc_9" -> CTCHaste::FrameUpdate.
- 0x00572672 / 0x00572689 "CTCHero::GiveTutorialForCategory" -> CTCInventoryExperience::UpdateExternalExperienceSpendStat /
  UpdateExternalExperienceSpendAbility (Ego_r.pdb symbols; callers 0x005746F7 / 0x005747A3).
- 0x00B2AD90 "CTCHero::GiveTutorialForCategory" -> CEngineSurfaceCompositionManager::ProcessLoading(bool, double) (caller
  CEngineResourceManager::ProcessLoading 0x00B3B7F0 on the singleton 0x01436EAC).
- 0x005CD2A6 "CTCHero::CTCHero" -> CTCInventoryStats::CTCInventoryStats; 0x0071AFE0 -> CTCEmoteIcon ctor; 0x0077F570 ->
  CTCHeroCentreDoorMarker ctor (vtable stores).
- 0x0075CC60 "CTCHeroCentre::CTCHeroCentre" -> CTCCameraPointScripted ctor; 0x00780820 -> CTCFlammable ctor (vtable stores).
- 0x00706EC0 "vfunc_12" -> Copy(const CTCBase&); 0x00707930 "EditFrameUpdate" -> UploadChangesToGraphics; 0x00707AD0
  "EditFrameUpdate" -> FrameUpdate (vtable 0x01264F0C slot 9).
- 0x006D6620 "GetSoundRadiusMultiplier" -> CTCHeroExperience::GetVisibilityMultiplier; 0x006D7DE0 "GetExperiencePickUpMultiplier"
  -> GetExperienceFallbackMultiplier; 0x006D7E10 "GetExperiencePickUpMultiplier" -> GetExperienceMultiplierFallbackIndex.
- Callee labels found propagated: 0x00686800 "CEngineSubPrimitive2DClockSprite" = CTCBase::CTCBase, 0x00686830
  "~CChunkedFileChunk" = CTCBase::~CTCBase, 0x006868D0 "$E2" = CTCBase::Copy, 0x00686A80 "CViewBase::GetDisplayEngine" =
  CTCBase::GetWorld, 0x0049D870 "GFGetPreMainMemoryUsed" = the world frame getter, 0x004C9840 "CBaseObject::OnPropertyChanged" =
  CThing::RemoveTC, 0x007EE540 "IS_THING_ALIVE<..>" = CTCVillageMember::GetPVillage, 0x006D65C0 / 0x005EDF1F
  "OnRegistedZSortingPatchRelocation" = SetExperienceSpentOnStat / SetExperienceSpentOnAbility, 0x0056FFC5 "GetTCHeroExperience" =
  GetTCInventoryAbilities, 0x0075C180 "CTCOpinionSource" = CTCCameraPointDefinitionBase ctor, 0x004533C5
  "CDisplayManager::CopyBackBufferToTexture" = map<unsigned long,long> ctor, 0x0066E734 "CBase" = NHeroInformationScreens::CBase
  ctor, 0x009FC410 "_Cons_val<..>" = the bank ProcessLoading(bool, double), 0x006D72D0 "BSTInterval_FindNode" =
  GetActiveComboMultiplierData, 0x006D73D0 "CEngine_GetExperienceMultiplier" = GetExperienceFallbackMultiplierForMultiplier.
