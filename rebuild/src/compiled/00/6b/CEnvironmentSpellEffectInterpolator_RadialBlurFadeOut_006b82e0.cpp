// CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut(float time, void* handle) -- fade the radial blur entry out of the screen
// effect priority stack (+0x90) over `time`
// Leaf: ?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B82E0: `add ecx,90h` and the (handle, time) pair handed to
//   NScreenEffect::CScreenEffectPriorityStack::DeleteRadialBlurEntry 0x00881820 (`ret 8`).
// Name (NF197, 2026-11-20): Ego_r.pdb 0x006F4280 + FableWin.pdb 0x0246CEA0 ?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z;
//   the callee is Ego_r.pdb 0x008C2320 + FableWin.pdb 0x02A58AB0 ?DeleteRadialBlurEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXPAXM@Z;
//   0x007A7D20 calls it right after RadialBlurFadeTo 0x006B8730 / RadialBlurSetCenterWorldPos 0x006B8310 on the same interpolator.
//   Was the propagated label std::_Cons_val<...pair<unsigned long,CAnimationEntry>...> over a free stand-in calling Sub::M; the
//   shape is the landed ScreenFilterFadeOut 0x006B83F0 (DeleteScreenFilterEntry).
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_DELETERADIALBLURENTRY
#include "engine/CEnvironmentSpellEffectInterpolator.h"
void CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut(float time, void* handle)
{
    ScreenEffectPriorityStack.DeleteRadialBlurEntry(handle, time);
}
