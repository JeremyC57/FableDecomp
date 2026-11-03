#ifndef FABLE_ENGINE_NSCRIPT_CQ_SECRETPASSAGESCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CQ_SECRETPASSAGESCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
// Original PDB owner and complete field order; retail base0x48, two12-byte maps.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 typedef CQ_SecretPassageScript TMyClass;
 typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
 CQ_SecretPassageScript(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CCharString __fastcall StatGetClassName();
 static CScriptDataBase* (__fastcall* __fastcall StatGetMasterScriptDataAlloc())();
 virtual void RegisterMain();
 bool MissionSucceeded;
 bool MissionFailed;
 bool MissionOver;
 bool MotherFound;
 bool DoorsBlocked;
 bool DoorsBeingDone;
 bool OkayToSpawn;
 bool QuestStartScreenShown;
 long GhostSightings;
 std::map<CCharString,CScriptGameResourceObjectScriptedThingBase,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CScriptGameResourceObjectScriptedThingBase> > > GlobalMymap;
 std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const ,bool> > > FlagMap;
 CScriptGameResourceObjectScriptedThingBase minions[11];
 CScriptGameResourceObjectScriptedThingBase seh_Mother;
 virtual void Init();
 virtual void Main();
 void InitialiseVariables();
 void WatchForLeaving();
 void WatchForTermination();
 void MotherTortureThread();
 void SwitchDoorsOnAndOff();
 void BossBlockingPrisonPassage();
 void DoMission();
 void PhaseOne();
 void PrisonEscape();
 void PrisonThread();
 void EndMission();
 class CSP_ScreamerTrigger;
 class CSP_NymphTrigger;
 class CSP_UndeadTrigger;
 class CSP_AmbushGuard;
 class CSP_SecretPassageGuard;
 class CSP_InvestigatingGuard;
 class CSP_DeskGuard;
 class CMotherTorture;
 class CTortureDoorCourtyard;
 class CDoorToMothersTortureChamber;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SecretPassageScript)==0x12c);
#endif
