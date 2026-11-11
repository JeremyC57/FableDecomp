// CTCSpotLight::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCSpotLight@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007325C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 8 of CTCSpotLight's vtable 0x01266FBC (the slot holds this body in 1 vtable(s), owner CTCSpotLight); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSpotLight@@UBE_NXZ; `ret` matches the signature.
// Was: CTCSpotLight::vfunc_8 (rebuild/src/compiled/00/73/gapscan_sub007325c0_007325c0.cpp).
#define FABLE_RETAIL_CTCSPOTLIGHT_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CTCSpotLight.h"
bool CTCSpotLight::HasFrameUpdate() const
{
    return true;
}
