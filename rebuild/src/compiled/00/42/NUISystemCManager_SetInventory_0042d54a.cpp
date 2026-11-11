// NUISystem::CManager::SetInventory(CTCInventory*) -- stores the inventory
// Leaf: ?SetInventory@CManager@NUISystem@@UAEXPAVCTCInventory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D54A, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 21 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetInventory(CTCInventory *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetInventory@CManager@NUISystem@@UAEXPAVCTCInventory@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_21 (rebuild/src/compiled/00/42/gapscan_sub0042d54a_0042d54a.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetInventory(CTCInventory* inventory)
{
    PInventory = inventory;
}
}
