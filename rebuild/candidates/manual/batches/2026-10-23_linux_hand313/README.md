# Chunk 313 -- notes from the authoring agent

40 rows from the 241-2048-byte band (mostly CAIStateGroup_* Update bodies). VC7.1 RTM 13.10.3077, no QFE.
Result: 7 matched, 28 parked, 5 skipped.

## What matched
- 00f04cc0 CAT_Phase3Archer::Main -- RELOCATION_MATCH (script Main; GetPos __forceinline)
- 00f051d0 CAT_Phase3Guard::Main -- RELOCATION_MATCH (C3DVector user copy ctor)
- 00f045e0 CAT_Sentry::Main -- RELOCATION_MATCH (for(;;) terminate-first loop; implicit handle dtor)
- 00f1a990 CArenaEnemy::Main -- RELOCATION_MATCH (nested inline CCharStringData ==; user byte offset for SIB order)
- 00cfd6c0 CATO_MagicBarrier::Main -- RELOCATION_MATCH (inline const-ref helper keeps IsAlive virtual)
- 009432c0 CAIStateGroup_WorkAtSchool::Update -- RELOCATION_MATCH (time-of-day pointer local; MoveAndFace/GFRandom per arm)
- 00713ee0 CActionFollowCreature::Update -- RELOCATION_MATCH (inline GetCreature accessor for the hoisted arm load; Offset(pos, vel) helper)

## Parked
- 00972f30 CAIStateGroup_JackOfBladesStage2::SetupArena DIFFER(1026v1026): frame slot layout only (masked diff 0) | tried: first-pass variants (see differ/00972f30.cpp header) | next: see Grade line
- 009756b0 CAIStateGroup_JackOfBladesStage3::Update DIFFER(1270v1276): register roles + loop rotation; first pass only | tried: first-pass variants (see differ/009756b0.cpp header) | next: see note; first pass only
- 00940bf0 CAIStateGroup_KidWatchHopscotch::Update DIFFER(1520v1563): register roles, fsqrt order, scheduling | tried: first-pass variants (see differ/00940bf0.cpp header) | next: see Grade line
- 00955620 CAIStateGroup_MazeDefensiveMeleeCombat::Update DIFFER(1635v1647): inline lower_bound regs, def release form, block order | tried: first-pass variants (see differ/00955620.cpp header) | next: see Grade line
- 00921990 CAIStateGroup_MinionFollowThing::Update DIFFER(1797v1651): frame/temporaries, register roles | tried: first-pass variants (see differ/00921990.cpp header) | next: see Grade line
- 009334f0 CAIStateGroup_MinionGuardReactToBribe::Update DIFFER(1357v1347): register roles (&PHero edi vs ebp), one extra frame slot; structure matches | tried: first-pass variants (see differ/009334f0.cpp header) | next: see Grade line
- 00932b50 CAIStateGroup_MinionGuardReactToCriminal::Update DIFFER(1692v1671): frame 0x68 v 0x48, x87 schedule, fps*5 fold, vector arg via temp | tried: first-pass variants (see differ/00932b50.cpp header) | next: see Grade line
- 00936c70 CAIStateGroup_MinionReactToCorpse::Update DIFFER(1031v1037): list end/iterator register roles, x87 2D offset order | tried: first-pass variants (see differ/00936c70.cpp header) | next: see Grade line
- 0092ee10 CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation DIFFER(1030v1059): register roles (this edi v ebp, constant 0x10 CSE in ebp) | tried: first-pass variants (see differ/0092ee10.cpp header) | next: see Grade line
- 00930480 CAIStateGroup_MinionReactToPlayerInteraction::Update DIFFER(1486v1513): register roles, TestTC narrowing, wife double test merged | tried: first-pass variants (see differ/00930480.cpp header) | next: see Grade line
- 009582d0 CAIStateGroup_MinionReactToTrespassing::Update DIFFER(1097v1098): one insn: mov bl,1 hoisted and reused as the dead-bit mask (advisor candidate) | tried: first-pass variants (see differ/009582d0.cpp header) | next: see Grade line
- 00923070 CAIStateGroup_ServeFood::IsRunnable DIFFER(1184v1184): frame slot sharing only (0x21 type word shared with tavern spill); masked 18 | tried: first-pass variants (see differ/00923070.cpp header) | next: see Grade line
- 00f03980 CAT_Phase1Guard::Main DIFFER(1448v1434): handle dtor inline expansion at the fall-through end (implicit: out of line; user: extra vptr store); rest matches | tried: first-pass variants (see differ/00f03980.cpp header) | next: see Grade line
- 0093fcd0 CAIStateGroup_VillageTaskSecuritySweep::Update DIFFER(1052v1053): first pass; register roles + position copy form | tried: first-pass variants (see differ/0093fcd0.cpp header) | next: see note; first pass only
- 0096df50 CAIStateGroup_ScreamerPursueTarget::Update DIFFER(1159v1150): first pass; vector exit path + block order | tried: first-pass variants (see differ/0096df50.cpp header) | next: see note; first pass only
- 0093ef30 CAIStateGroup_VillageTaskPatrol::InitCreaturePosition DIFFER(1176v1161): first pass; copy order + start-node lea + frame | tried: first-pass variants (see differ/0093ef30.cpp header) | next: see note; first pass only
- 00712340 CActionTalkBase::GetCameraSetupPositions DIFFER(1410v1404): switch case tail cross-jump only (advisor candidate) | tried: first-pass variants (see differ/00712340.cpp header) | next: see Grade line
- 0093f3c0 CAIStateGroup_VillageTaskPatrol::Update DIFFER(1182v1185): first pass; register roles + frame 0x160 v 0x150 | tried: first-pass variants (see differ/0093f3c0.cpp header) | next: see note; first pass only
- 009647c0 CAIStateGroup_StealStealableItem::Update DIFFER(1238v1238): register choice: const 0xAA probe registers only (5 insns); advisor by-value FindInterface and null-init levers both worse (whole-function swap) | tried: n24 HasTC word form + same-TU LowerBound; advisor by-value FindInterface helper (s1-s3) and null-initialised out (s4) both flip the whole function | next: none in-lane; register choice
- 009658b0 CAIStateGroup_RecoverStealableItem::Update DIFFER(1276v1276): register choice: pure esi/edi role swap (this v loop result); named/scoped alert fixes regs but moves the false push (+7 bytes); helper, ref-cast, early-return and reference-local levers unchanged | tried: o7..o9, t1-t10: named/scoped alert, forceinline helper, const-ref cast, early-return reshaping, reference local; R&D weighted-count lever tried via early returns and a reference local | next: find a source-level reference that lifts the loop result above this (weighted count) without changing bytes
- 0096b100 CAIStateGroup_WhisperPotionCombat::Update DIFFER(1293v1269): ebx/ebp role swap (boss pointer), x87 copy scheduling | tried: first-pass variants (see differ/0096b100.cpp header) | next: see Grade line
- 0094dd90 CAIStateGroup_RockTroll::Update DIFFER(1754v1761): first pass; block placement of the throwing branch, exit copies | tried: first-pass variants (see differ/0094dd90.cpp header) | next: see note; first pass only
- 00927f30 CAIStateGroup_SetupWares::Update DIFFER(1331v1331): register choice: retry count ecx v eax + one push order; advisor ternary/if-else/scoped-reached levers all worse | tried: r1..r14, u1-u5: ternary, if/else, scoped reached, inverted default | next: register choice; try the R&D weighted-count lever on the retry local
- 00926ed0 CAIStateGroup_OpenAndCloseShop::Update DIFFER(1389v1373): first pass; this ebx v ebp, first exit placement, wait-condition logical-not form | tried: first-pass variants (see differ/00926ed0.cpp header) | next: see note; first pass only
- 0095ef80 CAIStateGroup_OpinionReactionRoot::IsRunnable DIFFER(1536v1528): first pass; register roles, 0x20000 mask CSE, reaction-flag loop regs, frame 0x38 v 0x30 (jump table) | tried: first-pass variants (see differ/0095ef80.cpp header) | next: see note; first pass only
- 00943cf0 CAIStateGroup_WorkInWarehouse::MoveCrates DIFFER(1612v1541): first pass; register roles, list-node spill, frame 0x58 v 0x48, by-value argument temporaries | tried: first-pass variants (see differ/00943cf0.cpp header) | next: see note; first pass only
- 00962d50 CAIStateGroup_VersusRockTroll::Update DIFFER(1611v1679): first pass; x87 term order, call-temporary order, circling block layout | tried: first-pass variants (see differ/00962d50.cpp header) | next: see note; first pass only
- 00963a50 CAIStateGroup_VersusScorpionKing::Update DIFFER(1635v1695): first pass; circling-branch layout, random-roll placement, x87 copy scheduling | tried: first-pass variants (see differ/00963a50.cpp header) | next: see note; first pass only

## Skipped
- 00716de0 CActionTalkBase::Update -- not graded: known lane gap (inline fistp rounding helper, x2)
- 0092da20 CAIStateGroup_MinionSleep::Update -- not reached: 1.8-2 KB row, budget spent on first passes and advisor levers
- 0094c050 CAIStateGroup_SellToBuyer::Update -- not reached: 1.8-2 KB row, budget spent on first passes and advisor levers
- 00915b60 CAIStateGroup_MoveGoodsBase::PlaceGoods -- not reached: 1.8-2 KB row, budget spent on first passes and advisor levers
- 0097bda0 CAIStateGroup_ScorpionKing::UpdateCombat -- not reached: 1.8-2 KB row, budget spent on first passes and advisor levers

## Label corrections
- 0x005CC1B0 = IS_THING_DEAD (true when null or dead).
- 0x007AED10 = CTCBoss::GetPhase (reads +0x18 currentBossPhase).
- 0x00692620 = CCreatureActionBase::GetActionID.
- 0x00906AC0 / 0x00906B00 are two different drain-response action ctors.
- 0x008C78F0 = GetSubDef<rock troll def>; 0x0065F7B4 = GetSubDef<crate def> (labelled CAbilityDef).
- 0x0049D870 = CWorld::GetWorldFrame (labelled GFGetPreMainMemoryUsed); 0x0049D810 = world random-seed getter (labelled CVertexBufferWin32::DoSizeof).
- 0x008FCF40 = CAIStateGroupBase::Terminate (labelled CXMVPlaybackInfo ctor); 0x008FCFA0 = GetPThingWorld (labelled CPlayer::GetCurrentMode); 0x008FCFC0 = GetPWorldSeed/GetPWorld.
- 0x004C73D0 = CThing::GetPosition (labelled CVertexBufferWin32::DoSizeof).
- 0x0094D930 = RockTroll IsStillRunnable (labelled BriarRose).
- 0x00842C70 = place-sale-item action ctor (labelled ShopkeeperGetOut).
- 0x00662010 = creature life-state getter (labelled CPixelShader::CountReferences); 0x006621A0 = IsInCombat (labelled AreDefaultValsApplied).
- 0x00661CC0 = a float constant getter, not GetCombatMinDistance; 0x00658E90 = village GetPatrolTaskForThing (labelled HasPatroller).
- 0x007EE540 = GetVillage / GFGetPVillageInterface (labelled IS_THING_ALIVE<...>).
- 0x005CBDFA = fastcall IsSameThing(CThing*, const CIntelligentPointer&) (labelled CVillageTaskCarryCrate::IsCreatureAssignedToTask).
- 0x007446A0 / 0x0073A6E0 = small interface getters (labelled _Cons_val / CPixelShader::CountReferences).
- 0x00A14040 = C3DVector::RotateZ(float turns) (labelled Math_CartesianToSpherical).
- 0x00927B60 = fastcall sort of CIntelligentPointer<CThing> by compare 0x00926980 (labelled Sort_QuickSort_Main).
- 0x004DF88A = vector<CIntelligentPointer<CThing>> dtor from another TU (labelled CActiveFile::OnReadFinished).
- 0x0088B8F0 = CAIBrain::AddOutput (labelled CQuestManager::RemoveAvailableQuestCardType).
- 0x00799A60 = CTCShopKeeper::GetAreOtherPeopleInShop; 0x0082DE20 is a CThingBuilding money-bag test (labelled CTCBuyableHouse::CountMoneyBags).
- Constants: 0x0129177C = 1.1f, 0x01296368 = 3.5f, 0x0125A690 = 1.2f, 0x3B360A13 = 0.0027777f; 0x0122DEDC is read as a runtime float (0.0f in the file).
