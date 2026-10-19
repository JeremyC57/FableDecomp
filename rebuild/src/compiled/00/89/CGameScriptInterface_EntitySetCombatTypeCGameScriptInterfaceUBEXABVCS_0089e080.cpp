// CGameScriptInterface::EntitySetCombatType(const CScriptThing&,const CCharString&) -- assign a resolved combat type to a live combat component
// Retail0089E080,164 bytes. CScriptThing slot11=004AAA60 GetPThing; CThing native map44,presence20,life91 (donor44->48).
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "engine/CTCCombat.h"
#include "fable_string.h"
struct ComponentEntry{long Type;void* Component;};
struct ComponentMap{ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&);}; //0040F020
struct CDefinitionManagerMethods;
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char State[0x45];unsigned char LifeFlags;CDefinitionManagerMethods* GetDefinitionManager();}; //004C7990 (folded getter; no explicit parameters)
struct ScriptMethods:CScriptThing{};
union ScriptSlot{void* Native;CThingNative*(ScriptMethods::*GetThing)()const;};
struct RefCount{long Count;void Dec(){--Count;}long Get()const{return Count;}};
struct DefVirtual{virtual ~DefVirtual();virtual void Destroy();};
struct DefObject:DefVirtual,RefCount{void ReleaseTemporary(){Dec();if(Get()==0)Destroy();}};
struct DefPointer{DefObject* Object;DefPointer():Object(0){}~DefPointer(){if(Object){Object->ReleaseTemporary();}}};
struct CDefinitionManagerMethods{bool GetCombatType(const CCharString&,DefPointer&);}; //00669770 genericGetDef<CCombatTypeDef>
struct CTCCombatMethods:CTCCombat{void SetCombatType(const DefPointer&);}; //007355F0
struct CGameScriptInterfaceMethods:CGameScriptInterface{void EntitySetCombatType(const ScriptMethods&,const CCharString&)const;};
void CGameScriptInterfaceMethods::EntitySetCombatType(const ScriptMethods& script,const CCharString& name)const{
 CThingNative* thing=(script.*reinterpret_cast<ScriptSlot*>(script.__vftable)[11].GetThing)();
 DefPointer type;
 if(thing&&!(thing->LifeFlags&1)&&(thing->UsingComponents[1]&0x10000)){
 ComponentMap* map=&thing->Components;ComponentEntry* it=map->LowerBound(48);ComponentEntry* end=map->Finish;
 CTCCombatMethods* combat=static_cast<CTCCombatMethods*>((it==end||it->Type>48?end:it)->Component);
 if(thing->GetDefinitionManager()->GetCombatType(name,type))combat->SetCombatType(type);
 }
}