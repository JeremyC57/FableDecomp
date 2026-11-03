#ifndef FABLE_ENGINE_NSCRIPT_CQ_TRADERESCORTSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CQ_TRADERESCORTSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "engine/CCharString.h"
// Complete original PDB owner. Native factory00e0af60 allocates0x7c and
// stores012df16c; CommentTimer+0x48 and TraderToTalk+0x78 match donor minus0x14.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 typedef CQ_TraderEscortScript TMyClass;
 typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
 CQ_TraderEscortScript(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CCharString __fastcall StatGetClassName();
 static CScriptDataBase* (__fastcall* __fastcall StatGetMasterScriptDataAlloc())();
 virtual void RegisterMain();
 enum ETraderComment { TRADER_COMMENT_SINGLE=0, TRADER_COMMENT_MULTIPLE=1, TRADER_COMMENT_ATTACKED_SPEECH=2 };
 enum EInfectedState { INFECTED_NONE=0, INFECTED_ONLY_JUST=1, INFECTED_LOW=2, INFECTED_MEDIUM=3, INFECTED_HIGH=4, INFECTED_MAX=5 };
 enum ETRADER_STATE { TRADER_STATE_NONE=0, TRADER_STATE_JUST_ATTACKED=1, TRADER_STATE_FOLLOWING=2, TRADER_STATE_SCARED=3, TRADER_STATE_MAX=4 };
 CTimer CommentTimer; // +0x48
 long BalverinesToSurpriseHeroNeeded; // +0x4c
 long TradersStillAliveCounter; // +0x50
 bool InfectedTraderCanGetUp; // +0x54
 bool TradersShouldBeScared; // +0x55
 bool IntroFinished; // +0x56
 bool FollowInfoGiven; // +0x57
 bool EndStarted; // +0x58
 bool SavedInMiddle; // +0x59
 bool SavedNearEnd; // +0x5a
 long IncubationTimeLow; // +0x5c
 long IncubationTimeMedium; // +0x60
 long IncubationTimeHigh; // +0x64
 long TraderPickpocketedCount; // +0x68
 bool MissionSucceeded; // +0x6c
 bool MissionFailed; // +0x6d
 bool QuestStartScreened; // +0x6e
 bool GoneWrongWay; // +0x6f
 bool ShownBalverine; // +0x70
 bool ShownEarthTroll; // +0x71
 long TimesBantered; // +0x74
 CCharString TraderToTalk; // +0x78
 virtual void Init();
 virtual void OnPersist(CPersistContext&);
 virtual void Main();
 void WatchForMissionRules();
 void WatchForSurprisingBalverines();
 bool MakeTraderComment(const CCharString&,const CScriptThing&,ETraderComment);
 void TurnToBalv(CScriptThing);
 void WatchForPickpocketing(CScriptThing);
 class CDarkwoodTrader;
 class CFleshEatingBalverine;
 class CInfectedBalverine;
 class CTraderComment;
 class CDarkwoodAssassinSpawn;
 class CDarkwoodBalverineTrader;
 class CTE_BanditLeader;
 class CBanditCronies;
 class CRockTrollTrigger;
 class CDarkwoodRockTroll;
 class CMagicBarrier;
 class CDW5_Bandit;
 class CDW5_Balv;
 class CEndTrader;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_TraderEscortScript)==0x7c);
#endif
