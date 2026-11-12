// NUISystem::CManager::SetNumberAugmentations(unsigned long) -- stores the augmentation count
// Leaf: ?SetNumberAugmentations@CManager@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D572, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 30 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetNumberAugmentations(unsigned long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetNumberAugmentations@CManager@NUISystem@@UAEXK@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_30 (rebuild/src/compiled/00/42/gapscan_sub0042d572_0042d572.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetNumberAugmentations(unsigned long count)
{
    NumberAugmentations = count;
}
}
