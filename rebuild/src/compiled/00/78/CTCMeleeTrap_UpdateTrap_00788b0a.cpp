// CTCMeleeTrap::UpdateTrap() -- updates the trap animation, then the strike action
// Leaf: ?UpdateTrap@CTCMeleeTrap@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00788B0A, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 29 of CTCMeleeTrap's vtable 0x0126B884 (the slot holds this body in 1 vtable(s), owner CTCMeleeTrap); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts UpdateTrap(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?UpdateTrap@CTCMeleeTrap@@MAEXXZ; no `ret` to check (NORET).
// Was: CTCMeleeTrap::vfunc_29 (rebuild/src/compiled/00/78/unattributed_CallTwo00788555_00788b0a.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCMELEETRAP_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCMeleeTrap.h"
void CTCMeleeTrap::UpdateTrap()
{
    UpdateAnimation();
    UpdateStrikeAction();
}
