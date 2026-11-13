// CScriptInfoManager::RegisterScripts() -- fill the script table: the master script, then every important, quest, vignette, global-quest and test script with its allocator, its master script's data allocator and its code section
// Leaf: ?RegisterScripts@CScriptInfoManager@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD52D0, 24,717 bytes, thiscall, plain `ret`, frame 0x18, one basic block (no branch).
// Name: Ego_r.pdb + FableWin.pdb ?RegisterScripts@CScriptInfoManager@@IAEXXZ (protected); the only caller is
//   CScriptInfoManager::Init 0x00CB5D80 (call at 0x00CB5E12, after its "Registering Scripts" progress text).
// What it does (479 statements, decoded from the retail call sites by tools/map.py; map/events.txt):
//   6 progress texts (NProgressDisplay::DisplayProgress(text, -1.0f, false, false));
//   5 master scripts: AddScript(CScriptInfo(name, 1, true, <Script>::Alloc, <Script>::StatGetMasterScriptDataAlloc()), "")
//     -- Q_SunnyvaleMaster, alextest, DanyalTestScripts, joss_test_scripts, Empty;
//   156 scripts, three statements each: CheckSectionIsNotAlreadyRegistered(section) (an empty body in retail, it only destroys
//     its by-value argument), AddScript(CScriptInfo(name, 1, false, <Script>::Alloc, <its master's data allocator or NULL>),
//     section), SetScriptActiveStatus(name, 0).  126 of them belong to Q_SunnyvaleMaster, 10 to Empty, 5 to
//     DanyalTestScripts, 2 to joss_test_scripts, 1 to alextest; 12 (the Global_* and Expression_* scripts) have no master data.
// Callees: CCharString(const char*, long) 0x0099EBF0, CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0,
//   NProgressDisplay::DisplayProgress 0x009E9F40 (__fastcall, the first bool in dl),
//   CScriptInfoManager::CheckSectionIsNotAlreadyRegistered 0x00CB5AC0, CScriptInfoManager::AddScript 0x00CB5C90,
//   SetScriptActiveStatus 0x00CBFAB8 (__fastcall), and the addresses of 161 NScript::C<Name>Script::Alloc functions and of the
//   five master data allocators (0x00CDBD20 CQ_SunnyvaleMasterData, 0x00CD4A60 CalextestData, 0x00CD4AC0 CEmptyData,
//   0x00CD4B20 Cjoss_test_scriptsData, 0x00CD4B80 CDanyalTestScriptsData).
// Shape: every name is an implicit const char* -> CCharString conversion (one shared frame word at [esp+10h], its address
//   re-taken with `lea` for the copy into CScriptInfo::Name); the CScriptInfo temporary lives at [esp+14h] and its inline
//   constructor (FableWin.pdb ??0CScriptInfo@@QAE@ABVCCharString@@J_NP6I...@ZP6I...@Z: name, id, master script, alloc, master
//   data alloc) copies the name first and then stores ID, MasterScript, AllocFunc and MasterScriptDataAlloc in the parameter
//   order; the section is a by-value CCharString built in its argument slot.  c2 keeps 0 in ebx, 1 in edi and the Sunnyvale
//   data allocator in ebp for the whole function, and its scheduler lets the four stores sink between the argument pushes
//   (five times, at 0x00CD5745 / 0x00CD7AB2 / 0x00CD7BE4 / 0x00CD7D16 / 0x00CD7E48, they stay ahead of them: region cuts).
// The master data allocator comes from the master script class: FableWin.pdb has
//   ?StatGetMasterScriptDataAlloc@<Script>@NScript@@SIP6IPAVCScriptDataBase@@XZXZ for exactly the five master scripts.
// Script classes: every script name N in the table has a class NScript::C<N>Script with
//   ?Alloc@C<N>Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in both PDB name sets (161 of
//   161).  The row declares each class with that one static member only (the engine/C*Script.h headers that exist do not
//   declare the factory; see NOTES.md "Header debt").
// EScriptActiveStatus: the enumerators are not recovered (no PDB enum dump, no other caller of Set/GetScriptActiveStatus in
//   Fable.exe); the row passes the value 0 retail passes (`xor edx,edx`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTINFO_CTOR
#include <stddef.h>
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/NProgressDisplay.h"

enum EScriptActiveStatus { };
void FABLE_FASTCALL SetScriptActiveStatus(const CCharString& name, EScriptActiveStatus status);   // 0x00CBFAB8

namespace NScript {
#define FABLE_SCRIPT_CLASS(Class) \
    class Class { public: static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); }
#define FABLE_MASTER_SCRIPT_CLASS(Class, Data) \
    class Data { public: static CScriptDataBase* FABLE_FASTCALL Alloc(); }; \
    class Class { public: static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); \
                  static CScriptInfo::tMasterScriptDataAllocFunc FABLE_FASTCALL StatGetMasterScriptDataAlloc() { return Data::Alloc; } }
FABLE_MASTER_SCRIPT_CLASS(CQ_SunnyvaleMasterScript, CQ_SunnyvaleMasterData);     // 0x00CDD550, data 0x00CDBD20
FABLE_MASTER_SCRIPT_CLASS(CalextestScript, CalextestData);                       // 0x00EED220, data 0x00CD4A60
FABLE_MASTER_SCRIPT_CLASS(CEmptyScript, CEmptyData);                             // 0x00EEE940, data 0x00CD4AC0
FABLE_MASTER_SCRIPT_CLASS(Cjoss_test_scriptsScript, Cjoss_test_scriptsData);     // 0x00EF9EE0, data 0x00CD4B20
FABLE_MASTER_SCRIPT_CLASS(CDanyalTestScriptsScript, CDanyalTestScriptsData);     // 0x00EFB0B0, data 0x00CD4B80
FABLE_SCRIPT_CLASS(CHeroBoastsScript);                          // 0x00CE6C40
FABLE_SCRIPT_CLASS(CPersonalScriptMainScript);                  // 0x00CDE2F0
FABLE_SCRIPT_CLASS(CPersonalScript_GlobalThingsScript);         // 0x00CE19A0
FABLE_SCRIPT_CLASS(CGameflowScript);                            // 0x00CEF950
FABLE_SCRIPT_CLASS(CGameflowAssistanceScript);                  // 0x00CF0640
FABLE_SCRIPT_CLASS(CQ_ArenaHoldingScriptScript);                // 0x00CF8860
FABLE_SCRIPT_CLASS(CLadyGreyWifeManagerScript);                 // 0x00CF91C0
FABLE_SCRIPT_CLASS(CQ_AmbushTradersScript);                     // 0x00CFA2F0
FABLE_SCRIPT_CLASS(CQ_ArenaScript);                             // 0x00CFABF0
FABLE_SCRIPT_CLASS(CQ_AwakeningTheOracleScript);                // 0x00D00590
FABLE_SCRIPT_CLASS(CQ_BanditCampScript);                        // 0x00D03580
FABLE_SCRIPT_CLASS(CQ_BanditCampBossBattleScript);              // 0x00D12920
FABLE_SCRIPT_CLASS(CQ_BanditCampHoldingScriptScript);           // 0x00D12C50
FABLE_SCRIPT_CLASS(CQ_BountyHuntScript);                        // 0x00D1CBA0
FABLE_SCRIPT_CLASS(CQ_BreakSiegeScript);                        // 0x00D23C80
FABLE_SCRIPT_CLASS(CQ_BowerstoneTownLifeIntroScript);           // 0x00D253B0
FABLE_SCRIPT_CLASS(CQ_DragonBossFightScript);                   // 0x00D27240
FABLE_SCRIPT_CLASS(CQ_EndGameScript);                           // 0x00D2A7C0
FABLE_SCRIPT_CLASS(CQ_EndGameFocalSitesScript);                 // 0x00D2F5F0
FABLE_SCRIPT_CLASS(CQ_EndGameBossBattleScript);                 // 0x00D34410
FABLE_SCRIPT_CLASS(CQ_FireHeartScript);                         // 0x00D3AFC0
FABLE_SCRIPT_CLASS(CQ_GuildTrainingScript);                     // 0x00D50600
FABLE_SCRIPT_CLASS(CQ_GuildTrainingDepartureScript);            // 0x00D513C0
FABLE_SCRIPT_CLASS(CQ_GuildTrainingPreMeleeScript);             // 0x00D55D10
FABLE_SCRIPT_CLASS(CQ_GuildTrainingMeleeScript);                // 0x00D5A9F0
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWillScript);                 // 0x00D61BB0
FABLE_SCRIPT_CLASS(CQ_GuildTrainingSkillScript);                // 0x00D5DBF0
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWoodsDepartureScript);       // 0x00D664D0
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWoodsMeleeScript);           // 0x00D67740
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWoodsWillScript);            // 0x00D68EA0
FABLE_SCRIPT_CLASS(CQ_HangingTreeGoodScript);                   // 0x00D78290
FABLE_SCRIPT_CLASS(CQ_HangingTreeEvilScript);                   // 0x00D6ED10
FABLE_SCRIPT_CLASS(CQ_HeroSoulsScript);                         // 0x00D7AC50
FABLE_SCRIPT_CLASS(CQ_HeroSoulsThunderScript);                  // 0x00D7C6A0
FABLE_SCRIPT_CLASS(CQ_HeroSoulsArenaScript);                    // 0x00D80BA0
FABLE_SCRIPT_CLASS(CQ_HeroSoulsBriarScript);                    // 0x00D83300
FABLE_SCRIPT_CLASS(CQ_HeroSoulsMotherScript);                   // 0x00D84AF0
FABLE_SCRIPT_CLASS(CQ_HeroSoulsGuildmasterScript);              // 0x00D87A80
FABLE_SCRIPT_CLASS(CQ_HeroSoulsNostroScript);                   // 0x00D89CF0
FABLE_SCRIPT_CLASS(CQ_HerosOldHouseScript);                     // 0x00D8D5A0
FABLE_SCRIPT_CLASS(CQ_HobbeCaveScript);                         // 0x00D99DC0
FABLE_SCRIPT_CLASS(CQ_HobbeToothContestScript);                 // 0x00D9E190
FABLE_SCRIPT_CLASS(CQ_MinionCampScript);                        // 0x00DA2F40
FABLE_SCRIPT_CLASS(CQ_MinionClifftopChaseScript);               // 0x00DAAB70
FABLE_SCRIPT_CLASS(CQ_NewOakValeIntroScript);                   // 0x00DBEF70
FABLE_SCRIPT_CLASS(CQ_OakValeBanditRaidScript);                 // 0x00DC24E0
FABLE_SCRIPT_CLASS(CQ_OrchardFarmRaidScript);                   // 0x00DD1EF0
FABLE_SCRIPT_CLASS(CQ_OrchardFarmRaidEvilScript);               // 0x00DD2300
FABLE_SCRIPT_CLASS(CQ_OrchardFarmRaidGoodScript);               // 0x00DD2680
FABLE_SCRIPT_CLASS(CQ_OpeningGraveyardSecretPassageScript);     // 0x00DCBFD0
FABLE_SCRIPT_CLASS(CQ_PrisonEscapeRescueMotherScript);          // 0x00DDACB0
FABLE_SCRIPT_CLASS(CQ_PrisonRaceScript);                        // 0x00DDDC30
FABLE_SCRIPT_CLASS(CQ_PrisonWardenGameScript);                  // 0x00DE0290
FABLE_SCRIPT_CLASS(CQ_RansomVictimScript);                      // 0x00DEBC60
FABLE_SCRIPT_CLASS(CQ_RansomVictimChiefsHouseScript);           // 0x00DEC790
FABLE_SCRIPT_CLASS(CQ_SecretPassageScript);                     // 0x00DF1080
FABLE_SCRIPT_CLASS(CQ_SummoningTheShipScript);                  // 0x00DF4070
FABLE_SCRIPT_CLASS(CQ_TentacleKrakenBossFightScript);           // 0x00DF5C00
FABLE_SCRIPT_CLASS(CQ_TraderConflictEvilScript);                // 0x00DFA020
FABLE_SCRIPT_CLASS(CQ_TraderConflictGoodScript);                // 0x00E00610
FABLE_SCRIPT_CLASS(CQ_TraderEscortScript);                      // 0x00E0AF60
FABLE_SCRIPT_CLASS(CQ_WaspBossScript);                          // 0x00E0E9F0
FABLE_SCRIPT_CLASS(CQ_WhiteBalverineKnotholeGladeScript);       // 0x00E183B0
FABLE_SCRIPT_CLASS(CQ_UndeadRisingScript);                      // 0x00E0E760
FABLE_SCRIPT_CLASS(CQ_WhiteBalverineWWScript);                  // 0x00E1A770
FABLE_SCRIPT_CLASS(CQ_WizardBattleScript);                      // 0x00E1D9F0
FABLE_SCRIPT_CLASS(CQR_EscortTraderScript);                     // 0x00E24C30
FABLE_SCRIPT_CLASS(CQR_EscortTrader_ManagerScript);             // 0x00E25970
FABLE_SCRIPT_CLASS(CQS_GuardianSisterInfoScript);               // 0x00E26780
FABLE_SCRIPT_CLASS(CQS_GuardianSisterInfo2_SisterInBanditCampScript);// 0x00E277E0
FABLE_SCRIPT_CLASS(CQS_GuardianTrophyDealerInfoScript);         // 0x00E28CA0
FABLE_SCRIPT_CLASS(CQS_MeetSisterScript);                       // 0x00E29990
FABLE_SCRIPT_CLASS(CQS_ScytheInfoScript);                       // 0x00E2AB00
FABLE_SCRIPT_CLASS(CV_AmbushScamScript);                        // 0x00E322C0
FABLE_SCRIPT_CLASS(CV_ArcheryCompetitionScript);                // 0x00E37630
FABLE_SCRIPT_CLASS(CV_AssassinAttacksScript);                   // 0x00E39850
FABLE_SCRIPT_CLASS(CV_BanditTollScript);                        // 0x00E4F550
FABLE_SCRIPT_CLASS(CV_BanditCampPathScript);                    // 0x00E477A0
FABLE_SCRIPT_CLASS(CV_BeardyBaldyScript);                       // 0x00E54260
FABLE_SCRIPT_CLASS(CV_BeggarAndChildScript);                    // 0x00E5D0B0
FABLE_SCRIPT_CLASS(CV_BodyGuardScript);                         // 0x00E62730
FABLE_SCRIPT_CLASS(CV_BookCollectingScript);                    // 0x00E57A50
FABLE_SCRIPT_CLASS(CV_BordelloScript);                          // 0x00E46A00
FABLE_SCRIPT_CLASS(CV_ChapelOfEvilScript);                      // 0x00E6E230
FABLE_SCRIPT_CLASS(CV_ExposeMayorScript);                       // 0x00E7B290
FABLE_SCRIPT_CLASS(CV_ChickenKickingScript);                    // 0x00E68E60
FABLE_SCRIPT_CLASS(CV_DemonDoorsScript);                        // 0x00E75590
FABLE_SCRIPT_CLASS(CV_FishermanScript);                         // 0x00E7E0B0
FABLE_SCRIPT_CLASS(CV_FishingCompetitionScript);                // 0x00E81EB0
FABLE_SCRIPT_CLASS(CV_FisticuffsClubScript);                    // 0x00E8B700
FABLE_SCRIPT_CLASS(CV_GhostGrannyNecklaceScript);               // 0x00E906F0
FABLE_SCRIPT_CLASS(CV_GuildMasterScript);                       // 0x00E92900
FABLE_SCRIPT_CLASS(CV_HauntedHouseScript);                      // 0x00E96DD0
FABLE_SCRIPT_CLASS(CV_HeroDollsScript);                         // 0x00E98640
FABLE_SCRIPT_CLASS(CV_HeroDuelScript);                          // 0x00E9C300
FABLE_SCRIPT_CLASS(CV_HiddenBootyScript);                       // 0x00E93CF0
FABLE_SCRIPT_CLASS(CV_IntroductionToTrophiesScript);            // 0x00E9FB40
FABLE_SCRIPT_CLASS(CV_KnotholeGladeGatesScript);                // 0x00EA2C60
FABLE_SCRIPT_CLASS(CV_LostTraderScript);                        // 0x00EA7680
FABLE_SCRIPT_CLASS(CV_MazeResearchScript);                      // 0x00EA8690
FABLE_SCRIPT_CLASS(CV_MurderTwistScript);                       // 0x00EAE890
FABLE_SCRIPT_CLASS(CV_MurderTwistFinalScript);                  // 0x00EB6A60
FABLE_SCRIPT_CLASS(CV_MayorsInvitationScript);                  // 0x00EBCC10
FABLE_SCRIPT_CLASS(CV_OracleScript);                            // 0x00EBEE30
FABLE_SCRIPT_CLASS(CV_ParanoidWhispersScript);                  // 0x00EC11B0
FABLE_SCRIPT_CLASS(CV_PicnicAreaAfterWaspBossScript);           // 0x00EC1780
FABLE_SCRIPT_CLASS(CV_RandomPopulationSimScript);               // 0x00EC3BC0
FABLE_SCRIPT_CLASS(CV_RockTrollFirstEncounterScript);           // 0x00EC5360
FABLE_SCRIPT_CLASS(CV_SickChildScript);                         // 0x00ED0E00
FABLE_SCRIPT_CLASS(CV_SingingStonesScript);                     // 0x00ED39A0
FABLE_SCRIPT_CLASS(CV_StatueMasterScript);                      // 0x00ED4A40
FABLE_SCRIPT_CLASS(CV_SwordInTheStoneScript);                   // 0x00ED75F0
FABLE_SCRIPT_CLASS(CV_TalentlessBardScript);                    // 0x00EDACC0
FABLE_SCRIPT_CLASS(CV_TourGuideScript);                         // 0x00EE6C00
FABLE_SCRIPT_CLASS(CV_TempleOfLightScript);                     // 0x00EDEB40
FABLE_SCRIPT_CLASS(CV_TravellingHeroesScript);                  // 0x00EE40A0
FABLE_SCRIPT_CLASS(CV_TrophyDealerScript);                      // 0x00EE80A0
FABLE_SCRIPT_CLASS(CGlobal_WatchForHeroDeathScript);            // 0x00EE90A0
FABLE_SCRIPT_CLASS(CGlobal_DebugCycleThroughSpeechScript);      // 0x00EE9450
FABLE_SCRIPT_CLASS(CExpression_FishScript);                     // 0x00EE9F00
FABLE_SCRIPT_CLASS(CExpression_FollowScript);                   // 0x00EEA3E0
FABLE_SCRIPT_CLASS(CExpression_WaitScript);                     // 0x00EEA740
FABLE_SCRIPT_CLASS(CExpression_DigScript);                      // 0x00EEAE70
FABLE_SCRIPT_CLASS(CExpression_PickpocketScript);               // 0x00EEB730
FABLE_SCRIPT_CLASS(CExpression_PicklockScript);                 // 0x00EEBD70
FABLE_SCRIPT_CLASS(CExpression_StealScript);                    // 0x00EEC390
FABLE_SCRIPT_CLASS(CGlobal_TeleportToHeroGuildScript);          // 0x00CDDB60
FABLE_SCRIPT_CLASS(CGlobal_ToggleTimeDisplayScript);            // 0x00EED060
FABLE_SCRIPT_CLASS(CGlobal_GiveHeroItemsFromRewardChestScript); // 0x00EEC750
FABLE_SCRIPT_CLASS(CGlobal_OpenChestScript);                    // 0x00EECEA0
FABLE_SCRIPT_CLASS(CCS_OakValeRevisitedScript);                 // 0x00EE8BE0
FABLE_SCRIPT_CLASS(CPersonalScript_alextestScript);             // 0x00EEE6D0
FABLE_SCRIPT_CLASS(CProximityPodiumTestScript);                 // 0x00EFB610
FABLE_SCRIPT_CLASS(CHitSwitchTestScript);                       // 0x00EFBB70
FABLE_SCRIPT_CLASS(CGuildSealTriggerTestScript);                // 0x00EFC130
FABLE_SCRIPT_CLASS(CPersonalScript_AardvarkScript);             // 0x00EFC8D0
FABLE_SCRIPT_CLASS(CHeroTurningDebuggerScript);                 // 0x00EFC460
FABLE_SCRIPT_CLASS(Cguy_scriptScript);                          // 0x00EFA3C0
FABLE_SCRIPT_CLASS(Cbandit_level_cutsceneScript);               // 0x00EFAF30
FABLE_SCRIPT_CLASS(CTestTheInventoryScript);                    // 0x00EEEE40
FABLE_SCRIPT_CLASS(CPersonalScript_EmptyScript);                // 0x00EF9130
FABLE_SCRIPT_CLASS(CTestNewNav1Script);                         // 0x00EEF9C0
FABLE_SCRIPT_CLASS(CTestNewNav2Script);                         // 0x00EEFB20
FABLE_SCRIPT_CLASS(CTestNewNav3Script);                         // 0x00EEFC80
FABLE_SCRIPT_CLASS(CTestNewNav4Script);                         // 0x00EEFDE0
FABLE_SCRIPT_CLASS(CTestNewNav5Script);                         // 0x00EEFF40
FABLE_SCRIPT_CLASS(CTestQuestCard1Script);                      // 0x00EEF540
FABLE_SCRIPT_CLASS(CTestQuestCard2Script);                      // 0x00EEF6D0
FABLE_SCRIPT_CLASS(CTestQuestCard3Script);                      // 0x00EEF860
FABLE_SCRIPT_CLASS(CMarkTestScriptsScript);                     // 0x00EFE380
FABLE_SCRIPT_CLASS(CBenTestScriptsScript);                      // 0x00EFF000
FABLE_SCRIPT_CLASS(CTonyTestScriptsScript);                     // 0x00EFF770
FABLE_SCRIPT_CLASS(CShowTargetedThingHealthScript);             // 0x00EFFCD0
FABLE_SCRIPT_CLASS(COracleMinigameTestScript);                  // 0x00F01330
FABLE_SCRIPT_CLASS(CQ_CinemaTestScript);                        // 0x00F02460
FABLE_SCRIPT_CLASS(CCS_PlayCutsceneScript);                     // 0x00F01760
}

void CScriptInfoManager::RegisterScripts()
{
    NProgressDisplay::DisplayProgress("Registering Master Script", -1.0f, false, false);
    AddScript(CScriptInfo("Q_SunnyvaleMaster", 1, true, NScript::CQ_SunnyvaleMasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    NProgressDisplay::DisplayProgress("Registering Important Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("S_HB");
    AddScript(CScriptInfo("HeroBoasts", 1, false, NScript::CHeroBoastsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_HB");
    SetScriptActiveStatus("HeroBoasts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_PSM");
    AddScript(CScriptInfo("PersonalScriptMain", 1, false, NScript::CPersonalScriptMainScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_PSM");
    SetScriptActiveStatus("PersonalScriptMain", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_PSGT");
    AddScript(CScriptInfo("PersonalScript_GlobalThings", 1, false, NScript::CPersonalScript_GlobalThingsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_PSGT");
    SetScriptActiveStatus("PersonalScript_GlobalThings", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_GF");
    AddScript(CScriptInfo("Gameflow", 1, false, NScript::CGameflowScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_GF");
    SetScriptActiveStatus("Gameflow", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_GFA");
    AddScript(CScriptInfo("GameflowAssistance", 1, false, NScript::CGameflowAssistanceScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_GFA");
    SetScriptActiveStatus("GameflowAssistance", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_AHS");
    AddScript(CScriptInfo("Q_ArenaHoldingScript", 1, false, NScript::CQ_ArenaHoldingScriptScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_AHS");
    SetScriptActiveStatus("Q_ArenaHoldingScript", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_LGWM");
    AddScript(CScriptInfo("LadyGreyWifeManager", 1, false, NScript::CLadyGreyWifeManagerScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_LGWM");
    SetScriptActiveStatus("LadyGreyWifeManager", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Quest Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("S_QAT");
    AddScript(CScriptInfo("Q_AmbushTraders", 1, false, NScript::CQ_AmbushTradersScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QAT");
    SetScriptActiveStatus("Q_AmbushTraders", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QA");
    AddScript(CScriptInfo("Q_Arena", 1, false, NScript::CQ_ArenaScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QA");
    SetScriptActiveStatus("Q_Arena", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QATO");
    AddScript(CScriptInfo("Q_AwakeningTheOracle", 1, false, NScript::CQ_AwakeningTheOracleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QATO");
    SetScriptActiveStatus("Q_AwakeningTheOracle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBC");
    AddScript(CScriptInfo("Q_BanditCamp", 1, false, NScript::CQ_BanditCampScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBC");
    SetScriptActiveStatus("Q_BanditCamp", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBCBB");
    AddScript(CScriptInfo("Q_BanditCampBossBattle", 1, false, NScript::CQ_BanditCampBossBattleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBCBB");
    SetScriptActiveStatus("Q_BanditCampBossBattle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBCHS");
    AddScript(CScriptInfo("Q_BanditCampHoldingScript", 1, false, NScript::CQ_BanditCampHoldingScriptScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBCHS");
    SetScriptActiveStatus("Q_BanditCampHoldingScript", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBH");
    AddScript(CScriptInfo("Q_BountyHunt", 1, false, NScript::CQ_BountyHuntScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBH");
    SetScriptActiveStatus("Q_BountyHunt", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBS");
    AddScript(CScriptInfo("Q_BreakSiege", 1, false, NScript::CQ_BreakSiegeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBS");
    SetScriptActiveStatus("Q_BreakSiege", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBTLI");
    AddScript(CScriptInfo("Q_BowerstoneTownLifeIntro", 1, false, NScript::CQ_BowerstoneTownLifeIntroScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBTLI");
    SetScriptActiveStatus("Q_BowerstoneTownLifeIntro", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QDBF");
    AddScript(CScriptInfo("Q_DragonBossFight", 1, false, NScript::CQ_DragonBossFightScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QDBF");
    SetScriptActiveStatus("Q_DragonBossFight", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QEG");
    AddScript(CScriptInfo("Q_EndGame", 1, false, NScript::CQ_EndGameScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QEG");
    SetScriptActiveStatus("Q_EndGame", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QEGFS");
    AddScript(CScriptInfo("Q_EndGameFocalSites", 1, false, NScript::CQ_EndGameFocalSitesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QEGFS");
    SetScriptActiveStatus("Q_EndGameFocalSites", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QEGBB");
    AddScript(CScriptInfo("Q_EndGameBossBattle", 1, false, NScript::CQ_EndGameBossBattleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QEGBB");
    SetScriptActiveStatus("Q_EndGameBossBattle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QFHT");
    AddScript(CScriptInfo("Q_FireHeart", 1, false, NScript::CQ_FireHeartScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QFHT");
    SetScriptActiveStatus("Q_FireHeart", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGT");
    AddScript(CScriptInfo("Q_GuildTraining", 1, false, NScript::CQ_GuildTrainingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGT");
    SetScriptActiveStatus("Q_GuildTraining", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTD");
    AddScript(CScriptInfo("Q_GuildTrainingDeparture", 1, false, NScript::CQ_GuildTrainingDepartureScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTD");
    SetScriptActiveStatus("Q_GuildTrainingDeparture", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTPM");
    AddScript(CScriptInfo("Q_GuildTrainingPreMelee", 1, false, NScript::CQ_GuildTrainingPreMeleeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTPM");
    SetScriptActiveStatus("Q_GuildTrainingPreMelee", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTM");
    AddScript(CScriptInfo("Q_GuildTrainingMelee", 1, false, NScript::CQ_GuildTrainingMeleeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTM");
    SetScriptActiveStatus("Q_GuildTrainingMelee", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTW");
    AddScript(CScriptInfo("Q_GuildTrainingWill", 1, false, NScript::CQ_GuildTrainingWillScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTW");
    SetScriptActiveStatus("Q_GuildTrainingWill", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTS");
    AddScript(CScriptInfo("Q_GuildTrainingSkill", 1, false, NScript::CQ_GuildTrainingSkillScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTS");
    SetScriptActiveStatus("Q_GuildTrainingSkill", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTWD");
    AddScript(CScriptInfo("Q_GuildTrainingWoodsDeparture", 1, false, NScript::CQ_GuildTrainingWoodsDepartureScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTWD");
    SetScriptActiveStatus("Q_GuildTrainingWoodsDeparture", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTWM");
    AddScript(CScriptInfo("Q_GuildTrainingWoodsMelee", 1, false, NScript::CQ_GuildTrainingWoodsMeleeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTWM");
    SetScriptActiveStatus("Q_GuildTrainingWoodsMelee", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTWW");
    AddScript(CScriptInfo("Q_GuildTrainingWoodsWill", 1, false, NScript::CQ_GuildTrainingWoodsWillScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTWW");
    SetScriptActiveStatus("Q_GuildTrainingWoodsWill", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHTG");
    AddScript(CScriptInfo("Q_HangingTreeGood", 1, false, NScript::CQ_HangingTreeGoodScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHTG");
    SetScriptActiveStatus("Q_HangingTreeGood", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHTE");
    AddScript(CScriptInfo("Q_HangingTreeEvil", 1, false, NScript::CQ_HangingTreeEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHTE");
    SetScriptActiveStatus("Q_HangingTreeEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSS");
    AddScript(CScriptInfo("Q_HeroSouls", 1, false, NScript::CQ_HeroSoulsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSS");
    SetScriptActiveStatus("Q_HeroSouls", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHST");
    AddScript(CScriptInfo("Q_HeroSoulsThunder", 1, false, NScript::CQ_HeroSoulsThunderScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHST");
    SetScriptActiveStatus("Q_HeroSoulsThunder", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSA");
    AddScript(CScriptInfo("Q_HeroSoulsArena", 1, false, NScript::CQ_HeroSoulsArenaScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSA");
    SetScriptActiveStatus("Q_HeroSoulsArena", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSB");
    AddScript(CScriptInfo("Q_HeroSoulsBriar", 1, false, NScript::CQ_HeroSoulsBriarScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSB");
    SetScriptActiveStatus("Q_HeroSoulsBriar", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSM");
    AddScript(CScriptInfo("Q_HeroSoulsMother", 1, false, NScript::CQ_HeroSoulsMotherScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSM");
    SetScriptActiveStatus("Q_HeroSoulsMother", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSG");
    AddScript(CScriptInfo("Q_HeroSoulsGuildmaster", 1, false, NScript::CQ_HeroSoulsGuildmasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSG");
    SetScriptActiveStatus("Q_HeroSoulsGuildmaster", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSN");
    AddScript(CScriptInfo("Q_HeroSoulsNostro", 1, false, NScript::CQ_HeroSoulsNostroScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSN");
    SetScriptActiveStatus("Q_HeroSoulsNostro", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHOH");
    AddScript(CScriptInfo("Q_HerosOldHouse", 1, false, NScript::CQ_HerosOldHouseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHOH");
    SetScriptActiveStatus("Q_HerosOldHouse", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHC");
    AddScript(CScriptInfo("Q_HobbeCave", 1, false, NScript::CQ_HobbeCaveScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHC");
    SetScriptActiveStatus("Q_HobbeCave", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHTC");
    AddScript(CScriptInfo("Q_HobbeToothContest", 1, false, NScript::CQ_HobbeToothContestScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHTC");
    SetScriptActiveStatus("Q_HobbeToothContest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QMC");
    AddScript(CScriptInfo("Q_MinionCamp", 1, false, NScript::CQ_MinionCampScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QMC");
    SetScriptActiveStatus("Q_MinionCamp", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QMCC");
    AddScript(CScriptInfo("Q_MinionClifftopChase", 1, false, NScript::CQ_MinionClifftopChaseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QMCC");
    SetScriptActiveStatus("Q_MinionClifftopChase", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QNOVI");
    AddScript(CScriptInfo("Q_NewOakValeIntro", 1, false, NScript::CQ_NewOakValeIntroScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QNOVI");
    SetScriptActiveStatus("Q_NewOakValeIntro", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOVBR");
    AddScript(CScriptInfo("Q_OakValeBanditRaid", 1, false, NScript::CQ_OakValeBanditRaidScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOVBR");
    SetScriptActiveStatus("Q_OakValeBanditRaid", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOFR");
    AddScript(CScriptInfo("Q_OrchardFarmRaid", 1, false, NScript::CQ_OrchardFarmRaidScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOFR");
    SetScriptActiveStatus("Q_OrchardFarmRaid", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOFRE");
    AddScript(CScriptInfo("Q_OrchardFarmRaidEvil", 1, false, NScript::CQ_OrchardFarmRaidEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOFRE");
    SetScriptActiveStatus("Q_OrchardFarmRaidEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOFRG");
    AddScript(CScriptInfo("Q_OrchardFarmRaidGood", 1, false, NScript::CQ_OrchardFarmRaidGoodScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOFRG");
    SetScriptActiveStatus("Q_OrchardFarmRaidGood", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOGSP");
    AddScript(CScriptInfo("Q_OpeningGraveyardSecretPassage", 1, false, NScript::CQ_OpeningGraveyardSecretPassageScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOGSP");
    SetScriptActiveStatus("Q_OpeningGraveyardSecretPassage", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QPERM");
    AddScript(CScriptInfo("Q_PrisonEscapeRescueMother", 1, false, NScript::CQ_PrisonEscapeRescueMotherScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QPERM");
    SetScriptActiveStatus("Q_PrisonEscapeRescueMother", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QPR");
    AddScript(CScriptInfo("Q_PrisonRace", 1, false, NScript::CQ_PrisonRaceScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QPR");
    SetScriptActiveStatus("Q_PrisonRace", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QPWG");
    AddScript(CScriptInfo("Q_PrisonWardenGame", 1, false, NScript::CQ_PrisonWardenGameScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QPWG");
    SetScriptActiveStatus("Q_PrisonWardenGame", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRV");
    AddScript(CScriptInfo("Q_RansomVictim", 1, false, NScript::CQ_RansomVictimScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRV");
    SetScriptActiveStatus("Q_RansomVictim", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRVCH");
    AddScript(CScriptInfo("Q_RansomVictimChiefsHouse", 1, false, NScript::CQ_RansomVictimChiefsHouseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRVCH");
    SetScriptActiveStatus("Q_RansomVictimChiefsHouse", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSP");
    AddScript(CScriptInfo("Q_SecretPassage", 1, false, NScript::CQ_SecretPassageScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSP");
    SetScriptActiveStatus("Q_SecretPassage", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSTS");
    AddScript(CScriptInfo("Q_SummoningTheShip", 1, false, NScript::CQ_SummoningTheShipScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSTS");
    SetScriptActiveStatus("Q_SummoningTheShip", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTKBF");
    AddScript(CScriptInfo("Q_TentacleKrakenBossFight", 1, false, NScript::CQ_TentacleKrakenBossFightScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTKBF");
    SetScriptActiveStatus("Q_TentacleKrakenBossFight", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTCE");
    AddScript(CScriptInfo("Q_TraderConflictEvil", 1, false, NScript::CQ_TraderConflictEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTCE");
    SetScriptActiveStatus("Q_TraderConflictEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTCG");
    AddScript(CScriptInfo("Q_TraderConflictGood", 1, false, NScript::CQ_TraderConflictGoodScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTCG");
    SetScriptActiveStatus("Q_TraderConflictGood", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTE");
    AddScript(CScriptInfo("Q_TraderEscort", 1, false, NScript::CQ_TraderEscortScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTE");
    SetScriptActiveStatus("Q_TraderEscort", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWB");
    AddScript(CScriptInfo("Q_WaspBoss", 1, false, NScript::CQ_WaspBossScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWB");
    SetScriptActiveStatus("Q_WaspBoss", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWBKG");
    AddScript(CScriptInfo("Q_WhiteBalverineKnotholeGlade", 1, false, NScript::CQ_WhiteBalverineKnotholeGladeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWBKG");
    SetScriptActiveStatus("Q_WhiteBalverineKnotholeGlade", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QUR");
    AddScript(CScriptInfo("Q_UndeadRising", 1, false, NScript::CQ_UndeadRisingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QUR");
    SetScriptActiveStatus("Q_UndeadRising", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWBW");
    AddScript(CScriptInfo("Q_WhiteBalverineWW", 1, false, NScript::CQ_WhiteBalverineWWScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWBW");
    SetScriptActiveStatus("Q_WhiteBalverineWW", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWZB");
    AddScript(CScriptInfo("Q_WizardBattle", 1, false, NScript::CQ_WizardBattleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWZB");
    SetScriptActiveStatus("Q_WizardBattle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRET");
    AddScript(CScriptInfo("QR_EscortTrader", 1, false, NScript::CQR_EscortTraderScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRET");
    SetScriptActiveStatus("QR_EscortTrader", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRETM");
    AddScript(CScriptInfo("QR_EscortTrader_Manager", 1, false, NScript::CQR_EscortTrader_ManagerScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRETM");
    SetScriptActiveStatus("QR_EscortTrader_Manager", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSGSI");
    AddScript(CScriptInfo("QS_GuardianSisterInfo", 1, false, NScript::CQS_GuardianSisterInfoScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSGSI");
    SetScriptActiveStatus("QS_GuardianSisterInfo", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSGSI2");
    AddScript(CScriptInfo("QS_GuardianSisterInfo2_SisterInBanditCamp", 1, false, NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSGSI2");
    SetScriptActiveStatus("QS_GuardianSisterInfo2_SisterInBanditCamp", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSGTDI");
    AddScript(CScriptInfo("QS_GuardianTrophyDealerInfo", 1, false, NScript::CQS_GuardianTrophyDealerInfoScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSGTDI");
    SetScriptActiveStatus("QS_GuardianTrophyDealerInfo", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSMS");
    AddScript(CScriptInfo("QS_MeetSister", 1, false, NScript::CQS_MeetSisterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSMS");
    SetScriptActiveStatus("QS_MeetSister", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSSI");
    AddScript(CScriptInfo("QS_ScytheInfo", 1, false, NScript::CQS_ScytheInfoScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSSI");
    SetScriptActiveStatus("QS_ScytheInfo", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Vignette Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("S_VAS");
    AddScript(CScriptInfo("V_AmbushScam", 1, false, NScript::CV_AmbushScamScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VAS");
    SetScriptActiveStatus("V_AmbushScam", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VAC");
    AddScript(CScriptInfo("V_ArcheryCompetition", 1, false, NScript::CV_ArcheryCompetitionScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VAC");
    SetScriptActiveStatus("V_ArcheryCompetition", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VAA");
    AddScript(CScriptInfo("V_AssassinAttacks", 1, false, NScript::CV_AssassinAttacksScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VAA");
    SetScriptActiveStatus("V_AssassinAttacks", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBT");
    AddScript(CScriptInfo("V_BanditToll", 1, false, NScript::CV_BanditTollScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBT");
    SetScriptActiveStatus("V_BanditToll", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBCP");
    AddScript(CScriptInfo("V_BanditCampPath", 1, false, NScript::CV_BanditCampPathScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBCP");
    SetScriptActiveStatus("V_BanditCampPath", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBB");
    AddScript(CScriptInfo("V_BeardyBaldy", 1, false, NScript::CV_BeardyBaldyScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBB");
    SetScriptActiveStatus("V_BeardyBaldy", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBAC");
    AddScript(CScriptInfo("V_BeggarAndChild", 1, false, NScript::CV_BeggarAndChildScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBAC");
    SetScriptActiveStatus("V_BeggarAndChild", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBGu");
    AddScript(CScriptInfo("V_BodyGuard", 1, false, NScript::CV_BodyGuardScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBGu");
    SetScriptActiveStatus("V_BodyGuard", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBC");
    AddScript(CScriptInfo("V_BookCollecting", 1, false, NScript::CV_BookCollectingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBC");
    SetScriptActiveStatus("V_BookCollecting", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VB");
    AddScript(CScriptInfo("V_Bordello", 1, false, NScript::CV_BordelloScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VB");
    SetScriptActiveStatus("V_Bordello", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VCOE");
    AddScript(CScriptInfo("V_ChapelOfEvil", 1, false, NScript::CV_ChapelOfEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VCOE");
    SetScriptActiveStatus("V_ChapelOfEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VEM");
    AddScript(CScriptInfo("V_ExposeMayor", 1, false, NScript::CV_ExposeMayorScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VEM");
    SetScriptActiveStatus("V_ExposeMayor", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VCK");
    AddScript(CScriptInfo("V_ChickenKicking", 1, false, NScript::CV_ChickenKickingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VCK");
    SetScriptActiveStatus("V_ChickenKicking", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_DDRS");
    AddScript(CScriptInfo("V_DemonDoors", 1, false, NScript::CV_DemonDoorsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_DDRS");
    SetScriptActiveStatus("V_DemonDoors", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VFM");
    AddScript(CScriptInfo("V_Fisherman", 1, false, NScript::CV_FishermanScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VFM");
    SetScriptActiveStatus("V_Fisherman", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VFC");
    AddScript(CScriptInfo("V_FishingCompetition", 1, false, NScript::CV_FishingCompetitionScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VFC");
    SetScriptActiveStatus("V_FishingCompetition", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VFCC");
    AddScript(CScriptInfo("V_FisticuffsClub", 1, false, NScript::CV_FisticuffsClubScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VFCC");
    SetScriptActiveStatus("V_FisticuffsClub", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VGGN");
    AddScript(CScriptInfo("V_GhostGrannyNecklace", 1, false, NScript::CV_GhostGrannyNecklaceScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VGGN");
    SetScriptActiveStatus("V_GhostGrannyNecklace", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VGM");
    AddScript(CScriptInfo("V_GuildMaster", 1, false, NScript::CV_GuildMasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VGM");
    SetScriptActiveStatus("V_GuildMaster", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHH");
    AddScript(CScriptInfo("V_HauntedHouse", 1, false, NScript::CV_HauntedHouseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHH");
    SetScriptActiveStatus("V_HauntedHouse", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHDS");
    AddScript(CScriptInfo("V_HeroDolls", 1, false, NScript::CV_HeroDollsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHDS");
    SetScriptActiveStatus("V_HeroDolls", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHD");
    AddScript(CScriptInfo("V_HeroDuel", 1, false, NScript::CV_HeroDuelScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHD");
    SetScriptActiveStatus("V_HeroDuel", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHB");
    AddScript(CScriptInfo("V_HiddenBooty", 1, false, NScript::CV_HiddenBootyScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHB");
    SetScriptActiveStatus("V_HiddenBooty", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VITT");
    AddScript(CScriptInfo("V_IntroductionToTrophies", 1, false, NScript::CV_IntroductionToTrophiesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VITT");
    SetScriptActiveStatus("V_IntroductionToTrophies", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VKGG");
    AddScript(CScriptInfo("V_KnotholeGladeGates", 1, false, NScript::CV_KnotholeGladeGatesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VKGG");
    SetScriptActiveStatus("V_KnotholeGladeGates", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VLT");
    AddScript(CScriptInfo("V_LostTrader", 1, false, NScript::CV_LostTraderScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VLT");
    SetScriptActiveStatus("V_LostTrader", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMR");
    AddScript(CScriptInfo("V_MazeResearch", 1, false, NScript::CV_MazeResearchScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMR");
    SetScriptActiveStatus("V_MazeResearch", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMT");
    AddScript(CScriptInfo("V_MurderTwist", 1, false, NScript::CV_MurderTwistScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMT");
    SetScriptActiveStatus("V_MurderTwist", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMTF");
    AddScript(CScriptInfo("V_MurderTwistFinal", 1, false, NScript::CV_MurderTwistFinalScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMTF");
    SetScriptActiveStatus("V_MurderTwistFinal", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMI");
    AddScript(CScriptInfo("V_MayorsInvitation", 1, false, NScript::CV_MayorsInvitationScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMI");
    SetScriptActiveStatus("V_MayorsInvitation", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VO");
    AddScript(CScriptInfo("V_Oracle", 1, false, NScript::CV_OracleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VO");
    SetScriptActiveStatus("V_Oracle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VPW");
    AddScript(CScriptInfo("V_ParanoidWhispers", 1, false, NScript::CV_ParanoidWhispersScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VPW");
    SetScriptActiveStatus("V_ParanoidWhispers", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VPAAWB");
    AddScript(CScriptInfo("V_PicnicAreaAfterWaspBoss", 1, false, NScript::CV_PicnicAreaAfterWaspBossScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VPAAWB");
    SetScriptActiveStatus("V_PicnicAreaAfterWaspBoss", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VRPS");
    AddScript(CScriptInfo("V_RandomPopulationSim", 1, false, NScript::CV_RandomPopulationSimScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VRPS");
    SetScriptActiveStatus("V_RandomPopulationSim", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VRTFE");
    AddScript(CScriptInfo("V_RockTrollFirstEncounter", 1, false, NScript::CV_RockTrollFirstEncounterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VRTFE");
    SetScriptActiveStatus("V_RockTrollFirstEncounter", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSC");
    AddScript(CScriptInfo("V_SickChild", 1, false, NScript::CV_SickChildScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSC");
    SetScriptActiveStatus("V_SickChild", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSS");
    AddScript(CScriptInfo("V_SingingStones", 1, false, NScript::CV_SingingStonesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSS");
    SetScriptActiveStatus("V_SingingStones", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSM");
    AddScript(CScriptInfo("V_StatueMaster", 1, false, NScript::CV_StatueMasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSM");
    SetScriptActiveStatus("V_StatueMaster", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSITS");
    AddScript(CScriptInfo("V_SwordInTheStone", 1, false, NScript::CV_SwordInTheStoneScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSITS");
    SetScriptActiveStatus("V_SwordInTheStone", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTB");
    AddScript(CScriptInfo("V_TalentlessBard", 1, false, NScript::CV_TalentlessBardScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTB");
    SetScriptActiveStatus("V_TalentlessBard", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTG");
    AddScript(CScriptInfo("V_TourGuide", 1, false, NScript::CV_TourGuideScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTG");
    SetScriptActiveStatus("V_TourGuide", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTOL");
    AddScript(CScriptInfo("V_TempleOfLight", 1, false, NScript::CV_TempleOfLightScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTOL");
    SetScriptActiveStatus("V_TempleOfLight", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTH");
    AddScript(CScriptInfo("V_TravellingHeroes", 1, false, NScript::CV_TravellingHeroesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTH");
    SetScriptActiveStatus("V_TravellingHeroes", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTD");
    AddScript(CScriptInfo("V_TrophyDealer", 1, false, NScript::CV_TrophyDealerScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTD");
    SetScriptActiveStatus("V_TrophyDealer", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Global Quest Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_WatchForHeroDeath", 1, false, NScript::CGlobal_WatchForHeroDeathScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_WatchForHeroDeath", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_DebugCycleThroughSpeech", 1, false, NScript::CGlobal_DebugCycleThroughSpeechScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_DebugCycleThroughSpeech", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Fish", 1, false, NScript::CExpression_FishScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Fish", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Follow", 1, false, NScript::CExpression_FollowScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Follow", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Wait", 1, false, NScript::CExpression_WaitScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Wait", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Dig", 1, false, NScript::CExpression_DigScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Dig", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Pickpocket", 1, false, NScript::CExpression_PickpocketScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Pickpocket", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Picklock", 1, false, NScript::CExpression_PicklockScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Picklock", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Steal", 1, false, NScript::CExpression_StealScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Steal", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_TeleportToHeroGuild", 1, false, NScript::CGlobal_TeleportToHeroGuildScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("Global_TeleportToHeroGuild", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_ToggleTimeDisplay", 1, false, NScript::CGlobal_ToggleTimeDisplayScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_ToggleTimeDisplay", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_GiveHeroItemsFromRewardChest", 1, false, NScript::CGlobal_GiveHeroItemsFromRewardChestScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_GiveHeroItemsFromRewardChest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_OpenChest", 1, false, NScript::CGlobal_OpenChestScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_OpenChest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("CS_OVR");
    AddScript(CScriptInfo("CS_OakValeRevisited", 1, false, NScript::CCS_OakValeRevisitedScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "CS_OVR");
    SetScriptActiveStatus("CS_OakValeRevisited", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Test Scripts", -1.0f, false, false);
    AddScript(CScriptInfo("alextest", 1, true, NScript::CalextestScript::Alloc, NScript::CalextestScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("PersonalScript_alextest", 1, false, NScript::CPersonalScript_alextestScript::Alloc, NScript::CalextestScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("PersonalScript_alextest", (EScriptActiveStatus)0);
    AddScript(CScriptInfo("DanyalTestScripts", 1, true, NScript::CDanyalTestScriptsScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("ProximityPodiumTest", 1, false, NScript::CProximityPodiumTestScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("ProximityPodiumTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("HitSwitchTest", 1, false, NScript::CHitSwitchTestScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("HitSwitchTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("GuildSealTriggerTest", 1, false, NScript::CGuildSealTriggerTestScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("GuildSealTriggerTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("PersonalScript_Aardvark", 1, false, NScript::CPersonalScript_AardvarkScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("PersonalScript_Aardvark", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("HeroTurningDebugger", 1, false, NScript::CHeroTurningDebuggerScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("HeroTurningDebugger", (EScriptActiveStatus)0);
    AddScript(CScriptInfo("joss_test_scripts", 1, true, NScript::Cjoss_test_scriptsScript::Alloc, NScript::Cjoss_test_scriptsScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("guy_script", 1, false, NScript::Cguy_scriptScript::Alloc, NScript::Cjoss_test_scriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("guy_script", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("bandit_level_cutscene", 1, false, NScript::Cbandit_level_cutsceneScript::Alloc, NScript::Cjoss_test_scriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("bandit_level_cutscene", (EScriptActiveStatus)0);
    AddScript(CScriptInfo("Empty", 1, true, NScript::CEmptyScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestTheInventory", 1, false, NScript::CTestTheInventoryScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestTheInventory", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("PersonalScript_Empty", 1, false, NScript::CPersonalScript_EmptyScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("PersonalScript_Empty", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav1", 1, false, NScript::CTestNewNav1Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav1", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav2", 1, false, NScript::CTestNewNav2Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav2", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav3", 1, false, NScript::CTestNewNav3Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav3", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav4", 1, false, NScript::CTestNewNav4Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav4", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav5", 1, false, NScript::CTestNewNav5Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav5", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestQuestCard1", 1, false, NScript::CTestQuestCard1Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestQuestCard1", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestQuestCard2", 1, false, NScript::CTestQuestCard2Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestQuestCard2", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestQuestCard3", 1, false, NScript::CTestQuestCard3Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestQuestCard3", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("MarkTestScripts", 1, false, NScript::CMarkTestScriptsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("MarkTestScripts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("BenTestScripts", 1, false, NScript::CBenTestScriptsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("BenTestScripts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TonyTestScripts", 1, false, NScript::CTonyTestScriptsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TonyTestScripts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("ShowTargetedThingHealth", 1, false, NScript::CShowTargetedThingHealthScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("ShowTargetedThingHealth", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("OracleMinigameTest", 1, false, NScript::COracleMinigameTestScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("OracleMinigameTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Q_CinemaTest", 1, false, NScript::CQ_CinemaTestScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("Q_CinemaTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("CS_PlayCutscene", 1, false, NScript::CCS_PlayCutsceneScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("CS_PlayCutscene", (EScriptActiveStatus)0);
}
