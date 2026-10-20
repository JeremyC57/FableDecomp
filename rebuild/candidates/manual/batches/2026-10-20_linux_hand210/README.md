# Hand chunk 210 (CTCMeleeTrap .. CTCResurrectionItem): 26 of 40 byte-exact

All 26 sources in `src/` re-graded MATCH/RELOCATION_MATCH with gall.sh (grades.txt). All 26 tests pass under g++
(tall.sh) and VC7.1/Wine (tall.py). 14 rows are parked in `differ/`, none skipped.

## What matched (26)
- CTCPhotojournalCapture: DrawPreparePrimitivesForRendering 0x0055D5D0 (slot 11). The elapsed time and the component loop
  sit in their own block so the key slot can be reused, and `++FrameCounter == 2` is load/inc/compare/store.
  DestroyScreen 0x0055D6C0 needs `/GS`. It has `if (p) delete p;`, an STLport `erase(begin(), end())` through a trivial
  memmove copy, and the landed CGameEvent ctor (the dead Player = -1 store survives).
- CTCInventoryQuests ctor 0x0061A420 (the catalog says CTCOpinionSource). Uses the STLport `_Vector_base` and
  `_Rb_tree_base`/`_Rb_tree` shapes, every scalar as an initialiser, and a secondary NHeroInformationScreens::CBase.
- CTCRegionDisplay::SetCurrentAreaName 0x00639A90. `GetWorld()->GetWorldFrame()` keeps the GetWorld call;
  `CurrentAlpha = LastAlpha = 255`; `TimeOut = t != 0.0f` makes a full-register 0/1.
- CTCQuestCard: GetQuestName 0x00702310 and GetQuestObjective 0x00702470 (the catalog says GetQuestName). The def index is
  read twice (`if (PDef->X > 0) Get(PDef->X)`), which gives `mov ecx,[..] ... mov eax,ecx`. OnSerialise 0x00702720
  (slot 1): the literal defaults go in the dead `flag` slot.
- CTCPhysicsStandard: SetInteractionFlags 0x007234C0 (slot 43; plain MATCH). It is one `(flags >> n) & 1` per bool:1 flag,
  and VC merges the stores into each byte. PerformForceUpdates 0x008E5670 and its gravity-only twin 0x008E5F00 use a `||`
  gravity guard and an inline GetLengthSquared. GetPossibleInteractedWithList 0x008E57D0 uses named CanCollideOrInteract,
  OverlappingSphere and HasTC filters in a Bind3 temporary, and needs C3DVector's memberwise copy ctor.
- CTCRandomAppearanceMorph ctor 0x00728000. Two bases (CTCBase + CAGraphicAppearanceComponent) and an inline world-seed step
  through `&PeekComponent().WorldSeed`.
- CTCPerceiveThing::FrameUpdate 0x00745C70 (slot 9). The melee bool is kept in bl and the inline IsUpdatingThisFrame is reused.
- CTCPhysicsRigidBody::CalculateVertices 0x00759EC0. The rotated vertex is a NAMED local (read back from its frame slot), and
  elements are reached as `*(begin()+i)`.
- CTCObjectAugmentations ctor 0x007673C0. STLport vector and tree bases, pack(1) on the intelligent-pointer classes only
  (ProjectileTrailEffect +0x65), and the inline STLport `resize(n)` forwarding `T()`.
- CTCOpinionSource::AppliesToWitness 0x00771540. The witness test is the outer `if`, and PWorld/PFactionManager are scoped_ptrs
  read through operator-> (that puts the global load in eax).
- CTCProjectileTrap::TriggerTrap 0x00787CA5 and CTCMeleeTrap::UpdateAnimation 0x00788555 come from the size/frame-pointer trap
  TU (`optimize("y", off)` + `("s", on)` in the source). TriggerTrap needs `__forceinline` on the def release. UpdateAnimation
  needs the thing in a local, an unchecked component lookup, and a frame-time ctor whose initialiser list zeroes two fields and
  whose body stores the time.
- CTCPhysicalShieldSpell: TakeDamage 0x0079E250 (out-parameter GetInterface, `__forceinline` in the local model). FrameUpdate
  0x0079EB30 has the landed CancelMove tail verbatim.
- CTCMultiArrow::ApplyEffectOnWeapon 0x007CC060. It mirrors the landed RemoveEffectOnWeapon and uses one named CRGBColour.
- CTCOracleMinigame::PlaySoundW 0x007E60F0. The sample bank goes in a named local, and the CSoundPair and CSoundInit are ctor
  temporaries.
- CTCResurrectionItem::OnItemUsed 0x007F04C0. The def goes in a named local, the action is a temporary of an inline-ctor class,
  and `""` is passed as an implicit conversion.
- CTCNamedUserActionPoints::IsActionPointIndexBlocked 0x007FF0C0. The filters and the Bind3 are named locals, and the search
  tools are read into a named local.
- CTCPhysicsBase: HasSphereMeshCollidedFromTo 0x00880640 (named raised start/end points). HasCollidedWithThingFromTo
  0x00880730 (slot 90) applies the normal scale through an inline `C3DVector::operator*=`.

## Parked for Fable 5.1 (14)
- 006399b0 CTCRegionDisplay::GlobalUpdate DIFFER(193v218): the lane tail-merges the two `fdiv ; fmul 255 ; __ftol2 ; store ; ret 4` alpha tails | tried: early returns, if/else chain | next: lane gap: identical tails tail-merged (0x729B20 family)
- 006b0780 CTCPhysicsBase::OnCreate DIFFER(198v198): `mov ebx,[esp+10h] ... mov ecx,ebx` vs `mov ecx,.. ; mov ebx,ecx` for the fetched thing def | tried: pointer local + ReleaseDef, RAII CDefPointer, holder, holder + Velocity.Zero() | next: lane gap: copy-register allocation
- 006b0b30 CTCPhysicsBase::InitialActivate DIFFER(217v217): the Y sum in the PlaceInMapwho argument loads Position.Y first | tried: inline helper with const& offset, operator+(C2DVector, C3DVector) named result, returned temporary, a second operator+(C3DVector, C2DVector) | next: write that second operator's Y as `position.Y + offset.Y` too (one-try)
- 0071dd50 CTCHeroMorph::GetTattoos DIFFER(217v196): retail homes the def slot and the push_back tag in the dead parameter slot and keeps the def in ebx; ours opens a frame word and re-reads it | tried: holder (speed), optimize("s") | next: lane gap: one dead slot shared by the push_back value and its tag
- 00766260 CTCObjectAugmentations::GetHealthToAddThisFrame DIFFER(210v199): inline-asm fistp rounding helper (frame pointer, bare dword fistp) | tried: C++ rounding (one grade) | next: lane gap: inline-asm rounding
- 00766330 CTCObjectAugmentations::GetStaminaToAddThisFrame DIFFER(210v199): the same inline-asm rounding (the catalog says GetHealthToAddThisFrame) | tried: C++ rounding (one grade) | next: lane gap: inline-asm rounding
- 00768120 CTCObjectAugmentations::AttachAugmentationToSlot DIFFER(178v166): register colouring (this=ebx, def=esi, entry=edi) and the augmentation slot's `dec [ecx+4] ; jne` release (ours load/dec/store) | tried: RAII + holder, plain slot + holder + entry ref, non-const types | next: hand-release the augmentation slot through a pointer local read after SetupAugmentationSounds
- 007713c0 CTCOpinionSource::SetupFromDef DIFFER(236v219): an extra ebx copy of the def in the copy helper, g_pMainGameComponent loaded after the pushes, and the flag loop indexes [edi+eax] | tried: holder in the arm, manager local, raw pointer + hand release, `__forceinline CopyFromDef` (this=esi as retail) | next: holder inside the helper + g_pMainGameComponent local
- 0079dcc0 CTCPhysicalShieldSpell::OnHitAtPos DIFFER(178v178): the out-slot effects def is loaded via ecx then copied to esi (retail loads esi directly) | tried: `&&`, nested if, call through the slot member, holder | next: lane gap: copy-register allocation
- 0079e420 CTCPhysicalShieldSpell::PlayShieldEffects DIFFER(211v211): the appearance component is loaded into ecx after the argument pushes (retail before them, bool through eax) | tried: out-param GetInterface, HasTC + ternary GetTC, HasTC + if-form GetTC, named component local | next: lane gap: component loaded straight into ecx (chunks 161/162)
- 007cc710 CTCMultiArrow::ReleaseShot DIFFER(185v180): only the empty test (retail `mov eax,[esi+30h] ; cmp eax,[esi+34h]`; our size()==0 subtracts and masks) | tried: pointer compare, iterator-object back(), iterator objects throughout, empty() as size()==0 | next: `_M_start == _M_finish` written inline with back() through the iterator object
- 007fefe0 CTCNamedUserActionPoints::GetFullNamedActionPoint DIFFER(220v216): loop index register and the two c_str shapes; find's value and tag share the live `start` slot | tried: one grade | next: lane gap: tag over a live slot (24l); c_str as a preloaded "" getter for the parameter
- 00725b70 CTCPhysicsControlled::UpdateAdditionalImpulse DIFFER(205v201): frame-slot sharing of the area box, filter and for_each result temporaries (retail 0x28) and the vector freed through the cached start in edi | tried: chained call, named area + C3DVector copy, first/last locals, search in its own block | next: lane gap: frame-slot sharing (23d/24l)
- 00727c20 CTCRandomAppearanceMorph::GetMeshesUsed DIFFER(214v214): only the placement-copy load order of the 12-byte CMeshInfo in push_back | tried: one reused named CMeshInfo, store-order variant, ctor temporaries | next: lane gap: POD copy scheduling

## Skipped
- none (no row holds two bodies)

## Label corrections
- 0x0061A420 "CTCOpinionSource::CTCOpinionSource" is CTCInventoryQuests::CTCInventoryQuests(CThing&) (vtables 0x012585B4/0x012585A0).
- 0x00702470 "CTCQuestCard::GetQuestName" is CTCQuestCard::GetQuestObjective (ObjectiveName +0x1c, def QuestObjective +0x30).
- 0x00766330 "CTCObjectAugmentations::GetHealthToAddThisFrame" is GetStaminaToAddThisFrame (def +0x44/+0x48).
- 0x0071DD50 "CTCObjectAugmentations::GetDamageMultiplier" is CTCHeroMorph::GetTattoos(vector<CDefPointer<const CTattooDef>>&) const.
- Slot names: 0x007234C0 vfunc_43 = SetInteractionFlags, 0x00745C70 vfunc_9 = CTCPerceiveThing::FrameUpdate, 0x00702720 vfunc_1 =
  CTCQuestCard::OnSerialise, 0x0055D5D0 vfunc_11 = DrawPreparePrimitivesForRendering, 0x00880730 vfunc_90 = HasCollidedWithThingFromTo.
- Callees: 0x006AFD40 CTCBase::PeekThingSearchTools (not GetCombatManager); 0x006AFD30 CTCPhysicsBase::IsPhysicsMeshTypeMesh (not
  CCombatSequenceBase::IsInterruptable); 0x006B93D0 CFactionManager::GetPFaction(const CCharString&); 0x00415FBC
  CMainGameComponent::PeekDefinitionManager (a `jmp CDefinitionManager::Get`, not GFGetPreMainMemoryUsed); 0x007719C0 GetDef<COpinionSourceDef>
  and 0x006B9D90 GetDef<CFactionDef> (both labelled CThingPhysicalSwitchDef); 0x0042AF9E GetSubDef<CObjectAugmentationsDef> and
  0x005B1EFC GetSubDef<CTattooDef> (labelled CAbilityDef); 0x0076BF80 vector<CDefPointer<const CThingBaseDef>>::resize (labelled
  vector<unsigned int>); 0x0066B280 CTCSoundPlayer::PlayAttachedSample (returns unsigned long); 0x005BC6D6
  NInventory::CTCInventoryBase::RenderUIBackdropQuad(float) (not CSpellContainerList::AddChild); 0x00787C35 CTCTrapBase::SetTransitionParams
  (not ConstructString); 0x00787BF1 CTCTrapBase::Reset; 0x00787BFE ManualDeactivate; 0x00787C07 ManualActivate; 0x00788198
  CTCMeleeTrap::InitialiseContinuousPlay (not CEngine::ChangeCameraAngle); 0x0049D9D0 CWorld::PeekMyPlayer() const; 0x007447A0
  CTCPerceiveThing::SetThingToPerceive (not DeleteData); 0x004C7A70 CThing::PeekComponent() const; 0x0072AB70 static
  CTCSpecialAbilities::CreateParticleEffect; 0x00725F10 a GetAllThingsInRoughArea instantiation and 0x00726660
  _For_each<CThingCreatureBase**, CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature> (labels propagated); 0x004C7B80
  CThing slot 17 = IsToBePlacedInMap.
