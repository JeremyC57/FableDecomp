// NScript::CQ_HeroSoulsArenaScript::CBlockedEntrance::Main() -- lock the arena door and display its blocked message when used by hero.
// Leaf: ?Main@CBlockedEntrance@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ
// Native00D7F250,260 bytes. PDB nestedowner Main00D9F530; native resource TEXT_QST_B04_BLOCKED_ARENA_DOOR_10 confirms quest.
// Naming wave 35: identity reg 00d7cbe6 "BlockedEntrance" in 00d7c890; Alloc 00d7f1a0; vtable 012d3f04 slot 1; PDB ?Main@CBlockedEntrance@CQ_HeroSoulsArenaScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012d3f04::vfunc_1 (wave3/labels_alloc.tsv); PDB ?Main@CBlockedEntrance@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CBlockedEntrance@CQ_HeroSoulsArenaScript@NScript@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (base CEntityScriptBase made polymorphic); nested class fixed in place); same bytes.
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
namespace NScript { class CPredicateBase; }
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
 FABLE_FORCEINLINE bool MsgIsUsedBy(const CCharString& user)const{return(this->*reinterpret_cast<ScriptThingSlot*>(__vftable)[20].MsgIsUsedBy)(user);}
}; //nativeCScriptThing01238C8C slot20=004AAB10, PDB UBE_NABVCCharString
struct GameScriptCalls:CGameScriptInterface {
 FABLE_FORCEINLINE void EntitySetAsLocked(const CScriptThing& thing,bool locked)const{(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[518].EntitySetAsLocked)(thing,locked);}
 FABLE_FORCEINLINE void DisplayGameInfo(const CCharString& text)const{(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[115].DisplayGameInfo)(text);}
 FABLE_FORCEINLINE bool MsgIsGameInfoClickedPast()const{return(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[40].MsgIsGameInfoClickedPast)();}
 FABLE_FORCEINLINE void NewScriptFrame()const{(this->*reinterpret_cast<GameScriptSlot*>(__vftable)[7].NewScriptFrame)();}
}; //nativeCGSI01260F0C:818=89DC10,1CC=8929D0,A0=894370,1C=6E7410 audited native/PDB signatures.
class CEntityScriptBase { public:virtual ~CEntityScriptBase();   // retail slot 0 (was the vptr placeholder: the stand-in is polymorphic, as retail)
GameScriptCalls* Game;ScriptThingCalls Me;
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
}; //actual PDB sizeof20 Game4 Me8; native unchanged
namespace NScript {class CQ_HeroSoulsArenaScript { public:struct CBlockedEntrance:CEntityScriptBase {CQ_HeroSoulsArenaScript* ParentClass;void* PMasterData;
private:
    virtual void Main();                          // PDB ?Main@CBlockedEntrance@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ
};};}
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