// NUISystem::CManager::SetPlayerThing(CThing*) -- stores the player thing
// Leaf: ?SetPlayerThing@CManager@NUISystem@@UAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D568, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 26 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts SetPlayerThing(CThing *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetPlayerThing@CManager@NUISystem@@UAEXPAVCThing@@@Z; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_26 (rebuild/src/compiled/00/42/gapscan_sub0042d568_0042d568.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetPlayerThing(CThing* player)
{
    PPlayer = player;
}
}
