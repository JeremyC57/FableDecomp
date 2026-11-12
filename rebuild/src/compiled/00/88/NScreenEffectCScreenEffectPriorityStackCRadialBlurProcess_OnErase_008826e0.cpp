// NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess::OnErase(NScreenEffect::CScreenEffectPriorityStack::CStackEntry*) -- erasing a radial-blur entry needs no clean-up
// Leaf: ?OnErase@CRadialBlurProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXPAUCStackEntry@23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008826E0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 2 of NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess's vtable 0x0127803C (the slot holds this body in 1 vtable(s), owner NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts OnErase(NScreenEffect::CScreenEffectPriorityStack::CStackEntry *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnErase@CRadialBlurProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXPAUCStackEntry@23@@Z; `ret` matches the signature.
// Was: NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess::vfunc_2 (rebuild/src/compiled/00/88/gapscan_sub008826e0_008826e0.cpp).
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
namespace NScreenEffect {
void CScreenEffectPriorityStack::CRadialBlurProcess::OnErase(CStackEntry*)
{
}
}
