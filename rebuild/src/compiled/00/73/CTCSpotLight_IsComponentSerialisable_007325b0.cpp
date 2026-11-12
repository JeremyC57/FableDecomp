// CTCSpotLight::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCSpotLight@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007325B0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 23 of CTCSpotLight's vtable 0x01266FBC (the slot holds this body in 1 vtable(s), owner CTCSpotLight); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCSpotLight@@UBE_NXZ; `ret` matches the signature.
// Was: CTCSpotLight::vfunc_23 (rebuild/src/compiled/00/73/gapscan_sub007325b0_007325b0.cpp).
#define FABLE_RETAIL_CTCSPOTLIGHT_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CTCSpotLight.h"
bool CTCSpotLight::IsComponentSerialisable() const
{
    return true;
}
