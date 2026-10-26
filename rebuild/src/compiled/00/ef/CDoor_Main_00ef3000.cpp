// CDoor::Main -- open while the hero's hand lamp is lit; close when extinguished, until termination
// Actual CScriptThing/CGameScriptInterface headers. Predicate CIsThingAlive nativeVT012C32F0, copy4ABE90,dtorE241B0.
// Game native slots confirmed in cgsi_slots_aligned.tsv and retail dispatch:7 NewScriptFrame,185 IsHeroHandLampLit,425 OpenDoor,426 CloseDoor.
// SetPredicateF35B10; IsActiveThreadTerminatingF35B30. Native278 bytes.
// Leaf: Main
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
struct ScriptThingCopy:CScriptThing {ScriptThingCopy(const CScriptThing&);~ScriptThingCopy();};
namespace NScript {
class CPredicateBase { public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase { public:
 ScriptThingCopy Thing;
 __forceinline CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const;virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(); //00E241B0
};
}
struct GameCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot {void* Address;void(GameCalls::*NewScriptFrame)()const;bool(GameCalls::*IsHeroHandLampLit)()const;void(GameCalls::*Door)(const CScriptThing&)const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 void NewScriptFrame()const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[7].NewScriptFrame)();}
 bool IsHeroHandLampLit()const{return(this->*reinterpret_cast<const GameSlot*>(__vftable)[185].IsHeroHandLampLit)();}
 void OpenDoor(const CScriptThing& t)const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[425].Door)(t);}
 void CloseDoor(const CScriptThing& t)const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[426].Door)(t);}
};
struct CDoor {void* VTable;GameCalls* Game;CScriptThing Me;
 void SetPredicate(const NScript::CPredicateBase&);bool IsActiveThreadTerminating();void Main();
};
void CDoor::Main(){
 {NScript::CIsThingAlive p(Me);SetPredicate(p);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 while(!IsActiveThreadTerminating()){
  while(!Game->IsHeroHandLampLit()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  Game->OpenDoor(Me);
  while(Game->IsHeroHandLampLit()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  Game->CloseDoor(Me);Game->NewScriptFrame();
 }
}