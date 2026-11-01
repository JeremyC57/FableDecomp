// CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase() -- the compiler-generated destructor: release the counted PImp, then the inline CScriptGameResourceObjectBase destructor (its vtable store) and ~CBaseObject
// Leaf: ??1CScriptGameResourceObjectMovieBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7B80, 66 bytes, thiscall, tail `jmp ~CBaseObject`.
// Identity (proven): the ctor 0x006E7B60 stores vtable 0x01260EF4 (RTTI .?AVCScriptGameResourceObjectMovieBase@@ :
//   CScriptGameResourceObjectBase), whose slot-0 `??_G` 0x006E7BD0 expands this same body; the 1,132 callers are the
//   landed NScript quest scripts destroying MovieBase locals (engine/CScriptGameResourceObjectMovieBase.h, wave 8 per-site
//   expansion notes). Ego_r.pdb and FableWin.pdb both spell ??1CScriptGameResourceObjectMovieBase@@UAE@XZ; Ego_r layout
//   PImp CCountedPointer<CScriptGameResourceObjectMovieBase> +8 (0x10 bytes). No 0x01260EF4 store before the release: the
//   destructor is the implicit one. The byte twin 0x007E74D0 is ~CScriptGameResourceObjectScriptedThingBase (vtable
//   0x0127094C via ctor 0x007E72A0 / ??_G 0x007E7520): two symbols, not one.
// Callees: operator delete 0x00BFE9BC, the info block's DeleteFunc through +4, CBaseObject::~CBaseObject 0x0099A430
//   (??1CBaseObject@@MAE@XZ).
// Shape: PImp's inline CCountedPointer dtor (engine/CCountedPointer.h), then the user-written inline
//   `virtual ~CScriptGameResourceObjectBase() { }` keeps the 0x0126008C store before the tail jump. DestroyOne (a destruction
//   by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1` (chunks 656 / 657 / 696).
// Layout: the shared retail headers (FABLE_RETAIL_BASE_OBJECT).
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectMovieBase.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CScriptGameResourceObjectMovieBase* movie)
{
    movie->CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase();
}