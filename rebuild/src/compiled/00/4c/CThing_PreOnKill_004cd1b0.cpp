// CThing::PreOnKill() -- the base thing allows the kill: returns true
// Leaf: ?PreOnKill@CThing@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD1B0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 13 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 16 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts PreOnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?PreOnKill@CThing@@MAE_NXZ; `ret` matches the signature.
// Was: CThing::vfunc_13 (rebuild/src/compiled/00/4c/gapscan_sub004cd1b0_004cd1b0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::PreOnKill()
{
    return true;
}
