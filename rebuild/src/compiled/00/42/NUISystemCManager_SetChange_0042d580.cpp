// NUISystem::CManager::SetChange(NUISystem::CManager::EChange) -- stores the change state
// Leaf: ?SetChange@CManager@NUISystem@@UAEXW4EChange@12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D580, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 32 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetChange(NUISystem::CManager::EChange) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetChange@CManager@NUISystem@@UAEXW4EChange@12@@Z; no `ret` to check (UNKSIZE).
// Was: NUISystem::CManager::vfunc_32 (rebuild/src/compiled/00/42/gapscan_sub0042d580_0042d580.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetChange(CManager::EChange change)
{
    Change = change;
}
}
