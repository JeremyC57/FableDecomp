// CThing::RenderUpdate(CInterpolationInfo const*) -- the base thing has no render update: returns false
// Leaf: ?RenderUpdate@CThing@@UAE_NPBVCInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7460, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 21 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 16 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts RenderUpdate(CInterpolationInfo const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?RenderUpdate@CThing@@UAE_NPBVCInterpolationInfo@@@Z; `ret` matches the signature.
// Was: CThing::vfunc_21 (rebuild/src/compiled/00/4c/CThingFilterIsUnconscious_operator_004c7460.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::RenderUpdate(const CInterpolationInfo*)
{
    return false;
}
