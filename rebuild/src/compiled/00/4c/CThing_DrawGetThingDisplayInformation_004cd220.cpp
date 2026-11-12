// CThing::DrawGetThingDisplayInformation(CThingDrawInformation*) const -- the base thing has no display information: returns false
// Leaf: ?DrawGetThingDisplayInformation@CThing@@UBE_NPAVCThingDrawInformation@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD220, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 34 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 14 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts DrawGetThingDisplayInformation(CThingDrawInformation *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawGetThingDisplayInformation@CThing@@UBE_NPAVCThingDrawInformation@@@Z; `ret` matches the signature.
// Was: CThing::vfunc_34 (rebuild/src/compiled/00/4c/CThing_vfunc34_004cd220.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::DrawGetThingDisplayInformation(CThingDrawInformation*) const
{
    return false;
}
