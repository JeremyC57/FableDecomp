// CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float, void*) -- fade the screen filter with this handle out over the given time, via the priority stack
// Leaf: ?ScreenFilterFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B83F0, 24 bytes: loads (time, handle), pushes them as (handle, time), `add ecx,90h`
//   (ScreenEffectPriorityStack), `call 0x008818A0`, `ret 8`.
// Identity (two sources): Ego_r.pdb 0x006F4390 / FableWin.pdb 0x0246D190 ?ScreenFilterFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z;
//   the PDB-named forwarder CGameScriptInterface::ScreenFilterFadeOut 0x00890200 loads the environment's +0x1964 interpolator and jumps
//   here with its own (float, void*) arguments, and CTCBulletTime::Pause 0x0079CF30 binds this spelling. The catalog's
//   `_Cons_val<..CAnimationEntry..>` was a propagated label (the old row was a free-form `Alloc::ConsVal` stand-in).
// Callees: ?DeleteScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXPAXM@Z 0x008818A0 (Ego_r 0x008C2430, the third of
//   the DeleteRadialBlurEntry 0x00881820 / DeleteDisplacementEntry 0x00881860 run; argument order (handle, time) as pushed).
// Header: engine/CEnvironmentSpellEffectInterpolator.h (FADEOUT), engine/NScreenEffect_CScreenEffectPriorityStack.h (DELETESCREENFILTERENTRY).
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADEOUT
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_DELETESCREENFILTERENTRY
#include "engine/CEnvironmentSpellEffectInterpolator.h"
void CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float time, void* handle)
{
    ScreenEffectPriorityStack.DeleteScreenFilterEntry(handle, time);
}
