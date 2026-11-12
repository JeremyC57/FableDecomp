// NUISystem::CManager::GetChange() -- returns Change
// Leaf: ?GetChange@CManager@NUISystem@@UAE?AW4EChange@12@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D58A, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 33 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetChange(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetChange@CManager@NUISystem@@UAE?AW4EChange@12@XZ; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_33 (rebuild/src/compiled/00/42/gapscan_sub0042d58a_0042d58a.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CManager::EChange CManager::GetChange()
{
    return Change;
}
}
