// CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const -- the implementation's script thing, or an empty (null) CScriptThing when there is no implementation
// Leaf: ?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ,
//   slot 12 of vtable 0x0127094C (the shared header declares it so). Was defined on the stand-in CScriptGameResourceObjectScriptedThingBase_Methods
//   over the flattened layout and a CScriptedThingBaseVirtuals vtable view; now the retail header (FABLE_RETAIL_BASE_OBJECT, PImp +0x08).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E7490, length 57 bytes
// (`?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ`, a const virtual).
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0 (null arm); virtual slot 12 (+0x30) of the implementation.
// Shape: PImp.Data (+8) loaded straight into ecx and null-tested; the hidden result
// pointer read into esi once for both arms. Null arm: the inline default constructor of the returned
// CScriptThing -- the out-of-line CBaseClass base constructor, the CScriptThing vtable 0x01238C8C,
// then its PImp counted pointer zeroed by the member's own inline constructor (immediate zero stores
// after the vtable store). Else the virtual writes the result through the forwarded hidden pointer.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct CScriptThingImp;
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const
{
    CScriptGameResourceObjectScriptedThingBase* imp = PImp.Get();
    if (imp == 0)
        return CScriptThing();
    return imp->GetScriptThing();
}
