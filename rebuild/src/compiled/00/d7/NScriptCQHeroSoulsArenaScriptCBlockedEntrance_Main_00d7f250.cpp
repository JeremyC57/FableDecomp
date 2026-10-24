// NScript::CQ_HeroSoulsArenaScript::CBlockedEntrance::Main -- lock the arena door and display its blocked message when used by hero.
// Leaf: Main
// Native00D7F250,260 bytes. PDB nestedowner Main00D9F530; native resource TEXT_QST_B04_BLOCKED_ARENA_DOOR_10 confirms quest.
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
struct ScriptThingCalls;
struct GameScriptCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union ScriptThingSlot {void* Address;bool(ScriptThingCalls::*MsgIsUsedBy)(const CCharString&)const;};
union GameScriptSlot {void* Address;void(GameScriptCalls::*EntitySetAsLocked)(const CScriptThing&,bool)const;void(GameScriptCalls::*DisplayGameInfo)(const CCharString&)const;bool(GameScriptCalls::*MsgIsGameInfoClickedPast)()const;void(GameScriptCalls::*NewScriptFrame)()const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(ScriptThingSlot)==4);
FABLE_STATIC_ASSERT(sizeof(GameScriptSlot)==4);
#endif
struct ScriptThingCalls:CScriptThing {
 __forceinline bool MsgIsUsedBy(const CCharString& user)const{return(this->*reinterpret_cast<ScriptThingSlot*>(__vftable)[20].MsgIsUsedBy)(user);}
}; //nativeCScriptThing01238C8C slot20=004AAB10, PDB UBE_NABVCCharString
struct GameScriptCalls:CGameScriptInterface {
 __forceinline void EntitySetAsLocked(const CScriptThing& thing,bool locked)const{(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[518].EntitySetAsLocked)(thing,locked);}
 __forceinline void DisplayGameInfo(const CCharString& text)const{(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[115].DisplayGameInfo)(text);}
 __forceinline bool MsgIsGameInfoClickedPast()const{return(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[40].MsgIsGameInfoClickedPast)();}
 __forceinline void NewScriptFrame()const{(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[7].NewScriptFrame)();}
}; //nativeCGSI01260F0C:818=89DC10,1CC=8929D0,A0=894370,1C=6E7410 audited native/PDB signatures.
struct CEntityScriptBase {void* __vftable;GameScriptCalls* Game;ScriptThingCalls Me;
 bool IsActiveThreadTerminating(); //00F35B30: virtual GetParentScript slot3 then CScriptBase::IsActiveThreadTerminating00CB7940
}; //actual PDB sizeof20 Game4 Me8; native unchanged
namespace NScript {struct CQ_HeroSoulsArenaScript {struct CBlockedEntrance:CEntityScriptBase {CQ_HeroSoulsArenaScript* ParentClass;void* PMasterData;void Main();};};}
void NScript::CQ_HeroSoulsArenaScript::CBlockedEntrance::Main(){
 Game->EntitySetAsLocked(Me,true);
 while(!IsActiveThreadTerminating()){
  bool used;{CCharString hero("SCRIPT_NAME_HERO",-1);used=Me.MsgIsUsedBy(hero);}
  if(used){
   if(IsActiveThreadTerminating())return;
   {CCharString message("TEXT_QST_B04_BLOCKED_ARENA_DOOR_10",-1);Game->DisplayGameInfo(message);}
   while(!Game->MsgIsGameInfoClickedPast()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
   if(IsActiveThreadTerminating())return;
   Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
  }
  Game->NewScriptFrame();
 }
}