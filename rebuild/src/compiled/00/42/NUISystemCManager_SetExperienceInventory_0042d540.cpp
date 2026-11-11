// NUISystem::CManager::SetExperienceInventory(CTCInventoryExperience*) -- stores the experience inventory
// Leaf: ?SetExperienceInventory@CManager@NUISystem@@UAEXPAVCTCInventoryExperience@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D540, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 20 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetExperienceInventory(CTCInventoryExperience *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetExperienceInventory@CManager@NUISystem@@UAEXPAVCTCInventoryExperience@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_20 (rebuild/src/compiled/00/42/gapscan_sub0042d540_0042d540.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetExperienceInventory(CTCInventoryExperience* inventory)
{
    PExperienceInventory = inventory;
}
}
