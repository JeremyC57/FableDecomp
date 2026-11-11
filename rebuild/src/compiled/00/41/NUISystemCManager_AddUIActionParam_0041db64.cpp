// NUISystem::CManager::AddUIActionParam(NUISystem::CActionParam*) -- appends the param to UIActionParams (STLport push_back, out of line in this TU as in retail 0x0042B646)
// Leaf: ?AddUIActionParam@CManager@NUISystem@@UAEXPAVCActionParam@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041DB64, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 27 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts AddUIActionParam(NUISystem::CActionParam *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?AddUIActionParam@CManager@NUISystem@@UAEXPAVCActionParam@2@@Z; `ret` matches the signature.
// Was: _unattributed::vfunc_27 (rebuild/src/compiled/00/41/unattributed_vfunc27_0041db64.cpp).
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::AddUIActionParam(CActionParam* param)
{
    UIActionParams.push_back(param);
}
}
