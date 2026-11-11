// CTCMultiArrow::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCMultiArrow@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC470, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCMultiArrow's vtable 0x0126ED7C (the slot holds this body in 1 vtable(s), owner CTCMultiArrow); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCMultiArrow@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCMultiArrow::vfunc_14 (rebuild/src/compiled/00/7c/gapscan_sub007cc470_007cc470.cpp).
// Opt-ins as in the landed row CTCMultiArrow_HasFrameUpdate_007cc980.cpp.
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS2
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS
#include "engine/CTCMultiArrow.h"
void CTCMultiArrow::OnKill()
{
    FinalCleanup();
}
