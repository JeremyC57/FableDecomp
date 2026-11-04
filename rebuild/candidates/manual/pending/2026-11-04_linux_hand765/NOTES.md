# Chunk 765 notes (82 tiny rows: adjustor / vcall thunks and one-jump forwarders)

Result: 81 graded MATCH / RELOCATION_MATCH (in src/ + payload.json), 1 parked (duplicate symbol only; bytes match). Every row has a
passing g++ behaviour test. Final re-grade sweep of every source on the final mirror: final.tsv (all 82 match).

## Results

| address | name | grade | attempts |
|---|---|---|---|
| 0043e420 | NPlayerGui::CDrawBase::vcall{0} | MATCH | 2 |
| 004ae8a0 | CWorld::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 004c7a50 | CThing::GetThingSearchTools | RELOCATION_MATCH | 2 |
| 004c7a60 | CThing::PeekThingSearchTools | RELOCATION_MATCH | 2 |
| 004c7bc0 | CThing::CheckUID | RELOCATION_MATCH | 2 |
| 00520c70 | CThingManager::ClearPersistenceDataForSection | RELOCATION_MATCH | 2 |
| 00529930 | CThingGameObject::GetThingStandingOn | RELOCATION_MATCH | 2 |
| 0052d700 | NUISystem::CChangingStateComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0052f1a0 | NUISystem::CComponent::IsPositionIndependent | MATCH | 2 |
| 00535ab0 | NUISystem::CComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00535ac0 | NUISystem::CComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0053e8c0 | NUISystem::CMenuEntry::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0053e8d0 | NUISystem::CMenuEntry::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00541850 | NUISystem::CDynamicList::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00541860 | NUISystem::CDynamicList::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00544090 | NUISystem::CParticleEffect::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005440a0 | NUISystem::CParticleEffect::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00546080 | NUISystem::CSpellContainer::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00546090 | NUISystem::CSpellContainer::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005465b0 | NUISystem::CComponentContainer::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005465c0 | NUISystem::CComponentContainer::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00546c00 | NUISystem::CZoomingComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00546c10 | NUISystem::CZoomingComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00546ee0 | NUISystem::CTextContainer::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00546ef0 | NUISystem::CTextContainer::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00547240 | NUISystem::CScrollingComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00547250 | NUISystem::CScrollingComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00547980 | NUISystem::CSwappingStateComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00547990 | NUISystem::CSwappingStateComponent::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00548510 | NUISystem::CMovie::SetMovie | RELOCATION_MATCH | 2 |
| 00548530 | NUISystem::CMovie::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00548540 | NUISystem::CMovie::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054c340 | NUISystem::CListArrow::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054c350 | NUISystem::CListArrow::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054e1a0 | NUISystem::CFrontEndButton::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054e1b0 | NUISystem::CFrontEndButton::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054e560 | NUISystem::CFrontEndScreen::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054e570 | NUISystem::CFrontEndScreen::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054ec50 | NUISystem::CViewport::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0054ec60 | NUISystem::CViewport::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005562d0 | NUISystem::CScrollableList::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005562e0 | NUISystem::CScrollableList::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00557340 | NUISystem::CRedefinerList::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00557350 | NUISystem::CRedefinerList::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 00558b10 | NUISystem::CKeyRedefiner::`vector deleting destructor' | RELOCATION_MATCH | 3 |
| 00558b20 | NUISystem::CKeyRedefiner::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005595a0 | NUISystem::CScrollBarOutside::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005595b0 | NUISystem::CScrollBarOutside::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055ac50 | NUISystem::CDraggable::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055ac60 | NUISystem::CDraggable::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055b830 | NUISystem::CClickable::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055b840 | NUISystem::CClickable::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055c1f0 | NUISystem::CHoverable::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055c8b0 | NUISystem::CMouseCursor::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 0055c8c0 | NUISystem::CMouseCursor::`vector deleting destructor' | RELOCATION_MATCH | 2 |
| 005dbed0 | CAnimComponentFollowingOnAnimFilter::TransferBinaryIn | RELOCATION_MATCH | 3 |
| 005ea000 | CTCTavernGame::GetDef | RELOCATION_MATCH | 2 |
| 00657230 | NPlayerGui::CDrawNPCName::SetName | RELOCATION_MATCH | 3 |
| 00663600 | CThingCreatureBase::ClearQueuedActions | RELOCATION_MATCH | 2 |
| 0066a9d0 | CTCSoundPlayer::SetVoiceName | RELOCATION_MATCH | 3 |
| 00686a90 | CTCBase::GetSerialiseVersion | RELOCATION_MATCH | 2 |
| 00686d80 | CTCBase::GetThingSearchTools | RELOCATION_MATCH | 2 |
| 00686d90 | CTCBase::PeekThingSearchTools | PARKED_DUPLICATE_SYMBOL | 2 |
| 0069e1b0 | CGameCameraManager::GetAngleXY | RELOCATION_MATCH | 2 |
| 0069e1c0 | CGameCameraManager::GetAngleYZ | RELOCATION_MATCH | 2 |
| 006af270 | CMessageEventFilter_CanBeHeard::operator() | RELOCATION_MATCH | 2 |
| 006af780 | CTCPhysicsBase::UpdateInterpolation | MATCH | 2 |
| 006b83e0 | CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags | RELOCATION_MATCH | 3 |
| 006c9fd0 | CTCHero::FrameUpdate | RELOCATION_MATCH | 2 |
| 00700770 | COpinionReactionManager::SetThingOpinionSource | RELOCATION_MATCH | 2 |
| 007021e0 | CTCQuestCard::SetTeleporterRegionName | RELOCATION_MATCH | 2 |
| 00710530 | C3DAnimationTransitionState::FindFirstAnimation | MATCH | 3 |
| 007122b0 | NTCScriptedControl::CActionSummonerLightningAttackTarget::OnActivate | MATCH | 2 |
| 007128f0 | NTCScriptedControl::CActionPlayAnimation::OnActivate | RELOCATION_MATCH | 3 |
| 00712a70 | NTCScriptedControl::CActionPlayCombatAnimation::OnActivate | RELOCATION_MATCH | 2 |
| 007446a0 | CTCPerceiveThing::GetPerceivedThing | RELOCATION_MATCH | 2 |
| 007498d0 | CTCNoise::GetCreator | RELOCATION_MATCH | 2 |
| 00787740 | CTCBob::SetForceVisible | MATCH | 2 |
| 00798d80 | CTCShopKeeper::GetCustomer | RELOCATION_MATCH | 3 |
| 007de4e0 | CTCWallMount::GetObjectAttached | RELOCATION_MATCH | 2 |
| 007de4f0 | CTCWallMount::PeekObjectAttached | RELOCATION_MATCH | 2 |
| 007f6160 | CTCTargetingAI::ClearAllThingsToNotAttack | RELOCATION_MATCH | 2 |

## Parked
- 0x00686D90 CTCBase::PeekThingSearchTools -- RELOCATION_MATCH, parked only because the landed
  `rebuild/src/compiled/00/6a/CTCBase_PeekThingSearchTools_006afd40.cpp` holds the same leaf for 0x006AFD40. That address is
  really `?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ` (Ego_r order IsPhysicsMeshTypeMesh / GetThingSearchTools /
  FrameUpdate = retail 0x006AFD30 / 0x006AFD40 / 0x006AFD60; all four callers are CTCPhysics* methods; a landed caller already binds
  that spelling). Next idea: respell 0x006AFD40 (NAME_FIXES.tsv), then land differ/00686d90.cpp unchanged (drop the Grade line).

## Shapes proven
- NUISystem adjustor thunks `??_E<C>@NUISystem@@W3AEPAXI@Z` (+4, CObserver) / `WBI@AEPAXI@Z` (+0x18, CActionParam): emitted in any TU
  that defines a ctor or dtor of the class (the vtable). Identity from two sources each: the thunk is slot 0 of the +4 / +0x18 vtable and
  its jump target is slot 0 of the primary vtable (??_G). The RELOC_NAME note on the jmp (??_E vs ??_G) is benign.
- CComponent (0x00535AB0 / 0x00535AC0) and CWorld (0x004AE8A0) use the landed ctor / dtor TU as the definer (CComponent(long)
  0x005334A0; ~CWorld 0x004A6AB0); only the thunk is the row (the precedent: landed 0x0054DA50 CFrontEndList).
- `??_9@$BA@AE` (0x0043E420) is THE __thiscall vcall{0} thunk: VC7.1 names it without a class, one COMDAT per program; the definer TU is
  the landed CPlayerGui::ClearGui 0x00438230.
- `mov ecx,[0x013B8A1C] ; jmp` = a member that ignores `this` and calls `CThing::PCurrentThingManager` (private static; public inline
  __fastcall Get/PeekPCurrentThingManager).
- VC7.1 does not devirtualize a call on a member subobject: `TurnToFaceAction.OnActivate()` stays `jmp [edx+8]` (0x007122B0 MATCH).
- An inline virtual base body in a qualified call expands in place (CActionBase::OnActivate `Activated = true` inside the
  CActionPlay*Animation overrides) while its out-of-line copy (0x00711FD0) fills the base vtable.
- STLport `_List_base::clear` out of line: declaration-only explicit specialization in the TU (rows 0x00663600, 0x007F6160).

## Slot names proven
- CTCScriptedControl::CActionBase vtable 0x01265504: 0 ??_G, 1 Update (pure), 2 OnActivate 0x00711FD0, 3 OnAdd 0x00711FE0,
  4 GetActionName (pure), 5 OnCleanup 0x00712000; 0x00711FF0 (`mov byte [ecx+9],1`) is probably Terminate (not a row).
- C3DAnimationStateBase slots after CIVCountedPointeeBase: 2 GetType, 3 FindFirstAnimation, 4 FindDominantAnimation, 5 Validate.
- CTCBase slot 9 FrameUpdate (CTCHero 0x006C9FD0, CTCBob's SetForceVisible virtual call).
- CIDrawWorld: 0 DrawGetProceduralTexture, 1 DrawGetWorldMap, 2 DrawGetEnvironment, 3 virtual dtor (declared last).
- CThingGameObject slot 38 GetThingStandingOn(), slot 39 GetThingStandingOn() const (const overload declared first; reverse order).

## Header overlay ($D/hdr/engine)
New: CTCHero.h, CUserProfileManager.h, CTCBob.h, CTCNoise.h, CTCShopKeeper.h, CTCWallMount.h, CTCTargetingAI.h,
COpinionReactionManager.h, CMessageEventFilter_CanBeHeard.h, CEnvironmentSpellEffectInterpolator.h,
NScreenEffect_CScreenEffectPriorityStack.h, NPlayerGui_CDrawBase.h, NPlayerGui_CDrawNPCName.h,
NTCScriptedControl_CActionPlayAnimation.h, NTCScriptedControl_CActionPlayCombatAnimation.h,
NTCScriptedControl_CActionTurnToFacePosition.h, NTCScriptedControl_CActionSummonerLightningAttackTarget.h, and the NUISystem
classes CDynamicList, CParticleEffect, CSpellContainer, CZoomingComponent, CTextContainer, CScrollingComponent,
CSwappingStateComponent, CListArrow, CFrontEndButton, CFrontEndScreen, CScrollableList, CRedefinerList, CScrollBarOutside,
CMenuEntry, CMouseCursor (NUISystem_<C>.h).
Changed (new macro only; hdrcheck2 default=same and every existing macro same, see SIBLINGS.tsv): CThing.h
(FABLE_RETAIL_CTHING_MANAGERACCESS), CThingManager.h (_ACCESSORS), CTCBase.h (_SERIALISEVERSION), CThingCreatureBase.h (_CLEARQUEUED),
CThingAICreature.h (_MESSAGERANGE), CThingGameObject.h (_STANDINGON), CGameCameraManager.h (_ANGLES), CCamera.h (_ANGLES), CMovie.h
(_MEMBERS), CDefString.h (_TRANSFER), CAnimComponentFollowingOnAnimFilter.h (new retail branch), CTCSoundPlayer.h (_VOICENAME),
CTCQuestCard.h (_TELEPORTER), CTCPerceiveThing.h (new retail branch), CTCTavernGame.h (_GETDEF_OUTOFLINE), CTCScriptedControl.h
(_ACTIONBASE), C3DAnimationState.h (FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS), C3DAnimationTransitionState.h (new retail branch),
NThingManager_CEntityRuntimePersistence.h (_CLEAR), NUISystem_CComponentContainer.h (_CTORS), NUISystem_CManager.h (_MOUSECURSOR).

## Header debt
- NScreenEffect::CScreenEffectPriorityStack is member-less (Ego_r 0x120 bytes of list[3]/set stacks; retail sizes unknown).
- CEnvironmentSpellEffectInterpolator: the two CRadialBlurParams are opaque 0x30 byte arrays.
- NTCScriptedControl::CActionSummonerLightningAttackTarget::PTarget is an opaque CScriptThing (0xc).
- CTCHero / CUserProfileManager / CTCBob model only what the rows use (CTCHero members listed in a comment only).
- C3DAnimationStateBase::EType has no enumerators (MS forward enum).
- NUISystem headers: no virtual overrides beyond the dtor; members only where a row needed them (CMenuEntry, CScrollableList,
  CRedefinerList, CScrollBarOutside, CParticleEffect).

## Callee names used
?Get@CUserProfileManager@@SIAAV1@XZ 0x0040D2A0; ?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ 0x0040BB30;
?RemoveAnimationBlockingModes@NCreatureMode@@YIXAAVCThing@@@Z 0x0085F6A0 (landed);
?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@..@std@@PAX@Z 0x00881A40;
CThingManager GetSerialiseVersion 0x0051E2D0 / IsSerialisingGameState 0x0051E2E0 / PeekThingSearchTools 0x0051E480 /
GetThingSearchTools 0x0051E490 / ClearPersistenceDataForSection 0x00520C70 -> CEntityRuntimePersistence 0x00520390;
CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90; CCharString::operator= 0x0099EFB0; CWideString::operator=
0x0099B7D0; CCamera::GetAngleXY 0x00A0C380 / GetAngleYZ 0x00A0C3E0; CDefString::TransferBinaryIn 0x0044FC00;
CThingAICreature::IsMessageEventWithinHearingRange 0x008332E0; std::_List_base<..>::clear 0x00669B50 / 0x004B8050.

## Name suggestions (one source only; not in NAME_FIXES)
- 0x0053DA30 likely NUISystem::CMenuEntry::GetDataFromDef (catalog InitializeRumbleActions is in neither PDB).
- 0x004379D0 likely CPlayerGui::SetNPCNameDisplay; 0x00657280 likely NPlayerGui::CDrawNPCName::ShowScreenshotMessage.
- NPlayerGui::CDrawBase slot 0 is likely ClearPrimitives (landed ClearGui calls it Clear).

## GOTCHAS one-liners
- An adjustor thunk row (`sub ecx,N ; jmp ??_G`) needs only a TU that defines the class ctor or dtor; reuse the landed ctor/dtor body as the definer when the class is big (CComponent, CWorld).
- VC7.1 never devirtualizes a call through a member subobject (`member.VirtualFn()` stays an indirect `jmp [vtbl+k]`).
- `call Singleton::Get ; mov ecx,eax ; jmp Method` is `Singleton::Get().Method()` from a member whose own copy (the non-virtual wrapper just before it) was inlined.
- Check landed leaves for the symbol before landing a "new" name: 0x006AFD40 held CTCBase::PeekThingSearchTools though Ego_r order makes it CTCPhysicsBase::GetThingSearchTools.
- hdrcheck2.py takes the overlay DIRECTORY ($D/hdr), not a header file; a file argument prints nothing.
- vland warns SHARED_HEADER_AVAILABLE on the adjustor rows whose TU reuses an old landed ctor/dtor body (0x0043E420, 0x004AE8A0, 0x00535AB0/AC0): the local structs are the landed definer's, not this row's; the thunk itself needs none.

## vland (read-only)
vland_result.txt: WINS 81, every row parity MATCH / RELOCATION_MATCH and behaviour PASS. Warnings only: RELOC_NAME on every
adjustor's ??_E jmp (benign alias of ??_G) and on 0x0051E2D0 / 0x0051E2E0 (manifest labels GetMember_*; NAME_FIXES.tsv), and the
SHARED_HEADER_AVAILABLE notes above.
