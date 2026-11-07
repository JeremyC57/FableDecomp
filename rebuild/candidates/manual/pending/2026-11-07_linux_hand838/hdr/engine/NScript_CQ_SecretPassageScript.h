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
#ifdef FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE
// Retail opt-in (chunk 838): the nested entity script CMotherTorture (Ego_r.pdb, 0x1c: ParentClass +0x14, PMasterData +0x18,
// struct_layouts_egor.tsv; Alloc 0x00DEE1A0 stores the vtable 0x012DDE6C). Methods: Ego_r.pdb ?Main@CMotherTorture@...@EAEXXZ (private
// virtual, vtable slot 1, 0x00DEE2E0), Init 0x00DEE250 (slot 2), GetParentScript 0x00DEE240 (slot 3), OnPredicateFail 0x00DEE2B0 (slot 5).
}
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SecretPassageScript::CMotherTorture : public CEntityScriptBase {
public:
 typedef CQ_SecretPassageScript TParentClass;
 CMotherTorture(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);
 static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&); // 0x00DEE1A0
 virtual CScriptBase* GetParentScript() const;                        // 0x00DEE240
 CQ_SunnyvaleMasterData* GetMasterData();
protected:
 CQ_SecretPassageScript& ParentClass;                                 // +0x14
 CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
 virtual void Init();                                                 // 0x00DEE250 (slot 2)
 virtual void OnPredicateFail();                                      // 0x00DEE2B0 (slot 5)
 virtual void Main();                                                 // 0x00DEE2E0
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SecretPassageScript)==0x12c);
#ifdef FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SecretPassageScript::CMotherTorture)==0x1c);
#endif
#endif
