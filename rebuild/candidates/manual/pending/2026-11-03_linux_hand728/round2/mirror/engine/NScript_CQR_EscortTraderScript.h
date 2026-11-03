#ifndef FABLE_ENGINE_NSCRIPT_CQR_ESCORTTRADERSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CQR_ESCORTTRADERSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
// Original Ego_r/FableWin owner and full field order; native base0x48, vectors12bytes.
class CScriptThing;
class CScriptGameResourceObjectScriptedThingBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 typedef CQR_EscortTraderScript TMyClass;
 typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
 CQR_EscortTraderScript(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CCharString __fastcall StatGetClassName();
 static CScriptDataBase* (__fastcall* __fastcall StatGetMasterScriptDataAlloc())();
 virtual void RegisterMain();
 enum EscortTraderSuccessType { ET_SUCCESS_TYPE_LOW=0, ET_SUCCESS_TYPE_MEDIUM=1, ET_SUCCESS_TYPE_HIGH=2, ET_SUCCESS_TYPE_PERFECT=3, ET_SUCCESS_TYPE_MAX=4 };
 enum ETradeRoutes { ETR_OAK_VALE=0, ETR_ORCHARD_FARM=1, ETR_GRAVEYARD=2, ETR_NUM_ROUTES=3 };
 enum EscortTraderQuestStage { QUEST_STAGE_NONE=0, QUEST_STAGE_SETUP=1, QUEST_STAGE_HIRED_FOLLOWING=2, QUEST_STAGE_HIRED_WAITING=3, QUEST_STAGE_MAX=4 };
 enum SubObjectives { OBJECTIVE_NONE=0, OBJECTIVE_ESCORT_TRADER=1, OBJECTIVE_SECOND_OBJECTIVE=2, OBJECTIVE_THIRD_OBJECTIVE=3, OBJECTIVE_MAX=4 };
 bool MissionSucceeded;
 bool MissionFailed;
 bool HiredByTrader;
 bool TraderAtDestination;
 bool TraderDepartedLost;
 bool GreetedHero;
 bool TraderKilled;
 bool TraderHit;
 bool TraderFollowing;
 bool TraderDeparted;
 long BanditSpawnCount;
 long BanditKillCount;
 bool TraderIntroDone;
 bool DoneExpressions;
 bool TraderCreated;
 bool WrongTurnDetected;
 bool TheBlackGateIsClosed;
 bool AddedQuestInfo;
 bool GoneWrongWay;
 bool TotallyLost;
 long TraderHitCount;
 long TraderHealthBarID;
 float TraderInitialHealth;
 bool ExpressionDetectedFollow;
 bool ExpressionDetectedWait;
 bool AddedBonusObject;
 CCharString BonusObject;
 CTimer TraderComplainTimer;
 CTimer TraderExpressionDetectTimer;
 CTimer TraderHitDetectTimer;
 CTimer TraderRandomSpeechTimer;
 CCharString RandomSpeechFollow[10];
 CCharString RandomSpeechWait[5];
 ETradeRoutes TradeRoute;
 std::vector<CCharString> RouteDynamic[3];
 std::vector<CCharString> RouteStatic[3];
 CCharString PreviousLoadedRegion;
 CCharString CurrentLoadedRegion;
 bool RegionChangeDetected;
 EscortTraderQuestStage QuestStage;
 virtual void OnPersist(CPersistContext&);
 virtual void Init();
 virtual void Main();
 void WatchForFailure();
 void WatchForTermination();
 void WatchForRegionLoads();
 bool TravellingWrongPath();
 bool InFinalRegion();
 bool InInitialRegion();
 void DoMission();
 void EscortTrader();
 void EndMission();
 void WatchForExpressionFollow(CScriptThing);
 void WatchForExpressionWait(CScriptThing);
 void ResetTraderRandomSpeechTime();
 bool GetTraderRandomSpeechEnabled();
 class CTraderToEscort;
 void ExpressionEnableFollow(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&);
 void ExpressionEnableWait(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&);
 void ForceDisableFollow(const CScriptThing&);
 void ForceEnableWait(const CScriptThing&);
 EscortTraderSuccessType GetSuccessType(const CScriptThing&);
 void WatchForTraderHit(CScriptThing);
 void WatchForTraderDead(CScriptThing);
 class CM_BanditTrigger;
 class CAttackingBandit;
 void WatchForBanditDead(CScriptThing);
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQR_EscortTraderScript)==0x120);
#endif
