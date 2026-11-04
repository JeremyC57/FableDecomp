// CScriptGameResourceObjectScriptedThingBase::SetImplementation(CScriptGameResourceObjectScriptedThingBase* imp) -- make the handle own a new implementation (reset its counted pointer to imp)
// Leaf: ?SetImplementation@CScriptGameResourceObjectScriptedThingBase@@QAEXPAV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB960, 8 bytes: `add ecx,8 ; jmp 0x008AB230` (PImp.Reset(imp), tail call; imp stays on the stack).
// Identity (two sources): (1) FableWin.pdb ?SetImplementation@CScriptGameResourceObjectScriptedThingBase@@QAEXPAV1@@Z, already the binding
//   of the landed CScriptGameResourceObjectScriptedThing::CreateNullHandle 0x00903810 (and engine/CScriptGameResourceObjectScriptedThingBase.h
//   names this address); (2) both callers (CreateNullHandle 0x00903854, CGameScriptInterface::StartScriptingEntity 0x0089B6F2) build a
//   handle (vtable 0x0127094C, PImp zeroed at +8/+C), `new` a 0x28-byte implementation and pass it here; PImp is the Ego_r member at +8
//   and the target is the CCountedPointer<CScriptGameResourceObjectScriptedThingBase> Reset(T*) whose deleter 0x008AA7E0 it stores.
//   The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?Reset@?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@QAEXPAVCScriptGameResourceObjectScriptedThingBase@@@Z
//   0x008AB230 (out of line: VC7.1 does not expand the header's inline Reset here).
// Header: engine/CScriptGameResourceObjectScriptedThingBase.h (FABLE_RETAIL_BASE_OBJECT; it already declares SetImplementation), engine/CCountedPointer.h.
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
void CScriptGameResourceObjectScriptedThingBase::SetImplementation(CScriptGameResourceObjectScriptedThingBase* imp)
{
    PImp.Reset(imp);
}