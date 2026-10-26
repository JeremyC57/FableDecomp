// CVtable012E04FC::Main -- pause, acquire and release scripting ownership, then yield until termination
// Retail00E11BB0,274 bytes. RegistrationE0EE1E names QueenHornet; factoryE11AD0
// installs nativeVT012E04FC. PDB corroborates QueenHornet registration and native factory.
// Actual Thing, Game, Resource and BaseObject headers; named full-width PMFs for verified slots7/8.
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
 ~CIsThingAlive(){} //inline native vptr reset then Thing destructor4AA840
};
}
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
extern const unsigned char ResourceVTable[]; //0127094C
// Generated resource header flattens CBaseObject. This field-free API binds the
// genuine CBaseObject constructor 0099A380 (vptr129A7D8, IntelligentPointer+4=0).
// It is not the distinct CBaseIntelligentPointer constructor 00A01B10.
struct ScriptResource:CScriptGameResourceObjectScriptedThingBase {
 void InitializeCBaseObject(); //0099A380, this points to flattened base at offset0
 __forceinline ScriptResource(){InitializeCBaseObject();__vftable=(void*)ResourceVTable;PImp_Data=0;PImp_Info=0;}
 ~ScriptResource(); //007E74D0
 bool IsScripting(); //00CD23B9
 void Release(); //00CD2770
};
struct GameCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot {void* Address;void(GameCalls::*Frame)()const;bool(GameCalls::*Start)(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&,int);void(GameCalls::*Pause)(float)const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 void Pause(float seconds)const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[376].Pause)(seconds);}
 void NewScriptFrame()const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[7].Frame)();}
 bool StartScriptingEntity(const CScriptThing& t,ScriptResource& r,int priority){return(this->*reinterpret_cast<const GameSlot*>(__vftable)[8].Start)(t,r,priority);}
};
// Parent flag meaning remains inferred/unknown; only its native offset is asserted.
struct NativeParent {unsigned char UnrecoveredPrefix[0x4c];bool ScriptFlag;};
struct CVtable012E04FC {void* VTable;GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 void SetPredicate(const NScript::CPredicateBase&); //00F35B10
 bool IsActiveThreadTerminating(); //00F35B30
 void Main();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CVtable012E04FC,Parent)==0x14);
FABLE_STATIC_ASSERT(offsetof(NativeParent,ScriptFlag)==0x4c);
#endif
void CVtable012E04FC::Main(){
 {NScript::CIsThingAlive p(Me);SetPredicate(p);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 ScriptResource resource;Game->Pause(2.0f);
 if(resource.IsScripting())resource.Release();
 while(!Game->StartScriptingEntity(Me,resource,0)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(IsActiveThreadTerminating())return;
 resource.Release();
 do {Game->NewScriptFrame();}while(!IsActiveThreadTerminating());
}