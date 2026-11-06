// NScript::CQ_AmbushTradersScript::Init() -- set up the ambush-traders quest: register its four regions, reset the master-data spy flag and every quest flag / counter (spy wander reset on, five traders, guard mask 0x1f), arm the bandit and spy speech timers, clear the ConvoyTrigger1 cut-scene flag, switch the three regions' creature generators off, reset the master-data guard / trader / kill / hire tallies, fill the ten speech-line tables and register the seven boasts
// Leaf: ?Init@CQ_AmbushTradersScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF9A50, 2199 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00CFA2E6.
// Identity (proven): slot 3 (Init) of the quest-script vtable (level-script vtable 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist);
//   Ego_r.pdb name set ?Init@CQ_AmbushTradersScript@NScript@@UAEXXZ; the body writes the PDB members of engine/CQ_AmbushTradersScript.h
//   (QuestFailed +0x48 .. EnableSpeechBandit +0x6a, the counters +0x6c..+0x98, SpeechBanditTimer +0x9c / SpeechSpyTimer +0xa0, the ten
//   CCharString[5] speech tables +0xf0..+0x1b4, CSFlagMap +0x1b8) and the AmbushTraders boast / timer members of CScriptDef.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0,
//   std::map<CCharString, bool>::operator[] 0x008ADF10; CGameScriptInterfaceBase slots 271 AddQuestRegion (+0x43c), 303 AddBoast (+0x4bc),
//   558 SetCreatureGeneratorsEnabled (+0x8b8); the timer service pDSTGame 0x0143E8F8 slot 89 SetTimer (+0x164) through the inline
//   CTimer::operator=(long); pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>, first word).
// Data: CScriptDef (retail offsets, engine/CScriptDef.h): AmbushTradersNakedBoastCost +0x58 .. NoWeaponsBoastReward +0x6c,
//   AmbushTradersBoastCostKillAllGuards +0xa14 .. BoastRewardUnassisted +0xa30, AmbushTradersBanditSpeechTimerLong +0xa44,
//   AmbushTradersSpySpeechTimerLong +0xa50. CQ_SunnyvaleMasterData: AmbushTradersAllGuardsDead +0xc8, AllTradersDead +0xc9, KillCount +0xcc,
//   BanditHireCount +0xd0, SpyDead +0xd4.
// Shape: the landed CQ_MinionClifftopChaseScript::Init 0x00DA3100 model (literals straight to `const CCharString&` parameters, the zero
//   hoisted into ebx, the inline CTimer::operator= of the landed CQ_HeroSoulsMotherScript::Init 0x00D83470); stores in retail order.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: retail expands operator=(long) (slot 89 SetTimer) here; the body follows the include (engine/CTimer.h contract).
inline CTimer& CTimer::operator=(long time) { const long& index = TimerIndex; pDSTGame->SetTimer(index, time); return *this; }

void NScript::CQ_AmbushTradersScript::Init()
{
    Game->AddQuestRegion("Q_AmbushTraders", "BowerstonePosh");
    Game->AddQuestRegion("Q_AmbushTraders", "BowerstoneJail");
    Game->AddQuestRegion("Q_AmbushTraders", "DesertedFarm");
    Game->AddQuestRegion("Q_AmbushTraders", "GibbetWoods");
    MasterScriptData->AmbushTradersSpyDead = false;
    QuestFailed = false;
    QuestFinished = false;
    QuestSucceeded = false;
    QuestOver = false;
    AllBanditsDead = false;
    AllTradersDead = false;
    AllGuardsDead = false;
    HeroDead = false;
    SpyDead = false;
    ContactDead = false;
    ContactAttackedByHero = false;
    RegionInvalid = false;
    BanditIntroDone = false;
    ConvoyTrigger2 = false;
    TraderEscaped = false;
    NoEnemiesLeft = false;
    PhaseComplete = false;
    SentryAttackedByHero = false;
    SpyFollowing = false;
    SpyHeroProximity = false;
    SpyWanderReset = true;
    SpyEscaped = false;
    SpyContactMade = false;
    SpyExited = false;
    SpyReachedNoEntryMarker = false;
    AnyTraderEscaped = false;
    RegionClear = false;
    QuestSuccessSpeechComplete = false;
    SpyFollowingHero = false;
    ActivatedPhase3 = false;
    SpySeenBySentry = false;
    BanditContactIntroComplete = false;
    BanditContactKilledBeforeQuestEnd = false;
    BanditAttackedByHero = false;
    BanditAttackByHeroCount = 0;
    BanditSwitchAllegienceComplete = false;
    TraderAliveCount = 5;
    GuardAliveCount = 0x1f;
    ContactSeed = 0;
    HiredBanditCount = 0;
    SpeechBanditCount = 0;
    SpeechSpyCount = 0;
    SpeechSpyProximityCount = 0;
    SpeechSpySentryCount = 0;
    EnableSpeechSpy = false;
    EnableSpeechBandit = false;
    SpeechBanditTimer = pGlobalScriptDef->AmbushTradersBanditSpeechTimerLong;
    SpeechSpyTimer = pGlobalScriptDef->AmbushTradersSpySpeechTimerLong;
    SpyHealthBarID = 0;
    CSFlagMap["ConvoyTrigger1"] = false;
    Game->SetCreatureGeneratorsEnabled("BowerstoneJail", false);
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", false);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", false);
    MasterScriptData->AmbushTradersAllGuardsDead = false;
    MasterScriptData->AmbushTradersAllTradersDead = false;
    MasterScriptData->AmbushTradersKillCount = 0;
    MasterScriptData->AmbushTradersBanditHireCount = 0;
    BanditSpeechNormalThere[0] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_01";
    BanditSpeechNormalThere[1] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_02";
    BanditSpeechNormalThere[2] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_03";
    BanditSpeechNormalThere[3] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_04";
    BanditSpeechNormalThere[4] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_05";
    BanditSpeechRandomThere[0] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_01";
    BanditSpeechRandomThere[1] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_02";
    BanditSpeechRandomThere[2] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_03";
    BanditSpeechRandomThere[3] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_04";
    BanditSpeechRandomThere[4] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_05";
    BanditSpeechNormalBack[0] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_BACK_01";
    BanditSpeechNormalBack[1] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_BACK_02";
    BanditSpeechNormalBack[2] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_BACK_03";
    BanditSpeechNormalBack[3] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_BACK_04";
    BanditSpeechNormalBack[4] = "TEXT_QST_003_BANDIT_SPEECH_NORMAL_BACK_05";
    BanditSpeechRandomBack[0] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_BACK_01";
    BanditSpeechRandomBack[1] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_BACK_02";
    BanditSpeechRandomBack[2] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_BACK_03";
    BanditSpeechRandomBack[3] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_BACK_04";
    BanditSpeechRandomBack[4] = "TEXT_QST_003_BANDIT_SPEECH_RANDOM_BACK_05";
    SpySpeechNormal[0] = "TEXT_QST_003_SPY_SPEECH_NORMAL_01";
    SpySpeechNormal[1] = "TEXT_QST_003_SPY_SPEECH_NORMAL_02";
    SpySpeechNormal[2] = "TEXT_QST_003_SPY_SPEECH_NORMAL_03";
    SpySpeechNormal[3] = "TEXT_QST_003_SPY_SPEECH_NORMAL_04";
    SpySpeechNormal[4] = "TEXT_QST_003_SPY_SPEECH_NORMAL_05";
    SpySpeechRandom[0] = "TEXT_QST_003_SPY_SPEECH_RANDOM_01";
    SpySpeechRandom[1] = "TEXT_QST_003_SPY_SPEECH_RANDOM_02";
    SpySpeechRandom[2] = "TEXT_QST_003_SPY_SPEECH_RANDOM_03";
    SpySpeechRandom[3] = "TEXT_QST_003_SPY_SPEECH_RANDOM_04";
    SpySpeechRandom[4] = "TEXT_QST_003_SPY_SPEECH_RANDOM_05";
    SpySpeechProximityNormal[0] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_NORMAL_01";
    SpySpeechProximityNormal[1] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_NORMAL_02";
    SpySpeechProximityNormal[2] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_NORMAL_03";
    SpySpeechProximityNormal[3] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_NORMAL_04";
    SpySpeechProximityNormal[4] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_NORMAL_05";
    SpySpeechProximityRandom[0] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_RANDOM_01";
    SpySpeechProximityRandom[1] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_RANDOM_02";
    SpySpeechProximityRandom[2] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_RANDOM_03";
    SpySpeechProximityRandom[3] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_RANDOM_04";
    SpySpeechProximityRandom[4] = "TEXT_QST_003_SPY_SPEECH_PROXIMITY_RANDOM_05";
    SpySpeechSentryNormal[0] = "TEXT_QST_003_SPY_SPEECH_NORMAL_01";
    SpySpeechSentryNormal[1] = "TEXT_QST_003_SPY_SPEECH_NORMAL_02";
    SpySpeechSentryNormal[2] = "TEXT_QST_003_SPY_SPEECH_NORMAL_03";
    SpySpeechSentryNormal[3] = "TEXT_QST_003_SPY_SPEECH_NORMAL_04";
    SpySpeechSentryNormal[4] = "TEXT_QST_003_SPY_SPEECH_NORMAL_05";
    SpySpeechSentryRandom[0] = "TEXT_QST_003_SPY_SPEECH_RANDOM_01";
    SpySpeechSentryRandom[1] = "TEXT_QST_003_SPY_SPEECH_RANDOM_02";
    SpySpeechSentryRandom[2] = "TEXT_QST_003_SPY_SPEECH_RANDOM_03";
    SpySpeechSentryRandom[3] = "TEXT_QST_003_SPY_SPEECH_RANDOM_04";
    SpySpeechSentryRandom[4] = "TEXT_QST_003_SPY_SPEECH_RANDOM_05";
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->AmbushTradersNakedBoastCost,
                   pGlobalScriptDef->AmbushTradersNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->AmbushTradersNoDamageBoastCost,
                   pGlobalScriptDef->AmbushTradersNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->AmbushTradersNoWeaponsBoastCost,
                   pGlobalScriptDef->AmbushTradersNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLALLGUARDS", 11, pGlobalScriptDef->AmbushTradersBoastCostKillAllGuards,
                   pGlobalScriptDef->AmbushTradersBoastRewardKillAllGuards, true, "", 2);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLALLTRADERS", 12, pGlobalScriptDef->AmbushTradersBoastCostKillAllTraders,
                   pGlobalScriptDef->AmbushTradersBoastRewardKillAllTraders, true, "", 4);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNOONE", 13, pGlobalScriptDef->AmbushTradersBoastCostKillNone,
                   pGlobalScriptDef->AmbushTradersBoastRewardKillNone, false, "", 6);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_HIRENOBANDITS", 14, pGlobalScriptDef->AmbushTradersBoastCostUnassisted,
                   pGlobalScriptDef->AmbushTradersBoastRewardUnassisted, false, "", 0);
}