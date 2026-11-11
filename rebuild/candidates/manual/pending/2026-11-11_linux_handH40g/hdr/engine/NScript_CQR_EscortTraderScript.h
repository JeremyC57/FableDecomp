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
#ifdef FABLE_RETAIL_CQR_ESCORTTRADERSCRIPT_TRADERTOESCORT
// Retail opt-in (chunk 818): the escorted trader entity, Ego_r.pdb CTraderToEscort (44 bytes, member names as the PDB);
// vtable 0x012E151C stored by Alloc 0x00E1E7A0: slot 0 scalar deleting dtor 0x00E1E860, 1 Main 0x00E20600, 2 Init 0x00E1F7E0,
// 3 GetParentScript 0x00E1E840, 4 CEntityScriptBase::OnPersist 0x00CDEBC0, 5 OnPredicateFail 0x00E1E850. GetRandomSpeech 0x00E23F30.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQR_EscortTraderScript::CTraderToEscort:public CEntityScriptBase {
public:
 typedef CQR_EscortTraderScript TParentClass;
protected:
 TParentClass& ParentClass;                                              // +0x14
 CQ_SunnyvaleMasterData* PMasterData;                                    // +0x18
public:
 static CCountedPointer<CEntityScriptBase> __fastcall Alloc(TParentClass&,CScriptDataBase*,const CScriptThing&); // 0x00E1E7A0
 virtual CScriptBase* GetParentScript()const;                            // 0x00E1E840
private:
 long lastRandomSpeechFollowIdx1;                                        // +0x1c
 long lastRandomSpeechFollowIdx2;                                        // +0x20
 long lastRandomSpeechWaitIdx1;                                          // +0x24
 long lastRandomSpeechWaitIdx2;                                          // +0x28
 CCharString GetRandomSpeech();                                          // 0x00E23F30
 virtual void Init();                                                    // 0x00E1F7E0
 virtual void Main();                                                    // 0x00E20600
 virtual void OnPredicateFail();                                         // 0x00E1E850
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQR_EscortTraderScript::CTraderToEscort) == 0x2C);
#endif
#endif
