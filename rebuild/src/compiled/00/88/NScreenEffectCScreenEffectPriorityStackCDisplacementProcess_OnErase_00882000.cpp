// NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess::OnErase(NScreenEffect::CScreenEffectPriorityStack::CStackEntry*) -- erasing a displacement entry needs no clean-up
// Leaf: ?OnErase@CDisplacementProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXPAUCStackEntry@23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00882000, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 2 of NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess's vtable 0x01278014 (the slot holds this body in 1 vtable(s), owner NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts OnErase(NScreenEffect::CScreenEffectPriorityStack::CStackEntry *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnErase@CDisplacementProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXPAUCStackEntry@23@@Z; `ret` matches the signature.
// Was: NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess::vfunc_2 (rebuild/src/compiled/00/88/gapscan_sub00882000_00882000.cpp).
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
namespace NScreenEffect {
void CScreenEffectPriorityStack::CDisplacementProcess::OnErase(CStackEntry*)
{
}
}
