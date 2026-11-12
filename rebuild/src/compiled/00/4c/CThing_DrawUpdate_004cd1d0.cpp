// CThing::DrawUpdate(CInterpolationInfo const*) -- the base thing has no draw update
// Leaf: ?DrawUpdate@CThing@@UAEXPBVCInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD1D0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 26 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 14 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts DrawUpdate(CInterpolationInfo const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawUpdate@CThing@@UAEXPBVCInterpolationInfo@@@Z; `ret` matches the signature.
// Was: CThing::vfunc_26 (rebuild/src/compiled/00/4c/gapscan_sub004cd1d0_004cd1d0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
void CThing::DrawUpdate(const CInterpolationInfo*)
{

}
