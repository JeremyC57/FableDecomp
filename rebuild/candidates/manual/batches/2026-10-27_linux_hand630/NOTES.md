# Hand chunk 630 (CMenuEntry .. CTCSpecialAbilities): 3 of 10 byte-exact

All ten rows were parked same-length misses from the 2026-10-30/31 retry sweeps. Each best attempt was regraded first
(the 0078c150 differ file grades 202v192 -- its 192v192 was the sec3 hdr/ variant -- and 0092f240 195v193), then changed one
lever at a time.

## What matched (3)
- **NCameraModes::CBase::UpdateRaiseUp 0x0084D7E0** (RELOCATION_MATCH; catalog autoname "CTCGraphicAppearance_UpdateHeightInterpolation").
  Identity: Ego_r donor run 0x008452E0..0x00845420 (GetDefaultFOV .. IsStretchAroundHero, UpdateRaiseUp,
  GetObservedThingCurrentMeshHeight) lines up 1:1 with TLC 0x0084D790..0x0084D8B0; single caller 0x0084DDE5 (FrameUpdate). It is
  a camera-mode member, not a CTCGraphicAppearance one. Two levers: (1) the focal height is `offset.Z = GetFocalPos(1.0f).Z - thingPos.Z`
  on an otherwise unused C3DVector local -- the Z is read through the returned temporary's pointer (`fld [eax+8]`) and the local
  shares the dead temporary's words (frame 0xC; a named float takes its own slot, a whole vector with `-=` reads the frame copy);
  (2) the observed thing comes through an inline accessor ON the camera manager (`lea ecx,[eax+100h]`); reading the manager member
  from CBase gives `add ecx,100h`. CGameCameraManager from engine/CGameCameraManager.h via the Layout include pattern.
- **CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed 0x0092F240** (RELOCATION_MATCH). The filter is
  `IsOfType(0x14) && IsTargettedAtThing(GetCreature())` -- 0x006AF290 is the IsTargettedAtThing ctor and the searched template
  0x008D46D0 calls IsTargettedAtThing::operator() 0x006AF1E0 (TargetUID +0x28); the old model's IsOriginatedByThing was wrong.
  Levers: the FindMostRecentMessage template body compiled in this TU (real list walk, `__declspec(noinline)`) so VC7.1 knows the
  bound filter does not escape (stores the first pointer before the second filter's ctor call) and a NAMED criteria CCharString.
  Names: CAIStateGroupBase::PeekWorld/GetWorld (bound), CWorld::GetMessageEventManager (FableWin, by reference),
  CAIStateGroupBase::GetCreature / CAIBrain::GetCreature (FableWin, by reference), CTCSoundPlayer::PlayAttachedCriteria (Ego_r),
  InteractionFailed is private (AAE). CWorld and CAIStateGroupBase from their shared headers (Layout include pattern).
- **CSB::GetDistanceBetweenAgents 0x00A872F0** (MATCH). The one-expression `length - (agentRadius + r(other))` scheduled the
  `relative` byte load after the `fadd`; naming the length and the other radius (`float length = ...; float otherRadius = ...;`)
  puts it between the radius call and the fadd as in retail. Navigator slots named from vtable 0x0125EC9C + Ego_r overrides;
  CSB from engine/CSB.h via the Layout include pattern.

## Parked for Fable 5.1 (7)
- 0053e0c0 NUISystem::CMenuEntry::MakeUnavailable DIFFER(190v190): `mov edx,[esi] ; call [edx+184h]` v retail `mov eax,[esi] ; call [eax+184h]` (slot-97 vptr register) | tried: nested-if chain, || assignment chain, ==0 tests, split last arm (sec1b), inline early-return GetText helper (2 spellings), nested goto form, `|| (t = f()) != 0` chain, #pragma optimize("s"), /Oa | next: lane gap: register choice for a vptr load after a zero-tested call result
- 00ac82a0 COptimisedPrimitive::ComputeSubContractionCost DIFFER(192v192): `push ebx ; mov [esp+4],ecx ; ... push ebp ; imul ebx ; mov ebp,[ecx+4]` v retail `push ebx ; push ebp ; mov [esp+8],ecx ; mov ebx ; mov ebp ; imul ; imul` | tried: vertices pointer local, named index locals, *(first+n) (sec5), v2 declared first (retail schedule, swapped ebx/ebp), reversed index locals, one declarator, CQuadric operator+ (220), operator[](long), copied CSubContraction (198) | next: lane gap: schedule/register pairing of the two vertex refs
- 0078c150 CTCBerserk::PlayHitEffectAtPos DIFFER(192v192): `sub esp,0Ch` and every name/effectType lea 4 higher (retail lets the name temporary reuse the dead GetSubDef out slot, frame 8), `pop ebx` before the known-test je v after the CreateParticleEffect call | tried: block-scoped slot, header model, effectType first (sec3), function-scope out slot + pointer copy (fixed the release to `dec [esi+4] ; jne`, 202->192), inline helper slot, release spellings x4, CCharString temporaries (190) | next: lane gap: dead-slot reuse of a function-scope out slot by a later temporary
- 00488620 CPlayer::GetPNearestTCVillage DIFFER(196v196): comparator stores / ThingSearchTools load scheduled around the &village push, and `lea eax,[esp+0Ch]` key / out word (frame+4, the dead filter) v retail `lea eax,[esp+8]` (frame+0, the dead village) | tried: same-TU noinline LowerBound, ETCInterfaceType key temp, C3DVector memberwise copy ctor, village block scope x2 (sec1b), PDB accessors GetWorld/GetThingSearchTools + const GetPBestLeafThing + C3DVector user ctors | next: CThing::GetTC<CTCVillage>(CTCVillage**) (FableWin 0x01AD7EF0, right after this instantiation) as the lookup form, else lane gap: dead-slot choice
- 004886f0 CPlayer::GetPNearestTCVillage const DIFFER(196v196): same as 00488620 | tried: its forms | next: as 00488620 (FableWin: the const twin instantiates GetPBestLeafThing<..., const CThingVillage>)
- 00632830 GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites,std::less<...> > DIFFER(196v196): every frame displacement 4 lower (retail leaves L+0 unused and both traits objects share L+0xC) | tried: traits order, count first, named value_type, named insert result, hoisted pair (sec1b) | next: lane gap: frame packing of never-written traits objects (the hole may be the inlined Read's __int64 position)
- 0072b260 CTCSpecialAbilities::SetupDrainLifeSpell DIFFER(198v198): `lea ecx,[esp+10h] ; push ecx ; mov ecx,[edi+18h]` v retail `mov ecx,[edi+18h] ; ... lea eax,[esp+10h] ; push eax` | tried: out-param GetInterface, bare AddTC literal, IsDead (sec2b), full PDB respelling (CTCEnemy::AreThingsEnemies(const CThing&, const CThing&), CTCWillResponse::IsThingLifeDrainable(const CThing*), CThing::GetPlayer() CPlayer& + engine/CPlayer.h ZTargeting, AddTC const CTCParamsBase*), early-return flow, creature local at the call | next: lane gap: address-temp register for a by-reference CCharString argument (sibling 0x0072B0D0 has our shape)

## Skipped
None (no row holds two functions).

## Label corrections
- 0x0084D7E0 "CTCGraphicAppearance_UpdateHeightInterpolation" -> NCameraModes::CBase::UpdateRaiseUp (Ego_r donor run, caller).
- 0x00632830 "CPersistTraits<std::list<COpinionTransientOffset>>::TransferBinaryIn" -> GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites,
  std::less<NUISystem::ETableSprites> > (FableWin.pdb name; __fastcall stream/map, map clear + insert; single caller 0x006327CF, the
  CUIDef Sprites transfer). The chunk note's list label does not fit the bytes.
- 0x00530A40 "DynamicListEntry::MakeUnavailable" -> NUISystem::CComponent::MakeUnavailable (Ego_r run FindFirstChildOfType ..
  MakeUnavailable 0x005AA290..0x005AA880 = TLC 0x00530460..0x00530A40).
- 0x004C1060 "CTCGraphicAppearance::GetHelperPointOrCentre" -> NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) const.
- 0x0090DEE0 catalog "FindMostRecentMessage<Bind2<IsOfType,CanBeSeenOrHeard>>" -> <IsOfType,IsOriginatedByThing> (its body calls 0x006AF200).
- Landed file names: 0x006AF1C0 (named HasExtraData) is IsOfType::operator(); 0x006AF1E0 (named IsOriginatedByThing) is
  IsTargettedAtThing::operator(); 0x006AF290 (named IsOriginatedByThing ctor) is the IsTargettedAtThing ctor. See NAME_FIXES.tsv.

## Names: proven v descriptive
- Proven (two sources): every function/callee listed in the three matched sources except the ones below.
- Descriptive: the NCameraModes::CBase member at +0x40 (`CameraManager`, no PDB layout record) and the CGameCameraManager inline
  observed-thing accessor (`GetObservedThing`, no PDB accessor; CBase::GetObservedThing itself is Ego_r-proven).
- Navigator vtable slots 11/12 are the two GetNavigatorObstacles overloads; which overload is in which slot is unproven (not in the bytes).

## Header debt
- engine/CAIStateGroup_MinionReactToPlayerInteraction.h flattens its base and types ReactionType as bytes -> local derived model (as
  the landed IsRunnable 0x00930A70).
- engine/CGameCameraManager.h types PObservedThing as `unsigned char[8]` (the source reinterprets it as CIntelligentPointer<const
  CThingGameObject>); engine/CPlayer.h types PlayerCharacter the same way (0x00488620 differ model is local).
