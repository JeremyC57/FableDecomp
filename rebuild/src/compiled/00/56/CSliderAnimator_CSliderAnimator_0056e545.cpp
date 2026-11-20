#pragma optimize("y", off)
#pragma optimize("s", on)
// CSliderAnimator::CSliderAnimator(NUISystem::CComponent* slider, float startValue, float endValue, float maxValue, float overall)
//   -- store the slider and the four values, zero the running value `a`, clear both flags
// Leaf: ??0CSliderAnimator@@QAE@PAVCComponent@NUISystem@@MMMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056E545, length 51 bytes.
// Callees: none. A frame-pointer body with `[ebp+8]` argument reads and no spills: the size-optimised TU with
// global optimisation and /Oy off (`optimize("gsy",off)` then `("s",on)`, see GOTCHAS). The float is stored
// first through `fldz` right after `mov eax,ecx`; the bytes are immediate stores.
// Identity: FableWin.pdb ??0CSliderAnimator@@QAE@PAVCComponent@NUISystem@@MMMM@Z; the landed callers 0x00577216 / 0x00577AC0
//   (CTCInventoryExperience, `CSliderAnimator general(ExpSliders[0], ...)`) already bind this spelling. Layout: engine/CSliderAnimator.h
//   (Ego_r.pdb, 28 bytes: pSlider +0, StartValue +4, EndValue +8, MaxValue +0xC, a +0x10, Overall +0x14, Play +0x18, Finished +0x19).
// NF113 (PRIORITY q4771): was the address-named stand-in CRecord_0056e545(long x5); now the shared header's constructor. The
//   initialiser list follows the member order (as the header comment already recorded for the stand-in).
#include "engine/CSliderAnimator.h"

CSliderAnimator::CSliderAnimator(NUISystem::CComponent* slider, float startValue, float endValue, float maxValue, float overall)
    : pSlider(slider), StartValue(startValue), EndValue(endValue), MaxValue(maxValue), a(0.0f), Overall(overall),
      Play(false), Finished(false)
{
}
