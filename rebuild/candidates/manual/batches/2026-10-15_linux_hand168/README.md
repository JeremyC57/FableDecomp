# Hand chunk 168 (CTCParticleAttachmentTracker .. CTCQuestCard): 30 of 40 byte-exact

## What matched (30)
- CTCPhotojournalCapture: 0x0055D520 ctor (init list in decl order, the CTCBase base carries the vptr so the double does not pad it; 12-byte vector); 0x0055D770 ClosePhotojournalCapture (/GS; CGameEvent 0x43 + GetInterface(0x11)->StartGameRendering; ScreenOpen store shares the event's `xor al,al`).
- CTCPerceiveThing: 0x00744A00 SetThingLineOfSight / 0x00744A70 SetThingExtendedLineOfSight (add/remove the parent from CPlayerGui perceiver drawing via an inline UpdateDrawPerceivers); 0x00744DB0 ResetPerceptionVariables (the three setters inlined with constant false; SetPerceivedThing out of line).
- CTCPhysicalShieldSpell: 0x0079E300 CancelMove (sound string in its own block; the two effect strings are named locals in a block, not temporaries); 0x0079E3A0 Pause (inline IsThingAlive + Kill twice).
- CTCWillLightning 0x007A0400 / NCreatureMode::CModeThunderLightningStorm 0x008DA990 ResumeForCutscene (catalog "CTCPhysicalShieldSpell::CancelForCutscene"): named sound component + CCharString in its own block sharing the key slot.
- CTCPhysicsBase: 0x006AF640 GetBoundingInfo (`centre = C3DVector(...)` with the 3-float ctor only); 0x006AFBF0 GetPhysicsStateFlags (slot 88); 0x006AFD60 FrameUpdate (`Moving || OldMoving` folds to `test al,3`); 0x006B0850 GetPhysicsMeshObject (bank in a named local so GetWorld runs before the index push; inline ReleaseTemporary release); 0x00880220 HasCollidedWithLandscapeAt (named ground float).
- CTCPhysicsRigidBody: 0x00759870 GetPossibleCollidedWithArea (C2DBoxI with a __forceinline memberwise copy ctor for the copy-out); 0x00759900 ConstructFromParams; 0x0075AA80 ctor (vectors with zeroing ctors in decl order, PPhysicsMeshInfo a body store).
- CTCPhysicsStandard: 0x00723570 ConstructFromParams; 0x00723630 SetPhysicsEnabled (slot 36, chained `X = Y = Z = 0`); 0x00723760/0x007237E0/0x00723860 SetFacingAngleXY/YZ/XZ (angles filled member by member in retail store order); 0x00723AC0 UpdateCollisionGameInteractions (`(unsigned char)~flags & 1` keeps the 8-bit not); 0x008E5270 UpdateIsMoving; 0x008E52F0 CalculateNoStepsForStepBasedMovement; 0x008E55E0 UpdateOnSurface (two bitfield stores merge).
- C4DVector 0x00A5B7D0 Interpolate (catalog "CTCPhysicsStandard::GetFacingVector"): `(to - *this) * t + *this` with const by-value member operators (FableWin.pdb names).
- CTCPreCalculatedNavigationRoute 0x00800460 OnInitialActivate (temporary CIntelligentPointer<const CThing> assigned through inline operator=).
- CTCQuestCard 0x00702400 GetQuestSummary / 0x00702630 GetSuccessSummary (the CTCHero::GetTitleGuiText shape).

## Parked for Fable 5.1 (10)
- 0071a9c0 CTCParticleAttachmentTracker::UpdateAttachedParticles DIFFER(111v118): retail this in esi spilled to the push-ecx slot + time in ebp, ours this in ebx and time re-read | tried: GetInterface out-pointer, HasTC/GetTC, self local, /Oa | next: lane gap: register allocation
- 007445e0 CTCPerceiveThing::CTCPerceiveThing DIFFER(128v128): -1 store and ParentThing load scheduled differently | tried: body assignments in retail order, init list, UpdateOffset in init list | next: lane gap: store scheduling
- 007446b0 CTCPerceiveThing::SetPerceivedThing DIFFER(124v124): this edi / &PerceivedThing esi swapped (member address formed before the temp's GetPItem) | tried: temporary, named local in a block | next: same as 00729bc0, try an inline member SetPerceivedThing helper on the pointer
- 00744ed0 CTCPerceiveThing::OnCreate DIFFER(157v157): def out-pointer zero store after the GetDef pushes, def loaded into ebp first | tried: CDefPointer local, plain pointer, zeroing out struct | next: lane gap: CDefPointer zero store after the GetDef pushes
- 006afe60 CTCPhysicsBase::RespondToCollision DIFFER(145v145): edx/ecx response/def swap and Z dot term `fld v.Z ; fmul N.Z` | tried: DotProduct(n,v), DotProduct(v,n), explicit sum | next: lane gap: x87 term order
- 006b05a0 CTCPhysicsBase::CreatePPhysicsMeshInfo DIFFER(112v110): retail ref in edx + type in al, ours ref in eax + type in dl | tried: if/else, switch, type local, mesh local | next: lane gap: register choice
- 0075b2d0 CTCPhysicsRigidBody::OnCreate DIFFER(127v127): ParentThing load hoisted above the last RHSet dword pair | tried: C2DVector temp, member zero stores, pre-declared graphic local | next: lane gap: scheduling
- 008e5750 CTCPhysicsStandard::IsCollisionWithThingToCauseResponse DIFFER(129v127): component `mov eax,[eax+4] ... mov ecx,eax` before the tail jump | tried: GetTC()->F() (no tail call), filter local, GetInterface out-pointer | next: lane gap: looked-up component used as this loaded into ecx
- 007025a0 CTCQuestCard::GetRegionName DIFFER(134v131): inlined strcmp(text,"")==0 gives `sete al ; test al,al` where retail branches on the repe cmpsb flags | tried: if/return and ternary operator== (inline, __forceinline), nested ifs | next: an operator== whose null arm is `return !*text;`-style or an intrinsic-free compare
- 00729bc0 CTCActionUseScriptedHook::ResolveEntranceConnectedTo DIFFER(111v111): `add esi,20h` scheduled before the temp's GetPItem call | tried: temporary assignment, member-reference local | next: lane gap: scheduling (same as 007446b0)

## Skipped
- none

## Label corrections
- 0x007A0400 "CTCPhysicalShieldSpell::CancelForCutscene" is CTCWillLightning (m_lspDef +0x28, LoopingSound retail +0x98), restarts the looping sound: named ResumeForCutscene.
- 0x008DA990 "CTCPhysicalShieldSpell::CancelForCutscene" is NCreatureMode::CModeThunderLightningStorm (PLightningStormDef retail +0xa4, LoopingSound +0xc4), same restart shape.
- 0x00A5B7D0 "CTCPhysicsStandard::GetFacingVector" is C4DVector::Interpolate(const C4DVector&, float) const (FableWin.pdb symbol).
- 0x00702630 "CTCQuestCard::GetQuestSummary" reads CQuestCardDef::SuccessSummary (+0x34): GetSuccessSummary.
- 0x00729BC0 "CTCPreCalculatedNavigationRoute::OnInitialActivate" is a CTCActionUseScriptedHook member called only from its OnInitialActivate (0x00729F60) resolving EntranceConnectedToUID (+0x18) into PEntranceConnectedTo (+0x20).
- CTCPhysicsBase/Standard slot names from the override bodies: 36 SetPhysicsEnabled (0x00723630), 37 IsPhysicsEnabled, 43 SetInteractionFlags(long), 48 GetFeelsGravity, 52 IsImmoveable, 55 IsOnMesh, 56 IsOnGround, 57 GetCollidesWithThings, 58 GetCollidesWithThingsOneWay, 60 GetInteractsWithThings, 88 GetPhysicsStateFlags (0x006AFBF0), 93 GetPhysicsMeshInfo, 3 ConstructFromParams.
- 0x0079DDB0 "CCombatSequence_WillEnflame::OnStart" is CTCPhysicalShieldSpell::StopSound(unsigned long); 0x0079DE00 (landed "CTCBulletTime::PlaySoundW") takes (const CCharString&, bool).
- 0x004378F0 / 0x00437900 "_Cons_val<...>" are CPlayerGui::AddPerceivingThing / RemovePerceivingThing forwarders to CDrawPerceivers (+0x1b8).
- 0x006869D0 "_Cons_val<...>" is CTCBase::GetDefinitionManager (forwards to CThing 0x004C7990); 0x006AF630 "CThingTrackNode::DrawIsDrawable" is CTCBase::GetWorldMap; 0x00A24250/0x00A24280/0x00A242B0 "GetAbilityLevel" are mesh-bank table lookups; 0x00A229E0 "BuildEulerRotationMatrix" is a thiscall CRightHandedSet(const C3DVector&) builder; 0x00A3A150 is C2DBoxI::ValidateOrientationAndFlip.
