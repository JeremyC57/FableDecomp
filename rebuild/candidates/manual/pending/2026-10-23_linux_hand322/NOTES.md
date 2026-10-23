# Hand chunk 322 (CQ_HeroSoulsNostroScript .. CScriptDef): 28 of 40 byte-exact

## What matched (28)
- CQ_HeroSoulsNostroScript::Main 00d87cb0, CQ_HeroSoulsThunderScript::Main 00d7ae80 -- quest main loops (spawn helpers, WAIT_UNTIL loops); binding model of landed CV_ChickenKickingScript::Main with a forceinline CSpawnedFunc ctor / SpawnFunction.
- CQ_HobbeCaveScript::Init 00d8d640 / WatchForTermination 00d8e990 -- level setup; termination watcher. Literals go straight to `const CCharString&` parameters; every failure reason is a whole SetQuestAsFailed call in its own arm (VC cross-jumps the arms).
- CQ_HobbeToothWhisper::Main 00d9d030 -- a local in an inner `{}` so its dtor runs before the trailing NewScriptFrame.
- CQ_MinionClifftopChaseScript::ProcessMission 00da8510 -- forceinline CTimer accessors; a named def-value local read before the global.
- CQ_NewOakValeIntroScript::DoMission 00dbde40 / PostAttackStuff 00dbeb20 -- spawned-function model; inline ~CScriptThing plus inline_depth(0) after the first site.
- CQ_OakValeBanditRaidScript::Main 00dbf2d0 / PopulateLevel 00dc0300 -- forceinline binding ctor, unrolled frame waits, `for(;;)` idle tail; vector<CScriptThing> model.
- CQ_OpeningGraveyardSecretPassageScript::CheckDoor 00dc8c80 -- `#pragma inline_depth(0)` on selected returns and on the tail.
- CQ_OrchardFarmRaidScript::Init 00dcc140, CQ_PrisonEscapeRescueMotherScript::Init 00dd2720, CQ_TraderEscortScript::Init 00e006d0, CQ_SummoningTheShipScript::DoMission 00df36e0 -- linear script bodies, matched on the first try.
- CQ_RansomVictimScript::MakeNPCComment 00de3f90 -- virtual IsAlive via a `const CScriptThing&` alias (calls on a local object devirtualize); separate identical early returns (tail-merged); forceinline ==/!= through impl slot 78 IsEqualTo.
- CQ_SecretPassageScript::BossBlockingPrisonPassage 00df0200 / PrisonEscape 00df0b70 -- `for(;!term;frame)` outer loop with `if(loaded){...return;}`; forceinline CScriptThing op= over a forceinline counted-pointer op=.
- CQ_UndeadRisingScript::LampChecker 00e0c4c0 -- /GS; inline_depth(0) on the first IsAlive site; WAIT-while with `!= false`.
- CQ_WaspBossScript::DoMission 00e12580 -- inline counted-pointer op= (lib/vecb.inc); inline_depth(0) from the post-wait return; SpawnFunction region as a default `CCharString("")` reference parameter.
- CQ_WizardBattleScript::PhaseTwo 00e1c580 / PhaseOne 00e1cfd0 -- unused ScriptedThingBase local with an implicit dtor; inline_depth(0) on all but the first wait-loop return.
- CRingFighter::Main 00e8b020 -- `while(!IsDone())` with the whole loser path inside the health if; a C3DVector in its own block; forceinline GetPos; `const CScriptDef* def = g_pScriptDef;` for the edi preload; named flag predicate built before CIsThingAlive.
- CQuestManager / CRuntimeQuestPersistence::LoadFromGameStateScript 004b5080 -- landed sibling 0x00CB8440 model; in-loop map reference.
- CRegion::LoadBinary 006bc510 -- inline ReadLong/ReadFloat/ReadBool helpers returning values; RegionDefName set and def lookup inside a forceinline `SetRegionDefName(const CCharString&)` that receives the ReadPresizedString temp.
- CScorpionHome::Main 00d643a0 / 00d67270 -- entity-script model (IsDone 0xF35B30); inline GetPos and SetToKillOnLevelUnload.
- CScriptDef::CScriptDef 00f28f90 -- member list generated from Ego_r.pdb over the landed CParentDefClassBase model.

## Parked for Fable 5.1 (12)
- 00dc08c0 CQ_OakValeBanditRaidScript::WatchForGateTimer DIFFER(1714v1830): retail keeps three separate inline ~CTimer copies where ours cross-jumps | tried: inline_depth(0) on the six post-loop returns, for(;!term;frame) loop | next: per-return inline_depth variants with distinct timer scopes
- 00da71e0 CQ_MinionClifftopChaseScript::GuardChatter DIFFER(1507v1549): ~CScriptThing for the CIsThingAlive temp out of line except at the first return and the loop exit | tried: inline_depth per return | next: inline-cap placement of the temp dtor
- 004b05c0 CQuestManager::TransferBoasts DIFFER(1116v1126): retail stores the three per-boast defaults once per loop preheader after the empty-list guard | tried: 5 loop/local shapes | next: STLport list model with the defaults as a by-value temp
- 004b1d30 CQuestManager::SetQuestAsCompleted DIFFER(1347v1352): register/frame allocation (name in ebp, hero in ebx, shrink-wrapped ebx save, return-false epilogue at the end, dead param slot reused) | tried: early-out shapes, HasTC as masked word, named CWideString | next: GetTC/HasTC as CThing members from a shared header, early-out wrapped around the body
- 004b3ce0 CQuestManager::ActivateQuests DIFFER(1266v1285): this/quests registers swapped, cached `last` in edx with a redundant compare | tried: ~10 set/persist-lookup variants | next: STLport set find model with the real _Rb_tree layout
- 004b5ee0 CQuestManager::SaveGameState DIFFER(1170v1170): callee-saved register assignment only (this=edi vs ebp) | tried: card-loop alias, bool->long ternary, UID split | next: lane gap: register allocation
- 0086bfb0 CRandomAppearanceMorph::TransferBinaryIn DIFFER(1307v1307): `jmp +159` vs `jmp +148` (retail skips the zero temp's free check after erase) | tried: default-arg const-ref and by-value resize, named local, else-only temp | next: CVectorMap with an _STLP_alloc_proxy base so the temp's start is provably zero
- 00556a40 CRedefinerList::RefreshScriptThings DIFFER(1441v1457): `first` flag kept in memory (frame 0x1c vs 0x18), array in ebx, type key cached in ebp, ref-count release via a register copy | tried: cursor local, switch vs if/else, key local, release helpers | next: real CRedefinerList/CComponent headers and the STLport list
- 00a23190 CRightHandedSet::Interpolate DIFFER(366v1508): skeleton only (x87 dot gates, lerp, quaternion slerp, 0xbc frame) | tried: skeleton | next: full x87 model from the C3DVector/CQuaternion landed helpers
- 00a880c0 CSBCCorridor::Process DIFFER(115v1768): skeleton only (x87 steering maths, 8-byte aligned frame) | tried: skeleton | next: full model with the CSBCCorridor.h header
- 00a87960 CSBIAgents::Process DIFFER(115v1824): skeleton only (x87 steering maths, 8-byte aligned frame) | tried: skeleton | next: full model with the CSBIAgents.h header
- 006e5a00 CScriptConversation::FrameUpdate DIFFER(120v1772): ebp frame and fld/fadd 0.5/fistp/fild/fsub 1.0 round-down sequences | tried: skeleton | next: lane gap: inline-asm rounding helper

## Skipped
None.

## Label corrections
- 0xF35B30 "CCreatureAction_TrollWhackGroundBase::Initialise" is CEntityScriptBase::IsDone.
- 0x4AA840 "C3DClothPrimitive dtor" is ~CScriptThing.
- 0x8AC970 "~vector<CConversation>" is ~vector<CScriptThing>.
- 0x7E74D0 "MovieBase dtor" is the ScriptedThingBase dtor.
- 0x7E72A0 / 0x6E7B60 "CCarriedReadableDef ctor" are the scripted-thing handle ctor and the movie ctor.
- 0x4AF740 "CPhysicsMesh::GetNoDummyObjects" is CQuestManager::GetIndexFromRegisteredQuest.
- 0x4AF610 "CQuest_Array_FindByName" is CQuestManager::IsQuestActive.
- 0x4AF5B0 "CScriptBase::OnRegionLoaded" is a CQuestManager active-quest lookup by name (returns the quest; +0x24 cleared).
- 0x4B0C80 "GetActiveQuestCardFromScriptName" returns the quest-card thing (CThing*), not a TC.
- 0x9F0570 "CTCCameraPointScriptedSpline::AddAnimIndexToAvailableAnimList" is CCodeSectionManager::Contains (0x9F02C0 LoadSection is correct).
- 0xCD23B9 "C3DMeshInfo::HasPhysicsMesh" is IsScripting; 0xCD2770 "CMemoryDataOutputStream::Clear" is Release.
- 0x44FC00 "CPersistTraits<float>::TransferBinaryIn" is CDefString::TransferBinaryIn.
- 0x41E5F2 is CFrontEndManager::GetInstance with no arguments (the pushed value is CreateComponent's argument).
- 0x4C73D0 "CVertexBufferWin32::DoSizeof" is CThing::GetPos; 0x5BF2ED "CTCInventoryBase::GetItemDescription" is a bool accepts-item check taking a def index.
- 0x995030 "CPersistTraits<CFloatRange>::TransferBinaryIn" is a stream read of a C2DVector (thiscall on the stream).
- 0x533CC0 "Vector_PushBack_Or_Reallocate" is a CRedefinerList member taking a CCountedPointer<CComponent>.
