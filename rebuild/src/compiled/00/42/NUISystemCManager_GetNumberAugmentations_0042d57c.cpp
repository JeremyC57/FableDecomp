// NUISystem::CManager::GetNumberAugmentations() const -- returns NumberAugmentations
// Leaf: ?GetNumberAugmentations@CManager@NUISystem@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D57C, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 31 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetNumberAugmentations(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetNumberAugmentations@CManager@NUISystem@@UBEKXZ; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_31 (rebuild/src/compiled/00/42/gapscan_sub0042d57c_0042d57c.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
unsigned long CManager::GetNumberAugmentations() const
{
    return NumberAugmentations;
}
}
