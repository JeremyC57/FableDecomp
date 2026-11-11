// NUISystem::CManager::SetWeaponsInventory(CTCInventoryWeapons*) -- stores the weapons inventory
// Leaf: ?SetWeaponsInventory@CManager@NUISystem@@UAEXPAVCTCInventoryWeapons@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D50E, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 15 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetWeaponsInventory(CTCInventoryWeapons *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetWeaponsInventory@CManager@NUISystem@@UAEXPAVCTCInventoryWeapons@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_15 (rebuild/src/compiled/00/42/gapscan_sub0042d50e_0042d50e.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetWeaponsInventory(CTCInventoryWeapons* inventory)
{
    PWeaponsInventory = inventory;
}
}
