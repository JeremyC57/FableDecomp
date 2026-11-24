// Leaf: ?GetPrimitiveDrawDistanceFactor@CIEngine@@UBEMXZ
// CIEngine::GetPrimitiveDrawDistanceFactor @ 0x00b23360
//   fld DWORD PTR ds:0x122ded8
//   ret
//
// Virtual const accessor that returns a global float. The 'this' pointer
// (ecx) is unused: the value is a module-global loaded straight into st(0).
// Modeled as __fastcall (this in ecx, byte-identical to __fastcall for a
// this-only accessor). VC7.1 emits `fld DWORD PTR ds:<addr>; ret` for a
// function that simply returns a global float value.
//
// The address 0x122ded8 is relocation-masked in parity, so the global need
// not resolve to that exact address; declaring it extern lets the linker
// place it while the compiler still emits the fld/ret shape.

#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
extern float g_FloatOne;

float CIEngine::GetPrimitiveDrawDistanceFactor() const
{
    return g_FloatOne;
}
// Flatten route (flatten_pilot v4, NF324): CIEngine__GetPrimitiveDrawDistanceFactor -> CIEngine::GetPrimitiveDrawDistanceFactor, the RTTI-placed PDB member; class model: engine/CIEngine.h under FABLE_RETAIL_CIENGINE.
