#ifndef FABLE_ENGINE_NSCRIPT_CV_CHICKENKICKINGSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CV_CHICKENKICKINGSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
// Complete original fields from both PDBs; native base is0x14 smaller, native map is12bytes.
// Native factory00e63660 allocates0x20; vtable012e74b4 slot1 isMain00e63890.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChickenKickingScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 typedef CV_ChickenKickingScript TMyClass;
 typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
 CV_ChickenKickingScript(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CCharString __fastcall StatGetClassName();
 static CScriptDataBase* (__fastcall* __fastcall StatGetMasterScriptDataAlloc())();
 virtual void RegisterMain();
enum EPrizes {
 PRZ_NO_PRIZE=0,
 PRZ_LOW_PRIZE=1,
 PRZ_MEDIUM_PRIZE=2,
 PRZ_TOP_PRIZE=4,
 NUM_PRIZE_COMBINATIONS=8
};
 bool ChickenLanded;
 long FinalSector,DistanceBand;
 bool KnowGhostHasGone,SpectatorsCreated,RanOff,TalkedTo,SpectatorsUnderAttack;
 unsigned long PrizesWon;
 bool InfoDisplayed;
 std::map<CCharString,CCharString> csargs;
 CScriptGameResourceObjectScriptedThingBase seh_Chicken,seh_ChickenMaster;
 virtual void OnPersist(CPersistContext&);
 virtual void Init();
 virtual void Main();
 void PlayCutscene(CCharString);
 void CreateSpectators();
 void LookAfterOrganiser();
 class CChickenMaster;
 class CKickedChicken;
 class CChickenSign;
 class CSpectator:public CEntityScriptBase {
 public:
  typedef CV_ChickenKickingScript TParentClass;
 protected:
  TParentClass& ParentClass;
  CQ_SunnyvaleMasterData* PMasterData;
 public:
  CSpectator(TParentClass&,CScriptDataBase*,const CScriptThing&);
  static CCountedPointer<CEntityScriptBase> __fastcall Alloc(TParentClass&,CScriptDataBase*,const CScriptThing&);
  virtual CScriptBase* GetParentScript()const;
  CQ_SunnyvaleMasterData* GetMasterData();
 private:
  long SpectatorNumber;
  virtual void Init();
  virtual void Main();
 };
};
}
#ifdef FABLE_RETAIL_CV_CHICKENKICKINGSCRIPT_CHICKENMASTER
// Retail opt-in (chunk 818): the chicken master entity (Ego_r.pdb CChickenMaster, 32 bytes, entity offsets equal retail;
// vtable 0x012E747C stored by Alloc 0x00E634B0: slot 0 scalar deleting dtor 0x00E63570, 1 Main 0x00E64FB0, 2 Init 0x00E63560,
// 3 GetParentScript 0x00E63550, 4 OnPersist 0x00E641B0).
namespace NScript {
class CV_ChickenKickingScript::CChickenMaster:public CEntityScriptBase {
public:
 typedef CV_ChickenKickingScript TParentClass;
protected:
 TParentClass& ParentClass;                                              // +0x14
 CQ_SunnyvaleMasterData* PMasterData;                                    // +0x18
public:
 static CCountedPointer<CEntityScriptBase> __fastcall Alloc(TParentClass&,CScriptDataBase*,const CScriptThing&); // 0x00E634B0
 virtual CScriptBase* GetParentScript()const;                            // 0x00E63550
private:
 bool HaveTalked;                                                        // +0x1c
 bool HeroHasPlayed;                                                     // +0x1d
 bool KickingChickens;                                                   // +0x1e
 bool GhostChat;                                                         // +0x1f
 virtual void OnPersist(CPersistContext&);                               // 0x00E641B0
 virtual void Init();                                                    // 0x00E63560
 virtual void Main();                                                    // 0x00E64FB0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ChickenKickingScript::CChickenMaster) == 0x20);
#endif
#endif
