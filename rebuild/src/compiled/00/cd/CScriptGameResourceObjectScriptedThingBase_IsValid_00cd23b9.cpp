// CScriptGameResourceObjectScriptedThingBase::IsValid() const -- the scripted-thing handle is valid while it holds an implementation
// Leaf: ?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD23B9, 9 bytes, thiscall, plain `ret`; an unaligned address in the script-helper block.
// Identity (probable): the manifest label C3DMeshInfo::HasPhysicsMesh is propagated (C3DMeshInfo has nothing at +8). The 1,197
//   callers are all quest-script code calling it on CScriptGameResourceObjectScriptedThing handles (the landed callers model it
//   as that handle's method, under the invented name IsScripting); Ego_r.pdb CScriptGameResourceObjectScriptedThingBase has exactly
//   one non-virtual const bool method without arguments, ?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ, and its
//   PImp (CCountedPointer<CScriptGameResourceObjectScriptedThingBase>) Data is the tested +0x08 word.
// Layout: engine/CScriptGameResourceObjectScriptedThingBase.h (vptr +0, +4, PImp Data +0x08, Info +0x0c), included under a layout
//   name so the method can be added (the header carries data members only).
// Shape: a size-optimised TU (unaligned address): `#pragma optimize("s", on)` gives retail's `xor eax,eax ; cmp [ecx+8],eax`.
#pragma optimize("s", on)
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBase_Layout
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectScriptedThingBase_Layout {
public:
    bool IsValid() const;
};

bool CScriptGameResourceObjectScriptedThingBase::IsValid() const
{
    return PImp_Data != 0;
}