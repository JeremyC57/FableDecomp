// CGameScriptInterface::EntityBeginLoadingAnimation(const CScriptThing& thing, Value value) -- Forward the script request after the retail lifetime and type checks.
// Leaf: ?EntityBeginLoadingAnimation@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee006631c0 is CThingCreatureBase::PreloadAnimation.
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharString;class CCPPointerInfo;
// Layout: CThing active header is quarantined; ThingTypeFlags low byte+0x6c is read. All other fields are outside this call boundary.
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Layout: CThingCreatureBase is used only as an external method receiver at base offset0; no derived fields accessed.
class CThingCreatureBase:public CThing {public:void PreloadAnimation(const CCharString&);};
// Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityBeginLoadingAnimation(const CScriptThing& thing,const CCharString& value)const{if(thing.IsAlive()){CThing* p=thing.GetPThing();if(p->GetTypeFlags()&8)((CThingCreatureBase*)p)->PreloadAnimation(value);}}