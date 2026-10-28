# Chunk 515 (agent515, 2026-10-28): 37 of 40 re-queued single functions landed (under 128 bytes)

Re-queue chunk of 40 chunk_history orphans under 128 bytes (address order, 0x00722270..0x00898FA0): CGameScriptInterface
script calls, TC component predicates, thing filters, camera and AI-brain code. One Opus 5.5 agent at high effort.

- **Landed:** 37 rows with `vland.py --land` (3 MATCH: 0x0081F590 CMap::GetSoundAt, 0x0084D0A0 NCameraModes::CBase::Die,
  0x0088E540 CGameScriptInterface::EntityTeleportToThing; 34 RELOCATION_MATCH), every test PASS under g++ and VC7.1; no
  RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE. 22 more CGameScriptInterface script calls, the CMeshMorphSet `??_G`,
  CCombatAbility::IsValid, CTCFamilyMember::IsRelatedTo, std::copy (deque iterator into a back inserter),
  CTCHeroReceiveItems::HasPickedUpItem, CTCUserActionPointBase::IsActionPointIndexBlocked, the CEventKilledCreature
  constructor, CTCEntityEvents::HasEventOfTypeCreatedAfterFrame, CFixedDolly::UpdateDesiredPos and others.
- **Parked (3, pending/2026-10-27_linux_hand515/differ/):** 0x007F3D60 CThingFilter_IsElectrocutable::operator() (90v90,
  edx vs ecx for the second count), 0x007A9B10 CTCAssassinRush::ResetCameraOnStick (91v91, argument order), 0x00891070
  CameraUseCameraPoint (83v83, PhysicsTC tested before `push esi`).
- **Identities:** 36 proven labels written (ghidra_out/labels_chunk515_2026-10-27.tsv); probable: 0x00722270 (CMeshMorphSet
  destructor), 0x007A9B10.

Label corrections (NOTES.md): the three "CHitParameters::IsDivineWrathStrike" rows are IsRelatedTo, HasPickedUpItem and
IsActionPointIndexBlocked; 0x007B52B0 is std::copy (callees 0x007B51B0 `__copy_aux`, 0x007B5030 `__copy`); 0x007F3D60 is
CThingFilter_IsElectrocutable::operator(); 0x00811380 is HasEventOfTypeCreatedAfterFrame; 0x0051ED80
CThingManager::GetPThingWithUID; 0x006B8550 RadialBlurFadeTo (C3DVector); 0x006E7370 StartFadeScreen; 0x006D3E80
GetDef<CBrainDef>; 0x0084F1B0 GetObservedThing; 0x005BC0D3 ~CEngineTextureMorphSet.

Review items (no vland conflict, but landed spellings disagree): 0x00811E20 is STLport `__find_if` (tag reference in edx)
while its landed body says find_if; 0x00889440 is vector<CThemeWeight>::resize while landed as vector<CBrainBehaviour>, and
0x00887330 (landed CBrainBehaviourSet::Init) is SetOnlyTheme; landed caller 0x006A8880 binds 0x00778AB0 as
CTCFactionMembership::IsMember; landed 0x00897CF0 and 0x00897090 use an old spelling for 0x00487DC0.

Why the rows stalled: the old parks were graded under QFE 4035 and invented their classes (CScriptedShieldCameraAction,
CFractalParams, CEventList, CAnimatedAction), so they could not reuse the landed models of the real owners; nine catalog
labels were propagated from byte-similar functions.
