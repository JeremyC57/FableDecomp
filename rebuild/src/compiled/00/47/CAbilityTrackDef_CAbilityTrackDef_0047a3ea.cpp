// CAbilityTrackDef::CAbilityTrackDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CAbilityTrackDef vtable, then the empty Abilities vector
// Leaf: ??0CAbilityTrackDef@@QAE@XZ
// Retail: Fable.exe @ 0x0047A3EA. Identity: the vtable it stores, 0x0123649C, is RTTI .?AVCAbilityTrackDef@@ : CParentDefClassBase;
//   Ego_r.pdb / FableWin.pdb ??0CAbilityTrackDef@@QAE@XZ; engine/CAbilityTrackDef.h places the constructor here.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ).
// Model: the shared engine/CAbilityTrackDef.h (FABLE_RETAIL_CABILITYTRACKDEF_MEMBERS) and engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CABILITYTRACKDEF_MEMBERS
#include "engine/CParentDefClassBase.h"
#include "engine/CAbilityTrackDef.h"

CAbilityTrackDef::CAbilityTrackDef()
{
}
