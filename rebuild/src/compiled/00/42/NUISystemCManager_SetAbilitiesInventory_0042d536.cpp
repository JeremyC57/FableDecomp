// NUISystem::CManager::SetAbilitiesInventory(CTCInventoryAbilities*) -- stores the abilities inventory
// Leaf: ?SetAbilitiesInventory@CManager@NUISystem@@UAEXPAVCTCInventoryAbilities@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D536, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 19 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetAbilitiesInventory(CTCInventoryAbilities *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetAbilitiesInventory@CManager@NUISystem@@UAEXPAVCTCInventoryAbilities@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_19 (rebuild/src/compiled/00/42/gapscan_sub0042d536_0042d536.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetAbilitiesInventory(CTCInventoryAbilities* inventory)
{
    PAbilitiesInventory = inventory;
}
}
