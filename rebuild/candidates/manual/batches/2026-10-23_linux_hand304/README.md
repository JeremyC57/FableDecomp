# Hand chunk 304 (CTCKraken .. CTCQuickAccessMenu): 12 of 40 byte-exact

## What matched (12)
- 007f08b0 CTCKrakenTentacle::OnInitialActivate -- DoDieFX-style model, named thingDef local.
- 00752640 CTCLook::InitialiseHeadAnims -- CDefPointer operator-> for PAppearanceDef.
- 00753c70 CTCLook::vfunc_9 (FrameUpdate) -- out-param GetInterface + same-TU SetHeadTurnState stand-in.
- 007cc4d0 CTCMultiArrow::Init -- named sound string, out-param GetInterface(0x10).
- 007ccb00 CTCMultiStrike::StartMultiHit -- out-param GetInterface(0x68).
- 0080f870 CTCNymph::StopFlyingAround -- __forceinline IsAlive (the 4th use is outlined otherwise).
- 0080fad0 CTCNymph::FrameUpdate -- named block-local CIntelligentPointer temporaries.
- 0080f270 CTCNymph::PickNewTargetPos -- named def local; size() twice keeps the inlined range-zero test.
- 007f11d0 CTCKrakenTentacle::PerformStrikeDamage -- C3DVector origin(x,y,0) then .Z assign keeps Z on x87.
- 0072e290 CTCMeshAutomaticEntityCreator::CreateChildThings -- inline STLport list/vector dtors, _Destroy(first,last) by value.
- 006b0250 CTCPhysicsBase::GetPositionInterpolator -- __forceinline Interpolate, no user C3DVector operator=.
- 007f8890 CTCParticleAttacher::AttachParticles -- if-form two-bitfield bool, named appearance, `if (count > 0) for (i != count)`.

## Parked for Fable 5.1 (25)
- 005e4430 CTCQuestCompletionUI::Activate DIFFER(775v775): 18 difflines, eax/ecx naming at both GetDef call sites | tried: by-ref / T*& out, named mgr, inline wrapper, named / const& index, operator[] | next: near-miss sweep
- 00724010 CTCPhysicsStandard::OnCreate DIFFER(558v556): al vs dl for graphic type byte (18 difflines) | tried: named byte local, bool forms | next: bitfield read form
- 0075a530 CTCPhysicsRigidBody::SetPhysicsMeshBankIndex DIFFER(611v611): x87 operand load order in one product (6 difflines) | tried: operand orders | next: sequenced statements
- 00724630 CTCPhysicsStandard::SetPhysicsMeshBankIndex DIFFER(749v741): RMW bitfield clear + x87 load order (20 difflines) | tried: bitfield/byte forms | next: RMW lane gap
- 0080f5e0 CTCNymph::StartFlyingAround DIFFER(650v650): register rotation in GetDummyObject call + GetPosition copy (26 difflines) | tried: named locals | next: inline accessor
- 006b08f0 CTCPhysicsBase::OnSerialise DIFFER(564v565): x87 load order of marker LengthSquared + map-number scheduling (27) | next: sequenced dot
- 00726980 CTCPhysicsNavigator::PerformForcedMovement DIFFER(609v595): step-copy x87 forwarding + loop-head alignment (34) | tried: optimize pragmas | next: /Os TU question
- 007f0c70 CTCKrakenTentacle::TrackTarget DIFFER(718v715): ebp/edi swap + x87 dist fold + final fadd order | next: named distance
- 007f0f40 CTCKrakenTentacle::ReturnToDesiredPos DIFFER(561v559): frame slot swap dist/angle + x87 order (55) | next: scope blocks
- 007e8590 CTCOwnedEntity::SetPOwner DIFFER(645v650): constant CSE picks 0x21 vs mask 2 in ebx (70) | next: separate flag stores
- 006af7d0 CTCPhysicsBase::ImpulseRespondToCollision DIFFER(614v612): x87 axis caching via &axis memory operands (130) | next: const& axis
- 007243c0 CTCPhysicsStandard::UpdateVelocityForImpassability DIFFER(620v617): loop-head alignment + frame layout (140) | next: /Os question
- 00754050 CTCLook::UpdateTurningAnims DIFFER(613v588): ebx/ebp role swap, frame 0x24v0x20, push_back shape (156)
- 00766660 CTCObjectAugmentations::AttemptToStartParticle DIFFER(700v696): esi/edi swap + `or byte ptr` RMW lane gap (160) | tried: byte/bool/dword bitfields, member ref, optimize("s")
- 007e6450 CTCOracleMinigame::UpdateTints DIFFER(748v756): /GS + jump table; appearance register + ftol order (112)
- 0055d810 CTCPhotojournalCapture::OnCaptionEntered DIFFER(944v998): path tail wide-string helper chain unresolved (336)
- 0055d180 CTCPhotojournalCapture::TakeQuickScreenshot DIFFER(685v772): /GS; coarse, frame sizes + wide path chain (145)
- 0079deb0 CTCPhysicalShieldSpell::PlayEffect DIFFER(921v925): stack-home swap def pointer/effect types + outlined op= (275)
- 0079e5f0 CTCPhysicalShieldSpell::ctor DIFFER(892v916): jump table, no EH frame; colour arms cache vector begin (277)
- 0075aaf0 CTCPhysicsRigidBody::CheckForCollisions DIFFER(607v593): coarse; slot reuse, loop alignment, x87 scheduling (163)
- 0075b000 CTCPhysicsRigidBody::InitialiseBody DIFFER(861v709): coarse; inertia/config x87 + zeroing shape (222)
- 00759f80 CTCPhysicsRigidBody::Integrate DIFFER(1416v975): coarse; inline matrix ops build temporaries / rep movsd (457) | next: model CMatrix3x4 ops out of line
- 0075a7a0 CTCPhysicsRigidBody::ResolveCollisions DIFFER(672v726): coarse; this/config register homes, strength-reduced normal offset (312)
- 00726ce0 CTCPhysicsNavigator::Move DIFFER(1002v1011): frame and compare forms match; FastInvSqrt uses ebx/edi (retail edx/ecx) so edi is pushed in the prologue (227) | tried: copy ctor, flags byte, oldPos reuse, element-wise init
- 00625af0 CTCQuickAccessMenu::GetMagicListUI DIFFER(963v989): coarse; shape right, ebx/ebp swap and name-format temporaries (226)

## Skipped
- 007cd390 CTCMultiStrike::CTCMultiStrike -- row spans the ctor + jump table + three more functions.
- 007fc9d0 CTCKraken::FrameUpdate -- row spans FrameUpdate + jump table + following functions.
- 007e6750 CTCOracleMinigame::FrameUpdate -- row spans several functions after a jump table.

## Label corrections
- 0x007F87A0 is GetSubDef<CParticleAttacherDef> (pushes "CParticleAttacherDef"), not GetSubDef<CAICreatureWillPowerIndicatorDef>.
- 0x004BE000 GetNoDummyObjects; 0x004BDE90 GetNoHelperPoints.
- 0x00449960 CPlayerManager::GetMainPlayer const (engine_api "Catch@0040d959" wrong).
- 0x0049D870 GetWorldFrame (static), not GFGetPreMainMemoryUsed; 0x00686A70 GetWorld (labelled GetDisplayEngine).
- 0x0044C6B0 CDefinitionManager::Get (fastcall), not GFGetPreMainMemoryUsed.
- 0x005EDE93 CTCInventoryAbilities::GetAbilityLevel; 0x00662110 CThingCreatureBase::MakeNoise.
- 0x0079DE00 shield-spell PlaySound (labelled CTCBulletTime::PlaySoundW).
- 0x006E11F0 CTCDParticleEmitter::Create(long,long,float,const C3DVector&,bool); 0x0072AB70 CreateParticleEffect(long,const C3DVector&) fastcall.
- 0x0075BC40 GetAllThingsInRoughArea; 0x0063AB10 CWorldMap::GetGroundFaceNormalAt; 0x006AFD40 search-tools getter (labelled GetCombatManager).
- 0x0099B510 is ~CWideString (used on GetQuestSummary results), 0x0099AED0 CWideString(), 0x0099B6A0 CCharString->CWideString conversion (labelled with a CPredicate_And template); 0x0099E900 CCharString::operator!= (labelled less<CCharString>::operator()).
