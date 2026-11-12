// CThing::CopyGameState(CThing const&) -- the base thing copies no game state
// Leaf: ?CopyGameState@CThing@@UAEXABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C72A0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 3 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts CopyGameState(CThing const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CopyGameState@CThing@@UAEXABV1@@Z; `ret` matches the signature.
// Was: CThing::vfunc_7 (rebuild/src/compiled/00/4c/StubReturnVoid004c72a0_StubReturnVoid004c72a0_004c72a0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
void CThing::CopyGameState(const CThing&)
{

}
