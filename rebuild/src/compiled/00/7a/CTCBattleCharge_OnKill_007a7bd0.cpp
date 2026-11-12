// CTCBattleCharge::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCBattleCharge@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A7BD0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCBattleCharge's vtable 0x0126CBFC (the slot holds this body in 1 vtable(s), owner CTCBattleCharge); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCBattleCharge@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCBattleCharge::vfunc_14 (rebuild/src/compiled/00/7a/gapscan_sub007a7bd0_007a7bd0.cpp).
#define FABLE_RETAIL_CTCBATTLECHARGE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CTCBattleCharge.h"
void CTCBattleCharge::OnKill()
{
    FinalCleanup();
}
