// CThing::ConstructAlreadyExistingThingFromLevelScript(char const*) -- the base thing accepts the level script: returns true
// Leaf: ?ConstructAlreadyExistingThingFromLevelScript@CThing@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C72B0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 5 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 17 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts ConstructAlreadyExistingThingFromLevelScript(char const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?ConstructAlreadyExistingThingFromLevelScript@CThing@@UAE_NPBD@Z; `ret` matches the signature.
// Was: CThing::vfunc_5 (rebuild/src/compiled/00/4c/gapscan_sub004c72b0_004c72b0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::ConstructAlreadyExistingThingFromLevelScript(const char*)
{
    return true;
}
