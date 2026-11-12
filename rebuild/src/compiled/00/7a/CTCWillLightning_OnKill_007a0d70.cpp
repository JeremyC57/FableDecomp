// CTCWillLightning::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCWillLightning@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A0D70, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 14 of CTCWillLightning's vtable 0x0126C724 (the slot holds this body in 1 vtable(s), owner CTCWillLightning); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCWillLightning@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCWillLightning::vfunc_14 (rebuild/src/compiled/00/7a/gapscan_sub007a0d70_007a0d70.cpp).
#define FABLE_RETAIL_CTCWILLLIGHTNING_VSLOTS3
#define FABLE_RETAIL_CTCWILLLIGHTNING_VSLOTS
#define FABLE_RETAIL_CTCWILLLIGHTNING_AICASTING
#include "engine/CTCWillLightning.h"
void CTCWillLightning::OnKill()
{
    FinalCleanup();
}
