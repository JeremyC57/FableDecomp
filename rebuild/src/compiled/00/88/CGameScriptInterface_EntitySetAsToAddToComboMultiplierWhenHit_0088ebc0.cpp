// CGameScriptInterface::EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing, Value value) -- Forward the script request after the retail lifetime and type checks.
// Leaf: ?EntitySetAsToAddToComboMultiplierWhenHit@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee008389a0 is CThingGameObject::SetAddToComboMultiplierOnHit.
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharString;class CCPPointerInfo;
// Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
class CThing {public:unsigned char Prefix[0x6c];unsigned int ThingTypeFlags;};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Layout: CThingGameObject is used only as an external method receiver at base offset0; no derived fields accessed.
class CThingGameObject:public CThing {public:void SetAddToComboMultiplierOnHit(bool);};
// Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing& thing,bool value)const{if(thing.IsAlive()){CThing* p=thing.GetPThing();if(p->ThingTypeFlags&4)((CThingGameObject*)p)->SetAddToComboMultiplierOnHit(value);}}