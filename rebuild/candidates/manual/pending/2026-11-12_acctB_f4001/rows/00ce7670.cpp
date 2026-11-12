// NScript::CGameflowScript::Main() -- the game-flow master script: spawn the CoreQuestReminder and CheckBarrowFieldsGuards threads, then run the main story from the saved EGameflowPosition stage to the end of the game
// Inline-budget: /d2inlT89690 -- the credits cut-scene block at 0x00CEE906 runs at static frequency 6.5e-5, where retail expands the top layer of
// CScriptGameResourceObjectScriptedThingBase::operator= (c2 trace score 89689), the CScriptGameResourceObjectMovieBase ctor (72189) and dtor (67814) and the
// CWideScreenMagicPauseEntities ctor (63439) and calls the map ctor (164066), the scripted-thing dtor (203442) and the map dtor (807208); plain /O2 (T=74)
// calls all of them and T89689 still calls operator=.  No control-flow spelling raises the block's frequency by the factor of 1000 needed.
// The two members retail calls although they score lower are declared out of line by header opt-ins: the scripted-thing ctor
// 0x007E72A0 (18067) and the CScriptGameResourceObjectBase ctor 0x006E7A60 nested in the movie ctor (21).
// Leaf: ?Main@CGameflowScript@NScript@@UAEXXZ
// Flags: /d2inlT89690
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE7670, 32,057 bytes with its four jump tables, thiscall, frame 0x824.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
class CScriptGameResourceObjectMovieBase;
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/CGameflowScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CSpawnedFunc.h"
class CScriptGameResourceObjectScriptedThingBase;
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);   // 0x008AB1E0 (out of line in retail)
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2, SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
enum ETutorialCategory { TUTORIAL_CATEGORY_QUEST = 27, TUTORIAL_CATEGORY_RENOWN = 29 };   // misc_def_types.h
void FABLE_FASTCALL AddLogbookStoryEntry(long);                        // 0x00CBE87F
void FABLE_FASTCALL AddLogbookStoryEntryString(const CCharString&);    // 0x00CBE960
void FABLE_FASTCALL AddLogbookTutorialEntry(const CCharString&);       // 0x00CBE9EE
template <> std::vector<CCharString, std::allocator<CCharString> >::~vector();   // 0x00414EA0 (out of line in retail)

void NScript::CGameflowScript::Main()
{
    TRetVoid _Fail_Val;
    AddSpawnedFunction(new CSpawnedFunc<CGameflowScript>("CoreQuestReminder", 0, this, &CGameflowScript::CoreQuestReminder), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowScript>("CheckBarrowFieldsGuards", 0, this, &CGameflowScript::CheckBarrowFieldsGuards), CCharString(""));
    switch (MasterScriptData->PostSavePosition) {
    case EGP_INTRO:
        MasterScriptData->PostSavePosition = EGP_INTRO;
        Game->SetAllSoundsAsMuted(true);
        if (!Game->IsXbox()) {
            Game->GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1, true);
            Game->GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1, true);
            Game->GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1, true);
            Game->GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1, true);
            Game->GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1, true);
        }
        AddLogbookStoryEntry(10);
        Game->SetAllowScreenFadingOnNextRegionChange(false);
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_OAKVALE_INTRO", "Q_NewOakValeIntro", false);
        while (!Game->MsgOnQuestCompleted("Q_NewOakValeIntro")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CArray<CCharString> quests;
            quests.push_back("Hook_Fresco_07_OakValeRaid");
            quests.push_back("Hook_Fresco_09_TimePassing");
            quests.push_back("Hook_Fresco_10_UneasyAlliance");
            quests.push_back("Q_GuildTraining");
            Game->ActivateMultipleQuestsWithoutLoadingResources(quests);
        }
        AddLogbookStoryEntry(30);
        Game->AddNewRumourToCategory("GUILD_TRAINING", "TEXT_AI_GOSSIP_GUILD_TRAINING_GUILD");
        Game->SetCategoryActivity("GUILD_TRAINING", true);
        Game->AddGossipVillage("GUILD_TRAINING", "VILLAGE_GUILD_COMPLEX_INSIDE");
        {
            CArray<CCharString> quests;
            quests.push_back("CreatureGenerators");
            quests.push_back("Q_ArenaHoldingScript");
            quests.push_back("V_AssassinAttacks");
            quests.push_back("V_BanditToll");
            quests.push_back("V_TravellingHeroes");
            quests.push_back("V_BeardyBaldy");
            quests.push_back("V_BodyGuard");
            quests.push_back("Q_BowerstoneTownLifeIntro");
            quests.push_back("V_ChapelOfEvil");
            quests.push_back("V_DemonDoors");
            quests.push_back("V_Fisherman");
            quests.push_back("V_FisticuffsClub");
            quests.push_back("V_HauntedHouse");
            quests.push_back("Q_HerosOldHouse");
            quests.push_back("V_HiddenBooty");
            quests.push_back("V_RandomPopulationSim");
            quests.push_back("Q_RansomVictimChiefsHouse");
            quests.push_back("V_RockTrollFirstEncounter");
            quests.push_back("V_StatueMaster");
            quests.push_back("V_SwordInTheStone");
            quests.push_back("V_TempleOfLight");
            Game->ActivateMultipleQuestsWithoutLoadingResources(quests);
        }
    case EGP_TRAINING:
        MasterScriptData->PostSavePosition = EGP_TRAINING;
        while (!Game->MsgOnQuestCompleted("Q_GuildTraining")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->DeactivateQuest("Hook_Fresco_10_UneasyAlliance", 0);
        AddLogbookStoryEntry(50);
        while (!Game->IsLevelLoaded("HeroGuildComplex")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0);
        Game->DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0);
        Game->DeactivateQuestLater("Q_GuildTrainingPreMelee", 0);
        Game->DeactivateQuestLater("Q_GuildTrainingMelee", 0);
        Game->DeactivateQuestLater("Q_GuildTrainingWill", 0);
        Game->DeactivateQuestLater("Q_GuildTrainingSkill", 0);
        Game->DeactivateQuestLater("Q_GuildTrainingDeparture", 0);
        if (Game->GetThingWithScriptName("TheRealGuildmaster").IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityTeleportToThing(Game->GetThingWithScriptName("TheRealGuildmaster"), Game->GetThingWithScriptName("M_GuildmasterMarker"), false);
        }
        Game->SetHeroGuideToShowQuestCardsWhenSpokenTo(true);
        {
            CScriptThing GuildDoors = Game->GetThingWithScriptName("GuildDoors");
            if (GuildDoors.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->SetThingAsUsable(GuildDoors, true);
            }
        }
        Game->AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss", false, false);
        CoreQuestWaiting = true;
        Game->AddQuestCard("OBJECT_DUMMY_QUEST_CARD_DEFEAT_SNOW_TROLL", "DUMMY_QUEST_HAS_NO_SCRIPT", false, false);
        Game->AddQuestCard("OBJECT_DUMMY_QUEST_CARD_SUPPRESS_UPRISING", "DUMMY_QUEST_2", false, false);
        Game->AddQuestCard("OBJECT_DUMMY_QUEST_CARD_MINION_CAMP", "DUMMY_QUEST_HAS_NO_SCRIPT", false, false);
        Game->ActivateQuest("GameflowAssistance");
        Game->ActivateQuest("CS_OakValeRevisited");
        Game->ActivateQuest("V_BeggarAndChild");
        Game->ActivateQuest("V_GuildMaster");
        Game->ActivateQuest("Q_OrchardFarm_Barricade");
        Game->ActivateQuest("V_SickChild");
        Game->ActivateQuest("V_BookCollecting");
        Game->ActivateQuest("V_ChickenKicking");
        Game->ActivateQuest("V_Bordello");
        Game->SetTeleporterAsActive(Game->GetThingWithScriptName("WitchwoodTeleporter"), false);
        Game->RemoveRumourCategory("GUILD_TRAINING");
        Game->AddNewRumourToCategory("WASP_BOSS", "TEXT_AI_GOSSIP_WASP_BOSS_GLOBAL");
        Game->SetCategoryActivity("WASP_BOSS", true);
    case EGP_WAITING_FOR_WASP_BOSS:
        MasterScriptData->PostSavePosition = EGP_WAITING_FOR_WASP_BOSS;
        Game->AutoSave();
        while (!Game->IsQuestActive("Q_WaspBoss")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CoreQuestWaiting = false;
        if (Game->DisplayTutorial(TUTORIAL_CATEGORY_QUEST)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!Game->MsgIsTutorialClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game->RemoveRumourCategory("WASP_BOSS");
        Game->AddNewRumourToCategory("DOING_WASP_BOSS", "TEXT_AI_GOSSIP_DOING_WASP_BOSS_GLOBAL");
        Game->SetCategoryActivity("DOING_WASP_BOSS", true);
    case EGP_WASP_BOSS:
        MasterScriptData->PostSavePosition = EGP_WASP_BOSS;
        while (!Game->MsgOnQuestCompleted("Q_WaspBoss")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Game->DisplayTutorial(TUTORIAL_CATEGORY_RENOWN)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!Game->MsgIsTutorialClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_FIRST", "QS_GuardianSisterInfo", false);
        AddLogbookStoryEntry(60);
        Game->ActivateQuest("V_TourGuide");
        Game->RemoveRumourCategory("DOING_WASP_BOSS");
        Game->AddNewRumourToCategory("VISIT_MAZE_1_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_1_GLOBAL");
        Game->AddNewRumourToCategory("VISIT_MAZE_1_GLOBAL", "TEXT_AI_GOSSIP_WASPBOSS_KILLED");
        Game->SetCategoryActivity("VISIT_MAZE_1_GLOBAL", true);
        Game->AddNewRumourToCategory("VISIT_MAZE_1_BSSLUMS", "TEXT_AI_GOSSIP_VISIT_MAZE_1_BSSLUMS");
        Game->SetCategoryActivity("VISIT_MAZE_1_BSSLUMS", true);
        Game->AddGossipVillage("VISIT_MAZE_1_BSSLUMS", "VILLAGE_BOWERSTONE_SLUMS");
    case EGP_MAZE_MEETING:
        MasterScriptData->PostSavePosition = EGP_MAZE_MEETING;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("QS_GuardianSisterInfo")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ActivateQuest("Hook_BowerstoneTeleportTutorial");
        AddLogbookStoryEntry(70);
        Game->ActivateQuest("V_PicnicAreaAfterWaspBoss");
        Game->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
        Game->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
        Game->DeactivateQuest("V_BeggarAndChild", 0);
        Game->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false);
        Game->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false);
        CoreQuestWaiting = true;
        Game->RemoveRumourCategory("VISIT_MAZE_1_GLOBAL");
        Game->RemoveRumourCategory("VISIT_MAZE_1_BSSLUMS");
        Game->AddNewRumourToCategory("PRE_ORCH_FARM", "TEXT_AI_GOSSIP_PRE_ORCH_FARM_GLOBAL");
        Game->SetCategoryActivity("PRE_ORCH_FARM", true);
    case EGP_WAITING_FOR_ORCHARD_FARM:
        MasterScriptData->PostSavePosition = EGP_WAITING_FOR_ORCHARD_FARM;
        Game->AutoSave();
        while (true) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->IsQuestActive("Q_OrchardFarmRaidGood")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->RemoveQuestCardFromGuild("Q_OrchardFarmRaidEvil");
                break;
            }
            if (Game->IsQuestActive("Q_OrchardFarmRaidEvil")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->RemoveQuestCardFromGuild("Q_OrchardFarmRaidGood");
                break;
            }
            Game->NewScriptFrame();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CoreQuestWaiting = false;
        AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_BOASTS");
        Game->RemoveRumourCategory("PRE_ORCH_FARM");
        Game->AddNewRumourToCategory("DOING_ORCH_FARM", "TEXT_AI_GOSSIP_DOING_ORCH_FARM_GLOBAL");
        Game->SetCategoryActivity("DOING_ORCH_FARM", true);
    case EGP_ORCHARD_FARM:
        MasterScriptData->PostSavePosition = EGP_ORCHARD_FARM;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_OrchardFarmRaidEvil") && !Game->MsgOnQuestCompleted("Q_OrchardFarmRaidGood")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true);
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", true);
        Game->ActivateQuest("V_IntroductionToTrophies");
        Game->ActivateQuest("QR_EscortTrader_Manager");
        Game->AddQuestCard("OBJECT_QUEST_CARD_DARKWOOD_TRADER_ESCORT", "Q_TraderEscort", false, false);
        Game->AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CAVE", "Q_HobbeCave", false, false);
        Game->AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CONTEST", "Q_HobbeToothContest", false, false);
        CoreQuestWaiting = true;
        Game->RemoveRumourCategory("DOING_ORCH_FARM");
        Game->AddNewRumourToCategory("TRADER_ESCORT_GLOBAL", "TEXT_AI_GOSSIP_TRADER_ESCORT_GLOBAL");
        Game->SetCategoryActivity("TRADER_ESCORT_GLOBAL", true);
        Game->AddNewRumourToCategory("TRADER_ESCORT_BSSLUMS", "TEXT_AI_GOSSIP_TRADER_ESCORT_BSSLUMS");
        Game->SetCategoryActivity("TRADER_ESCORT_BSSLUMS", true);
        Game->AddGossipVillage("TRADER_ESCORT_BSSLUMS", "VILLAGE_BOWERSTONE_SLUMS");
    case EGP_WAITING_FOR_TRADER_ESCORT:
        MasterScriptData->PostSavePosition = EGP_WAITING_FOR_TRADER_ESCORT;
        Game->AutoSave();
        while (!Game->IsQuestActive("Q_TraderEscort")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CoreQuestWaiting = false;
    case EGP_TRADER_ESCORT:
        MasterScriptData->PostSavePosition = EGP_TRADER_ESCORT;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_TraderEscort")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_AGAIN", "", true, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_SECOND", "QS_GuardianSisterInfo2_SisterInBanditCamp", false);
        AddLogbookStoryEntry(90);
        Game->DeactivateQuest("V_IntroductionToTrophies", 0);
        Game->ActivateQuest("V_TalentlessBard");
        Game->RemoveRumourCategory("TRADER_ESCORT_GLOBAL");
        Game->RemoveRumourCategory("TRADER_ESCORT_BSSLUMS");
        Game->AddNewRumourToCategory("VISIT_MAZE_2_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_2_GLOBAL");
        Game->SetCategoryActivity("VISIT_MAZE_2_GLOBAL", true);
        Game->AddNewRumourToCategory("VISIT_MAZE_2_BSSLUMS", "TEXT_AI_GOSSIP_VISIT_MAZE_2_BSSLUMS");
        Game->SetCategoryActivity("VISIT_MAZE_2_BSSLUMS", true);
        Game->AddGossipVillage("VISIT_MAZE_2_BSSLUMS", "VILLAGE_OAKVALE");
    case EGP_SECOND_MAZE_MEETING:
        MasterScriptData->PostSavePosition = EGP_SECOND_MAZE_MEETING;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CoreQuestWaiting = false;
        AddLogbookStoryEntry(110);
        Game->RemoveRumourCategory("VISIT_MAZE_2_GLOBAL");
        Game->RemoveRumourCategory("VISIT_MAZE_2_BSSLUMS");
        Game->AddNewRumourToCategory("DOING_BANDIT_CAMP_GLOBAL", "TEXT_AI_GOSSIP_DOING_BANDIT_CAMP_GLOBAL");
        Game->SetCategoryActivity("DOING_BANDIT_CAMP_GLOBAL", true);
        Game->AddNewRumourToCategory("DOING_BANDIT_CAMP_BANDITCAMP", "TEXT_AI_GOSSIP_DOING_BANDIT_CAMP_BANDITCAMP");
        Game->SetCategoryActivity("DOING_BANDIT_CAMP_BANDITCAMP", true);
        Game->AddGossipVillage("DOING_BANDIT_CAMP_BANDITCAMP", "VILLAGE_BANDIT_CAMP_MAIN");
        Game->AddGossipVillage("DOING_BANDIT_CAMP_BANDITCAMP", "VILLAGE_BANDIT_CAMP_RESIDENTIAL");
    case EGP_BANDIT_CAMP:
        MasterScriptData->PostSavePosition = EGP_BANDIT_CAMP;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_BanditCamp")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AddLogbookStoryEntry(120);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_YET_AGAIN", "", true, true);
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_TROPHY_DEALER_INFO", "QS_GuardianTrophyDealerInfo", false);
        Game->DeactivateQuest("V_BanditToll", 0);
        Game->ActivateQuest("Hook_Fresco_08_SistersStory");
        Game->RemoveRumourCategory("DOING_BANDIT_CAMP_GLOBAL");
        Game->RemoveRumourCategory("DOING_BANDIT_CAMP_BANDITCAMP");
        Game->AddNewRumourToCategory("VISIT_MAZE_3_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_3_GLOBAL");
        Game->SetCategoryActivity("VISIT_MAZE_3_GLOBAL", true);
        Game->AddNewRumourToCategory("VISIT_MAZE_3_GUILD", "TEXT_AI_GOSSIP_VISIT_MAZE_3_GUILD");
        Game->SetCategoryActivity("VISIT_MAZE_3_GUILD", true);
        Game->AddGossipVillage("VISIT_MAZE_3_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE");
        Game->AddQuestCard("OBJECT_QUEST_CARD_TRADER_CONFLICT_EVIL", "Q_TraderConflictEvil", false, false);
        Game->AddQuestCard("OBJECT_QUEST_CARD_TRADER_CONFLICT_GOOD", "Q_TraderConflictGood", false, false);
        Game->ActivateQuest("V_MurderTwistFinal");
    case EGP_MAZE_TELEPORT_TO_WW:
        MasterScriptData->PostSavePosition = EGP_MAZE_TELEPORT_TO_WW;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("QS_GuardianTrophyDealerInfo")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ActivateQuestWithoutLoadingResources("Hook_BalverinesInKHGPreWhiteBalv");
        Game->RemoveRumourCategory("VISIT_MAZE_3_GLOBAL");
        Game->RemoveRumourCategory("VISIT_MAZE_3_GUILD");
        Game->AddNewRumourToCategory("FIND_ARCHAEOLOGIST", "TEXT_AI_GOSSIP_FIND_ARCHAEOLOGIST_GLOBAL");
        Game->SetCategoryActivity("FIND_ARCHAEOLOGIST", true);
    case EGP_TROPHY_DEALER:
        MasterScriptData->PostSavePosition = EGP_TROPHY_DEALER;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("V_TrophyDealer")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ActivateQuest("V_AmbushScam");
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FOUND_TROPHY_DEALER", "", true, true);
        AddLogbookStoryEntry(140);
        Game->AddQuestCard("OBJECT_QUEST_CARD_WHITE_BALVERINE_KNOTHOLE_GLADE", "Q_WhiteBalverineKnotholeGlade", false, false);
        Game->RemoveRumourCategory("FIND_ARCHAEOLOGIST");
        Game->AddNewRumourToCategory("PRE_WHITE_BALV_GLOBAL", "TEXT_AI_GOSSIP_PRE_WHITE_BALV_GLOBAL");
        Game->SetCategoryActivity("PRE_WHITE_BALV_GLOBAL", true);
        Game->SetCategoryActivity("PRE_WHITE_BALV_WITCHWOOD", true);
    case EGP_WITCHWOOD_POST_TROPHY_DEALER:
        MasterScriptData->PostSavePosition = EGP_WITCHWOOD_POST_TROPHY_DEALER;
        Game->AutoSave();
        CoreQuestWaiting = true;
        {
            bool GaveMessage = false;
            while (!Game->IsQuestActive("Q_WhiteBalverineKnotholeGlade")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->IsRegionLoaded("KnotholeGlade")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!GaveMessage) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_KNOTHOLE_GLADE_CLOSED", "", true, true);
                        GaveMessage = true;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    GaveMessage = false;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        CoreQuestWaiting = false;
        Game->DeactivateQuest("Hook_BalverinesInKHGPreWhiteBalv", 0);
        Game->RemoveRumourCategory("PRE_WHITE_BALV_GLOBAL");
        Game->RemoveRumourCategory("PRE_WHITE_BALV_WITCHWOOD");
        Game->AddNewRumourToCategory("DOING_WHITE_BALV_GLOBAL", "TEXT_AI_GOSSIP_DOING_WHITE_BALV_GLOBAL");
        Game->SetCategoryActivity("DOING_WHITE_BALV_GLOBAL", true);
        Game->AddNewRumourToCategory("DOING_WHITE_BALV_KHG", "TEXT_AI_GOSSIP_DOING_WHITE_BALV_KHG");
        Game->SetCategoryActivity("DOING_WHITE_BALV_KHG", true);
        Game->AddGossipVillage("DOING_WHITE_BALV_KHG", "VILLAGE_KNOTHOLE_GLADE");
    case EGP_WHITE_BALVERINE_KHG:
    case EGP_WHITE_BALVERINE_WITCHWOOD:
        MasterScriptData->PostSavePosition = EGP_WHITE_BALVERINE_WITCHWOOD;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_WhiteBalverineKnotholeGlade")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AddLogbookStoryEntry(150);
        AddLogbookStoryEntry(160);
        Game->AddQuestCard("OBJECT_QUEST_CARD_ARENA", "Q_Arena", false, false);
        CoreQuestWaiting = true;
        Game->RemoveQuestCardFromGuild("Q_HobbeToothContest");
        Game->ActivateQuest("V_ArcheryCompetition");
        Game->RemoveRumourCategory("DOING_WHITE_BALV_GLOBAL");
        Game->RemoveRumourCategory("DOING_WHITE_BALV_KHG");
        Game->AddNewRumourToCategory("PRE_ARENA_GLOBAL", "TEXT_AI_GOSSIP_PRE_ARENA_GLOBAL");
        Game->SetCategoryActivity("PRE_ARENA_GLOBAL", true);
        Game->AddNewRumourToCategory("PRE_ARENA_KHG", "TEXT_AI_GOSSIP_PRE_ARENA_KHG");
        Game->SetCategoryActivity("PRE_ARENA_KHG", true);
        Game->AddGossipVillage("PRE_ARENA_KHG", "VILLAGE_KNOTHOLE_GLADE");
    case EGP_WITCHWOOD_WAITING_FOR_ARENA:
        MasterScriptData->PostSavePosition = EGP_WITCHWOOD_WAITING_FOR_ARENA;
        Game->AutoSave();
        while (!Game->IsQuestActive("Q_Arena")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CoreQuestWaiting = false;
    case EGP_ARENA:
        MasterScriptData->PostSavePosition = EGP_ARENA;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_Arena")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (MasterScriptData->WhisperKilledByHero) {
            AddLogbookStoryEntry(175);
        } else {
            AddLogbookStoryEntry(170);
        }
        AddLogbookStoryEntry(180);
        Game->ActivateQuest("Hook_Fresco_05_MothersStory");
        Game->ActivateQuest("Hook_Fresco_13_KilledScorpion");
        Game->ActivateQuest("V_GhostGrannyNecklace");
        Game->ActivateQuest("V_MayorsInvitation");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MEET_SISTER", "QS_MeetSister", true);
        Game->AddQuestCard("OBJECT_QUEST_CARD_BREAK_SIEGE", "Q_BreakSiege", false, true);
        Game->AddQuestCard("OBJECT_QUEST_CARD_LOST_TRADER", "V_LostTrader", false, true);
        Game->RemoveQuestCardFromGuild("Q_HobbeCave");
        Game->DeactivateQuest("Q_HobbeToothContest", 0);
        if (MasterScriptData->WhisperKilledByHero) {
            Game->AddNewRumourToCategory("WHISPERS_FATE", "TEXT_AI_GOSSIP_KILLED_WHISPER_GLOBAL");
            Game->SetCategoryActivity("WHISPERS_FATE", true);
        } else {
            Game->AddNewRumourToCategory("WHISPERS_FATE", "TEXT_AI_GOSSIP_SPARED_WHISPER_GLOBAL");
            Game->SetCategoryActivity("WHISPERS_FATE", true);
        }
    case EGP_MEET_SISTER:
        MasterScriptData->PostSavePosition = EGP_MEET_SISTER;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("QS_MeetSister")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AddLogbookStoryEntry(190);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->AddQuestCard("OBJECT_QUEST_CARD_MINION_CLIFFTOP_CHASE", "Q_MinionClifftopChase", false, false);
        CoreQuestWaiting = true;
        Game->RemoveRumourCategory("PRE_ARENA_GLOBAL");
        Game->RemoveRumourCategory("PRE_ARENA_KHG");
        Game->AddNewRumourToCategory("PRE_MCC_GLOBAL", "TEXT_AI_GOSSIP_PRE_MCC_GLOBAL");
        Game->SetCategoryActivity("PRE_MCC_GLOBAL", true);
        Game->AddNewRumourToCategory("PRE_MCC_BSTONES", "TEXT_AI_GOSSIP_PRE_MCC_BSTONES");
        Game->SetCategoryActivity("PRE_MCC_BSTONES", true);
        Game->AddGossipVillage("PRE_MCC_BSTONES", "VILLAGE_BOWERSTONE_SLUMS");
        Game->AddGossipVillage("PRE_MCC_BSTONES", "VILLAGE_BOWERSTONE_POSH");
    case EGP_WAITING_FOR_MCC:
        MasterScriptData->PostSavePosition = EGP_WAITING_FOR_MCC;
        Game->AutoSave();
        while (!Game->IsQuestActive("Q_MinionClifftopChase")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CoreQuestWaiting = false;
        Game->RemoveRumourCategory("PRE_MCC_GLOBAL");
        Game->RemoveRumourCategory("PRE_MCC_STONES");
        Game->AddNewRumourToCategory("DOING_MCC_GLOBAL", "TEXT_AI_GOSSIP_DOING_MCC_GLOBAL");
        Game->SetCategoryActivity("DOING_MCC_GLOBAL", true);
    case EGP_MINION_CLIFFTOP_CHASE:
        MasterScriptData->PostSavePosition = EGP_MINION_CLIFFTOP_CHASE;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_MinionClifftopChase")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_EVIL", "Q_HangingTreeEvil", false, false);
        Game->AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_GOOD", "Q_HangingTreeGood", false, false);
        AddLogbookStoryEntry(200);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GRAVEYARD_SECRET_PASSAGE", "Q_OpeningGraveyardSecretPassage", false);
        Game->RemoveRumourCategory("DOING_MCC_GLOBAL");
        Game->AddNewRumourToCategory("DOING_GRAVEYARD_GLOBAL", "TEXT_AI_GOSSIP_DOING_GRAVEYARD_GLOBAL");
        Game->SetCategoryActivity("DOING_GRAVEYARD_GLOBAL", true);
        Game->SetCategoryActivity("DOING_GRAVEYARD_GRAVEYARD", true);
        Game->AddQuestCard("OBJECT_QUEST_CARD_MINION_CAMP", "Q_MinionCamp", false, false);
    case EGP_GRAVEYARD:
        MasterScriptData->PostSavePosition = EGP_GRAVEYARD;
        Game->AutoSave();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game->MsgOnQuestCompleted("Q_OpeningGraveyardSecretPassage")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->AddQuestCard("OBJECT_QUEST_CARD_BOUNTY_HUNT", "Q_BountyHunt", false, false);
        Game->RemoveRumourCategory("DOING_GRAVEYARD_GLOBAL");
        Game->RemoveRumourCategory("DOING_GRAVEYARD_GRAVEYARD");
        Game->RemoveRumourCategory("WHISPERS_FATE");
    case EGP_PRISON:
        MasterScriptData->PostSavePosition = EGP_PRISON;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_PrisonEscapeRescueMother")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AddLogbookStoryEntry(230);
        Game->ActivateQuest("Hook_Fresco_03_JacksObsession");
        Game->ActivateQuest("Hook_Fresco_06_Prison");
        Game->ActivateQuest("Hook_Fresco_04_Kraken");
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_GUILD_SEAL_ACTIVE", "", true, true);
        Game->AddQuestCard("OBJECT_QUEST_CARD_AMBUSH_TRADERS", "Q_AmbushTraders", false, false);
        Game->DeactivateQuest("V_BeardyBaldy", 0);
        Game->DeactivateQuest("V_ParanoidWhispers", 0);
        Game->AddNewRumourToCategory("HOOK_COAST_GATEWAY_GLOBAL", "TEXT_AI_GOSSIP_HOOK_COAST_GATEWAY_GLOBAL");
        Game->AddNewRumourToCategory("HOOK_COAST_GATEWAY_GLOBAL", "TEXT_AI_GOSSIP_AFTER_PRISON_GLOBAL");
        Game->SetCategoryActivity("HOOK_COAST_GATEWAY_GLOBAL", true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GATEWAY_TO_HOOK_COAST", "Q_EndGame", false);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    case EGP_HOOK_COAST:
        MasterScriptData->PostSavePosition = EGP_HOOK_COAST;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_EndGame")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    case EGP_WAITING_FOR_WIZARD_BATTLE:
        MasterScriptData->PostSavePosition = EGP_WAITING_FOR_WIZARD_BATTLE;
        CoreQuestWaiting = true;
        {
            bool GaveMessage = false;
            while (!Game->IsQuestActive("Q_WizardBattle")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->IsRegionLoaded("HeroGuildComplexInside")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!GaveMessage) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->AutoSave();
                        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MOTHER_LEFT_YOU_THIS", "", true, true);
                        GaveMessage = true;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    GaveMessage = false;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        CoreQuestWaiting = false;
        Game->RemoveRumourCategory("HOOK_COAST_GATEWAY_GLOBAL");
        Game->RemoveRumourCategory("HOOK_COAST_GATEWAY_DARKWOOD");
        Game->AddNewRumourToCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL");
        Game->SetCategoryActivity("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL", true);
        Game->AddNewRumourToCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST", "TEXT_AI_GOSSIP_IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST");
        Game->SetCategoryActivity("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST", true);
        Game->AddGossipVillage("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST", "VILLAGE_HOOK_COAST");
        Game->DeactivateQuest("Q_RansomVictimChiefsHouse", 0);
        Game->PrepareQuestsWhenFinalQuestIsActivated("Q_WizardBattle");
        Game->DeactivateQuest("V_RandomPopulationSim", 0);
        Game->ActivateQuest("Q_RansomVictimChiefsHouse");
    case EGP_WIZARD_BATTLE:
        MasterScriptData->PostSavePosition = EGP_WIZARD_BATTLE;
        while (!Game->MsgOnQuestCompleted("Q_WizardBattle")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL");
        Game->RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST");
        Game->AddNewRumourToCategory("AFTER_WIZARD_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_WIZARD_BATTLE_GLOBAL");
        Game->SetCategoryActivity("AFTER_WIZARD_BATTLE_GLOBAL", true);
        Game->ActivateQuest("Hook_Fresco_10_UneasyAlliance");
    case EGP_WAITING_FOR_FOCAL_SITES:
        MasterScriptData->PostSavePosition = EGP_WAITING_FOR_FOCAL_SITES;
        Game->AutoSave();
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_GO_TO_WITCHWOOD_FOR_FOCAL_SITES", "", true, true);
        while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ActivateQuest("Q_EndGameFocalSites");
        AddLogbookStoryEntry(270);
        Game->DeactivateQuest("V_TourGuide", 0);
        Game->DeactivateQuest("QR_EscortTrader_Manager", 0);
        Game->DeactivateQuest("V_GhostGrannyNecklace", 0);
        Game->DeactivateQuest("V_SickChild", 0);
        Game->DeactivateQuest("V_SickChildBarrowFields", 0);
        Game->DeactivateQuest("V_SickChild_Activate", 0);
        Game->RemoveRumourCategory("AFTER_WIZARD_BATTLE_GLOBAL");
        Game->AddNewRumourToCategory("DOING_FOCAL_SITES_GLOBAL", "TEXT_AI_GOSSIP_DOING_FOCAL_SITES_GLOBAL");
        Game->SetCategoryActivity("DOING_FOCAL_SITES_GLOBAL", true);
        Game->AddNewRumourToCategory("DOING_FOCAL_SITES_GUILD", "TEXT_AI_GOSSIP_DOING_FOCAL_SITES_GUILD");
        Game->SetCategoryActivity("DOING_FOCAL_SITES_GUILD", true);
        Game->AddGossipVillage("DOING_FOCAL_SITES_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE");
    case EGP_FOCAL_SITES:
        MasterScriptData->PostSavePosition = EGP_FOCAL_SITES;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_EndGameFocalSites")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AddLogbookStoryEntry(280);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->DeactivateQuest("V_GuildMaster", 0);
        Game->ActivateQuest("Q_EndGameBossBattle");
        Game->RemoveRumourCategory("DOING_FOCAL_SITES_GLOBAL");
        Game->RemoveRumourCategory("DOING_FOCAL_SITES_GUILD");
        Game->AddNewRumourToCategory("FINAL_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_FINAL_BATTLE_GLOBAL");
        Game->SetCategoryActivity("FINAL_BATTLE_GLOBAL", true);
        Game->AddNewRumourToCategory("FINAL_BATTLE_GUILD", "TEXT_AI_GOSSIP_FINAL_BATTLE_GUILD");
        Game->SetCategoryActivity("FINAL_BATTLE_GUILD", true);
        Game->AddGossipVillage("FINAL_BATTLE_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE");
    case EGP_JACK_BOSS_FIGHT:
        MasterScriptData->PostSavePosition = EGP_JACK_BOSS_FIGHT;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_EndGameBossBattle")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (Game->IsQuestActive("Q_EndGameBossBattle") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->RemoveRumourCategory("FINAL_BATTLE_GLOBAL");
        Game->RemoveRumourCategory("FINAL_BATTLE_GUILD");
        Game->AddNewRumourToCategory("AFTER_FINAL_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_GLOBAL");
        Game->AddNewRumourToCategory("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL");
        Game->AddNewRumourToCategory("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL");
        Game->AddNewRumourToCategory("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL");
        Game->AddNewRumourToCategory("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_NOTGOT_SWORD_GLOBAL");
        Game->SetCategoryActivity("AFTER_FINAL_BATTLE_GLOBAL", true);
        if (MasterScriptData->JackBossBattleResult == 1) {
            Game->SetCategoryActivity("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", true);
            Game->SetCategoryActivity("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", true);
            AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_305");
        } else if (MasterScriptData->JackBossBattleResult == 2) {
            Game->SetCategoryActivity("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", true);
            Game->SetCategoryActivity("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", true);
            AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_300");
        }
        Game->ActivateQuest("V_RandomPopulationSim");
        Game->PrepareQuestsWhenFinalQuestIsCompleted();
        CoreQuestWaiting = false;
        Game->RemoveQuestCardFromGuild("Q_BreakSiege");
        Game->DeactivateQuest("Q_BreakSiege", 0);
        MasterScriptData->BreakSiegeFinished = true;
        Game->RemoveQuestCardFromGuild("Q_MinionCamp");
        Game->DeactivateQuest("Q_MinionCamp", 0);
        Game->AddQuestCard("OBJECT_QUEST_CARD_RANSOM_VICTIM", "Q_RansomVictim", false, false);
        Game->DeactivateQuest("V_AmbushScam", 0);
        Game->ActivateQuest("DummyQuestForScarletRoseStatue");
        Game->AddNewRumourToCategory("LOOKOUT_POINT_DEMON_DOOR_READY", "TEXT_AI_GOSSIP_LOOKOUT_POINT_DEMON_DOOR_READY");
        Game->SetCategoryActivity("LOOKOUT_POINT_DEMON_DOOR_READY", true);
        while (!Game->IsRegionLoaded("OakvaleMemorialGarden")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FIRE_HEART", "Q_FireHeart", false);
    case EGP_FIRE_HEART:
        MasterScriptData->PostSavePosition = EGP_FIRE_HEART;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_FireHeart")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ActivateQuest("V_GuildMaster");
        Game->RemoveRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SUMMONING_THE_SHIP", "Q_SummoningTheShip", false);
        if (MasterScriptData->JackBossBattleResult == 2) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MAZE_RESEARCH", "V_MazeResearch", false);
        }
        Game->AddNewRumourToCategory("SUMMON_THE_SHIP", "TEXT_AI_GOSSIP_SUMMON_THE_SHIP");
        Game->SetCategoryActivity("SUMMON_THE_SHIP", true);
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_310");
    case EGP_THE_SHIP:
        MasterScriptData->PostSavePosition = EGP_THE_SHIP;
        Game->AutoSave();
        Game->FadeScreenIn();
        while (!Game->MsgOnQuestCompleted("Q_SummoningTheShip")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game->IsRegionLoaded("LostBay")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ASK_ORACLE", "Q_AwakeningTheOracle", false);
        Game->RemoveRumourCategory("SUMMON_THE_SHIP");
        Game->AddNewRumourToCategory("NORTHERN_WASTES_OPEN", "TEXT_AI_GOSSIP_NORTHERN_WASTES_OPEN");
        Game->SetCategoryActivity("NORTHERN_WASTES_OPEN", true);
        Game->AddNewRumourToCategory("SCARY_NECROPOLIS", "TEXT_AI_GOSSIP_SCARY_NECROPOLIS");
        Game->SetCategoryActivity("SCARY_NECROPOLIS", true);
        Game->SetCategoryActivity("SNOWSPIRE_ARRIVAL", true);
        Game->AddGossipVillage("SNOWSPIRE_ARRIVAL", "VILLAGE_SNOWSPIRE");
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_320");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    case EGP_THE_ORACLE:
        MasterScriptData->PostSavePosition = EGP_THE_ORACLE;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_AwakeningTheOracle")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SCYTHE_INFO", "QS_ScytheInfo", false);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    case EGP_SCYTHE_MEETING:
        MasterScriptData->PostSavePosition = EGP_SCYTHE_MEETING;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("QS_ScytheInfo")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS", "Q_HeroSouls", false);
        Game->RemoveRumourCategory("SCARY_NECROPOLIS");
        Game->AddNewRumourToCategory("NECROPOLIS_FINISHED", "TEXT_AI_GOSSIP_NECROPOLIS_FINISHED");
        Game->SetCategoryActivity("NECROPOLIS_FINISHED", true);
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_330");
        Game->ActivateQuest("V_Oracle");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    case EGP_HERO_SOULS:
        MasterScriptData->PostSavePosition = EGP_HERO_SOULS;
        Game->AutoSave();
        while (!Game->MsgOnQuestCompleted("Q_HeroSouls")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->AddNewRumourToCategory("DRAGON_GATE_OPEN", "TEXT_AI_GOSSIP_DRAGON_GATE_OPEN");
        Game->SetCategoryActivity("DRAGON_GATE_OPEN", true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_DRAGON_BOSS", "Q_DragonBossFight", false);
    case EGP_DRAGON_FIGHT:
        MasterScriptData->PostSavePosition = EGP_DRAGON_FIGHT;
        Game->AutoSave();
        Game->FadeScreenIn();
        while (!Game->MsgOnQuestCompleted("Q_DragonBossFight")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (Game->IsQuestActive("Q_DragonBossFight") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ActivateQuest("Hook_Fresco_12_KilledDragon");
        Game->SetRegionTextDisplayAsActive(false);
        Game->SetAllowScreenFadingOnNextRegionChange(false);
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(1.0f, 0.0f);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("FrescoDomeHSP"), false);
        while (!Game->IsLevelLoaded("FrescoDome")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"] = seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            RunCutsceneMacro("CS_FABLE_CREDITS", cast, 0, 0, false, true);
        }
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("NW3BronzeDoorHSP"), false);
        while (!Game->IsRegionLoaded("NorthernWastes3")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        MasterScriptData->BodyGuardsInLimbo = false;
        Game->TeleportAllFollowersToHeroPosition();
        Game->SetRegionTextDisplayAsActive(true);
        Game->SetAllowScreenFadingOnNextRegionChange(true);
        MasterScriptData->PostSavePosition = EGP_FINISHED_GAME;
        Game->GiveHeroExpression("EXPRESSION_SHIT", -1, false);
        Game->GiveHeroExpression("EXPRESSION_THANKS", -1, false);
        Game->GiveHeroExpression("EXPRESSION_HEROIC_STANCE", -1, false);
        Game->GiveHeroExpression("EXPRESSION_FLIRT", -1, false);
        Game->GiveHeroExpression("EXPRESSION_APOLOGY", -1, false);
        Game->GiveHeroExpression("EXPRESSION_SNEER", -1, false);
        Game->GiveHeroExpression("EXPRESSION_EVIL_LAUGH", -1, false);
        Game->GiveHeroExpression("EXPRESSION_BATTLE_CRY", -1, false);
        Game->GiveHeroExpression("EXPRESSION_PELVIC_THRUST", -1, false);
        Game->GiveHeroExpression("EXPRESSION_MIDDLE_FINGER", -1, false);
        Game->GiveHeroExpression("EXPRESSION_BELCH", -1, false);
        Game->GiveHeroExpression("EXPRESSION_FART", -1, false);
        Game->GiveHeroExpression("EXPRESSION_VICTORY_PUMP", -1, false);
        Game->GiveHeroExpression("EXPRESSION_COCK_A_DOODLE_DO", -1, false);
        Game->GiveHeroExpression("EXPRESSION_CROTCH_GRAB", -1, false);
        Game->GiveHeroExpression("EXPRESSION_KISS_MY_ASS", -1, false);
        Game->GiveHeroExpression("EXPRESSION_FLAMENCO", -1, false);
        Game->GiveHeroExpression("EXPRESSION_COSSACK", -1, false);
        Game->GiveHeroExpression("EXPRESSION_AIR_GUITAR", -1, false);
        Game->GiveHeroExpression("EXPRESSION_BALLET", -1, false);
        Game->GiveHeroExpression("EXPRESSION_SATURDAY_NIGHT_FEVER", -1, false);
        Game->GiveHeroExpression("EXPRESSION_TAP", -1, false);
        Game->AutoSave();
        Game->FadeScreenIn();
    case EGP_FINISHED_GAME:
        do {
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}
