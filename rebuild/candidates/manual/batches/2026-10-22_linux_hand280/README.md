# Hand chunk 280 (CAIStateGroup_SummonerCharge .. CBRIAR_ROSE_IMPOSTER): 12 of 40 byte-exact

## What matched (12)
All 12 are RELOCATION_MATCH under RTM cl 13.10.3077. The behaviour tests in test/ pass under g++ (tall.sh) and VC7.1 (tall.py).
- 0x0096FCD0 CAIStateGroup_SummonerLightningAttack::Update (568)
- 0x00968BC0 CAIStateGroup_TavernKeeper::OnReplace (570)
- 0x00969470 CAIStateGroup_TavernKeeper::Update (698)
- 0x00942EE0 CAIStateGroup_TellStory::Update (767)
- 0x0096C2E0 CAIStateGroup_Thunder::IsRunnable (524)
- 0x009474F0 CAIStateGroup_WanderAroundShops::Update (647)
- 0x00E6AAC0 CAT_BanditContact::Init (594). The body is really an Acolyte entity script's Init.
- 0x00F08C30 CAT_ConvoyMember::Init (608)
- 0x00E383A0 CAttackingAssassin::Main (547)
- 0x00E1EE30 CAttackingBandit::Main (656)
- 0x00D16DC0 CBH_GangLeader::Init (519)
- 0x00D81820 CBRIAR_ROSE_IMPOSTER::Main (822)

## Parked for Fable 5.1 (23)
- 0096ff10 CAIStateGroup_SummonerCharge::Update DIFFER(628v628): scratch-register rotation only. Structure, frame and size are identical. | tried: if/else of two perform calls (fixed the two-slot frame), named result, named GetPItem | next: an inline accessor for PBrain at the loop head
- 0096f770 CAIStateGroup_SummonerLightningAttack::IsRunnable DIFFER(676v674): frame 0x48 v 0x44. Retail homes the LOS distance float in the +0xc IP-temp word, and loads the camera-manager `this` straight into ecx. | tried: 10 grades (explicit/__forceinline IP ctor, scope blocks, named/inline distance) | next: put the distance float and the IP temporary in one block scope
- 00968fa0 CAIStateGroup_TavernKeeper::FillJug DIFFER(788v788): frame-slot colouring only (the +4/+8 temporaries are swapped). | tried: declaration order and scope blocks | next: type-class ordering of the filter v the name temporary
- 009673b0 CAIStateGroup_TavernServer::OnReplace DIFFER(657v657): esi/edi swap of `this` and the IP vtable constant. | tried: named and inline IP ctor | next: advisor
- 0091f140 CAIStateGroup_TestTestTest::Update DIFFER(956v949): zero register re-materialised at the loop head; found.start in ebx v edi; empty filter tag at +2 v +0x1e. | tried: inline vector copy ctor + _M_insert_overflow stand-in, hoisted Bind2 stores | next: no-user-ctor filter tag sharing a named bool
- 0096a8b0 CAIStateGroup_WhisperPotionCombat::Initialise DIFFER(536v542): the trap goes into edx and is copied to ebx for the IP temporary; two load-schedule swaps. | tried: 8-byte IP with a real virtual base | next: a named trap local
- 0096ac20 CAIStateGroup_WhisperRetreat::JumpToSafety DIFFER(557v557): one PBrain load scheduled inside the fadd sequence. | tried: inline/named GetForward | next: const& forward vector
- 00944730 CAIStateGroup_WorkSawingWood::IsRunnable DIFFER(526v526): eax/ecx scratch swap only. | tried: GetInterface ternary forms | next: advisor
- 00f04000 CAT_Phase1Archer::Main DIFFER(624v630): VC7.1 puts the inlined counted-pointer release at a different site. | tried: __forceinline and non-inline dtor | next: a named counted-pointer copy
- 00871430 NCreatureAction::CActionEventShot::Dispatch DIFFER(859v871): callee-saved permutation; strike-weapon local not homed in the dead arg slot (frame 0x44 v 0x40). | tried: nested ifs for the shared release tail | next: dead-arg-slot homing (lane gap)
- 00871a80 NCreatureAction::CActionEventTriggerParticle::Dispatch DIFFER(547v547): scratch-register rotation after HasHelperPoint. | tried: named/inline this loads | next: advisor
- 00714370 NTCScriptedControl::CActionMoveTo::OnActivate DIFFER(950v948): the uninitialised route pointer takes its own word (frame 0x54 v 0x50). | tried: out-param GetInterface | next: share the key word through the address-taken out slot (chunk 278 idiom)
- 007132c0 NTCScriptedControl::CActionTalkToThing::DisplayText DIFFER(584v583): the copied line text is destroyed after the ToWideString temporaries, not before. | tried: named and temporary text | next: a block-scoped text copy
- 0070e2a0 CAnimateThing::CalcCachedAnims DIFFER(618v618): temporary slot colouring and one scratch register. | tried: scope blocks | next: advisor
- 0070e8b0 CAnimateThing::UpdateFrameLogic DIFFER(893v881): clamped alpha not homed in the dead arg slot; ebx/ebp swap; loop entered by je. | tried: erase-first, ctor-built set info | next: dead-arg-slot homing (lane gap)
- 00a90ae0 N3DPrimitive::CAnimatedBlock::InitialiseFromMeshFileData DIFFER(620v629): ebx/ebp role swap (mesh v zero register), so `cmp dl,bl` becomes a test form; nested weight-loop registers. | tried: 12-byte set, sub-struct bone copy, uninitialised resize temporary, release through a local | next: the mesh as a const& or a named copy
- 005dd090 CAnimationEntry::TransferBinaryIn DIFFER(602v590): the pair temporary's dtor reloads its slot; the key read temporary takes its own slot. | tried: _M_insert_overflow stand-in | next: stand-in reads its const& value
- 005dd440 CAnimationSet::TransferBinaryIn DIFFER(548v545): release form of the counted pointer (3 bytes). | tried: inline release forms | next: release through a local copy
- 00c5d7f0 CAudioWaveOut::Initialize DIFFER(592v592) /GS: ebx/ebp swap only. | tried: byte-array cookie trigger | next: advisor
- 00727220 CApplyAdditionalImpulseIfInCombatAndTooClose::operator() DIFFER(732v732): the length and both scalings read the offset Y in x87 operand order from the dy CSE copy at +0x10; the owner-creature load uses ecx v eax. | tried: 12 grades (explicit bool filter chain, GetXYLengthSquared, operator*=/(/=) by value/const&, explicit statements, operator order, named strength) | next: advisor
- 00726340 CApplyAdditionalPlayerImpulseIfTooClose::operator() DIFFER(729v713): both inline offset scalings keep the scalar on the x87 stack and pop it, where retail consumes it on Z (`fmul [Z]`). | tried: member/void/const&/friend/comma/explicit-statement forms | next: a non-inline scale call that VC7.1 still inlines
- 006b1f30 CAtmosProcess::UpdateInsideBuildings DIFFER(811v811): only the frame layout is wrong (0x7c v 0x44; camera-position temporary +0x54 v +0x2c; bool +0x13 v +0x12); every instruction matches. | tried: stand-ins, named strings, inline helpers (frame 0x50) AdvB3 tried 12 more forms with no match: declaring the vector before the GetPosition call gives the temporary its own slot (frame 0x50) but hoists the zero stores; about 44 bytes of retail's frame are never touched; see probe/advisor/006b1f30/NOTES.md | next: a local struct holding the vector and the temporary, a user-ctor filter
- 00c72200 CAudioSource::UpdateGainState DIFFER(737v770): the catalog row over-captures the next function at 0x00C724F0 (padding plus a getter after the jump tables), so the row cannot match until the catalog is split. Also left: the null-envelope exit shares `xor al,al`, and the Gain stores are cross-jumped. | tried: single-exit and statement order | next: split the catalog row, then retry

## Skipped
These rows are known lane gaps and were not attempted, per the brief:
- 0x00966DA0 CAIStateGroup_TavernServer::GetServingPoint (0v768): inlined lower_bound eax copy at 0x00966FA1.
- 0x00967D40 CAIStateGroup_TavernServer::Update (0v768): three inlined lower_bound searches.
- 0x00945060 CAIStateGroup_WorkHammerAnvil::Update (0v944): inlined lower_bound at 0x009450D0.
- 0x00944940 CAIStateGroup_WorkSawingWood::Update (0v883): inlined lower_bound at 0x009449C0 and 0x00944A70.
- 0x00716B30 CActionTalkBase::SetUpTextAndSpeech (0v688): two inline-asm fistp rounding helpers.

## Label corrections
- 0x00E6AAC0 (CAT_BanditContact::Init) is an Acolyte entity script's Init.
- 0x0099A380 is the CBaseIntelligentPointer constructor.
- 0x005E39E0 is a vector resize twin.
- 0x00C3C360 and 0x00C3C370 are the audio alloc and free.
- 0x0073A6E0 is GetCurrentMode.
- 0x00F35B10 is SetPredicate and 0x00F35B30 is IsDone.
- The GetSubDef label on 0x008E3240 is propagated.
- 0x0096C250 FindHitEvent is labelled as a JackDragon method.
- 0x007AED40 is an interface-0xAD getter.
- 0x00705D00 and 0x00705CC0 are the jug item-id getters.
- 0x009687B0 is called on the TavernKeeper.
- 0x009670A0 is the TavernServer free-serving-point search.
- 0x007EE540 is a method of the 0x23 component (GFGetPVillageInterface).
- 0x009697E0 is a 4-argument nearest search.
- 0x0096BF10 and 0x0096BF90 are WhisperSommersault constructors.
- 0x00844140 and 0x008441D0 are PickUpJug and PutDownJug.
- 0x005DFA50 LowerBound is used for EAnimComponent.
- 0x007119C0 is the list<CAnimSet> range insert.
- 0x004408A0 is the vector<float> InsertGrow.
- 0x00871CC0 is a CCreatureActionBase IP push.
- 0x0040DCE0 is the __fastcall IS_THING_ALIVE.
- 0x00692620, labelled DrawGetEnvironment, is the current action's GetActionCode.
- 0x00970190 is an unlabelled interface-0x42 method (8-byte out, attacker).
- 0x00970240, 0x00970370 and 0x00970420 are unlabelled summoner action constructors.
- 0x00A96E40, labelled "std::list_Copy", is a set/_Rb_tree copy constructor.
- 0x00942920 StandAndAct is landed as (long) but is called with a float.
- 0x009206A0, labelled "Sort_Init", is a fastcall sort with a by-value nearest comparator.
- 0x00C595E0, labelled Math_Exp2, computes e^x (fldl2e first), not 2^x. It is an out-of-line `exp` wrapper, and 0x00C59600 is its `log` twin.
- 0x006B2720, labelled GetPBestThingInRoughArea<IsPotentialCustomer>, is called here as a GetAllThingsInRoughArea (box, empty filter, vector&). The label is propagated.
- 0x009AD3B0, labelled LTextSystem::GetIDFromSymbol, is called on CDefinitionManager with a CCharString type name and returns a def count. The label is propagated.
- 0x004FB490, labelled CTCInventoryBase::GetCategoryWithIndex, is called on the world's slot-12 region list. The label is propagated.
