// CGameScriptInterface::EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, Value value) -- Forward the script request after the retail lifetime and type checks.
// Leaf: ?EntitySetAsToAddToComboMultiplierWhenHit@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee008389a0 is CThingGameObject::SetAddToComboMultiplierOnHit.
#define CGameScriptInterface CGameScriptInterface_HeaderLayout
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharString;class CCPPointerInfo;
// Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
class CThing {public:unsigned char Prefix[0x6c];unsigned int ThingTypeFlags;};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
class CScriptThing {public:
 virtual void UnusedSlot000();
 virtual void UnusedSlot001();
 virtual void UnusedSlot002();
 virtual void UnusedSlot003();
 virtual void UnusedSlot004();
 virtual void UnusedSlot005();
 virtual void UnusedSlot006();
 virtual void UnusedSlot007();
 virtual void UnusedSlot008();
 virtual void UnusedSlot009();
 virtual void UnusedSlot010();
 virtual CThing* GetPThing()const;
 virtual void UnusedSlot012();
 virtual void UnusedSlot013();
 virtual void UnusedSlot014();
 virtual void UnusedSlot015();
 virtual void UnusedSlot016();
 virtual void UnusedSlot017();
 virtual void UnusedSlot018();
 virtual void UnusedSlot019();
 virtual void UnusedSlot020();
 virtual void UnusedSlot021();
 virtual void UnusedSlot022();
 virtual void UnusedSlot023();
 virtual void UnusedSlot024();
 virtual void UnusedSlot025();
 virtual void UnusedSlot026();
 virtual void UnusedSlot027();
 virtual void UnusedSlot028();
 virtual void UnusedSlot029();
 virtual void UnusedSlot030();
 virtual void UnusedSlot031();
 virtual void UnusedSlot032();
 virtual void UnusedSlot033();
 virtual void UnusedSlot034();
 virtual void UnusedSlot035();
 virtual void UnusedSlot036();
 virtual void UnusedSlot037();
 virtual void UnusedSlot038();
 virtual void UnusedSlot039();
 virtual void UnusedSlot040();
 virtual void UnusedSlot041();
 virtual void UnusedSlot042();
 virtual void UnusedSlot043();
 virtual void UnusedSlot044();
 virtual void UnusedSlot045();
 virtual void UnusedSlot046();
 virtual void UnusedSlot047();
 virtual void UnusedSlot048();
 virtual void UnusedSlot049();
 virtual void UnusedSlot050();
 virtual void UnusedSlot051();
 virtual void UnusedSlot052();
 virtual void UnusedSlot053();
 virtual void UnusedSlot054();
 virtual void UnusedSlot055();
 virtual void UnusedSlot056();
 virtual void UnusedSlot057();
 virtual void UnusedSlot058();
 virtual void UnusedSlot059();
 virtual void UnusedSlot060();
 virtual void UnusedSlot061();
 virtual void UnusedSlot062();
 virtual void UnusedSlot063();
 virtual void UnusedSlot064();
 virtual void UnusedSlot065();
 virtual void UnusedSlot066();
 virtual void UnusedSlot067();
 virtual void UnusedSlot068();
 virtual void UnusedSlot069();
 virtual void UnusedSlot070();
 virtual void UnusedSlot071();
 virtual void UnusedSlot072();
 virtual void UnusedSlot073();
 virtual void UnusedSlot074();
 virtual bool IsAlive()const;
 CScriptThing* PImp_Data;CCPPointerInfo* PImp_Info;
};
// Layout: CThingGameObject is used only as an external method receiver at base offset0; no derived fields accessed.
class CThingGameObject:public CThing {public:void SetAddToComboMultiplierOnHit(bool);};
// Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.
class CGameScriptInterface {public:virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing,bool value)const;unsigned char OwnerFields[0x4c];};
void CGameScriptInterface::EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing,bool value)const{if(thing.IsAlive()){CThing* p=thing.GetPThing();if(p->ThingTypeFlags&4)((CThingGameObject*)p)->SetAddToComboMultiplierOnHit(value);}}