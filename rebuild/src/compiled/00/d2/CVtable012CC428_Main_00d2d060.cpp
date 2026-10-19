// CVtable012CC428::Main -- acquire scripting ownership and await parent flag or termination
// Retail00D2D060,308 bytes. RegistrationD2AE6B names EndGameScarletRobe; factoryD2CF80
// installs nativeVT012CC428. PDB corroborates EndGameScarletRobe registration and factory.
// Actual Thing, Game, Resource and BaseObject headers; named full-width PMFs for verified slots7/8.
// Leaf: Main
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
struct CCPPointerInfo {unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;};
extern const unsigned char ResourceBaseVTable[]; //0126008C genuine CScriptGameResourceObjectBase
extern const unsigned char ResourceVTable[]; //0127094C
// Generated resource header flattens CBaseObject. This field-free API binds the
// genuine CBaseObject constructor 0099A380 (vptr129A7D8, IntelligentPointer+4=0).
// It is not the distinct CBaseIntelligentPointer constructor 00A01B10.
struct ScriptResource:CScriptGameResourceObjectScriptedThingBase {
 void InitializeCBaseObject(); //0099A380, this points to flattened base at offset0
 __forceinline ScriptResource(){InitializeCBaseObject();__vftable=(void*)ResourceVTable;PImp_Data=0;PImp_Info=0;}
 void DestroyCBaseObject(); //0099A430
 ~ScriptResource(){ //007E74D0; same body inlined on the early acquisition-failure exit
  if(PImp_Info){--PImp_Info->RefCount;if(PImp_Info->RefCount==0){PImp_Info->DeleteFunc(PImp_Info->Data);delete PImp_Info;}}
  PImp_Data=0;PImp_Info=0;__vftable=(void*)ResourceBaseVTable;DestroyCBaseObject();
 }
 bool IsScripting(); //00CD23B9
 void Release(); //00CD2770
};
struct GameCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot {void* Address;void(GameCalls::*Frame)()const;bool(GameCalls::*Start)(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&,int);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 void NewScriptFrame()const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[7].Frame)();}
 bool StartScriptingEntity(const CScriptThing& t,ScriptResource& r,int priority){return(this->*reinterpret_cast<const GameSlot*>(__vftable)[8].Start)(t,r,priority);}
};
// Parent flag meaning remains inferred/unknown; only its native offset is asserted.
struct NativeParent {unsigned char UnrecoveredPrefix[0x81];bool ScriptFlag;};
struct CVtable012CC428 {void* VTable;GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 bool IsActiveThreadTerminating(); //00F35B30
 void Main();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CVtable012CC428,Parent)==0x14);
FABLE_STATIC_ASSERT(offsetof(NativeParent,ScriptFlag)==0x81);
#endif
void CVtable012CC428::Main(){
 ScriptResource resource;
 if(resource.IsScripting())resource.Release();
 while(!Game->StartScriptingEntity(Me,resource,4)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(!IsActiveThreadTerminating()){
  while(!Parent->ScriptFlag){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  if(resource.IsScripting())resource.Release();Game->NewScriptFrame();IsActiveThreadTerminating();
 }
}