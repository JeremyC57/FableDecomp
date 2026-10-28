# Chunk 515 (re-queue, 40 rows <= 95 bytes) -- agent515 notes

Lane: Linux Wine, VC7.1 RTM 13.10.3077. 37 rows hand back in src/ (3 MATCH, 34 RELOCATION_MATCH), 3 are parked in
differ/. No retail_asm rows, no SKIP rows. Every src row has a test that prints `<addr>_TEST PASS` under g++ (tall.sh)
and VC7.1 (tall.py). gall.sh: 37/37 graded. vland dry run: 37 WINS, no RELOC_IDENTITY / RELOC_CONFLICT /
RELOC_DUPLICATE; only RELOC_NAME label warnings and SHARED_HEADER_AVAILABLE warnings (see "Header debt").

## Proven
(the name comes from an RTTI vtable slot, a PDB symbol for the exact signature, or a landed caller/callee chain)
0x007332F0 CCombatAbility::IsValid(const CCombatAbilityData&, unsigned long*) const
0x00778AB0 CTCFamilyMember::IsRelatedTo(const CThing&) const
0x007B52B0 std::copy<deque<CCrimeWaitingForBribe>::iterator, back_insert_iterator<vector<CCrimeWaitingForBribe> > > -- only caller is the landed CTCGuard::OnSerialise 0x007B3760, which binds it as this copy
0x007CA800 CTCHeroReceiveItems::HasPickedUpItem(long) const
0x007FEC30 CTCUserActionPointBase::IsActionPointIndexBlocked(long) (protected)
0x0080C140 (anonymous namespace)::CThingFilter_IsEvadeExplosion::IsTravellingAway(const C3DVector&, const C3DVector&) const
0x00811190 NEntityEvents::CEventKilledCreature::CEventKilledCreature(const CThing&, const CThing&) -- stores RTTI vtable 0x012723A8; FableWin.pdb ctor signature
0x00811380 CTCEntityEvents::HasEventOfTypeCreatedAfterFrame(NEntityEvents::EEventType, long) const -- the PDB's only find_if with CIsEventOfTypeAfterFrame; Events +0x0C
0x0081F590 CMap::GetSoundAt(const C2DCoordI&) const
0x0084D0A0 NCameraModes::CBase::Die()
0x00866FB0 CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::FinishAction() -- vtable 0x01271A74; calls the Base FinishAction 0x00866E80
0x0087B690 NCameraModes::CFixedDolly::UpdateDesiredPos(C3DVector&) -- vtable 0x0125C4F8
0x00887380 NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::RestoreDefaultTheme() -- landed caller ProcessTransitions 0x00887A30
0x0088B980 CAIBrain::IsStateGroupPresentAndEnabled(const CCharString&) const
0x0088B9E0 CAIBrain::SetStateGroupEnabled(const CCharString&, bool)
0x0088E260 CGameScriptInterface::IsThingWithThisUIDAlive(unsigned __int64) const
0x0088E540 CGameScriptInterface::EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const
0x008900D0 CGameScriptInterface::RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const
0x00890110 CGameScriptInterface::RadialBlurFadeTo(float, float, float, float, float, float, float) const
0x00890820 CGameScriptInterface::FadeScreenOut(float, float, CRGBColour)
0x00890990 CGameScriptInterface::EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const
0x00894B90 CGameScriptInterface::SetCreatureBrain(const CScriptThing&, const CCharString&) const
0x00896060 CGameScriptInterface::SetVillageLimbo(const CScriptThing&, bool) const
0x008960C0 CGameScriptInterface::IsEntityUnderScriptedControl(const CScriptThing&) const
0x008961B0 CGameScriptInterface::ClearHeroEnemyOfGuards(const CScriptThing&) const
0x00896270 CGameScriptInterface::EnableGuards(const CScriptThing&, bool) const
0x008962D0 CGameScriptInterface::EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const
0x00896330 CGameScriptInterface::IsChestOpen(const CScriptThing&) const
0x00897030 CGameScriptInterface::IsToFailQuestOnDeath() const
0x00897150 CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const
0x00897880 CGameScriptInterface::GetHeroTitle() const
0x008978E0 CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const
0x00897C90 CGameScriptInterface::GetNumberOfTimesHeroHasHadSex() const
0x00898E60 CGameScriptInterface::IsHeroHandLampLit() const
0x00898F40 CGameScriptInterface::GiveHeroGold(long) const
0x00898FA0 CGameScriptInterface::GetHeroGold() const
Parked, name proven:
0x007F3D60 (anonymous namespace)::CThingFilter_IsElectrocutable::operator()(const CThingObject&) const -- its caller 0x007F3E30 is CFilter_Bind3<CThingFilter_In3DRange, unary_negate<CThingFilter_HasTC>, CThingFilter_IsElectrocutable>::operator()<CThingObject> (F3 +8); flag = CThingObjectDef::Electrocutable (bit 4, retail +0x98)
0x00891070 CGameScriptInterface::CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const -- vtable 0x01260F0C slot 413
Callees proven this pass:
0x0051ED80 CThingManager::GetPThingWithUID(unsigned __int64) (Peek is 0x0051ED60, donor order Peek-then-Get)
0x006B8550 CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo (C3DVector overload)
0x006B8730 CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo (float overload)
0x006E7370 CGameScriptInterface::StartFadeScreen (protected)
0x006D3E80 CDefinitionManager::GetDef<CBrainDef>
0x00701840 COpinionReactionManager::GetThingAttitudeToPlayer(const CThing&) const
0x0084F1B0 NCameraModes::CBase::GetObservedThing() const
0x007B51B0 std::__copy_aux<deque iterator, back_insert_iterator<vector<CCrimeWaitingForBribe> > >
0x007B5030 std::__copy<deque iterator, back_insert_iterator<vector<CCrimeWaitingForBribe> >, int>(..., const random_access_iterator_tag&, int*)
0x00889440 std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight>::resize(size_type, const CThemeWeight&)
0x00811E20 std::__find_if<list<CCountedPointer<CEventBase> >::const_iterator, CIsEventOfTypeAfterFrame>(..., const input_iterator_tag&)

## Probable
0x00722270 CTCAppearanceMorphBase::CMeshMorphSet::`scalar deleting destructor' -- the body is exactly the CMeshMorphSet member teardown (OriginalMeshTextureMorphs, then MeshParts' TextureMorphs, then the block); no surviving caller names it
0x007A9B10 (parked) CTCAssassinRush::ResetCameraOnStick() -- only caller is the landed CTCAssassinRush::SetupCameraOnStick 0x007A9C80 on `this`; uses the protected CTCBase::GetWorld and CTCAssassinRush ParentThing / LocalRelativeVectorForCamera (+0x74) / RelativeVectorForCamera (+0x80); of the PDB's still-unbound CTCAssassinRush void() members (ResetCameraOnStick, RushForwards, ResumeForCutscene, SetFakeCollideWithSomething) only ResetCameraOnStick fits a body that just installs a camera-on-a-stick module

## Descriptive
Names that are not retail symbols:
- `OnDie` (0x0084D0A0: NCameraModes::CBase slot 22, `_purecall` in CBase's own vtable, no PDB name)
- `ClearBypassScreenFilter` (0x00897150: the inline CTCCarrying setter; member name not recovered)
- CTCWife's permitted-to-region-follow byte +0x1c (0x008978E0: setter is ?SetAsPermittedToRegionFollow@CTCWife@@QAEX_N@Z, member name descriptive)
- the enum placeholders `EHERO_TITLE_NONE_PLACEHOLDER`, `ETUTORIAL_CATEGORY_PLACEHOLDER`, `EVENT_TYPE_PLACEHOLDER` (only the enum types matter for the mangling)
- the killer / killed parameter names of 0x00811190 (the body reads only the second CThing&)
- same-TU emitters and stand-ins (no retail address): `DeleteMeshMorphSet_00722270`, `IsTravellingAway_0080c140`, `CallIsElectrocutable_007f3d60`, `FindDef_SetCreatureBrain_StandIn` and the GetDef / GetSubDef / __destroy_aux / __find / __find_if / __copy_aux / CBasicString::Compare stand-in bodies (these are the real callees' bodies or shapes, compiled noinline so the caller's frame matches)
- 0x007AB450's first parameter is kept as the landed `void*` spelling (the target type is not recovered)

## Label corrections
- 0x00722270: catalog "CActiveFile::~CActiveFile" -> CTCAppearanceMorphBase::CMeshMorphSet scalar deleting destructor; its callee 0x005BC0D3 (labelled "StdList_Dtor_005bc0d3") is CEngineTextureMorphSet::~CEngineTextureMorphSet.
- 0x00778AB0 / 0x007CA800 / 0x007FEC30: catalog "CHitParameters::IsDivineWrathStrike" (propagated) -> CTCFamilyMember::IsRelatedTo / CTCHeroReceiveItems::HasPickedUpItem / CTCUserActionPointBase::IsActionPointIndexBlocked. The landed caller 0x006A8880 binds 0x00778AB0 as CTCFactionMembership::IsMember, which is wrong.
- 0x007A9B10: catalog "CCreatureAction_ReleasePhysicalShieldSpell::FinishAction" -> CTCAssassinRush::ResetCameraOnStick (probable); the old park's CScriptedShieldCameraAction class was invented.
- 0x007B52B0: catalog "CEditControlCentre::GenerateNewFractal" -> std::copy (deque iterator -> back_insert_iterator<vector<CCrimeWaitingForBribe> >); 0x007B51B0 "Fractal_InitDensityMap" -> std::__copy_aux; 0x007B5030 "Vector_InsertRange_8byte_Deque" -> std::__copy(..., const random_access_iterator_tag&, int*). The old park's CFractalParams class was invented.
- 0x007F3D60: catalog "CCombatActionBase::GetDamageMultiplier" -> (anonymous)::CThingFilter_IsElectrocutable::operator(); the old park's CAnimatedAction / CAnimationEntry classes were invented.
- 0x0080C140: catalog "CCompareTargetedPositions::operator()" -> (anonymous)::CThingFilter_IsEvadeExplosion::IsTravellingAway.
- 0x00811380: catalog "CCombatActionBase::GetPActiveStrikeEvent" -> CTCEntityEvents::HasEventOfTypeCreatedAfterFrame; the old park's CEventList class was invented. Its callee 0x00811E20 (landed as `std::find_if`) is STLport `std::__find_if(first, last, pred, const input_iterator_tag&)`: the tag reference arrives in edx.
- 0x00887330: landed "CBrainBehaviourSet_00887330::Init" -> CEnvironmentThemePriorityStackEntry::SetOnlyTheme (the landed ProcessTransitions 0x00887A30 calls it by that name); 0x00889440 landed as vector<CBrainBehaviour>::resize -> vector<CEnvironmentThemePriorityStackEntry::CThemeWeight>::resize (the hand953 J112 correction, still open in the repo).
- 0x0051ED80: labelled "CWorld::GetThingWithUID" (and "CStopWatch::GetTicks" in another label set) -> CThingManager::GetPThingWithUID.
- 0x006B8550: labelled "_Ucopy" -> CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo (the C3DVector overload).
- 0x006E7370: labelled "ConstructString" -> CGameScriptInterface::StartFadeScreen.
- 0x006D3E80: labelled "GetDef<CHeroPostcardGeneratorDef>" -> GetDef<CBrainDef>.
- 0x0084F1B0: labelled "_Cons_val" -> NCameraModes::CBase::GetObservedThing() const.
- 0x008302C0 / 0x008302D0: landed as "_Cons_val" -> the CThingAICreature::IsStateGroupPresentAndEnabled / SetStateGroupEnabled forwarders.
- 0x00487DC0 / 0x00487DD0: labels "CPlayer::GetHeroCreature" / "CPlayer::GetControlledCreature" -> GetPControlledCreature() const / PeekPControlledCreature() const (the settled identities). Landed 0x00897CF0 and 0x00897090 still use the old GetPPlayerCharacter spelling for 0x00487DC0.

## Header debt
vland lists SHARED_HEADER_AVAILABLE for CCombatAbility(Data), CDefinitionManager, CDefClassBase, CTCEntityEvents, CGameCameraManager,
CCreatureActionBase, CAIStateGroupBase, CAIBrain, CWorld, CGameScriptInterface, CScriptThing, CEnvironment, CFadeInFadeOutBase,
CDisplayEngine, CTCVillage, CTCWife, CTCHeroStats, CPlayer, CPlayerManager. They are modelled locally because:
- the engine headers are Ego_r.pdb (VS2005 Dinkumware) layouts: every 16-byte container there is 12 bytes in retail (STLport), so the
  retail offsets these rows read (CTCEntityEvents::Events +0x0C is fine, but CAIBrain::StateGroupList, CThing components +0x44,
  CCreatureDef::CreatureGroup +0xAC, CThingObjectDef flags +0x98, CTCAssassinRush vectors +0x74/+0x80, CFixedDolly::DollyVector +0xC0)
  do not line up with the header members;
- CGameScriptInterface / CScriptThing / CWorld rows need the virtual slot layout (vtable 0x01260F0C, CScriptThing slots 11 / 75),
  which the data-only headers flatten;
- engine/CAIBrain.h does not carry the CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater> StateGroupList at +0x28.
No header carries CDefPointer / CCountedPointer / CIntelligentPointer with their inline release paths; every row restates them.
