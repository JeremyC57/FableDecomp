// CTCInventoryExperience::OnChangeCategory() -- zeroes the stat indicator position and maximum, then re-seeds them
// Leaf: ?OnChangeCategory@CTCInventoryExperience@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056EC79, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 37 of CTCInventoryExperience's vtable 0x0124F11C (the slot holds this body in 1 vtable(s), owner CTCInventoryExperience); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts OnChangeCategory(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnChangeCategory@CTCInventoryExperience@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCInventoryExperience::vfunc_37 (rebuild/src/compiled/00/56/CTCInventoryExperience_vfunc37_0056ec79.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryExperience.h"
void CTCInventoryExperience::OnChangeCategory()
{
    statIndicatorPos = 0;
    statIndicatorMax = 0;
    ResetStatIndicatorValues();
}
