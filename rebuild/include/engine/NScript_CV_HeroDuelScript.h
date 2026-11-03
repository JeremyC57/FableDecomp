#ifndef FABLE_ENGINE_NSCRIPT_CV_HERODUELSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CV_HERODUELSCRIPT_H
#include "engine/CLevelScriptBase.h"
// Exact original PDB owner and all fields; native0x5c, donor0x70 due basecontainers.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HeroDuelScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 typedef CV_HeroDuelScript TMyClass;
 typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
 CV_HeroDuelScript(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CCharString __fastcall StatGetClassName();
 static CScriptDataBase* (__fastcall* __fastcall StatGetMasterScriptDataAlloc())();
 virtual void RegisterMain();
 bool MissionSucceeded;
 bool MissionFailed;
 bool MissionOver;
 bool GotToExecutionTree;
 bool DuelStarted;
 bool ThunderDefeated;
 bool OkToShowHealthBar;
 bool ThunderEnteredCaveFirst;
 bool OkToCheer;
 bool SupportThunder;
 bool StopSupportersCheering;
 bool SaidSurprisedLine;
 long TimesHitSpectators;
 unsigned long MyHealth;
 virtual void Init();
 virtual void Main();
 void CleanUpHeroDuel();
 void WatchForLeaving();
 void WatchForHeroEnteringCave();
 void DoMission();
 void DuelChallenge();
 void Duel();
 void FirstFleeCutscene();
 void SecondFleeCutscene();
 void DuelLost();
 void ShowEnemyHealth();
 class CThunder;
 class CSpectator;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_HeroDuelScript)==0x5c);
#endif
