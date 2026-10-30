// NScript::CQ_EndGameFocalSitesScript::CBattleGuard::Main() -- guard stays villager while near hero, then fades out unless terminating.
// Leaf: Main
// Native00D2C530,273 bytes. RegistrationD2B006 stores allocatorD2C430 for BattleGuard; allocation stores VT12CC320 whose Main is D2C530.
// Parent ctorD2A820 is CQ_EndGameFocalSitesScript; PDB nestedMainD36CB0. Predicate12C32F0 is genuine CIsThingAlive (operator()/Clone/dtor).
#include "fable_string.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#define CScriptGameResourceObjectScriptedThingBase CScriptedThingResource_PDB
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#define __fastcall
#endif
class C3DVector;
struct ScriptThingCopy:CScriptThing {ScriptThingCopy(const CScriptThing&);~ScriptThingCopy();}; //004ABE90/004AA840 genuine copy/release, field-free native projection
namespace NScript {
class CPredicateBase { public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase { public:ScriptThingCopy Thing;__forceinline CIsThingAlive(const CScriptThing& t):Thing(t){}~CIsThingAlive(){}virtual bool operator()()const;virtual CPredicateBase* Clone()const;}; //CDF120/CDF130/implicitdtorCDF160, VT12C32F0
}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCPPointerInfo;
enum EScriptEntityMoveType { ScriptMoveDefault=0 };enum ETextGroupSelectionMethod { TextGroupDefault=0 };
class CScriptGameResourceObjectBase:public CBaseObject{ public:virtual ~CScriptGameResourceObjectBase();}; //native base8, ordinary cleanup6E7090; PDB size8
class CScriptGameResourceObjectScriptedThingBase:public CScriptGameResourceObjectBase { public:
 CScriptGameResourceObjectScriptedThingBase* PImp_Data;CCPPointerInfo* PImp_Info;
 __forceinline CScriptGameResourceObjectScriptedThingBase():PImp_Data(0),PImp_Info(0){}
 virtual ~CScriptGameResourceObjectScriptedThingBase(); //007E74D0 ordinary shared resource cleanup
 virtual void Validate()const; //1 7E72C0
 virtual void SummonerLightningOrbAttackTarget(const CScriptThing&); //2 7E72D0
 virtual void FireProjectileWeaponAtTarget(const CScriptThing&); //3 7E72E0
 virtual void MoveToPosition(const C3DVector&,float,EScriptEntityMoveType,bool,bool); //4 7E72F0
 virtual void MoveToThing(const CScriptThing&,float,EScriptEntityMoveType,bool,bool,bool,bool); //5 7E7300
 virtual void FollowPreCalculatedRoute(const CScriptThing&,EScriptEntityMoveType,bool,bool); //6 7E7310
 virtual void FollowThing(const CScriptThing&,float,bool); //7 7E7320
 virtual void StopFollowingThing(const CScriptThing&); //8 7E7330
 virtual bool IsFollowActionRunning(const CScriptThing&)const; //9 7E7340
 virtual void ClearCommands(); //10 7E7360
 virtual void WaitWhilePerformingTasks(); //11 7E7370
 virtual CScriptThing GetScriptThing()const; //12 7E7490
 virtual void Speak(const CScriptThing&,unsigned long,ETextGroupSelectionMethod,bool,bool,bool); //14 7E7380
 virtual void Speak(const CScriptThing&,const char*,ETextGroupSelectionMethod,bool,bool,bool); //13 7E7390
 virtual void AskHeroQuestion(const CScriptThing&,const char*,const CCharString&,const CCharString&,const CCharString&,bool,bool); //15 7E73A0
 virtual void Converse(const CScriptGameResourceObjectScriptedThingBase&,const char*); //16 7E73B0
 virtual void PerformExpression(const CScriptThing&,const CCharString&); //17 7E73C0
 virtual void PlayAnimation(const CCharString&,bool,bool,bool,bool,bool,bool,bool); //18 7E73D0
 virtual void PlayCombatAnimation(const CCharString&,bool,bool,bool,bool,bool,bool); //19 7E73E0
 virtual void PlayLoopingAnimation(const CCharString&,long,bool,bool,bool,bool,bool,bool,bool); //20 7E73F0
 virtual void ClearAllActions(); //21 7E7400
 virtual void ClearAllActionsIncludingLoopingAnimations(); //22 7E7410
 virtual void MoveToAndPickUpGenericBox(CScriptThing&,EScriptEntityMoveType,bool); //23 7E7420
 virtual void DropGenericBox(); //24 7E7430
 virtual void UnsheatheWeapons(); //25 7E7440
 virtual bool IsPerformingScriptTask()const; //26 7E7450
 virtual bool IsFollowingThing()const; //27 7E7460
 virtual void WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase&); //28 7E7470
 virtual void Wait(float); //29 7E7480
}; //VT0127094C slots0..29 audited via rtti_port_compare PDB signatures; no invented padding slots
struct GameScriptCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot{void* Address;void(GameScriptCalls::*NewScriptFrame)()const;CScriptThing&(GameScriptCalls::*GetHero)();void(GameScriptCalls::*EntitySetInFaction)(const CScriptThing&,const CCharString&)const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct GameScriptCalls:CGameScriptInterface {
 __forceinline void NewScriptFrame()const{(this->*reinterpret_cast<GameSlot*>(__vftable)[7].NewScriptFrame)();}
 __forceinline CScriptThing& GetHero(){return(this->*reinterpret_cast<GameSlot*>(__vftable)[70].GetHero)();}
 __forceinline void EntitySetInFaction(const CScriptThing& t,const CCharString& f)const{(this->*reinterpret_cast<GameSlot*>(__vftable)[601].EntitySetInFaction)(t,f);}
}; //7=6E7410,70=891CA0,601=89C190
extern "C" int rand(); //00BFEB16
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float); //00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing&,float,bool,bool); //00CBE716
class CEntityScriptBase { public:void* __vftable;GameScriptCalls* Game;CScriptThing Me;void SetPredicate(const NScript::CPredicateBase&);bool IsActiveThreadTerminating() const;}; //00F35B10/00F35B30
namespace NScript{class CQ_EndGameFocalSitesScript{ public:struct CBattleGuard:CEntityScriptBase{CQ_EndGameFocalSitesScript* ParentClass;void* PMasterData;void Main();};};}
void NScript::CQ_EndGameFocalSitesScript::CBattleGuard::Main(){
 SetPredicate(CIsThingAlive(Me));Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 rand();CScriptGameResourceObjectScriptedThingBase resource;
 {CCharString faction("FACTION_VILLAGERS",-1);Game->EntitySetInFaction(Me,faction);}
 while(IsDistanceBetweenThingsUnder(Me,Game->GetHero(),18.0f)){
  Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 }
 if(!IsActiveThreadTerminating())FadeOutAndKillEntity(Me,1.0f,true,true);
}