# Hand chunk 260 (40 rows: 19 matched, 21 parked, 0 skipped)

## What matched
All RELOCATION_MATCH; each has a behaviour test that passes under g++ and VC7.1.
- 00410120 CActionInputControl::TransferBinaryIn
- 0062f3b0 CCombatActionBase::CCombatActionBase (copy ctor)
- 0070c590 CAnimateThing::PreloadAnims
- 00714730 NTCScriptedControl::CActionMoveTo::OnCleanup
- 00715710 NTCScriptedControl::CActionFollowCreature::OnActivate
- 00887830 CBlendedEnvironmentTheme::BlendTextureSets
- 009cd480 CBankFile::CBankFile
- 009ce8d0 CBankFile::BeginUpdateEntries
- 00a8e8a0 CBaseBlock::LoadBinary
- 00a8eb10 CAnimatedBlock::LoadBinary
- 00b6d0b0 CRetailVector_SRecord24::InsertGrow (catalog CActiveFile::ExpandAndFillVertices)
- 00c0df50 CString_00c0e0f0::Mid (catalog CCharString::SubstringToNewString), Flags: /EHsc, Leaf: Mid
- 00c71430 CAudioMixer::EnumerateAndOpenDevice, Flags: /GS
- 00e1af30 CChasingScreamer::Main
- 00e7fb80 CBountyHuntActivationChecker::RemoveFishermen
- 00e9dac0 NScript::CBriarRose::Init
- 00ef3200 CBeggar::Main
- 00ef3aa0 CCham::Main (InMap1/InMap2 walker)
- 00f083a0 NScript::CAT_Phase1Guard::Init

## Parked for Fable 5.1
- 006f2c00 CAttackerPositionEvaluator::EvaluateAttacker DIFFER(462v468): first-pass model; frame 0x2c with a saved side local and a dead target GetPosition, FP schedule | tried: 2 models | next: model the dead GetPosition temp and the side local
- 006f3a30 CBinaryTree_Alt::Rebuild (really an STLport list::sort) DIFFER(496v481): retail keeps carry.Node in ebx, frame 0x10c | tried: counter[64] merge sort with inline splice/merge | next: try the counter array as list<T> objects with _M_node members and an out-of-line merge
- 0070c900 CAnimSet::GetMeshAnim DIFFER(457v455): inline C3DAnimationState base stores are scheduled before the fstp; &Anims[1] becomes add esi,2 | tried: 6 variants | next: initialisation order of the state base vs time
- 0070cad0 CAnimSet::GetLastMeshAnim (catalog GetStartMeshAnim) DIFFER(405v405): register choice only (alpha load into dead esi) | tried: 5 variants | next: alpha as a named local before the vcall
- 0070cc70 CAnimSet::GetStartMeshAnim DIFFER(405v405): same gap as 0070cad0 | tried: 5 | next: same as 0070cad0
- 0070ce10 CAnimSet::UpdateDistanceMoved DIFFER(475v495): the empty allocator temp gets its own frame byte (retail reuses the arg slot); speeds store then reload | tried: 2 | next: vector built through a different ctor
- 0070d580 CAnimateThing::SetMainAnim DIFFER(512v504): argument type is not CCompleteAnimation (+8 null test, flags at +0x20) | tried: 1 | next: identify the argument class, for_each 0x7100A0
- 0070e710 CAnimateThing::CAnimateThing (catalog CAnimSet::Ctor) DIFFER(404v416): edi=0 / ebx=blend state, push-ecx frame, delayed-queue push_back shape | tried: 1 | next: member init order and the delayed queue type
- 00712e40 CActionTalkBase::GetNextCameraTagIndex DIFFER(464v464): register allocation only (or esi,-1 order; Kind store temps) | tried: 5 | next: loop rotation / local declaration order
- 00715110 CActionFireProjectileWeapon::OnAdd DIFFER(505v477): inline-once dtor band (first ~CCountedPointer out of line 0x48C5C0) | tried: 3 | next: lane gap; advisor
- 00726160 CApplyAdditionalImpulseIfInCombatAndTooClose::operator() DIFFER(473v469): frame matches; Contains operand load order, Y*Y/X*X slots, sign store schedule | tried: 5 | next: expression order of the length-squared
- 00870e00 CActionEventTriggerParticle::SetDataFromString DIFFER(438v438): esi vs ecx load register only | tried: 5 | next: same as 00871290
- 00871290 CActionEventExplosion::SetDataFromString DIFFER(408v408): esi vs ecx load register only | tried: 6 | next: where the token string pointer is held
- 009a08b0 CCharString::FormatDouble DIFFER(458v491): retail ebp=0 / esi block across the strip loop, cross-jumped Format, inline release twice | tried: 3 | next: zero as a named local
- 009c38c0 CActiveFile::OnWriteFinished DIFFER(414v409): MakeBackupName 0x9C33A0 is an LTCG esi-result callee (lane gap) | tried: 2 | next: none in lane
- 00a24d70 CAnimationData::BuildSharedAnimation DIFFER(413v414): bank/decoder caching, hidden-result slot for Decode 0xA99510 | tried: 2 | next: by-value result temp shape
- 00a60fb0 CChecksumCacheFile::GetChecksum DIFFER(412v411): register permutation only | tried: 5 | next: advisor
- 00a90040 CAnimatedBlock::ConvertToVBFormatBump DIFFER(384v403): retail stack temporaries for position/normal | tried: 1 | next: C3DVector temporaries with user copy ctor
- 00c4dc40 CAudioBuffer::Create DIFFER(451v446): GetProcAddress dllimport not cached in ebp by the lane | tried: 3 | next: lane gap
- 00dcdc50 CArtefact::Main DIFFER(423v454): inline-once dtor band | tried: 2 | next: lane gap
- 00ef84d0 CCham::Main (Leader follower) DIFFER(415v499): everything up to SetPredicate matches; retail inlines ~CScriptThing once on the early return and calls 0x4AA840 at the end | tried: 2 | next: inline-once dtor band (lane gap)

## Skipped
None.

## Label corrections
- 0x00B6D0B0 is the STLport vector growth insert (InsertGrow), not CActiveFile::ExpandAndFillVertices; 0x00B6D250 is its deallocate helper (propagated label CActiveFile::OnReadFinished).
- 0x00C0DF50 is MFC-style CString::Mid, not CCharString::SubstringToNewString.
- 0x006F3A30 is an STLport list::sort, not CBinaryTree_Alt::Rebuild.
- 0x0070CAD0 is GetLastMeshAnim (reads the last values), not a second GetStartMeshAnim.
- 0x0070E710 is the CAnimateThing ctor, not CAnimSet::Ctor.
- 0x004C9D60 is CThing::AddTC(const CCharString&, bool, const CPhysicsSnapshot&).
- 0x0085F3E0 IsAbleToMove and 0x0085F600 RemoveMovementBlockingModes are __fastcall(CThing*).
- 0x009C33A0 MakeBackupName is an LTCG esi-result callee.
- 0x0099B6B0 is CWideString(const char*); 0x0099AED0 is CWideString().
- 0x007119C0 is list insert-range.
- 0x009CCB20 is CBankFile::WriteZeros(unsigned long) (manifest label propagated); 0x009CD480 helper label CBankFile_ParseHeader is propagated.
- 0x009D1EE0 is CCountedPointer<CDiskFileWin32>::Reset.
- 0x00A60C90 is CChecksumCacheFile(long bufferSize); 0x00A608F0 is map find (catalog BinaryTree_FindByKey_LeftBias); 0x00A617E0 is map operator[] (catalog TreeNode_GetByKey).
- 0x00995030 is CMemoryDataInputStream::ReadC2DVector (by-value).
- 0x0062F610 is the list<CCountedPointer<CStrikeEvent>> copy ctor (catalog std_list_Initialize_Window).
- 0x00F08590 (catalog Script_Hero_Morphs_Thread) is CQ_AmbushTradersScript::WatchForEnemyDeath.
- 0x00F35B30 is CEntityScriptBase::IsActiveThreadTerminating (catalog CCreatureAction_TrollWhackGroundBase::Initialise).
- 0x00E9DC80 is CV_IntroductionToTrophiesScript::WatchForAttack.
- 0x00EF7AB0 is ~CIsThing; 0x00EF8740 is the CAnd(filter, filter) ctor; 0x007E7320 / 0x007E7450 are non-virtual move/is-moving methods on the scripted-thing handle.
