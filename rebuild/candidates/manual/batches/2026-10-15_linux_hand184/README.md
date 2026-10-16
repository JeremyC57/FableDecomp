# Hand chunk 184 (CAIStateGroup_SummonerSummon .. CAnimComponentCombatComboChain): 21 of 40 byte-exact

## What matched (21)
- CAIStateGroup_SummonerSummon::IsRunnable (0096e540): MinionReactToEnemy model plus a second HasTC/FindEntry; the converting pointer temporary built by the out-of-line `CIntelligentPointer<const CThing>(CThing*)` ctor must be a named local (an unnamed one reuses the ctor's eax).
- CAIStateGroup_TavernServer::GetJug (00967180): entry-returning FindEntry kept in edi; the creature argument is a named local (its load is scheduled ahead of the component load); returned CIntelligentPointer temporary shares the key slot.
- CAIStateGroup_TavernServer::GetOldJugPoint (009672c0) and CAIStateGroup_TavernKeeper::GetOldJugPoint (00968860): `CCharString name("ACTION_POINT_MAID"/"..._BARMAN", -1); name += "_0"; name += GFIntToCharString(i + 1);` then the by-value name to GetFreeNamedActionPoint with the creature a named local; the keeper twin presets the component to null (`xor ebx,ebx`).
- CAIStateGroup_UsesSeat::IsStillValid (00938960): x87 squared length in retail order `s = z*z; s += y*y; s += x*x` and the 0.1f literal; out-parameter GetInterface with dead-slot miss.
- CAIStateGroup_VillageTaskPickCrops::IsRunnable (00940180): inline villager accessor `if (GetInterface(&v, 0x23)) GFGetPVillageInterface(v); return v;` (the discarded first call), village not null-tested.
- CAIStateGroup_WanderAroundShops::OnReplace (00946e70): out-parameter GetInterface for the shop (eax), HasTC + FindEntry for the idle scheduler (ecx).
- CAIStateGroup_WifeGiveYouGift::IsRunnable (00925f90): GiftToGiveDef stored before its `> 0` test; `s = z*z; s += x*x; s += y*y` vs 1600.0f (`test ah,5 ; jp` is `<`).
- CAIStateGroup_WorkAtSchool::StandAndTalk (00942c70): `long fps = g_FramesPerSecond;` read before the frame call gives retail's fild order; key and component in the dead `seconds` slot.
- CAT_BanditAlly::SwitchBanditAllegience (00f079d0, MATCH): CAT_ConvoySpy model; slot names from ghidra_out/cgsi_slots_aligned.tsv.
- CAction::~CAction (0053fd00): head-only lists with inline clear/dtor; `if (node->value->m_ownership != 1) delete node->value;` -- a cached element local lets VC drop delete's null test.
- NTCScriptedControl::CActionPerformExpression ctor (00905360): CActionFireProjectileWeapon + CM_HiddenBooty CScriptThing models (inline ctor/dtor, operator= from GetScriptThing).
- NTCScriptedControl::CActionTalkToThing ctor, text-name overload (00717aa0): local CArray with an inline dtor built on a by-value `_Destroy(first, last)` helper (the end pointer cached in ebp).
- CActionUseDef::Transfer (006773f0): four inline CPersistContext::Transfer<T> (CActivateQuestDef model) on the engine header.
- CActiveFile ctor (009c4330): CCompressorZlib inline ctor `: CACompressor(GetWorkspaceSize()) { Init(); }` (static size getter), two sentinel lists; layout local past +0x38 (retail zlib object 0x58, lists 4 bytes).
- CActiveQuest ctor (004b0310): two inline CCountedPointer(T*) ctors (0x008C5980 model) with per-type destroy functions; retail layout = PDB - 4 after Regions.
- CAnimComponentCombatComboChain::TransferBinaryIn (005dbb60): two inline CPersistTraits<long> reads (0x007AD7F0 model) on the engine header.
- Landing retries (one graded try each): CAIStateGroup_VillageTaskPickCrops::Cleanup (00940240) with the creature a named local before RemoveThingFromTask; CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding (00938bf0) and its byte twin (009431e0) with `if (building->GetInterface(&comp, 0x21))` (the component lands in eax); CActiveEntity::~CActiveEntity (00cb5b90) with the list cleared through an inline `clear() { erase(begin(), end()); }`.

## Parked for Fable 5.1 (19)
- 009683a0 CAIStateGroup_TavernKeeper::GetPaymentRequest DIFFER(191v191): retail forms &type into the Bind2 slot and loads the creature via eax before the UID ctor; ours keeps &type in edi | tried: inline Bind2 ctor, named IsOfType, aggregate-assigned Bind2 | next: lane gap: scheduling
- 00968460 CAIStateGroup_TavernKeeper::GetPaymentMessage(0x30) DIFFER(172v172): &PPayee edi / creature-filter this ebx swapped | tried: left-grouped chain, named creature filter, right-grouped `a && (b && c)` (fixes the build order) | next: lane gap: register allocation
- 00968510 CAIStateGroup_TavernKeeper::GetPaymentCancelledMessage(0x31) DIFFER(172v172): as 00968460 | tried: same | next: lane gap: register allocation
- 00966cf0 CAIStateGroup_TavernServer::IsCarryingJug DIFFER(174v174): &def pushed before the jug fetch; release vtable via edx (retail eax) | tried: flat locals, block-scoped def, named jug-def local | next: block-scoped def + `PJug.GetPItem()->PDef->` operator-> form
- 009687b0 CAIStateGroup_TavernServer::IsCarryingJug (twin, item id +0x30) DIFFER(174v174): as 00966cf0 | tried: same source | next: as 00966cf0
- 00967930 CAIStateGroup_TavernServer::PickUpJug DIFFER(232v232): ebx/edi swap only | tried: inline creature, named creature, named creature + jug | next: lane gap: register allocation
- 00940820 CAIStateGroup_VillageTaskHorn::IsRunnable DIFFER(164v164): creature arg pushed through edx (retail ecx) | tried: horn local, direct assignment, brain local | next: lane gap: register allocation
- 0093fbc0 CAIStateGroup_VillageTaskSecuritySweep::IsRunnable DIFFER(175v173): `mov edx,ecx ; push edx` v `push ecx` | tried: plain, const param | next: lane gap: register allocation (same as Horn)
- 00943b40 CAIStateGroup_WorkInWarehouse::Cleanup DIFFER(215v215): ebx/ebp swap + base-Cleanup this load order | tried: double-if spill form, named creature local, base call in own test | next: lane gap: register allocation
- 00943c20 CAIStateGroup_WorkInWarehouse::IsRunnable DIFFER(206v206): ecx/edx/eax rotation in the two villager tests | tried: masked-word HasTC, bool HasTC | next: lane gap: register rotation
- 00944db0 CAIStateGroup_WorkHammerAnvil::OnReplace DIFFER(206v206): esi/edi swap only | tried: nested if, single && chain | next: lane gap: register allocation
- 0096a260 CAIStateGroup_WhisperRetreat::FindHitEvent DIFFER(227v232): last search's out/frame slots swapped and the bool re-normalised (`test al,al ; setne al`) | tried: function locals, block hit locals, block shot local (frame fixed) | next: `return GetEvent(...) != 0` with the frame filter named | landing retry: named CIsEventCreatedAfter + `!= 0` -- DIFFER(233v232), out/frame slots still swapped
- 009425f0 CAIStateGroup_WorkAtSchool::MoveAndFace DIFFER(223v218): `reached` homed in the radius arg slot (retail) v a push-ecx byte | tried: inline vector difference + sqrt | next: lane gap: dead-argument-slot temporary
- 00a778d0 CANavQuadTreeNode::SaveToFile DIFFER(207v209): retail re-reads the returned iterator from its slot (`mov eax,[esp+28h]`), ours `mov eax,[eax]` | tried: temp iterator, named iterator, iterator with dtor | next: explicit out-parameter find
- 007e71d0 NTCScriptedControl::CActionMoveTo ctor DIFFER(162v162): second CScriptThing's vtable store after its PImp zeros | tried: struct copy Pos, member-wise copy ctor, separate CScriptThing root symbol | next: lane gap: scheduling
- 00712a80 NTCScriptedControl::CActionPlayCombatAnimation::Update DIFFER(216v216): release via edx and idle-path creature via eax in retail | tried: long param, bool param, early-return tail | next: lane gap: register allocation
- 00717950 NTCScriptedControl::CActionTalkBase ctor DIFFER(198v198): UpdateCounter store placed among the fstp pair in retail | tried: init list, body assignment | next: lane gap: scheduling
- 00717c50 NTCScriptedControl::CActionTalkToThing ctor (text-index overload) DIFFER(283v237): retail folds the STLport push_back capacity test on the fresh local | tried: inline push_back | next: call _M_insert_overflow directly; lane gap: dead-argument-slot tag
- 005c2c65 STLport vector<CCategory>::_M_insert_overflow DIFFER(178v177): __false_type tags in the atEnd slot high byte (retail) | tried: implicit tag ctor, user tag ctor, __forceinline helpers | next: lane gap: dead-argument-slot temporary

## Skipped
- none (no row was already landed; no row covers two functions).

## Label corrections
- 0x00968860 is CAIStateGroup_TavernKeeper::GetOldJugPoint ("ACTION_POINT_BARMAN", keeper offsets), not TavernServer; 0x009687B0 (item id +0x30, keeper address range) is likely CAIStateGroup_TavernKeeper::IsCarryingJug.
- 0x00968460 uses message type 0x30 (payment); only 0x00968510 (type 0x31) is GetPaymentCancelledMessage.
- 0x00967110 FreeBarPoint is called from TavernServer::PickUpJug and sits in the server range (manifest: TavernKeeper).
- 0x00CB5B90 "CActiveEntity::ResetParticleMorphs" is a destructor (frees both vectors; callers 0x004A83A0 scalar-deleting dtor and 0x004A9B70 delete helper) with the reset inlined.
- 0x005C2C65 "CActiveFile::AllocateAndInitializeCategoryNodes" is STLport vector<CCategory>::_M_insert_overflow.
- 0x00969760 / 0x0091A670 "CCOE_AltarEvil::Main" are CAIStateGroup message waits (WaitForMessage<Filter>(mode, filter, &event)).
- 0x0096BAC0 / 0x0096BB70 share one GetEvent<CEventHitBy, Bind3<HitByMelee,...>> label; one is the ranged twin.
- 0x007FDF60 CTCAISummon::GetMaxCreatureCount; 0x008FCF90 is the state group's player-manager accessor (manifest CPixelShader::CountReferences); 0x00A3A7D0 is CCompressorZlib::GetWorkspaceSize (`xor eax,eax`), 0x00A3AA10 CCompressorZlib::Init; 0x004CEA60/0x004CEBB0 GetMessageFromTextIndex/GetMessagesFromTextName; 0x0071A160 CArray<CSubtitledMessage>::operator=; 0x004D1DB0 ~CSubtitledMessage; 0x0040FEF0 is used as CPersistTraits<ETutorialCategory>::TransferBinaryIn.
- Real CGSI slot names for CAT_BanditAlly: 361 ClearThingHasInformation, 600 EntityUnsetThingAsAllyOfThing, 597 EntitySetThingAsEnemyOfThing, 338 RemoveQuestInfoElement, 230 EntityStopFollowing, 235 EntitySetAsAllowedToFollowHero.
