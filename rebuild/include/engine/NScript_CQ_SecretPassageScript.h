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
#ifdef FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_UNDEADTRIGGER
// NF186 opt-in (q4874 / q4875, rows 0x00DEF700 GetParentScript / 0x00DEF720 OnPredicateFail): the undead-trigger entity script.
//   Ego_r.pdb layout (struct_layouts_egor.tsv CSP_UndeadTrigger; entity offsets equal retail's, Dinkumware's 16-byte vector is STLport's
//   12 here, so TempCreature sits at retail +0x28, as the ??_G 0x00DEF730 shows: ~CScriptThing on +0x28, ~vector<CScriptThing> 0x008AC970
//   on +0x1C): ParentClass +0x14, PMasterData +0x18, UndeadVec +0x1C, TempCreature +0x28 (0x34 bytes). Entity vtable 0x012DDF3C (read
//   from retail .rdata): 0 scalar deleting dtor 0x00DEF730, 1 Main 0x00DEF760, 2 Init 0x00DEF710, 3 GetParentScript 0x00DEF700,
//   4 / 6 the CEntityScriptBase defaults, 5 OnPredicateFail 0x00DEF720. Names: Ego_r.pdb / FableWin.pdb
//   ?<Name>@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@ (GetParentScript UBE public; Main / Init / OnPredicateFail EAE private;
//   ??0 QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z out of line at 0x00DEF6A0). Declarations only.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SecretPassageScript::CSP_UndeadTrigger : public CEntityScriptBase {
public:
 static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);
 CSP_UndeadTrigger(CQ_SecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00DEF6A0
 virtual CScriptBase* GetParentScript() const;                         // 0x00DEF700
private:
 virtual void Main();                                                  // 0x00DEF760
 virtual void Init();                                                  // 0x00DEF710
 virtual void OnPredicateFail();                                       // 0x00DEF720
protected:
 CQ_SecretPassageScript* ParentClass;                                  // +0x14
 CQ_SunnyvaleMasterData* PMasterData;                                  // +0x18
 std::vector<CScriptThing> UndeadVec;                                  // +0x1c
 CScriptThing TempCreature;                                            // +0x28
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SecretPassageScript::CSP_UndeadTrigger)==0x34);
#endif
#ifdef FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE
// Row opt-in (0x00DEE2E0 CMotherTorture::Main): the mother-torture entity script. Ego_r.pdb layout (struct_layouts_egor.tsv
//   CMotherTorture, 28 bytes): ParentClass CQ_SecretPassageScript* +0x14, PMasterData +0x18. Entity vtable 0x012DDE6C (stored by Alloc
//   0x00DEE1A0): 0 scalar deleting dtor 0x00DEE2C0, 1 Main 0x00DEE2E0, 2 Init 0x00DEE250, 3 GetParentScript 0x00DEE240,
//   5 OnPredicateFail 0x00DEE2B0. Names: FableWin.pdb ?<Name>@CMotherTorture@CQ_SecretPassageScript@NScript@@ (GetParentScript UBE public;
//   Main / Init / OnPredicateFail EAE private; ??0 QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z). Declarations only.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SecretPassageScript::CMotherTorture : public CEntityScriptBase {
public:
 static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);
 CMotherTorture(CQ_SecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me);
 virtual CScriptBase* GetParentScript() const;                         // 0x00DEE240
private:
 virtual void Main();                                                  // 0x00DEE2E0
 virtual void Init();                                                  // 0x00DEE250
 virtual void OnPredicateFail();                                       // 0x00DEE2B0
protected:
 CQ_SecretPassageScript* ParentClass;                                  // +0x14
 CQ_SunnyvaleMasterData* PMasterData;                                  // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SecretPassageScript::CMotherTorture)==0x1c);
#endif
#endif
