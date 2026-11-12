// NUISystem::CManager::GetMetaLayer() -- returns MetaLayer
// Leaf: ?GetMetaLayer@CManager@NUISystem@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D58E, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 35 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetMetaLayer(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMetaLayer@CManager@NUISystem@@UAEJXZ; `ret` matches the signature.
// Was: NUISystem::CManager::vfunc_35 (rebuild/src/compiled/00/42/gapscan_sub0042d58e_0042d58e.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
long CManager::GetMetaLayer()
{
    return MetaLayer;
}
}
