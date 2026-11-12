// CThing::OnSetInLimbo(bool) -- the base thing ignores limbo changes
// Leaf: ?OnSetInLimbo@CThing@@MAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD1C0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 16 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts OnSetInLimbo(bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSetInLimbo@CThing@@MAEX_N@Z; `ret` matches the signature.
// Was: CThing::vfunc_14 (rebuild/src/compiled/00/4c/gapscan_sub004cd1c0_004cd1c0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
void CThing::OnSetInLimbo(bool)
{

}
