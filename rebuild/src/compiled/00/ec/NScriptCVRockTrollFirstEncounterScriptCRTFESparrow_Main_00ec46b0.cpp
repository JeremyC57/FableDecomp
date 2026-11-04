// NScript::CV_RockTrollFirstEncounterScript::CRTFE_Sparrow::Main() -- acquire scripting ownership and await parent flag or termination
// Leaf: ?Main@CRTFE_Sparrow@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ
// Retail00EC46B0,259 bytes. RegistrationEC3D93 names RTFE_Sparrow; factoryEC45C0
// installs nativeVT012F16E0. PDB corroborates NScript::CV_RockTrollFirstEncounterScript::CRTFE_Sparrow.
// Actual Thing, Game, Resource and BaseObject headers; named full-width PMFs for verified slots7/8.
// Naming wave 26 (family pred): CEntityScriptBase is the global PDB class (Ego_r.pdb / FableWin.pdb) with
//   ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z 0x00F35B10 and the const
//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30; the predicate parameter is an NScript::CPredicateBase.
//   Was: SetPredicate declared on the entity class (?SetPredicate@CRTFE_Sparrow@@...).
// Naming wave 35: identity reg 00ec3d93 "RTFE_Sparrow" in 00ec3d40; Alloc 00ec45c0; vtable 012f16e0 slot 1; PDB ?Main@CRTFE_Sparrow@CV_RockTrollFirstEncounterScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012f16e0::vfunc_1 (wave3/labels_alloc.tsv); PDB ?Main@CRTFE_Sparrow@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CVtable012F16E0@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); class CVtable012F16E0 renamed CRTFE_Sparrow); same bytes.
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
extern const unsigned char ResourceVTable[]; //0127094C
// Generated resource header flattens CBaseObject. This field-free API binds the
// genuine CBaseObject constructor 0099A380 (vptr129A7D8, IntelligentPointer+4=0).
// It is not the distinct CBaseIntelligentPointer constructor 00A01B10.
struct ScriptResource:CScriptGameResourceObjectScriptedThingBase {
 void InitializeCBaseObject(); //0099A380, this points to flattened base at offset0
 __forceinline ScriptResource(){InitializeCBaseObject();__vftable=(void*)ResourceVTable;PImp_Data=0;PImp_Info=0;}
 ~ScriptResource(); //007E74D0
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
struct NativeParent {unsigned char UnrecoveredPrefix[0x4c];bool ScriptFlag;};
class CEntityScriptBase {
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)
 public:   // the PDB base the accessor belongs to (empty here: offset 0)
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
namespace NScript {
class CV_RockTrollFirstEncounterScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CRTFE_Sparrow;
};
}
using NScript::CV_RockTrollFirstEncounterScript;
namespace NScript {
class CV_RockTrollFirstEncounterScript::CRTFE_Sparrow : public CEntityScriptBase {
public:GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
private:
    virtual void Main();                          // PDB ?Main@CRTFE_Sparrow@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ
};
}
typedef NScript::CV_RockTrollFirstEncounterScript::CRTFE_Sparrow CRTFE_Sparrow;    // global spelling used below
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CRTFE_Sparrow,Parent)==0x14);
FABLE_STATIC_ASSERT(offsetof(NativeParent,ScriptFlag)==0x4c);
#endif
void NScript::CV_RockTrollFirstEncounterScript::CRTFE_Sparrow::Main(){
 {NScript::CIsThingAlive p(Me);SetPredicate(p);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 ScriptResource resource;
 if(resource.IsValid())resource.Release();
 while(!Game->StartScriptingEntity(Me,resource,4)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(!IsActiveThreadTerminating()){
  while(!Parent->ScriptFlag){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  IsActiveThreadTerminating();
 }
}