// NUISystem::CManager::SetInventoryClothing(CTCInventoryClothing*) -- stores the clothing inventory
// Leaf: ?SetInventoryClothing@CManager@NUISystem@@UAEXPAVCTCInventoryClothing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D554, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 22 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetInventoryClothing(CTCInventoryClothing *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetInventoryClothing@CManager@NUISystem@@UAEXPAVCTCInventoryClothing@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_22 (rebuild/src/compiled/00/42/gapscan_sub0042d554_0042d554.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetInventoryClothing(CTCInventoryClothing* inventory)
{
    PInventoryClothing = inventory;
}
}
