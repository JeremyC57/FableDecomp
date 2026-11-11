// CThing::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const -- the base thing has no physics debug info: returns false
// Leaf: ?DrawGetPhysicsDebugInfo@CThing@@UBE_NAAVCThingPhysicalDebugInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD230, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 35 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 3 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawGetPhysicsDebugInfo@CThing@@UBE_NAAVCThingPhysicalDebugInfo@@@Z; `ret` matches the signature.
// Was: CThing::vfunc_35 (rebuild/src/compiled/00/4c/CThing_vfunc35_004cd230.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const
{
    return false;
}
