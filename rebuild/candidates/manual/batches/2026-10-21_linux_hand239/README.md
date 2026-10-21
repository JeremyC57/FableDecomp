# Hand chunk 239 (C2DBoxI .. CAIStateGroup_HandyMan): 26 of 40 byte-exact

## What matched (26)
- C2DBoxI::ClipLineTo (00a39fe0) -- clip a segment against the four box edges in turn; matched once each edge is a named local line.
- C3DAnimFileXLoader::C3DAnimFileXLoader (00aa4710) -- registers the chunk handlers; each handler is a named counted-pointer local (Leaf ??0C3DAnimFileXLoader).
- C3DAnimation2::RelocateData (00a4e960) -- pointer fix-up after a move; multiple inheritance with CMovableResource at +0x2C gives the this-adjust.
- C3DAnimationManager::CombineBlendedBaseAnimations (00a9f2f0) -- blend base animations; an InterpolateFloat helper and a named index local.
- C3DAnimationManager::CombineBaseAnimationsBoneList (00aa0c50) -- bone-list union; a forwarder plus a childDepth parameter reproduce the rotated tail recursion.
- C3DGeneratorObject::LoadBinary (00a2e5b0) -- read the generator fields from the binary stream.
- C3DMesh2::RemapBoneIndices (00a8b820) -- remap primitive bone indices through the map.
- C3DMeshFileXBoundingSphereChunk::ReadFromFile (00aa4e40) -- four floats through ReadFloat helpers; Leaf line needed.
- C3DMeshPartitionBuilderElement::BuildOutput (00ac2a80) -- copy the builder element into the output element.
- C3DMeshPartitionElement::WriteToFile (00ac27a0) -- C3DVector user copy ctor and the GetCenter() temporaries.
- CAIStateGroupBase::AIAction_LookAtPos (0097d580) -- look-at brain output; the flag bools written in the call are hoisted with setg into frame bytes; the creature is a named local.
- CAIStateGroupBehaviourBase::UpdateEmote (00957a20) -- one-ternary GetInterface into ecx and the GetCreature() accessor.
- CAIStateGroup_ApprenticeHero::GetDummy (0092a0c0) -- a CCharString-returning switch helper.
- CAIStateGroup_BanditSitRoundCampfire::Drink (0093a8c0) -- the landed Singsong body; a same-TU LowerBound stand-in lets the fild temporary reuse the key slot.
- CAIStateGroup_BriarRoseWillUse::Update (00956fe0) -- GetSubDef stand-in, def pointer reuse, GetCreature in both arms.
- CAIStateGroup_ChickenPanic::IsRunnable / Update (00911c20, 00911d90) -- stand-ins for the const LowerBound and GetEvent; nested blocks for slot sharing.
- CAIStateGroup_ChickenWander::Update (009118a0) -- the inline world-seed Random and the idle loop.
- CAIStateGroup_CreatureAdvanceInCover::Update (009790f0) -- forceinline IS_THING_ALIVE overloads, GetCreature, the crouch action padded to 0x124.
- CAIStateGroup_CreatureFindCover::Update (0097ccd0) -- enemy alive test, perception re-aware, duck loop on a named fsqrt distance.
- CAIStateGroup_CreatureUseCoverBase::OnActivate (00970610) -- HasAnim through an inline (const CCharString&, const CAnimFilterBase&) group helper (name ctor before the filter vtable store, base vtable restored before ~CCharString, && with ebx flag mask); dialogue def add-ref'd from the raw pointer; creature then world as named locals so the SaySpeech pushes follow GetPWorld.
- CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPos (00970930) -- masked-dword HasTC (`test cl,cl ; jns`) + one-ternary GetTC; the offset spelled per component with the repeated (radius * 0.75f) so the CSE dies on the Z product.
- CAIStateGroup_EatAtTable::Eat (0096c9a0) -- the TU's derived pick-up action with an inline ctor taking the name as an implicit-conversion temporary; Perform-argument action temporaries; frame rate read into a local first and Random spelled through *seed so the div stays ahead of GFGetCurrentFrame.
- CAIStateGroup_GameMaster::IsRunnable (0094c900) -- locally modelled polymorphic base (virtual slots 10/14); work building copied through the inline intelligent-pointer operator=; named interface components before the creature argument; raw-pointer IS_THING_ALIVE; const LowerBound stand-in for the 8-byte frame.
- CAIStateGroup_GoToBed::OnReplace (0092c740) -- release the bed (occupant get/set 0x87C3E0 / 0x87C390).
- CAIStateGroup_GoToBed::IsUnreportedCrimeKnown (0092d060) -- CTCVillageMember::GetPVillage 0x7EE540 and nested ifs for the epilogue.

## Parked for Fable 5.1 (14)
- 00c1d1c0 InvertMatrix3x4 DIFFER(347v347): 7 commutative fmul pairs emit the other operand first | tried: swapped source operand order per product, temporaries, cofactor locals | next: lane gap: fmul operand canonicalisation
- 00c5a9d0 CAudioLibVoice::SetRampParams DIFFER(390v382): /Op adds a rounding store/reload of the int->float volume; without /Op the `/ steps` become reciprocal multiplies | tried: /Op on and off, float vs double locals, conversion placement | next: lane gap: per-TU float-consistency flag mix
- 00a26ad0 C3DAnimationInfo::BuildAnimationInstance DIFFER(383v383): regalloc (sete cl vs al, bank pointer edx vs ecx, first InitialiseAnimationInstance argument order) | tried: inline data-present helpers, frame-byte flag, switch retry loop forms | next: lane gap: regalloc
- 00914460 CAIStateGroup_BirdGroundWander::Update DIFFER(395v395): loop entry `lea edi,[edx+1] ; cmp ; jbe` vs folded `inc ; je` | tried: for/while/do counters, signed/unsigned count, count local | next: lane gap: loop-entry fold
- 00914650 CAIStateGroup_BirdStartled::IsRunnable DIFFER(308v340): retail ebp frame around an inline-asm round (fld 2.5 ; fadd 0.5 ; fistp ; fild ; fsub 1.0 ; dec) | tried: C++ round helper | next: lane gap: inline asm (byte purity forbids __asm)
- 00956e70 CAIStateGroup_BriarRose::Update DIFFER(363v363): sub-def holder in ebx (pushed up front) vs retail edi with ebx pushed mid-function | tried: GetSubDef stand-in, holder forms, decl order | next: lane gap: regalloc
- 0092ace0 CAIStateGroup_ApprenticeHero::GoToLibrary DIFFER(393v389): MoveToPos failure block laid out at the end, 1/|XY| scaling scheduled differently | tried: shared failure tail, reciprocal local, by-value vector ctor | next: lane gap: block layout + x87 scheduling
- 0092ae70 CAIStateGroup_ApprenticeHero::LookAtLeaderBoard DIFFER(354v354): approach-distance global store scheduled inside the by-value vector copy | tried: store placement before/after the vector, named target | next: lane gap: store scheduling
- 00961640 CAIStateGroup_BanditKingThrowWeapons::Update DIFFER(323v323): brain/creature loaded before the target GetPItem (edi) vs retail after (ecx) | tried: named target/creature locals, converting-copy argument forms | next: lane gap: argument evaluation order
- 0091b190 CAIStateGroup_HandyMan::Update DIFFER(332v332): loop-head regalloc in the inlined lower_bound (ecx/eax/edx vs eax/eax/ecx) | tried: GetInterfaceT forms, failed-bool arm, goto done | next: lane gap: regalloc
- 0094a6d0 CAIStateGroup_GoToPub::WalkToPoint DIFFER(368v361): retail keeps constant 1 in ebx and the look-at vector in memory; ours enregisters the vector | tried: goto/early return, bool ok variable for args+return, forward copy local, by-value operator+, plain/one-expression/helper IS_THING_ALIVE, for(;;)+break | next: lane gap: regalloc
- 00977820 CAIStateGroup_Guildmaster::GetBetterTeleportPos DIFFER(374v374): first distance x87 cleanup order (retail fstp st(2) ; fstp st(0) ; fstp [best]) | tried: helper temp/named C2DVector, named dx/dy, double sqrt, separate decl, copy via second variable, function-scope distance, named positions, hoisted iterator | next: lane gap: x87 stack scheduling
- 0097f870 CAIStateGroupBase::AIAction_MinionWander DIFFER(397v397): ebx/ebp swap between navigation and building | tried: declaration order, building != 0, swapped if/else, nested if vs &&, do-while counter, found flag vs goto | next: lane gap: regalloc
- 00a89740 C3DMesh2::GetHelperPointPositionAnimated DIFFER(348v389): x87 spill/copy scheduling of the helper position and the scaled vector | tried: const& and by-value matrix operator, named copy + assignment, combined return, implicit vs user copy ctor, declaration order, branch flip | next: a header CMatrix3x4 point-transform helper taking the vector by value; else lane gap: x87 scheduling

## Skipped
- none (no row holds two functions).

## Label corrections
- 0x00C1D1C0 is not C3DMeshObject::SetInversePositionMatrixFromPositionMatrix: a free InvertMatrix3x4 (__fastcall) in the AI steering library (callers 0xC0B67A, 0xC1BFA2).
- 0x00C5A9D0 is not C3DMeshFileXBoundingBoxChunk::ReadFromFile: an audio-library voice SetRampParams (callers in 0xC5A3C0..0xC5BF00).
- 0x00957210 is the CCreatureAction_BriarRoseIntoInvulnerable ctor (manifest ForcePush is wrong).
- 0x007EE540 is CTCVillageMember::GetPVillage; 0x007AED10 is CTCBoss::GetBossPhase (manifest CWorld::DrawGetEnvironment).
- 0x009FC440 is not CTCActivationTrigger::OnKill (the anim data's resource touch in BuildAnimationInstance); 0x00A242C0 is the mesh bank's GetAnimationInfo.
- 0x00A4CD70 is C3DAnimationSequenceData::RelocateData; 0x00A71550 is a vector insert_overflow.
- 0x0087C3E0 / 0x0087C390 are the bed occupant get / set; 0x007446A0 is CTCPerceiveThing::GetPerceivedThing (manifest _Cons_val is propagated).
- 0x006A4D00 is CThing::GetPosition const (manifest CVertexBufferWin32::DoSizeof); 0x008FCF40 is CAIStateGroupBase::Terminate.
- 0x00A14440 is C3DVector::Normalise (in place; manifest GetScaled is wrong); 0x008FCF90 is CAIStateGroupBase::GetPPlayerManager (manifest CPixelShader::CountReferences); 0x006CB6A0 is the navigation region query (manifest CIsHitEventBlocked::operator()).
