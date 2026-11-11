// NUISystem::CManager::SetTCHeroExperienceInventory(CTCHeroExperience*) -- stores the hero experience component
// Leaf: ?SetTCHeroExperienceInventory@CManager@NUISystem@@UAEXPAVCTCHeroExperience@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D52C, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 18 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetTCHeroExperienceInventory(CTCHeroExperience *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetTCHeroExperienceInventory@CManager@NUISystem@@UAEXPAVCTCHeroExperience@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_18 (rebuild/src/compiled/00/42/gapscan_sub0042d52c_0042d52c.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetTCHeroExperienceInventory(CTCHeroExperience* experience)
{
    PTCHeroExperience = experience;
}
}
