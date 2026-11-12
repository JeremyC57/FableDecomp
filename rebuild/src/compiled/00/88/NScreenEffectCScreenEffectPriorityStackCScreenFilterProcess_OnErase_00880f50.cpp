// NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess::OnErase(NScreenEffect::CScreenEffectPriorityStack::CStackEntry*) -- erasing a screen-filter entry resets it (CScreenFilterEntry::Reset)
// Leaf: ?OnErase@CScreenFilterProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXPAUCStackEntry@23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00880F50, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 2 of NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess's vtable 0x01278028 (the slot holds this body in 1 vtable(s), owner NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts OnErase(NScreenEffect::CScreenEffectPriorityStack::CStackEntry *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnErase@CScreenFilterProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXPAUCStackEntry@23@@Z; `ret` matches the signature.
// Was: _unattributed::vfunc_2 (rebuild/src/compiled/00/88/unattributed_vfunc2_00880f50.cpp).
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
namespace NScreenEffect {
void CScreenEffectPriorityStack::CScreenFilterProcess::OnErase(CStackEntry* entry)
{
    static_cast<CScreenFilterEntry*>(entry)->Reset();
}
}
