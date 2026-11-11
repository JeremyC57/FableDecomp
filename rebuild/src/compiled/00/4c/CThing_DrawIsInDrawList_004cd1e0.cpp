// CThing::DrawIsInDrawList() const -- in the draw list when drawable (the slot-27 DrawIsDrawable)
// Leaf: ?DrawIsInDrawList@CThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD1E0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 30 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 17 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts DrawIsInDrawList(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawIsInDrawList@CThing@@UBE_NXZ; no `ret` to check (NORET).
// Was: CThing::vfunc_30 (rebuild/src/compiled/00/4c/CThing_vfunc30_004cd1e0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::DrawIsInDrawList() const
{
    return DrawIsDrawable();
}
