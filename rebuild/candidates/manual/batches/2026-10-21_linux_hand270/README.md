# Hand chunk 270 (CTCDoor .. CTCInventoryExperience): 11 of 40 byte-exact

## What matched (11)
- CTCHeroExperience (006d77c0 AddExperience, 006d7f20 UpdateComboMultiplier, 006d83b0 OnStatLevelChange): ternary
  GetInterface pick plus a list-tail local; same-TU real GetTotalStatLevel body keeps ecx; UpdateComboMultiplier first try.
- CTCInventoryBase ctor 005bff07: the manager pointer as a named local before the call.
- CTCFireballSpell (007c8de0 ProcessLevellingUpFX, 007c93b0 FrameUpdate, 007c87f0 ctor): implicit-literal `&&` temp plus
  an uninitialised emitter local (OnDie/Kraken shapes); FrameUpdate first try on that model; the ctor needs
  `(float)stamina` inside the loop condition (hoisted through a stack slot, loop aligned).
- CTCHeroAttachableAppearanceModifiers 00706950 AddAppearanceModifierWithReplace: twin of landed 0x00706880 plus a named
  world local per message.
- CTCInventory 00582754 RevertVideoChanges: first try (size TU without a frame pointer, the loop index in the push-ecx slot).
- CTCInventory 00590d32 ctor: STLport vectors built with the alloc-proxy third member (`_M_end_of_storage(a, (T*)0)`)
  fix the map `lea` scheduling.
- CTCInventoryAbilities 005f05e5 OpenInventory: first try (uninitialised interface local plus the landed by-value
  GetPDef() box twin).

## Parked for Fable 5.1 (26)
- 006d68d0 CTCHeroExperience::IsBlockRecoilAnimToConserveAnimationHandedness DIFFER(495v495): one fmul/store swap at the stat-5 clamp join | tried: advisor v16/t16 helper shapes | next: advisor's helper form (named `long level`, clamped `long maxLevel`, `float f = (float)level; return f / maxLevel;`) on the remaining join
- 006d6cb0 CTCHeroExperience::IsToAutoBlock DIFFER(495v495): same swap as 006d68d0 | tried: same | next: same
- 00730c00 CTCDoor::Open DIFFER(456v456): one register pick (ecx v edx) in the base-def temp copy+release | tried: GetPDef temp forms, named def | next: advisor
- 00730dd0 CTCDoor::Close DIFFER(446v446): same single register pick as Open | tried: same | next: advisor
- 00798770 CTCForcePushed::FinishBeingForcePushed DIFFER(453v453): position-copy load/push scheduling | tried: by-value/ref position | next: compiler-build artefact
- 00815420 CTCHitLocations::GetBoundingSphere DIFFER(498v498): this/iterator edi/ebx swap plus an extra ebx save | tried: loop forms | next: park
- 005bd699 CTCInventoryBase::UpdateInventoryAnim DIFFER(405v405): GetPDef temp `dec` scheduled before the fmul (retail between fmul/fdivr) | tried: 7 variants | next: park
- 007f4990 CTCHasName::GetNPCShortName DIFFER(448v449): 64-bit UID argument load order plus the stored-name frame slot | tried: UID by value/ref | next: park
- 00789ff0 CTCHeroRangedAttacks::CancelAimingWeapon DIFFER(451v451): sound component load sunk past the id test | tried: GetInterface out-param, HasTC+FindTC | next: advisor
- 0057aeb4 CTCHeroStats::GetFavouriteWeapon DIFFER(427v427): def slots cached in edi/ebx across calls (whole-program gap) | tried: named defs | next: lane gap
- 0057dc22 CTCHeroStats::GetCrimeReport DIFFER(498v498): esi/edi swap (crime index v newline literal) plus a dead-arg temp slot | tried: 6 variants | next: park
- 0077c960 CTCHighlightItem::DoMovement DIFFER(502v502): only the bbox-centre fld/fadd operand order (Max.Z first) | tried: member/free/copy-and-add operator+ | next: GetCentre helper body shapes
- 0047d296 CTCInGameMenu::PCSetQuitToFrontEnd DIFFER(482v482): `push ecx` float slot before the x fsubp | tried: named y, single statement | next: compiler-build artefact
- 0047dd9d CTCInGameMenu::PCSetToLoad DIFFER(493v493): twin of 0047d296, same swap | tried: same | next: same
- 00583e26 CTCInventory::OnItemSelected DIFFER(465v484): IS_THING_ALIVE temp and lookup keys not homed in the dead `item` argument slot | tried: 4 variants | next: lane gap (dead-arg homing)
- 00587847 CTCInventory::ClosePCInventory DIFFER(506v506): two result-less bitset tests load via eax+lea in retail v ecx+add | tried: bare HasTC statement | next: an inline that returns the thing pointer
- 005f6793 CTCInventoryAbilities::CycleAssignedExpressions DIFFER(440v442): this=edi/zero=esi in retail v ebx/edi; early-empty free not tail-merged | tried: if(!empty) wrap (427), CSoundInit 0x20 | next: advisor
- 005f64dd CTCInventoryAbilities::CycleAssignedAbilities DIFFER(510v508): retail keeps the page-start iterator memory-resident and this in ebx; ours spills this | tried: && temp (560), nested if (510), count before original (508 size), decl order, split Release | next: advisor
- 0057770a CTCInventoryExperience vfunc_89 DIFFER(448v448): only the ability-undo loop's esi/edi swap | tried: hoisted locals, early iterator, helper adds, slot constant | next: advisor
- 005778ca CTCInventoryExperience vfunc_90 DIFFER(502v502): twin, same swap only | tried: shared model | next: same as 0057770a
- 005b3aef CTCInventoryClothing::OnInitialActivate DIFFER(438v422): retail keeps this memory-only and the starting-items def in edi | tried: member Release, named def pointer | next: restructure the loop locals
- 005bfd5e CTCInventoryBase::OnSerialise DIFFER(421v425): an unexplained early zero store to the name-temp slot plus a this/zero register pick | tried: first model | next: find the source of `[ebp-8]=0`
- 005be8f3 CTCInventoryBase::CreateInventoryObject DIFFER(483v482): ParentThing hoisted before the name temp in ours (retail keeps thing=0 in ebx for the zero pushes) | tried: CThingObject->CThing temp (489 -> 483), early return | next: park
- 007800b0 CTCHeroGuide::OpenQuestSelectInterface DIFFER(466v466): shape matches, frame 0x40 v 0x3c (the no-quests string is not sharing the key slot) | tried: mask HasTC, named subsystem, per-arm GetPItem, value GetTC | next: ternary of the two GetPItem calls
- 007e8a40 CTCFlourishTarget::ProcessActionMovementDelta DIFFER(482v482): x87 temp/frame packing (0x34 v 0x3c) plus the side-dot term order | tried: named parent/target (478 -> 482), delta->Dot | next: park (x87)
- 00c11d00 unnamed audio constructor DIFFER(507v507): vptr store and push scheduling only (/EHsc) | tried: identity blocks 5+4, long + two quaternions, polymorphic base | next: park

## Skipped
- 00750e30 CTCExplodingObject::FrameUpdate: lane gap, inline fistp rounding helper.
- 00c6b050 (catalog CTCHeroStats::InitializeAudioEffects): lane gap, inline fstp/fld/fistp rounding (audio reverb; name wrong).
- 004c0240 CTCGraphicAppearance::SetPMeshEffect: lane gap, a no-frame speed TU with bool temps homed in dead argument slots.

## Label corrections
- 006d68d0 = CTCHeroExperience::IsBlockRecoilAnimToConserveAnimationHandedness; 006d6cb0 = IsToAutoBlock (both catalogued as ...ComboChain).
- 007c93b0 is the real CTCFireballSpell::FrameUpdate (the catalog label sits on 0x7C9180).
- 00c11d00 is not CTCHero::Initialize: it is a 7-argument audio-library constructor (vtable 0x012B2748).
- 0057770a / 005778ca are CTCInventoryExperience vtable slots 89 and 90 (undo the spend and close; bool undo).
- Callees: 0x71BE20/0x71BF40 SetStrength/SetSkill; 0x5BCEF8 UpdateInventoryAnimZAxis; 0x686800 CTCBase ctor;
  0x686ED0 AddToGlobalUpdateTCs; 0x415B80 CGraphicBankParams ctor; 0x419108 ~CCountedPointer<CGraphicDataBank>;
  0x669D30 GetEffectFromString; 0x70B460 CTCAnimationComplex::GetAnimation; 0x70C050 CAnimInit::Set; 0x99EAE0
  ~CCharString; 0x42B0A2 GetSubDef; 0x787580 CTCSwitchableNavigation setter; 0x4C9840 RemoveTC; 0x686A70/0x686A80
  GetWorld; 0x5DA920 weapon ammo-slot getter; 0x9C9070 GetPGroupEntry; 0x5BE8D3 (landed GetBoughtPrice) returns a record
  pointer; 0x5D97F0 IsWeapon-like fastcall; 0x5D95C0 item name (CWideString fastcall); 0x6869D0 GetDefinitions;
  0x686D20 GetWorldMap (not GetDisplayEngine); 0xA24260 graphic lookup (not GetAbilityLevel); 0x41CC14/0x41CC2B screen
  width/height; 0x5D8E50 ReturnToInventory; 0x562E70 not ~CParticleEffect; 0x5EDCF0 GetNumberOfAssignableSlots /
  GetNumAbilitiesPerRow (label _Dest_val); 0x5F66D9 SetAssignedExpression (label CRandomAppearanceMorph...); 0x5F6423
  SetAssignedAbility (label AbilityMap_SetData); 0x4C7A80 GetSampleBank (label GetMeshBank); 0x686D50 player-manager
  getter; 0x445950/0x445960 slot counts; 0x414EA0 vector<CCharString> dtor; 0x5EE01A abilities SetAbilityLevel (label
  CEngineLandscapeRenderer::...); 0x579EA3/0x579ECC the experience undo-map clears (labels CTCCarryable::OnKill);
  0x5BD597 OnInventoryClosed (label SetQuestPersistentFlagForItems); 0x485AA7 vector<CComponent*>::erase(first,last);
  0x4855A3 list clear (label SortTreeRecursively); 0xA14440 C3DVector::Normalise in place (label GetScaled).
