// NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe::Main() -- acquire scripting ownership and await parent flag or termination
// Leaf: ?Main@CEndGameScarletRobe@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail00D2D060,308 bytes. RegistrationD2AE6B names EndGameScarletRobe; factoryD2CF80
// installs nativeVT012CC428. PDB corroborates EndGameScarletRobe registration and factory.
// Actual Thing, Game, Resource and BaseObject headers; named full-width PMFs for verified slots7/8.
// Naming wave 35: identity reg 00d2ae6b "EndGameScarletRobe" in 00d2aa40; Alloc 00d2cf80; vtable 012cc428 slot 1; PDB ?Main@CEndGameScarletRobe@CQ_EndGameFocalSitesScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012cc428::vfunc_1 (wave3/labels_alloc.tsv); PDB ?Main@CEndGameScarletRobe@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CVtable012CC428@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); class CVtable012CC428 renamed CEndGameScarletRobe); same bytes.
#include "fable_compiler.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CBaseObject.h"
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBaseLayout
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectScriptedThingBaseLayout
{
public:
    void Release();   // 0x00CD2770
    bool IsValid() const;   // 0x00CD23B9 (Ego_r / FableWin: QBE)
};
class CCPPointerInfo { public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;};
extern const unsigned char ResourceBaseVTable[]; //0126008C genuine CScriptGameResourceObjectBase
extern const unsigned char ResourceVTable[]; //0127094C
// Generated resource header flattens CBaseObject. This field-free API binds the
// genuine CBaseObject constructor 0099A380 (vptr129A7D8, IntelligentPointer+4=0).
// It is not the distinct CBaseIntelligentPointer constructor 00A01B10.
struct ScriptResource:CScriptGameResourceObjectScriptedThingBase {
 void InitializeCBaseObject(); //0099A380, this points to flattened base at offset0
 FABLE_FORCEINLINE ScriptResource(){InitializeCBaseObject();__vftable=(void*)ResourceVTable;PImp_Data=0;PImp_Info=0;}
 void DestroyCBaseObject(); //0099A430
 ~ScriptResource(){ //007E74D0; same body inlined on the early acquisition-failure exit
  if(PImp_Info){--PImp_Info->RefCount;if(PImp_Info->RefCount==0){PImp_Info->DeleteFunc(PImp_Info->Data);delete PImp_Info;}}
  PImp_Data=0;PImp_Info=0;__vftable=(void*)ResourceBaseVTable;DestroyCBaseObject();
 }
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
class CEntityScriptBase {
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)
 public:   // the PDB base the accessor belongs to (empty here: offset 0)
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};
namespace NScript {
class CQ_EndGameFocalSitesScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CEndGameScarletRobe;
};
}
using NScript::CQ_EndGameFocalSitesScript;
namespace NScript {
class CQ_EndGameFocalSitesScript::CEndGameScarletRobe : public CEntityScriptBase {
public:GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
private:
    virtual void Main();                          // PDB ?Main@CEndGameScarletRobe@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
};
}
typedef NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe CEndGameScarletRobe;    // global spelling used below
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CEndGameScarletRobe,Parent)==0x14);
FABLE_STATIC_ASSERT(offsetof(NativeParent,ScriptFlag)==0x81);
#endif
void NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe::Main(){
 ScriptResource resource;
 if(resource.IsValid())resource.Release();
 while(!Game->StartScriptingEntity(Me,resource,4)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(!IsActiveThreadTerminating()){
  while(!Parent->ScriptFlag){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  if(resource.IsValid())resource.Release();Game->NewScriptFrame();IsActiveThreadTerminating();
 }
}