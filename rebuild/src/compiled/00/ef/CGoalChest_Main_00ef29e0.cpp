// CGoalChest::Main -- watch the chest until hero use ends or script termination, then flag the parent
// Retail 00EF29E0 length221. Predicate VT012C32F0; copy4ABE90, SetPredicateF35B10, predicate dtorE241B0, terminatingF35B30.
// Actual CScriptThing and CGameScriptInterface headers retained; native field-free method projections.
// String125D1C8 is SCRIPT_NAME_HERO. Slot20 CScriptThing::MsgIsUsedBy=004AAB10; game slot7 NewScriptFrame=006E7410.
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
struct ScriptThingCopy:CScriptThing { ScriptThingCopy(const CScriptThing&); ~ScriptThingCopy(); };
namespace NScript {
struct CPredicateBase { virtual bool operator()() const=0; virtual CPredicateBase* Clone() const=0; virtual ~CPredicateBase(){} };
struct CIsThingAlive:CPredicateBase {
 ScriptThingCopy Thing;
 __forceinline CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const; virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(); //00E241B0 out of line
};
}
struct GameCalls; struct ThingCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot { void* Address; void(GameCalls::*NewScriptFrame)(); };
union ThingSlot { void* Address; unsigned char(ThingCalls::*MsgIsUsedBy)(const CCharString&)const; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4); FABLE_STATIC_ASSERT(sizeof(ThingSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 __forceinline void NewScriptFrame(){(this->*reinterpret_cast<GameSlot*>(__vftable)[7].NewScriptFrame)();}
};
struct ThingCalls:CScriptThing {
 __forceinline bool NotUsedByHero() const { CCharString name("SCRIPT_NAME_HERO",-1); return MsgIsUsedBy(name) ? false : true; }
 __forceinline unsigned char MsgIsUsedBy(const CCharString& name)const {return(this->*reinterpret_cast<const ThingSlot*>(__vftable)[20].MsgIsUsedBy)(name);}
};
struct CPersonalScript_EmptyScript { unsigned char Unknown[0x48]; bool GoalChestFinished; };
struct CGoalChest {
 void* VTable; GameCalls* Game; ThingCalls Me; CPersonalScript_EmptyScript* ParentClass; void* PMasterData;
 void SetPredicate(const NScript::CPredicateBase&); bool IsActiveThreadTerminating();
 void Main();
};
void CGoalChest::Main() {
 { NScript::CIsThingAlive filter(Me); SetPredicate(filter); }
 Game->NewScriptFrame();
 if(!IsActiveThreadTerminating()) {
  while(Me.NotUsedByHero()) {
   Game->NewScriptFrame();
   if(IsActiveThreadTerminating()) return;
  }
  if(!IsActiveThreadTerminating()) ParentClass->GoalChestFinished=true;
 }
}