// NUISystem::CManager::SetInventoryTrade(CTCInventoryTrade*) -- stores the trade inventory
// Leaf: ?SetInventoryTrade@CManager@NUISystem@@UAEXPAVCTCInventoryTrade@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D55E, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 23 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetInventoryTrade(CTCInventoryTrade *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetInventoryTrade@CManager@NUISystem@@UAEXPAVCTCInventoryTrade@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_23 (rebuild/src/compiled/00/42/gapscan_sub0042d55e_0042d55e.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetInventoryTrade(CTCInventoryTrade* inventory)
{
    PInventoryTrade = inventory;
}
}
