// CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase() -- the compiler-generated destructor: release the counted PImp, then the inline CScriptGameResourceObjectBase destructor (its vtable store) and ~CBaseObject
// Leaf: ??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E74D0, 66 bytes, thiscall, tail `jmp ~CBaseObject`.
// Identity (proven): the ctor 0x007E72A0 stores vtable 0x0127094C (RTTI .?AVCScriptGameResourceObjectScriptedThingBase@@ :
//   CScriptGameResourceObjectBase), whose slot-0 `??_G` 0x007E7520 expands this same body; the 1,380 callers are the landed
//   NScript quest / entity scripts destroying ScriptedThingBase locals (engine/CScriptGameResourceObjectScriptedThingBase.h
//   names the ctor/dtor pair 0x007E72A0 / 0x007E74D0). Ego_r.pdb and FableWin.pdb both spell
//   ??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ; Ego_r layout PImp CCountedPointer<..ScriptedThingBase> +8 (0x10).
//   The catalog label "~CScriptGameResourceObjectMovieBase" is propagated from the byte twin 0x006E7B80 (MovieBase, vtable
//   0x01260EF4): two symbols with byte-identical implicit bodies.
// Callees: operator delete 0x00BFE9BC, the info block's DeleteFunc through +4, CBaseObject::~CBaseObject 0x0099A430
//   (??1CBaseObject@@MAE@XZ).
// Shape: implicit destructor (no 0x0127094C store); PImp's inline CCountedPointer dtor, then the user-written inline
//   ~CScriptGameResourceObjectBase (0x0126008C store) before the tail jump. DestroyOne (ungraded) emits the out-of-line ??1.
// Layout: the shared retail headers (FABLE_RETAIL_BASE_OBJECT).
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CScriptGameResourceObjectScriptedThingBase* thing)
{
    thing->CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase();
}