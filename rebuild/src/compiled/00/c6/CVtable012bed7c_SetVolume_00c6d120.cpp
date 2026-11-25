// CSalRenderPanDelay::SetGain(float fGain, const float* pfGainPan, bool bImmediate) -- vtable slot 3 of the GameCODA HRTF-delay panner:
//   rebuild the filter parameters only when the gain changes
// Leaf: ?SetGain@CSalRenderPanDelay@@UAEXMPBM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D120, thiscall `ret 0Ch`; slot 3 of the vtables 0x012BED7C, 0x012B8A64 and 0x012B8AA0 (the SIMD
//   classes inherit it).
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelay::SetGain(float, const float*, bool), slot
//   3 of CSalRender (engine/CSalRender.h), and the SDK Release ?SetGain@CSalRenderPanDelay is 12 instructions, this body's count (NF441
//   CLASS_TABLE); retail pops 12 argument bytes = that signature (NF445 sigcheck_19.tsv). pfGainPan is not read (as in the SDK body). Was
//   ?SetVolume@CVtable_012bed7c@@UAEXMK_N@Z (descriptive stand-in).
// Callee: the private CSalRenderPanDelay::UpdateFilters(const bool) 0x00C6CF40 (the SDK Release SetGain relocates to it at +0x1D; retail
//   `call 0x00C6CF40` at 0x00C6D13C; NF447 ports it in the same batch).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C) : CSalRender (m_fGain +0x20)

void CSalRenderPanDelay::SetGain(float fGain, const float*, bool bImmediate)
{
    if (m_fGain != fGain) {
        m_fGain = fGain;
        UpdateFilters(bImmediate);
    }
}
