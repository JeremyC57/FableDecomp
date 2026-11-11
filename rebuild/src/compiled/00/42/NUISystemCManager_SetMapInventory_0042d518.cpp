// NUISystem::CManager::SetMapInventory(CTCInventoryMap*) -- stores the map inventory
// Leaf: ?SetMapInventory@CManager@NUISystem@@UAEXPAVCTCInventoryMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D518, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 16 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetMapInventory(CTCInventoryMap *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetMapInventory@CManager@NUISystem@@UAEXPAVCTCInventoryMap@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_16 (rebuild/src/compiled/00/42/gapscan_sub0042d518_0042d518.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetMapInventory(CTCInventoryMap* inventory)
{
    PMapInventory = inventory;
}
}
