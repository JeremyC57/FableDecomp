# Hand chunk 500 (CQ_GuildTrainingMeleeScript::CMeleeOpponent .. CQ_TraderEscortScript::CEndTrader): 40 of 40 byte-exact

All 40 rows are NScript quest-script code: 30 entity-script virtuals (Init slot 2, OnPredicateFail slot 5, one Main slot 1)
and 6 quest-script Init overrides (quest vtable slot 3), plus the rest of the same two families. 9 MATCH, 31 RELOCATION_MATCH,
0 retail_asm. 36 matched on the first grade, 4 on the second. Every test passes under g++ and the lane's VC7.1; the vland dry run
reports 40 wins with no RELOC_IDENTITY / RELOC_DUPLICATE / WARN lines.

## What matched (40)

- Entity scripts (CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, then ParentClass +0x14, PMasterData +0x18): one model for all;
  `Me.MsgIsKilledBy("")` (CScriptThing slot 18) with the CCharString temporary; interface calls through CGameScriptInterfaceBase slots
  from ghidra_out/cgsi_slots_aligned.tsv; parent quest members at retail = Ego_r.pdb - 0x14 (minus a further 4 per Dinkumware
  container before them: CQ_HeroSoulsMotherScript map, CQ_OakValeBanditRaidScript vector).
- Quest scripts (CScriptBase 0x40 + interface +0x40 + master data +0x44): plain member stores in source order.
- Shapes: an empty `if` keeps a dropped call result (0x00D6A080); `if (call) {} else {...}` gives retail's `jne` over the arm (0x00D6AE30);
  nested ifs, not `&&`, after a call with a temporary (0x00E04AE0); a named bool lets the GetDataString temporary die before the store
  (0x00DF2310, 0x00DF2A40); `Game->F(Me, Game->GetHero())` reads the outer vtable before the inner call; the by-value
  `SetIsPushableByHero(CScriptThing, bool)` needs CScriptThing's inline copy ctor over CBaseClass (two vtable stores, counted-pointer add-ref).

- 0x00D56750 NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Init (MATCH): start the melee opponent: the parent's tutorial state goes to 1, BadHit = 0 and RepeatMelee = true.
- 0x00D5AA80 NScript::CQ_GuildTrainingSkillScript::Init (MATCH): reset the skill tutorial: TargetsHit 0, TutorialState 1, GenericTutorialCounter 0, TotalTrainingDummiesCounter 3.
- 0x00D5DC80 NScript::CQ_GuildTrainingWillScript::Init (MATCH): reset the will tutorial: TargetsHit 0, TutorialState 1, both counters 0, TestFinished and BanditsDefeated false.
- 0x00D69EE0 NScript::CQ_HangingTreeEvilScript::CHTE_Prisoner::OnPredicateFail (RELOCATION_MATCH): the prisoner's predicate failed: if it was killed, fail the quest with TEXT_QST_029_FAILURE_PRISONER_KILLED.
- 0x00D6A080 NScript::CQ_HangingTreeEvilScript::CHTE_Executioner::OnPredicateFail (RELOCATION_MATCH): the executioner's predicate failed: ask whether it was killed and do nothing with the answer.
- 0x00D6A480 NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Init (RELOCATION_MATCH): HeroHitMe = 0; make Me and the hero allies both ways and put Me in FACTION_BANDITS_FRIENDLY.
- 0x00D6A4F0 NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::OnPredicateFail (RELOCATION_MATCH): a freedom fighter died: one bandit fewer, flag HangingTreeBanditKilled in the master data and BanditKilled in the parent.
- 0x00D6AE30 NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::OnPredicateFail (RELOCATION_MATCH): an assault guard's predicate failed without it being killed: carry one more guard over.
- 0x00D705D0 NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Init (RELOCATION_MATCH): make Me an ally of the hero, give it the GUARD narrator and a green mini-map orb.
- 0x00D70650 NScript::CQ_HangingTreeGoodScript::CHTG_Guard::OnPredicateFail (RELOCATION_MATCH): a guard's predicate failed: if killed, one guard fewer and HangingTreeGuardKilled; otherwise HTG_State = 1.
- 0x00D71A30 NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Init (RELOCATION_MATCH): put the quest-core orb on Me's mini-map marker and clear GuardsCalledAfterMe.
- 0x00D73280 NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::Init (MATCH): set Me's combat ability to 100.
- 0x00D83F10 NScript::CQ_HeroSoulsMotherScript::CHS_Screamer::Init (MATCH): one more screamer alive in the parent; SuperScreamer = false.
- 0x00D8A700 NScript::CQ_HerosOldHouseScript::CFishermansWife::Init (RELOCATION_MATCH): clear TalkedTo/LeavingHappy, remember Me's facing angle, clear its information flags and, while the hero is helping but has not yet helped, give it the vignette mini-map orb.
- 0x00D8A770 NScript::CQ_HerosOldHouseScript::CFishermansWife::OnPredicateFail (RELOCATION_MATCH): the hero killed the fisherman's wife: take HOH_MoralityLoss morality and flag WifeAttackedAndLeft.
- 0x00D90050 NScript::CQ_HobbeCaveScript::CJames::OnPredicateFail (RELOCATION_MATCH): James was killed: fail the quest (ReasonForFailure 1, MissionFailed) and remove his health bar.
- 0x00D90F40 NScript::CQ_HobbeCaveScript::CBanditSacrifice::Init (RELOCATION_MATCH): reset the AI states, health bar id and comment state, give Me the BANDIT narrator and make it want no money.
- 0x00D90FA0 NScript::CQ_HobbeCaveScript::CBanditSacrifice::OnPredicateFail (RELOCATION_MATCH): the sacrificed bandit was killed: remove its health bar.
- 0x00D9C610 NScript::CQ_HobbeToothContestScript::COrchardFarmFarmer::Init (RELOCATION_MATCH): make the farmer unkillable and give him the quest-core mini-map orb.
- 0x00DA0160 NScript::CQ_MinionCampScript::CMC_SpawnPoint::Init (RELOCATION_MATCH): clear DoneIntroMinion and file Me under the parent's left (data string 0) or right (1) spawn points unless the thread is terminating.
- 0x00DA4540 NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Init (RELOCATION_MATCH): a minion escort spawned while PrisonPath is loaded is special: count it in the parent; otherwise not special.
- 0x00DA4590 NScript::CQ_MinionClifftopChaseScript::CMinionEscort::OnPredicateFail (MATCH): a special minion escort's predicate failed: one minion fewer in the parent.
- 0x00DA4740 NScript::CQ_MinionClifftopChaseScript::CMinionEscortEnd::OnPredicateFail (RELOCATION_MATCH): a guarding minion was killed: count it in the parent's KilledGuardingMinions.
- 0x00DB8290 NScript::CQ_NewOakValeIntroScript::COVI_DeadFather::Init (RELOCATION_MATCH): give the dead father the quest-core mini-map orb and make him not pushable by the hero.
- 0x00DBF1E0 NScript::CQ_OakValeBanditRaidScript::Init (MATCH): reset the bandit-raid state (FirstSetup = true, the rest false/0).
- 0x00DCFA10 NScript::CQ_OrchardFarmRaidScript::CArtefact::Init (RELOCATION_MATCH): count one more crate in the parent, add Me to its crate list and give it the quest-core mini-map orb.
- 0x00DD4A50 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Init (RELOCATION_MATCH): put the accomplice in FACTION_BODYGUARD, stop it following the hero, clear ReleasedAndFollowing and switch its opinion reactions off.
- 0x00DD8850 NScript::CQ_PrisonEscapeRescueMotherScript::CInventoryChest::Main (RELOCATION_MATCH): until the hero has found his weapons, close the inventory chest and put the hero's equipment bag in it.
- 0x00DDAD60 NScript::CQ_PrisonRaceScript::Init (MATCH): reset the race: flags false, speeds and indices 0, nearest-marker distance 1000, conversation index -1.
- 0x00DDC490 NScript::CQ_PrisonRaceScript::CPrisonRacer::Init (RELOCATION_MATCH): RaceWaypointIndex = 0; make the racer unkillable and give it a green mini-map orb.
- 0x00DE1B60 NScript::CQ_RansomVictimScript::CVictim::Init (RELOCATION_MATCH): WaitingForThingsToDie false, ToldHeroOfThingsAttackingHim true, GotOutOfCave false; make the victim not pushable by the hero.
- 0x00DE1C50 NScript::CQ_RansomVictimScript::CRansommer::OnPredicateFail (RELOCATION_MATCH): the ransommer was killed: flag RescuedVictim and RansommerDied and count one more kidnapper killed.
- 0x00DE1DB0 NScript::CQ_RansomVictimScript::CRansomHelper::OnPredicateFail (RELOCATION_MATCH): a ransom helper's predicate failed: if killed, one more kidnapper killed; otherwise RansomHelperSpoken = false.
- 0x00DE25D0 NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Init (RELOCATION_MATCH): make the father's guard not pushable by the hero and reset TimesHit.
- 0x00DEE250 NScript::CQ_SecretPassageScript::CMotherTorture::Init (RELOCATION_MATCH): switch all of the mother's state groups off except SG_MINION_FOLLOW_THING and stop her responding to follow/wait expressions.
- 0x00DF22A0 NScript::CQ_SummoningTheShipScript::CSummonerAttacker::OnPredicateFail (RELOCATION_MATCH): a summoner attacker's predicate failed: one summoner fewer; if it was killed, advance the attack wave.
- 0x00DF2310 NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Init (RELOCATION_MATCH): one more summoner alive; WaveID is 2 when Me's data string is WAVE2, else 1.
- 0x00DF2A40 NScript::CQ_SummoningTheShipScript::CSummonerMinion::Init (RELOCATION_MATCH): WaveID is 2 when Me's data string is WAVE2, else 1.
- 0x00DF4130 NScript::CQ_TentacleKrakenBossFightScript::Init (MATCH): clear MissionSucceeded, MissionFailed, KrakenAutoSaveCheckPoint and the master data's MotherDodgingKraken.
- 0x00E04AE0 NScript::CQ_TraderEscortScript::CEndTrader::OnPredicateFail (RELOCATION_MATCH): the end trader was killed after the ending could start: flag EndStarted in the parent.

## Parked for Fable 5.1 (0)

None.

## Skipped

None.

## Proven

Each name: the row is slot N of the vtable that the class's Alloc (or ctor) stores (tools/vt.py over the retail .rdata + xref),
the Alloc/ctor is already named in the catalog, and the exact mangled name is in both Ego_r.pdb and FableWin.pdb name sets.

00d56750 NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Init
00d5aa80 NScript::CQ_GuildTrainingSkillScript::Init
00d5dc80 NScript::CQ_GuildTrainingWillScript::Init
00d69ee0 NScript::CQ_HangingTreeEvilScript::CHTE_Prisoner::OnPredicateFail
00d6a080 NScript::CQ_HangingTreeEvilScript::CHTE_Executioner::OnPredicateFail
00d6a480 NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Init
00d6a4f0 NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::OnPredicateFail
00d6ae30 NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::OnPredicateFail
00d705d0 NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Init
00d70650 NScript::CQ_HangingTreeGoodScript::CHTG_Guard::OnPredicateFail
00d71a30 NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Init
00d73280 NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::Init
00d83f10 NScript::CQ_HeroSoulsMotherScript::CHS_Screamer::Init
00d8a700 NScript::CQ_HerosOldHouseScript::CFishermansWife::Init
00d8a770 NScript::CQ_HerosOldHouseScript::CFishermansWife::OnPredicateFail
00d90050 NScript::CQ_HobbeCaveScript::CJames::OnPredicateFail
00d90f40 NScript::CQ_HobbeCaveScript::CBanditSacrifice::Init
00d90fa0 NScript::CQ_HobbeCaveScript::CBanditSacrifice::OnPredicateFail
00d9c610 NScript::CQ_HobbeToothContestScript::COrchardFarmFarmer::Init
00da0160 NScript::CQ_MinionCampScript::CMC_SpawnPoint::Init
00da4540 NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Init
00da4590 NScript::CQ_MinionClifftopChaseScript::CMinionEscort::OnPredicateFail
00da4740 NScript::CQ_MinionClifftopChaseScript::CMinionEscortEnd::OnPredicateFail
00db8290 NScript::CQ_NewOakValeIntroScript::COVI_DeadFather::Init
00dbf1e0 NScript::CQ_OakValeBanditRaidScript::Init
00dcfa10 NScript::CQ_OrchardFarmRaidScript::CArtefact::Init
00dd4a50 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Init
00dd8850 NScript::CQ_PrisonEscapeRescueMotherScript::CInventoryChest::Main
00ddad60 NScript::CQ_PrisonRaceScript::Init
00ddc490 NScript::CQ_PrisonRaceScript::CPrisonRacer::Init
00de1b60 NScript::CQ_RansomVictimScript::CVictim::Init
00de1c50 NScript::CQ_RansomVictimScript::CRansommer::OnPredicateFail
00de1db0 NScript::CQ_RansomVictimScript::CRansomHelper::OnPredicateFail
00de25d0 NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Init
00dee250 NScript::CQ_SecretPassageScript::CMotherTorture::Init
00df22a0 NScript::CQ_SummoningTheShipScript::CSummonerAttacker::OnPredicateFail
00df2310 NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Init
00df2a40 NScript::CQ_SummoningTheShipScript::CSummonerMinion::Init
00df4130 NScript::CQ_TentacleKrakenBossFightScript::Init
00e04ae0 NScript::CQ_TraderEscortScript::CEndTrader::OnPredicateFail

## Probable

None.

## Descriptive

None for functions. Two member names are descriptive: the quest scripts' +0x40 `Game` and +0x44 `PMasterData` (the ctor's
CGameScriptInterfaceBase& and CScriptDataBase* arguments; the PDB layouts list those words as base padding).

## Label corrections

- All 40 manifest labels were gap-scan placeholders `sub_<addr>`; the names above replace them.
- 0x00D73220 is the NScript::CQ_HangingTreeGoodScript::CHTG_Executioner ctor (stores vtable 0x012D29B8, called by the Alloc 0x00D731A0); the catalog says "CACompressor::CACompressor".
- 0x00D5DC50 is the NScript::CQ_GuildTrainingWillScript ctor (stores vtable 0x012D1038); the manifest says "CEngineSubPrimitiveBase".
- 0x00DBF190 is the NScript::CQ_OakValeBanditRaidScript ctor; the catalog says "CScript_012d9e18::CScript_012d9e18".
- Landed entity-script GetParentScript bodies (slot 3, `return ParentClass;`) are landed as `T::Get` over a pack(1) pad struct, e.g. 0x00D69ED0 (CHTE_Prisoner), 0x00D6A070 (CHTE_Executioner); PDB spelling `?GetParentScript@<Entity>@<Quest>@NScript@@UBEPAVCScriptBase@@XZ`.

## Callee names

- 0x0099EBF0 `??0CCharString@@QAE@PBDJ@Z`, 0x0099EAE0 `??1CCharString@@QAE@XZ`, 0x0099EFE0 `??4CCharString@@QAEABV0@PBD@Z` (PDB: const-ref return).
- 0x00F35B30 `?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ` (PDB: const; reloc_bindings holds ~40 other spellings for this address).
- 0x00411570 `?Compare@?$CBasicString@D@@KIJPBD0@Z` (protected static __fastcall).
- 0x0099E7F0 `?GFCharStringToInt@@YIJABVCCharString@@@Z`.
- 0x00DA01E0 `std::list<CScriptThing>::push_back(const CScriptThing&)` and 0x008ADF90 `std::vector<CScriptThing>::push_back(const CScriptThing&)`, both spelled with the default allocator argument (PDB form); the landed 0x008ADF90 itself uses a one-parameter `vector<_Tp>` (Review at landing).
- Virtual calls (no relocation): CGameScriptInterfaceBase slots 12, 70, 156, 235, 237, 338, 350, 360, 362, 436, 466, 478, 517, 585, 590, 591, 594, 599, 601, 844; CScriptThing slots 3 GetDataString, 10 GetAngleXY, 18 MsgIsKilledBy.

## Header debt

- engine/CScriptThing.h and engine/CGameScriptInterface.h model the vptr as a data member (`void* __vftable`), so virtual calls cannot go through them; every source keeps a local polymorphic CScriptThing (over CBaseClass) and a slot-filled CGameScriptInterfaceBase, as the landed entity scripts do. CEntityScriptBase, CScriptBase and the quest/entity classes have no engine header.
- Retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54): quest members sit 0x14 below the PDB offsets, and a further 4 below per Dinkumware container.
