// CTCMultiArrow::OnRemove() -- the remove path: FinalCleanup
// Leaf: ?OnRemove@CTCMultiArrow@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC480, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 2 of CTCMultiArrow's vtable 0x0126ED7C (the slot holds this body in 1 vtable(s), owner CTCMultiArrow); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnRemove(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnRemove@CTCMultiArrow@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCMultiArrow::vfunc_2 (rebuild/src/compiled/00/7c/CTCMultiArrow_FinalCleanup_007cc480.cpp).
// Opt-ins as in the landed row CTCMultiArrow_HasFrameUpdate_007cc980.cpp.
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS2
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS
#include "engine/CTCMultiArrow.h"
void CTCMultiArrow::OnRemove()
{
    FinalCleanup();
}
