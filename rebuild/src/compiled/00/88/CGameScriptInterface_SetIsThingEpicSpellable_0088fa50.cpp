// CGameScriptInterface::SetIsThingEpicSpellable(const CScriptThing& thing, Value value) -- Forward the script request after the retail lifetime and type checks.
// Leaf: ?SetIsThingEpicSpellable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Identity: PDB exact virtual const API; script slot11 GetPThing and75 IsAlive are independently named retail004aaa60/004ab130; own callee007e4330 is CTCWillResponse::SetThingIsEpicSpellable.
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
// Layout: CTCWillResponse is a static-method name scope only; no object or its shared header fields are used.
class CTCWillResponse {public:static void __fastcall SetThingIsEpicSpellable(CThing*,bool);};
// Layout: shared CGameScriptInterface is a flattened vptr POD; real virtual declaration below preserves calling convention. No owner fields are accessed; body reserves retail0x50 bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetIsThingEpicSpellable(const CScriptThing& thing,bool value)const{if(thing.IsAlive())CTCWillResponse::SetThingIsEpicSpellable(thing.GetPThing(),value);}