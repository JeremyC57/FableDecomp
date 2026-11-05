// CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) -- memberwise assignment: the leading part through its out-of-line operator=, then the counted pointer (release the old block, adopt, add-ref)
// Leaf: ??4CScriptGameResourceObjectScriptedThingBase@@QAEAAV0@ABV0@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008abd10, 80 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??4CScriptGameResourceObjectScriptedThingBase@@QAEAAV0@ABV0@@Z (bound in reloc_bindings); RTTI bases CScriptGameResourceObjectBase : CBaseObject : CBaseClass, Ego_r.pdb PImp (CCountedPointer<CScriptGameResourceObjectScriptedThingBase>) at +8; callers CGameScriptInterface::StartScriptingEntity 0x0089B5B0 and the cutscene macro runner 0x00CBFB7D.
// Callees: CBaseObject::operator=(const CBaseObject&) 0x0099A3B0 (Ego_r.pdb IAE), operator delete 0x00BFE9BC (the old count block); CCountedPointer's operator= inline.
// Shape (family L088): the compiler-generated copy assignment, written out (same body) so the translation unit emits it.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject {
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);
};
CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other)
{
    CScriptGameResourceObjectBase::operator=(other);
    PImp = other.PImp;
    return *this;
}