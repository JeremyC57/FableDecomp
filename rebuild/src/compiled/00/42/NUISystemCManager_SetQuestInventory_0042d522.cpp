// NUISystem::CManager::SetQuestInventory(CTCInventoryQuests*) -- stores the quest inventory
// Leaf: ?SetQuestInventory@CManager@NUISystem@@UAEXPAVCTCInventoryQuests@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D522, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 17 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetQuestInventory(CTCInventoryQuests *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetQuestInventory@CManager@NUISystem@@UAEXPAVCTCInventoryQuests@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_17 (rebuild/src/compiled/00/42/gapscan_sub0042d522_0042d522.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetQuestInventory(CTCInventoryQuests* inventory)
{
    PQuestInventory = inventory;
}
}
