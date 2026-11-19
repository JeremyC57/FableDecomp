// CFloatDiffuser::Process(float* pfBuffer, unsigned int uiSamples) -- run a float block in place through the diffuser's two chained first-order allpass sections (coefficient 0.7): each section reads its delayed sample, writes `delayed * 0.7 + input` back at the read position, advances its index with wrap, and passes `delayed - value * 0.7` on
// Leaf: ?Process@CFloatDiffuser@@QAEXPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75140, 700 bytes, thiscall `ret 8`. No callees. Constant 0.7f @ 0x01255318.
// Name (NF102, q11551; was the descriptive CStereoDelayLine::Process over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatDiffuser@@QAEXPAMI@Z. Two sources: (1) this body is byte-identical (700 bytes, relocations masked) to that
//   CODAApi.lib FloatDiffuser.obj member; (2) the byte-identical SDK CFloatReverbDspEngine::ProcessShortReverbInputBuffer 0x00C6A080
//   calls it at REL32 +0xc0 on its CFloatDiffuser member. Members: the CodeView layout on engine/CFloatDiffuser.h (m_allpass[2],
//   CFloatAllpassSection m_pBuffer / m_length / m_index); the per-sample step is the SDK CFloatAllpassSection::ProcessSample.
// Shape: VC7.1 /O2 unrolls the post-incremented pointer loop by four (count >= 4 block of (count-4)/4+1 passes, then the
//   remainder loop). The inline ProcessSample takes its input by value: the first section's delayed sample is copied through
//   an integer register into a frame slot and its new value kept with `fst`, because both feed the second section's inlined
//   body (`delayed - value * 0.7f` is evaluated there, after the second section's own delayed sample is loaded).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_PROCESSSAMPLE
#define FABLE_RETAIL_CFLOATDIFFUSER_PROCESS
#include "engine/CFloatDiffuser.h"

void CFloatDiffuser::Process(float* pfBuffer, unsigned int uiSamples)
{
    for (unsigned int i = 0; i < uiSamples; ++i) {
        *pfBuffer = m_allpass[1].ProcessSample(m_allpass[0].ProcessSample(*pfBuffer));
        pfBuffer++;
    }
}
