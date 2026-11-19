// CFloatShortReverbModule::ProcessCrossfeed(const float* pfInputs, const float* pfFeedback, const float* pfGains, unsigned int uiSamples) -- write one crossfed block into the module's cyclic buffer: each sample is the feedback sample plus the four modules' blocks weighted by this module's crossfeed row, stored at the write cursor with wrap; then store the cursor
// Leaf: ?ProcessCrossfeed@CFloatShortReverbModule@@QAEXPBM00I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89AA0, 147 bytes, thiscall `ret 10h`, no calls.
// Name (NF105, q11515; was the descriptive CReverbFilterTap::Feedback over a local model): GameCODA 2.10 CodeView public
//   ?ProcessCrossfeed@CFloatShortReverbModule@@QAEXPBM00I@Z. Two sources: (1) this body is byte-identical (147 bytes) to that
//   CODAApi.lib FloatShortReverbModule.obj member; (2) the byte-identical SDK CFloatShortReverb::Process 0x00C75CE0 (REL32 +0x192) and
//   its fading overload 0x00C75E90 (+0xc7) call it at the retail call sites with the four 128-float blocks, the feedback block and a
//   row of sc_fDefaultShortReverbCrossfeedGainMatrix. Layout: m_Buffer +0x18 (m_pBase, length +0x1c, cursor +0x20).
// Shape: the landed body with the CodeView names: the four-block sum is an inner `for (j < 4)` accumulating onto the feedback
//   sample (VC7.1 fully unrolls it in source order and leaves the outer loop rolled); the blocks are 128 floats apart; the wrap is
//   `cursor + 1 < length ? cursor + 1 : cursor + 1 - length` with the length re-read every sample; the Dbg build ends with
//   CFloatCyclicBuffer::SetWriteCursor.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include "engine/CFloatShortReverbModule.h"

void CFloatShortReverbModule::ProcessCrossfeed(const float* pfInputs, const float* pfFeedback, const float* pfGains, unsigned int uiSamples)
{
    float* pfData = m_Buffer.m_pBase;
    unsigned int uiCursor = m_Buffer.GetWriteCursor();
    for (unsigned int i = 0; i < uiSamples; i++) {
        float fSum = pfFeedback[i];
        for (unsigned int j = 0; j < 4; j++)
            fSum += pfInputs[j * 128 + i] * pfGains[j];
        pfData[uiCursor] = fSum;
        if (uiCursor + 1 < m_Buffer.GetLength())
            uiCursor = uiCursor + 1;
        else
            uiCursor = uiCursor + 1 - m_Buffer.GetLength();
    }
    m_Buffer.SetWriteCursor(uiCursor);
}
