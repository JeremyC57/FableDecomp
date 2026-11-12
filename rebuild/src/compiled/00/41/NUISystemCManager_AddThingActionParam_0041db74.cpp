// NUISystem::CManager::AddThingActionParam(long) -- appends the thing id to ThingActionParams (STLport push_back, out of line in this TU)
// Leaf: ?AddThingActionParam@CManager@NUISystem@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041DB74, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 24 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts AddThingActionParam(long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?AddThingActionParam@CManager@NUISystem@@UAEXJ@Z; `ret` matches the signature.
// Was: _unattributed::vfunc_24 (rebuild/src/compiled/00/41/unattributed_vfunc24_0041db74.cpp).
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::AddThingActionParam(long param)
{
    ThingActionParams.push_back(param);
}
}
