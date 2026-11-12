// CTCBattleCharge::HasFrameUpdate() const -- the battle charge is frame-updated: returns true
// Leaf: ?HasFrameUpdate@CTCBattleCharge@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A92B0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 8 of CTCBattleCharge's vtable 0x0126CBFC (the slot holds this body in 1 vtable(s), owner CTCBattleCharge); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBattleCharge@@UBE_NXZ; `ret` matches the signature.
// Was: CTCBattleCharge::vfunc_8 (rebuild/src/compiled/00/7a/gapscan_sub007a92b0_007a92b0.cpp).
#define FABLE_RETAIL_CTCBATTLECHARGE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CTCBattleCharge.h"
bool CTCBattleCharge::HasFrameUpdate() const
{
    return true;
}
