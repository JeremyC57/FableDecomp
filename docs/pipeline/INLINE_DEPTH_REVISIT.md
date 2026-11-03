# Revisit list: `#pragma inline_depth` rows and two-view CScriptThing rows

Owner decision (2026-11-02): **keep** these landed rows. They are byte-exact and count as coverage, but each one is marked so it can be
redone when someone finds how retail actually produced its code.

## Why they are marked
(Superseded 2026-11-03: VC7.1 decides per call site from c2's static block frequency; see the last section. The text below is the 2026-11-02
understanding.) VC7.1 makes the plain-`inline` decision for a destructor (above all `~CScriptThing`, the quest-script handle) once for the whole function: every
exit expands it inline, or none does. Retail often mixes the two in one function: some exits have the cleanup inline, others call
`??1CScriptThing@@UAE@XZ` 0x004AA840. The only source shape found that reproduces this is a `__forceinline` destructor with
`#pragma inline_depth(0)` ... `#pragma inline_depth()` around the exits retail calls out of line (GOTCHAS "Mixed inline/out-of-line
~CScriptThing"). The pragma emits no code. The bytes are retail's, but Lionhead almost certainly did not write these pragmas. The real
mechanism (a compiler setting, the header's dtor definition, or the TU layout) is still unknown. The chunk 694 pilot re-authored two rows
pragma-free with 13 probes each and matched neither (0x00EF5CB0 706v817, 0x00EF87D0 1047v1003; `batches/2026-10-27_linux_hand694/README.md`).

The two-view rows reach the same inline/out-of-line mix a different way: each one models CScriptThing as two classes, one with an inline copy
and one with the out-of-line copy 0x004ABE90 and dtor 0x004AA840. Retail had one class (naming wave 30, `DECISIONS_W30_RESOLVED.tsv` D1).

## How they are marked
Line 2 of every such source is a marker comment, so the list stays correct through renames:
- `// Revisit: inline-depth -- ...` (129 rows when marked; 90 after landing 2026-11-03d redid 39 pragma-free)
- `// Revisit: two-view-CScriptThing -- ...` (17 rows)

List them at any time with `grep -rl '^// Revisit: inline-depth' rebuild/src` (or `two-view-CScriptThing`). The tables below are the snapshot taken
when the markers were added (rechecked with the markers in place: all 146 RELOCATION_MATCH + PASS, `rebuild/candidates/manual/revisit/2026-11-02_inline_depth/recheck_grades.tsv`).

## Rules
- No new row may land with a `#pragma inline_depth` region or a second CScriptThing view (EXTRA_RULES; "no modelling device retail never had").
- When a pragma-free shape is found for one row, try it on the whole family and record the result here. A redone row drops its marker in the same commit.
- Five sources mention `inline_depth` only in comments (0x00A64910, 0x00DD1AF0, 0x00E1CFD0, 0x00E9BBC0, 0x00F34160). Migration wave 8 or a parked
  note left the word there. They carry no directive and are not marked.

## inline-depth rows still marked (90 of the original 129)
| Address | Function | Source |
|---|---|---|
| 0x005518E0 | `005518e0 NUISystem::CTable::ConstructHorizontalLine.` | `rebuild/src/compiled/00/55/NUISystemCTable_ConstructHorizontalLine_005518e0.cpp` |
| 0x00551BC0 | `00551bc0 NUISystem::CTable::ConstructVerticalLine.` | `rebuild/src/compiled/00/55/NUISystemCTable_ConstructVerticalLine_00551bc0.cpp` |
| 0x0066BD00 | `CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d)` | `rebuild/src/compiled/00/66/CTCSoundPlayer_PlayAttachedCriteria_0066bd00.cpp` |
| 0x0066BDF0 | `CTCSoundPlayer::PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool atta` | `rebuild/src/compiled/00/66/CTCSoundPlayer_PlayUnattachedCriteria_0066bdf0.cpp` |
| 0x00715110 | `Levers: in-body `#pragma inline_depth(0)` around the reload `Actions.Add(CCountedPointer(new ...))` keeps that` | `rebuild/src/compiled/00/71/NTCScriptedControlCActionFireProjectileWeapon_OnAdd_00715110.cpp` |
| 0x009F7890 | `std::_Construct(CVsyncCallbackTimer::CPendingQuery* p, const CVsyncCallbackTimer::CPendingQuery& val)` | `rebuild/src/compiled/00/9f/STL_Construct_009f7890.cpp` |
| 0x00BC9B80 | `CEngineTreeFilerBase::FindID(unsigned long* key)` | `rebuild/src/compiled/00/bc/CEngineTreeFilerBase_FindID_00bc9b80.cpp` |
| 0x00CE0600 | `CMagicBarrierGraveyard::Main() (0x00CE0600)` | `rebuild/src/compiled/00/ce/CMagicBarrierGraveyard_Main_00ce0600.cpp` |
| 0x00CE0920 | `CMK_HS2_SOUL::Main()` | `rebuild/src/compiled/00/ce/CMKHS2SOUL_Main_00ce0920.cpp` |
| 0x00CE0B00 | `CMK_HS2_SOUL::Main()` | `rebuild/src/compiled/00/ce/CMKHS2SOUL_Main_00ce0b00.cpp` |
| 0x00CE0CE0 | `CMK_HS2_SOUL::Main()` | `rebuild/src/compiled/00/ce/CMKHS2SOUL_Main_00ce0ce0.cpp` |
| 0x00CE1280 | `NScript::CPersonalScript_GlobalThingsScript::CTestGateSwitch::Main()` | `rebuild/src/compiled/00/ce/CTestGateSwitch_Main_00ce1280.cpp` |
| 0x00CEF3B0 | `00cef3b0 NScript::CGameflowScript::CoreQuestReminder` | `rebuild/src/compiled/00/ce/NScriptCGameflowScript_CoreQuestReminder_00cef3b0.cpp` |
| 0x00CF02A0 | `CGameflowAssistanceScript::Tutorial_RenownLevelIncrease()` | `rebuild/src/compiled/00/cf/CGameflowAssistanceScript_TutorialRenownLevelIncrease_00cf02a0.cpp` |
| 0x00D043A0 | `Leaf: CheckForSecondAreaDoorHelp` | `rebuild/src/compiled/00/d0/NScriptCQBanditCampScript_CheckForSecondAreaDoorHelp_00d043a0.cpp` |
| 0x00D08B40 | `NScript::CQ_BanditCampBossBattleScript::CCampHostageDoor::Main()` | `rebuild/src/compiled/00/d0/NScriptCQBanditCampBossBattleScriptCCampHostageDoor_Main_00d08b40.cpp` |
| 0x00D151D0 | `CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor()` | `rebuild/src/compiled/00/d1/CQBountyHuntScript_WatchForEnableFisherCreekHutDoor_00d151d0.cpp` |
| 0x00D16460 | `NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Init()` | `rebuild/src/compiled/00/d1/NScriptCQBountyHuntScriptCBHGangDeputy1_Init_00d16460.cpp` |
| 0x00D16570 | `CQ_BountyHuntScript::WatchForDeputy1Killed(CScriptThing deputy)` | `rebuild/src/compiled/00/d1/NScriptCQBountyHuntScript_WatchForDeputy1Killed_00d16570.cpp` |
| 0x00D1F610 | `CBanditAttacker::Main()` | `rebuild/src/compiled/00/d1/CBanditAttacker_Main_00d1f610.cpp` |
| 0x00D20790 | `CQ_BreakSiegeScript::WatchForWinLoseConditions()` | `rebuild/src/compiled/00/d2/NScriptCQBreakSiegeScript_WatchForWinLoseConditions_00d20790.cpp` |
| 0x00D38F70 | `CProphetCage::Main()` | `rebuild/src/compiled/00/d3/CProphetCage_Main_00d38f70.cpp` |
| 0x00D701C0 | `CHTG_FreedomFighter::Main()` | `rebuild/src/compiled/00/d7/CHTGFreedomFighter_Main_00d701c0.cpp` |
| 0x00D73F40 | `CHTG_Executioner::Main()` | `rebuild/src/compiled/00/d7/CHTGExecutioner_Main_00d73f40.cpp` |
| 0x00D79D50 | `CEndScythe::Main()` | `rebuild/src/compiled/00/d7/CEndScythe_Main_00d79d50.cpp` |
| 0x00D7A360 | `CEndBriarRose::Main()` | `rebuild/src/compiled/00/d7/CEndBriarRose_Main_00d7a360.cpp` |
| 0x00D7D8D0 | `NScript::CQ_HeroSoulsArenaScript::WatchArenaCellsToTurnOffVillage()` | `rebuild/src/compiled/00/d7/NScriptCQHeroSoulsArenaScript_WatchArenaCellsToTurnOffVillage_00d7d8d0.cpp` |
| 0x00D81C50 | `CHSB_MagicBarrier::Main()` | `rebuild/src/compiled/00/d8/CHSBMagicBarrier_Main_00d81c50.cpp` |
| 0x00D83FA0 | `CHS_Screamer::Main()` | `rebuild/src/compiled/00/d8/CHSScreamer_Main_00d83fa0.cpp` |
| 0x00D86C30 | `CSoulInfoAcolyte::Main()` | `rebuild/src/compiled/00/d8/CSoulInfoAcolyte_Main_00d86c30.cpp` |
| 0x00D8F040 | `NScript::CQ_HobbeCaveScript::WatchForGrannysRegion()` | `rebuild/src/compiled/00/d8/NScriptCQHobbeCaveScript_WatchForGrannysRegion_00d8f040.cpp` |
| 0x00D91740 | `CQ_HobbeCaveScript::RescuedBandit()` | `rebuild/src/compiled/00/d9/NScriptCQHobbeCaveScript_RescuedBandit_00d91740.cpp` |
| 0x00D97170 | `Levers: for(;;) with the terminating return first; in-body `#pragma inline_depth(0)` around the hit-block term` | `rebuild/src/compiled/00/d9/NScriptCQHobbeCaveScript_CheckJamesKilled_00d97170.cpp` |
| 0x00D99A30 | `NScript::CQ_HobbeCaveScript::CheckIfGotTreasure()` | `rebuild/src/compiled/00/d9/NScriptCQHobbeCaveScript_CheckIfGotTreasure_00d99a30.cpp` |
| 0x00D9F270 | `NScript::CQ_MinionCampScript::CMC_Minion::Main()` | `rebuild/src/compiled/00/d9/NScriptCQMinionCampScriptCMCMinion_Main_00d9f270.cpp` |
| 0x00D9F6C0 | `CFadeinSummoner::Main()` | `rebuild/src/compiled/00/d9/CFadeinSummoner_Main_00d9f6c0.cpp` |
| 0x00DA4E30 | `CMagicBarrier::Main()` | `rebuild/src/compiled/00/da/CMagicBarrier_Main_00da4e30.cpp` |
| 0x00DA5490 | `NScript::CQ_MinionClifftopChaseScript::CMagicBarrierGraveyard::Main() (0x00DA5490)` | `rebuild/src/compiled/00/da/CMagicBarrierGraveyard_Main_00da5490.cpp` |
| 0x00DA68E0 | `CGuardNotThisWay::Main()` | `rebuild/src/compiled/00/da/CGuardNotThisWay_Main_00da68e0.cpp` |
| 0x00DA7D30 | `CGuardToLeadTheWay::Main()` | `rebuild/src/compiled/00/da/CGuardToLeadTheWay_Main_00da7d30.cpp` |
| 0x00DAA260 | `CMinionFieldGenerator::Main()` | `rebuild/src/compiled/00/da/CMinionFieldGenerator_Main_00daa260.cpp` |
| 0x00DAA700 | `CMinionTrollSummoner::Main()` | `rebuild/src/compiled/00/da/CMinionTrollSummoner_Main_00daa700.cpp` |
| 0x00DBF9E0 | `CQ_OakValeBanditRaidScript::WatchForWhistling()` | `rebuild/src/compiled/00/db/NScriptCQOakValeBanditRaidScript_WatchForWhistling_00dbf9e0.cpp` |
| 0x00DC21F0 | `CM_RestartMarker::Main()` | `rebuild/src/compiled/00/dc/CMRestartMarker_Main_00dc21f0.cpp` |
| 0x00DC8C80 | `CQ_OpeningGraveyardSecretPassageScript::CheckDoor()` | `rebuild/src/compiled/00/dc/NScriptCQOpeningGraveyardSecretPassageScript_CheckDoor_00dc8c80.cpp` |
| 0x00DCDC50 | `Levers: ~CScriptThing __forceinline; `#pragma inline_depth(0)` around the CIsThingAlive class definition keeps` | `rebuild/src/compiled/00/dc/CArtefact_Main_00dcdc50.cpp` |
| 0x00DD6440 | `CPrisonEscapeGuard::Main()` | `rebuild/src/compiled/00/dd/CPrisonEscapeGuard_Main_00dd6440.cpp` |
| 0x00DDC980 | `CPrisonRacer::Main()` | `rebuild/src/compiled/00/dd/CPrisonRacer_Main_00ddc980.cpp` |
| 0x00DDE230 | `CQ_PrisonWardenGameScript::WardenKeyMiniGame()` | `rebuild/src/compiled/00/dd/NScriptCQPrisonWardenGameScript_WardenKeyMiniGame_00dde230.cpp` |
| 0x00DEF760 | `CSP_UndeadTrigger::Main()` | `rebuild/src/compiled/00/de/CSPUndeadTrigger_Main_00def760.cpp` |
| 0x00E02E50 | `CDarkwoodAssassinSpawn::Main()` | `rebuild/src/compiled/00/e0/CDarkwoodAssassinSpawn_Main_00e02e50.cpp` |
| 0x00E04F10 | `CQ_TraderEscortScript::WatchForPickpocketing(CScriptThing trader)` | `rebuild/src/compiled/00/e0/NScriptCQTraderEscortScript_WatchForPickpocketing_00e04f10.cpp` |
| 0x00E05360 | `NScript::CQ_TraderEscortScript::CTraderComment::Main()` | `rebuild/src/compiled/00/e0/NScriptCQTraderEscortScriptCTraderComment_Main_00e05360.cpp` |
| 0x00E0C4C0 | `CQ_UndeadRisingScript::LampChecker()` | `rebuild/src/compiled/00/e0/NScriptCQUndeadRisingScript_LampChecker_00e0c4c0.cpp` |
| 0x00E0DAA0 | `NScript::CQ_UndeadRisingScript::CUndeadRisingZombieAttacker::Init()` | `rebuild/src/compiled/00/e0/NScriptCQUndeadRisingScriptCUndeadRisingZombieAttacker_Init_00e0daa0.cpp` |
| 0x00E12580 | `CQ_WaspBossScript::DoMission()` | `rebuild/src/compiled/00/e1/NScriptCQWaspBossScript_DoMission_00e12580.cpp` |
| 0x00E2A320 | `CScytheNearOracle::Main()` | `rebuild/src/compiled/00/e2/CScytheNearOracle_Main_00e2a320.cpp` |
| 0x00E32080 | `CV_AmbushScamScript::WatchForExpressions(CScriptThing buddy)` | `rebuild/src/compiled/00/e3/NScriptCVAmbushScamScript_WatchForExpressions_00e32080.cpp` |
| 0x00E331F0 | `CV_ArcheryCompetitionScript::WatchForOwnerKilled()` | `rebuild/src/compiled/00/e3/NScriptCVArcheryCompetitionScript_WatchForOwnerKilled_00e331f0.cpp` |
| 0x00E36390 | `Levers: ~CScriptThing __forceinline; for(;;) with the terminating return first, the dead branch first, then th` | `rebuild/src/compiled/00/e3/NScriptCVArcheryCompetitionScript_WatchForAttack_00e36390.cpp` |
| 0x00E386E0 | `NScript::CV_AssassinAttacksScript::CChestHidingAssassin::Main() (0x00E386E0)` | `rebuild/src/compiled/00/e3/CMeleeThunder_Main_00e386e0.cpp` |
| 0x00E39140 | `NScript::CV_AssassinAttacksScript::WatchForAssassinDead(CScriptThing assassin)` | `rebuild/src/compiled/00/e3/NScriptCVAssassinAttacksScript_WatchForAssassinDead_00e39140.cpp` |
| 0x00E504C0 | `CV_BeardyBaldyScript::WatchForBarberLadyDeath()` | `rebuild/src/compiled/00/e5/NScriptCVBeardyBaldyScript_WatchForBarberLadyDeath_00e504c0.cpp` |
| 0x00E53D90 | `Levers: ~CScriptThing __forceinline; for(;;) with the terminating return first, the dead branch (`if (term) re` | `rebuild/src/compiled/00/e5/NScriptCVBeardyBaldyScript_WatchForAttack_00e53d90.cpp` |
| 0x00E6AF20 | `CV_ChapelOfEvilScript::WatchForAcolyteHit(CScriptThing acolyte)` | `rebuild/src/compiled/00/e6/CVChapelOfEvilScript_WatchForAcolyteHit_00e6af20.cpp` |
| 0x00E6B750 | `Levers: for(;;) with the terminating return first; in-body `#pragma inline_depth(0)` around the region-hit ter` | `rebuild/src/compiled/00/e6/NScriptCVChapelOfEvilScript_WatchForRegionEntry_00e6b750.cpp` |
| 0x00E7BD60 | `NScript::CV_FishermanScript::CFisherman::Init()` | `rebuild/src/compiled/00/e7/NScriptCVFishermanScriptCFisherman_Init_00e7bd60.cpp` |
| 0x00E82950 | `CFisticuffsClubRing::Main()` | `rebuild/src/compiled/00/e8/CFisticuffsClubRing_Main_00e82950.cpp` |
| 0x00E9DC80 | `CV_IntroductionToTrophiesScript::WatchForAttack(CScriptThing briar)` | `rebuild/src/compiled/00/e9/NScriptCVIntroductionToTrophiesScript_WatchForAttack_00e9dc80.cpp` |
| 0x00EC0BA0 | `NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::Main()` | `rebuild/src/compiled/00/ec/CSecretWhisperingFriend_Main_00ec0ba0.cpp` |
| 0x00EDCB80 | `CV_TempleOfLightScript::WatchForAcolyteHit(CScriptThing acolyte)` | `rebuild/src/compiled/00/ed/NScriptCVTempleOfLightScript_WatchForAcolyteHit_00edcb80.cpp` |
| 0x00EE6A40 | `Levers: vector dtor __forceinline with the finish pointer cached in a local (`last`, edi); for(;;) with the te` | `rebuild/src/compiled/00/ee/NScriptCVTourGuideScript_WatchForNoFollowers_00ee6a40.cpp` |
| 0x00EE7A20 | `CDemonDoorFace::Main()` | `rebuild/src/compiled/00/ee/CDemonDoorFace_Main_00ee7a20.cpp` |
| 0x00EF12F0 | `NScript::CPersonalScript_EmptyScript::CCrateDeathMarker::Main()` | `rebuild/src/compiled/00/ef/CCrateDeathMarker_Main_00ef12f0.cpp` |
| 0x00EF5200 | `NScript::CPersonalScript_EmptyScript::CheckForHeroActions()` | `rebuild/src/compiled/00/ef/NScriptCPersonalScriptEmptyScript_CheckForHeroActions_00ef5200.cpp` |
| 0x00EF6FE0 | `NScript::CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAliv` | `rebuild/src/compiled/00/ef/NScriptCPredicateAndCPredicateAndCPredicateAndCIsThingAliveCIsThingAliveCIsThing_CPredicateAnd_00ef6fe0.cpp` |
| 0x00EF7800 | `COpinionMan::Main()` | `rebuild/src/compiled/00/ef/COpinionMan_Main_00ef7800.cpp` |
| 0x00EF87D0 | `NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main()` | `rebuild/src/compiled/00/ef/NScriptCPersonalScriptEmptyScriptCUndeadFellow_Main_00ef87d0.cpp` |
| 0x00EFAB40 | `Cwelcome_guy::Main()` | `rebuild/src/compiled/00/ef/Cwelcomeguy_Main_00efab40.cpp` |
| 0x00EFF930 | `CShowTargetedThingHealthScript::ShowTargetedThingsHealth()` | `rebuild/src/compiled/00/ef/CShowTargetedThingHealthScript_ShowTargetedThingsHealth_00eff930.cpp` |
| 0x00F045E0 | `CAT_Sentry::Main()` | `rebuild/src/compiled/00/f0/CATSentry_Main_00f045e0.cpp` |
| 0x00F07B80 | `NScript::CQ_AmbushTradersScript::WatchForBanditSpeechMarkers(CScriptThing)` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForBanditSpeechMarkers_00f07b80.cpp` |
| 0x00F08590 | `NScript::CQ_AmbushTradersScript::WatchForEnemyDeath(CScriptThing enemy)` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForEnemyDeath_00f08590.cpp` |
| 0x00F09260 | `Levers: for(;;) with the terminating return first (rotated; the loop exit returns without the trailing test), ` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForHeroAttackOnSentry_00f09260.cpp` |
| 0x00F09420 | `NScript::CQ_AmbushTradersScript::CAT_Phase3Archer::Init()` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScriptCATPhase3Archer_Init_00f09420.cpp` |
| 0x00F09530 | `NScript::CQ_AmbushTradersScript::CAT_Phase3Guard::Init()` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScriptCATPhase3Guard_Init_00f09530.cpp` |
| 0x00F09950 | `Levers: same for(;;) shape as 0x00F09260, but the hit block does not break: it sets ContactAttackedByHero and ` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForHeroAttackOnContact_00f09950.cpp` |
| 0x00F0A470 | `CQ_AmbushTradersScript::WatchForQuestSuccess()` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForQuestSuccess_00f0a470.cpp` |
| 0x00F0B130 | `CQ_AmbushTradersScript::DoQuest()` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_DoQuest_00f0b130.cpp` |
| 0x00F0EF60 | `CQ_AmbushTradersScript::WatchForSpySpeechMarkers(CScriptThing spy)` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForSpySpeechMarkers_00f0ef60.cpp` |

## Redone pragma-free in landing 2026-11-03d (39 rows)
`rebuild/candidates/manual/batches/2026-11-03_landing_d/` (sources from `batches/2026-11-03_inline_split_research/matched/revisit/`). Each row lost its
`// Revisit: inline-depth` marker, carries no `#pragma inline_depth`, no `/d2inl*` flag and no forced destructor (out-of-class
`__forceinline X::~X()` became `inline X::~X()`), and re-landed RELOCATION_MATCH + PASS on the final tree. The `#pragma inline_depth` lines
were also dropped from the 14 of their tests that copied them. Count by rewrite: plain 27, dwall 8, dw 3, mfc_assert 1.

| Address | Rewrite | Function | Source |
|---|---|---|---|
| 0x005BB190 | plain: pragmas deleted (any forced dtor made plain inline) | `005bb190 std::_List_base<CDefPointer<const CAppearanceModifierDef>,std::allocator<CDefPointer<const CAppearanc` | `rebuild/src/compiled/00/5b/stdListbaseCDefPointerconstCAppearanceModifierDefstdallocatorCDefPointerconstCAp_clear_005bb190.cpp` |
| 0x006C8270 | plain: pragmas deleted (any forced dtor made plain inline) | `std::map<long, COpinionDeed* >::~map()` | `rebuild/src/compiled/00/6c/stdmaplongCOpinionDeed_map_006c8270.cpp` |
| 0x00894500 | plain: pragmas deleted (any forced dtor made plain inline) | `00894500 CGameScriptInterface::CreateCreatureOnEntity` | `rebuild/src/compiled/00/89/CGameScriptInterface_CreateCreatureOnEntity_00894500.cpp` |
| 0x009AC130 | plain: pragmas deleted (any forced dtor made plain inline) | `std::map<CCharString, NBankFileManager::CContainedBank >::~map()` | `rebuild/src/compiled/00/9a/stdmapCCharStringNBankFileManagerCContainedBank_map_009ac130.cpp` |
| 0x00C0C270 | MFC-style `do { } while (0)` LIB_ASSERT + plain-inline GetData | `CString::ConcatInPlace(int nSrcLen, const char* lpszSrcData)` | `rebuild/src/compiled/00/c0/CString_ConcatInPlace_00c0c270.cpp` |
| 0x00CBABA0 | plain: pragmas deleted (any forced dtor made plain inline) | `00cbaba0 std::__push_heap` | `rebuild/src/compiled/00/cb/std_pushheap_00cbaba0.cpp` |
| 0x00CFFB60 | plain: pragmas deleted (any forced dtor made plain inline) | `CFinalTabletSarcophagus::Main()` | `rebuild/src/compiled/00/cf/CFinalTabletSarcophagus_Main_00cffb60.cpp` |
| 0x00D16920 | plain: pragmas deleted (any forced dtor made plain inline) | `NScript::CQ_BountyHuntScript::WatchForHostage1Killed(CScriptThing thing)` | `rebuild/src/compiled/00/d1/NScriptCQBountyHuntScript_WatchForHostage1Killed_00d16920.cpp` |
| 0x00D16D00 | plain: pragmas deleted (any forced dtor made plain inline) | `NScript::CQ_BountyHuntScript::WatchForHostage2Killed(CScriptThing thing)` | `rebuild/src/compiled/00/d1/NScriptCQBountyHuntScript_WatchForHostage2Killed_00d16d00.cpp` |
| 0x00D1CAC0 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `CQ_BountyHuntScript::WatchForAllLeaderChargeGruntsKilled()` | `rebuild/src/compiled/00/d1/NScriptCQBountyHuntScript_WatchForAllLeaderChargeGruntsKilled_00d1cac0.cpp` |
| 0x00D2C810 | plain: pragmas deleted (any forced dtor made plain inline) | `CEndGameThunder::Main()` | `rebuild/src/compiled/00/d2/CEndGameThunder_Main_00d2c810.cpp` |
| 0x00D58080 | plain: pragmas deleted (any forced dtor made plain inline) | `NScript::CQ_GuildTrainingMeleeScript::CMeleeThunder::Main() (0x00D58080)` | `rebuild/src/compiled/00/d5/CMeleeThunder_Main_00d58080.cpp` |
| 0x00D643A0 | plain: pragmas deleted (any forced dtor made plain inline) | `CQ_GuildTrainingWoodsDepartureScript::CScorpionHome::Main()` | `rebuild/src/compiled/00/d6/NScriptCQGuildTrainingWoodsDepartureScriptCScorpionHome_Main_00d643a0.cpp` |
| 0x00D85B40 | plain: pragmas deleted (any forced dtor made plain inline) | `CGuildmaster_HS::Main()` | `rebuild/src/compiled/00/d8/CGuildmasterHS_Main_00d85b40.cpp` |
| 0x00D9B420 | plain: pragmas deleted (any forced dtor made plain inline) | `CQ_HobbeToothContestScript::DisplayTimer()` | `rebuild/src/compiled/00/d9/NScriptCQHobbeToothContestScript_DisplayTimer_00d9b420.cpp` |
| 0x00DA0820 | plain: pragmas deleted (any forced dtor made plain inline) | `CQ_MinionCampScript::RunFirstWaveControl()` | `rebuild/src/compiled/00/da/NScriptCQMinionCampScript_RunFirstWaveControl_00da0820.cpp` |
| 0x00DA8970 | plain: pragmas deleted (any forced dtor made plain inline) | `CQ_MinionClifftopChaseScript::GuardComment(CCharString comment)` | `rebuild/src/compiled/00/da/NScriptCQMinionClifftopChaseScript_GuardComment_00da8970.cpp` |
| 0x00DA9C70 | plain: pragmas deleted (any forced dtor made plain inline) | `CMinionWarningGuard::Main()` | `rebuild/src/compiled/00/da/CMinionWarningGuard_Main_00da9c70.cpp` |
| 0x00DBE4E0 | plain: pragmas deleted (any forced dtor made plain inline) | `NScript::CQ_NewOakValeIntroScript::ManageQuestCoreMarkers()` | `rebuild/src/compiled/00/db/NScriptCQNewOakValeIntroScript_ManageQuestCoreMarkers_00dbe4e0.cpp` |
| 0x00E06050 | plain: pragmas deleted (any forced dtor made plain inline) | `IsAlive wait loop tests it through reinterpret_cast<const CScriptThingView&>(balverine).Thing(); VC7.1 then ho` | `rebuild/src/compiled/00/e0/NScriptCQTraderEscortScript_WatchForSurprisingBalverines_00e06050.cpp` |
| 0x00E1C580 | plain: pragmas deleted (any forced dtor made plain inline) | `CQ_WizardBattleScript::PhaseTwo()` | `rebuild/src/compiled/00/e1/NScriptCQWizardBattleScript_PhaseTwo_00e1c580.cpp` |
| 0x00E1FCF0 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `Levers: for(;;) with the terminating return first, then `if (alive) { ... } else { if (term) return; break; }`` | `rebuild/src/compiled/00/e1/CQREscortTraderScript_WatchForExpressionFollow_00e1fcf0.cpp` |
| 0x00E1FEF0 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `Levers: twin of 0x00E1FCF0 (EXPRESSION_WAIT, ExpressionDetectedWait): for(;;) with the terminating return firs` | `rebuild/src/compiled/00/e1/CQREscortTraderScript_WatchForExpressionWait_00e1fef0.cpp` |
| 0x00E2E520 | plain inline dtor + only some exits wrapped (line-2 `// Inline-split:` note: retail's construct unproven) | `CV_AmbushScamScript::WatchForHeroAttack(CScriptThing target)` | `rebuild/src/compiled/00/e2/NScriptCVAmbushScamScript_WatchForHeroAttack_00e2e520.cpp` |
| 0x00E4E0E0 | plain: pragmas deleted (any forced dtor made plain inline) | `CTollBanditScript::HandleBanditToll()` | `rebuild/src/compiled/00/e4/CTollBanditScript_HandleBanditToll_00e4e0e0.cpp` |
| 0x00E5F8F0 | plain: pragmas deleted (any forced dtor made plain inline) | `CHenchmanRegionExitComment::Main()` | `rebuild/src/compiled/00/e5/CHenchmanRegionExitComment_Main_00e5f8f0.cpp` |
| 0x00E6A0B0 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `CV_ChapelOfEvilScript::WatchForInsideChapel()` | `rebuild/src/compiled/00/e6/CVChapelOfEvilScript_WatchForInsideChapel_00e6a0b0.cpp` |
| 0x00E6EA30 | plain: pragmas deleted (any forced dtor made plain inline) | `CDF_ShineLight::Main()` | `rebuild/src/compiled/00/e6/CDFShineLight_Main_00e6ea30.cpp` |
| 0x00E71980 | plain: pragmas deleted (any forced dtor made plain inline) | `CDF_GiveGift::Main()` | `rebuild/src/compiled/00/e7/CDFGiveGift_Main_00e71980.cpp` |
| 0x00E8B020 | plain: pragmas deleted (any forced dtor made plain inline) | `CRingFighter::Main()` | `rebuild/src/compiled/00/e8/NScriptCVFisticuffsClubScriptCRingFighter_Main_00e8b020.cpp` |
| 0x00E9CB20 | plain inline dtor + only some exits wrapped (line-2 `// Inline-split:` note: retail's construct unproven) | `CV_IntroductionToTrophiesScript::WatchForInsideTavern()` | `rebuild/src/compiled/00/e9/NScriptCVIntroductionToTrophiesScript_WatchForInsideTavern_00e9cb20.cpp` |
| 0x00EC1EF0 | plain: pragmas deleted (any forced dtor made plain inline) | `CRandomPopulationCreature::Main()` | `rebuild/src/compiled/00/ec/CRandomPopulationCreature_Main_00ec1ef0.cpp` |
| 0x00ED7320 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull(CScriptThing puller)` | `rebuild/src/compiled/00/ed/NScriptCVSwordInTheStoneScript_WatchForDeathDuringSwordPull_00ed7320.cpp` |
| 0x00ED9790 | plain inline dtor + only some exits wrapped (line-2 `// Inline-split:` note: retail's construct unproven) | `Levers: ~CScriptThing __forceinline; after the predicate + yield prologue a for(;;) with the terminating retur` | `rebuild/src/compiled/00/ed/NScriptCVTalentlessBardScript_WatchForAttack_00ed9790.cpp` |
| 0x00EDBD80 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `CV_TempleOfLightScript::WatchForInsideTemple()` | `rebuild/src/compiled/00/ed/NScriptCVTempleOfLightScript_WatchForInsideTemple_00edbd80.cpp` |
| 0x00EF5CB0 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `NScript::CPersonalScript_EmptyScript::CLookAtMe1::Main()` | `rebuild/src/compiled/00/ef/NScriptCPersonalScriptEmptyScriptCLookAtMe1_Main_00ef5cb0.cpp` |
| 0x00EF84D0 | plain: pragmas deleted (any forced dtor made plain inline) | `Levers: inline CScriptThing model (CBaseClass + CCountedPointer, __forceinline dtor); the CAnd filter comes fr` | `rebuild/src/compiled/00/ef/CCham_Main_00ef84d0.cpp` |
| 0x00F061E0 | plain inline dtor + every terminate check `do { if (...) return; } while (0);` | `CQ_AmbushTradersScript::WatchForHeroAttackOnBandit(CScriptThing bandit)` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_WatchForHeroAttackOnBandit_00f061e0.cpp` |
| 0x00F07ED0 | plain: pragmas deleted (any forced dtor made plain inline) | `NScript::CQ_AmbushTradersScript::AskBanditHireQuestion(const CScriptThing&, CScriptGameResourceObjectScriptedT` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_AskBanditHireQuestion_00f07ed0.cpp` |

Remaining list at any time: `grep -rl 'Revisit: inline-depth' rebuild/src/compiled` (90 after this landing).

## two-view-CScriptThing rows (17)
| Address | Function | Source |
|---|---|---|
| 0x00D167C0 | `CBH_Hostage1::Init()` | `rebuild/src/compiled/00/d1/CBHHostage1_Init_00d167c0.cpp` |
| 0x00D169E0 | `CAT_Phase3Archer::Init()` | `rebuild/src/compiled/00/d1/CATPhase3Archer_Init_00d169e0.cpp` |
| 0x00D16BB0 | `CBH_Hostage2::Init()` | `rebuild/src/compiled/00/d1/CBHHostage2_Init_00d16bb0.cpp` |
| 0x00D16DC0 | `CBH_GangLeader::Init()` | `rebuild/src/compiled/00/d1/CBHGangLeader_Init_00d16dc0.cpp` |
| 0x00D874F0 | `CGuardOutsideWoods::Main()` | `rebuild/src/compiled/00/d8/CGuardOutsideWoods_Main_00d874f0.cpp` |
| 0x00D9D4F0 | `CQ_HobbeToothContestScript::CQ_HobbeToothWhisper::Init()` | `rebuild/src/compiled/00/d9/NScriptCQHobbeToothContestScriptCQHobbeToothWhisper_Init_00d9d4f0.cpp` |
| 0x00E1F3E0 | `CAttackingBandit::Init()` | `rebuild/src/compiled/00/e1/NScriptCQREscortTraderScriptCAttackingBandit_Init_00e1f3e0.cpp` |
| 0x00E38F80 | `CAttackingAssassin::Init()` | `rebuild/src/compiled/00/e3/CAttackingAssassin_Init_00e38f80.cpp` |
| 0x00E6AAC0 | `CAT_BanditContact::Init()` | `rebuild/src/compiled/00/e6/CATBanditContact_Init_00e6aac0.cpp` |
| 0x00E9DAC0 | `NScript::CBriarRose::Init()` | `rebuild/src/compiled/00/e9/NScriptCBriarRose_Init_00e9dac0.cpp` |
| 0x00EDC720 | `CTOL_AcolyteGood::Init()` | `rebuild/src/compiled/00/ed/CTOLAcolyteGood_Init_00edc720.cpp` |
| 0x00F051D0 | `CAT_Phase3Guard::Main()` | `rebuild/src/compiled/00/f0/CATPhase3Guard_Main_00f051d0.cpp` |
| 0x00F081E0 | `CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase` | `rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScript_BanditHireSetup_00f081e0.cpp` |
| 0x00F083A0 | `NScript::CAT_Phase1Guard::Init()` | `rebuild/src/compiled/00/f0/NScriptCATPhase1Guard_Init_00f083a0.cpp` |
| 0x00F08A30 | `CAT_Phase1Archer::Init()` | `rebuild/src/compiled/00/f0/CATPhase1Archer_Init_00f08a30.cpp` |
| 0x00F08C30 | `CAT_ConvoyMember::Init()` | `rebuild/src/compiled/00/f0/CATConvoyMember_Init_00f08c30.cpp` |
| 0x00F09640 | `CAT_BanditContact::Init()` | `rebuild/src/compiled/00/f0/CATBanditContact_Init_00f09640.cpp` |

## 2026-11-03: the pragma-free mechanism (inline split research)
`rebuild/candidates/manual/batches/2026-11-03_inline_split_research/FINDINGS.md` (probes in `probes/`, per-row table in
`family/summary.tsv`). VC7.1 RTM decides plain-`inline` expansion **per call site**: it inlines when
`(c/f + 6c)/7 < 74` (site frequency f <= 1) or `(c/f + 2c)/3 < 74` (f > 1), with c = the callee's optimised size and f = c2's
static block-frequency estimate (every `if` 50/50, `&&` 0.25, `||` 0.75, loop bodies x5-8, all exits of one loop share its entry
frequency, `do { } while (0)` counts as a loop, code after a guarded `for` keeps the loop's frequency). No depth rule, no
pragma. Levers, in order of preference:
1. Delete the pragma and make the forced callee plain `inline` (27 of the 129 rows match this way).
2. The statement-macro shape `do { if (IsActiveThreadTerminating()) return; } while (0);` at **every** terminate check (8 more).
   It is the standard statement-macro wrapper (MFC's ASSERT is exactly this shape: 0x00C0C270 CString::ConcatInPlace matches
   with a do-while(0) ASSERT and a plain-inline GetData).
3. Wrapping only the exits retail inlines (3 rows: 0x00E2E520, 0x00E9CB20, 0x00ED9790). Retail's construct there is unproven;
   such rows carry a line-2 note saying so.
Loop guards (`if (!v.empty())` around a `for`) move the frequency of the code after the loop (0x0074BC20 DrawDebugInfo's split).
`/d2inlT<N>` and the other `-inl*` c2 knobs are diagnostics only, never landing flags. The remaining 90 rows: 66 need per-row
shape work on their returns, 18 have the pragma around temporaries / Reset / assignments, 6 use other depths.
