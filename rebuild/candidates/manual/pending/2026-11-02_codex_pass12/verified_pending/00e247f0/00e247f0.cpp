// NScript::CQR_EscortTraderScript::Init() -- initialise escort state, timers, routes and speech identifiers.
// Leaf: ?Init@CQR_EscortTraderScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
// Flags: /Gr
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SCRIPT_QUEST
#define FABLE_RETAIL_ESCORT_TRADER
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/NScript_CQR_EscortTraderScript.h"
#include "engine/CGameScriptInterfaceBase.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer& CTimer::operator=(long time) {pDSTGame->SetTimer(TimerIndex,time);return *this;}
void NScript::CQR_EscortTraderScript::Init()
{
 QuestStage=QUEST_STAGE_NONE;
 BanditSpawnCount=0;
 BanditKillCount=0;
 TraderHitCount=0;
 TraderInitialHealth=0.0f;
 MissionSucceeded=false;
 MissionFailed=false;
 RegionChangeDetected=false;
 GreetedHero=false;
 TraderKilled=false;
 TraderCreated=false;
 TraderHit=false;
 TraderFollowing=false;
 TraderDepartedLost=false;
 TraderDeparted=false;
 WrongTurnDetected=false;
 AddedQuestInfo=false;
 GoneWrongWay=false;
 TotallyLost=false;
 TraderIntroDone=false;
 DoneExpressions=false;
 TheBlackGateIsClosed=false;
 ExpressionDetectedFollow=false;
 ExpressionDetectedWait=false;
 TraderComplainTimer=0;
 TraderExpressionDetectTimer=0;
 TraderHitDetectTimer=0;
 RouteDynamic[ETR_ORCHARD_FARM].push_back("LookoutPoint");
 RouteDynamic[ETR_ORCHARD_FARM].push_back("GreatwoodEntrance");
 RouteDynamic[ETR_ORCHARD_FARM].push_back("GreatwoodLake");
 RouteDynamic[ETR_ORCHARD_FARM].push_back("OrchardFarm");
 RouteStatic[ETR_ORCHARD_FARM].push_back("LookoutPoint");
 RouteStatic[ETR_ORCHARD_FARM].push_back("GreatwoodEntrance");
 RouteStatic[ETR_ORCHARD_FARM].push_back("GreatwoodLake");
 RouteStatic[ETR_ORCHARD_FARM].push_back("OrchardFarm");
 TradeRoute=ETR_ORCHARD_FARM;
 HiredByTrader=false;
 TraderAtDestination=false;
 CurrentLoadedRegion=RouteDynamic[TradeRoute][0];
 PreviousLoadedRegion=CurrentLoadedRegion;
 RandomSpeechFollow[0]="TEXT_QST_053_RANDOM_FOLLOW_00";
 RandomSpeechFollow[1]="TEXT_QST_053_RANDOM_FOLLOW_10";
 RandomSpeechFollow[2]="TEXT_QST_053_RANDOM_FOLLOW_20";
 RandomSpeechFollow[3]="TEXT_QST_053_RANDOM_FOLLOW_30";
 RandomSpeechFollow[4]="TEXT_QST_053_RANDOM_FOLLOW_40";
 RandomSpeechFollow[5]="TEXT_QST_053_RANDOM_FOLLOW_50";
 RandomSpeechFollow[6]="TEXT_QST_053_RANDOM_FOLLOW_60";
 RandomSpeechFollow[7]="TEXT_QST_053_RANDOM_FOLLOW_70";
 RandomSpeechFollow[8]="TEXT_QST_053_RANDOM_FOLLOW_80";
 RandomSpeechFollow[9]="TEXT_QST_053_RANDOM_FOLLOW_90";
 RandomSpeechWait[0]="TEXT_QST_053_RANDOM_WAIT_00";
 RandomSpeechWait[1]="TEXT_QST_053_RANDOM_WAIT_10";
 RandomSpeechWait[2]="TEXT_QST_053_RANDOM_WAIT_20";
 RandomSpeechWait[3]="TEXT_QST_053_RANDOM_WAIT_30";
 RandomSpeechWait[4]="TEXT_QST_053_RANDOM_WAIT_40";
 AddedBonusObject=false;
}
