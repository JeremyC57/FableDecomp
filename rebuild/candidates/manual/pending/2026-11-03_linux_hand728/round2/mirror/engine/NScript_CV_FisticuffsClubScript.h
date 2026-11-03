#ifndef FABLE_ENGINE_NSCRIPT_CV_FISTICUFFSCLUBSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CV_FISTICUFFSCLUBSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
// Complete original owner and CTyler, both raw PDB field lists and native accesses.
// Owner donor0xdc/native0xc8 follows the base-size difference; entity native0x2c.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 typedef CV_FisticuffsClubScript TMyClass;
 typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
 CV_FisticuffsClubScript(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
 static CCharString __fastcall StatGetClassName();
 static CScriptDataBase* (__fastcall* __fastcall StatGetMasterScriptDataAlloc())();
 virtual void RegisterMain();
 enum { REGION_BOWERSTONE=1,REGION_OAKVALE=2,REGION_BANDITCAMP=3,REGION_KNOTHOLE=4,REGION_MAX=5,NOT_GOT_REGION=6 } RelevantRegion;
 enum { FC_STATUS_REACHING_FC=0,FC_STATUS_LEAVING_FC=1,FC_STATUS_FIGHTING=2,FC_STATUS_CHEERING=3,FC_STATUS_WAITING=4,FC_STATUS_PRE_FIGHT_END=5,FC_STATUS_FIGHT_END=6 };
 bool FighterOneInRing;
 bool FighterTwoInRing;
 bool ChallengeAccepted;
 bool FightFinished;
 bool HeroInsideRing;
 bool HeroOfferedFisticuffsClubEntry;
 bool HeroPaidEntryFee;
 bool HeroInFisticuffsClub;
 bool TylerInFight;
 bool HeroBeatTyler;
 bool TimeOut;
 bool RingCleared;
 bool RoundStarted;
 CTimer Fighter1Timer;
 CTimer Fighter2Timer;
 CScriptThing SFighterOne;
 CScriptThing SFighterTwo;
 long HitID;
 long HitCount;
 long AttackWarnings;
 long FightCount;
 bool EverybodyAttack;
 bool LostWhileOutside;
 bool FisticuffsActive;
 long AllowedHits;
 bool CompletedThisPunchClub;
 bool CompletedKnotholePunchClub;
 bool CompletedBowerstonePunchClub;
 bool CompletedBanditPunchClub;
 bool CompletedOakvalePunchClub;
 CScriptThing Village;
 bool PlayerLost;
 float CheerNumber;
 float BooNumber;
 CCharString RegionNames[5];
 virtual void OnPersist(CPersistContext&);
 virtual void Init();
 virtual void Main();
 void InitialiseVariables();
 void BeenAttacked();
 CCharString GetQuestText(CCharString);
 void SetAllowedHits(int);
 void SetFightCount();
 void SetCompletedThisClub();
 CCharString GetCutsceneName(CCharString);
 CCharString GetFighterName();
 CCharString GetLeaderName();
 void KeepVariablesUpToDate();
 void ActivateFisticuffsAtNight();
 bool IsKnotholeGladeNotAPunchClub();
 void FisticuffsChallenge();
 void AddQuestInfo();
 class CFisticuffsClubRing;
 class CRingFighter;
 class CFisticuffsMember;
 class CTyler:public CEntityScriptBase {
 public:
  typedef CV_FisticuffsClubScript TParentClass;
 protected:
  TParentClass& ParentClass;
  CQ_SunnyvaleMasterData* PMasterData;
 public:
  CTyler(TParentClass&,CScriptDataBase*,const CScriptThing&);
  static CCountedPointer<CEntityScriptBase> __fastcall Alloc(TParentClass&,CScriptDataBase*,const CScriptThing&);
  virtual CScriptBase* GetParentScript()const;
  CQ_SunnyvaleMasterData* GetMasterData();
 private:
  bool AnsweredYes;
  bool ShownItem;
  bool IntroducedPunchClub;
  long Money;
  CCharString PunchClubLeftReason;
  bool OutstandingCrimes;
  virtual void Init();
  virtual void OnPersist(CPersistContext&);
  virtual void OnPredicateFail();
  void CleanupAfterFight();
  void FightWarmupRounds();
  virtual void Main();
 };
};
}
#endif
