// CFloatEcho::Process(const CFloatEchoParams* pParams, const float* pfInput, float* pfOutput, unsigned int uiSamples) -- one echo block: read the echo buffer back by the feedback delay scaled by the feedback gain, write the input block at the write cursor, add the scaled echo on top, mix the echo into the output with the depth (when non-zero) and advance the cursor
// Leaf: ?Process@CFloatEcho@@QAEXPBUCFloatEchoParams@@PBMPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76B60, 183 bytes, thiscall `ret 10h`, ebp frame with `and esp,-16` (128-float block).
// Name (NF105, q11557; was the descriptive CReverbDelay::Process over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatEcho@@QAEXPBUCFloatEchoParams@@PBMPAMI@Z. Two sources: (1) this body is byte-identical (183 bytes, relocations
//   masked) to that CODAApi.lib FloatEcho.obj member, whose relocations name the retail callees (+0x41 CReverbFns::ScaleSamples
//   0x00C764E0, +0x53 CopySamples 0x00C76460, +0x66 AddSamples 0x00C766F0, m_pDSP 0x0143CCDC); (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessShortReverbInputBuffer 0x00C6A080 calls it at REL32 +0xaf, the retail call site. Layout: the CodeView
//   CFloatEcho (m_buffer +4, m_iMyFaderToken +0x10) and CFloatEchoParams (m_uiFeedbackDelay, m_fFeedbackGain, m_fDepth).
// Shape: the 128-float scratch block is a 16-byte-aligned local (`and esp,-16 ; sub esp,204h`); the read position is an if/else on
//   `cursor >= delay`; the cursor advance is the SDK inline CFloatCyclicBuffer::AddWriteCursor; virtual slot 7 ScaleAddSamples mixes.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATECHO_PROCESS
#include "engine/CFloatEchoParams.h"
#include "engine/CFloatEcho.h"

void CFloatEcho::Process(const CFloatEchoParams* pParams, const float* pfInput, float* pfOutput, unsigned int uiSamples)
{
    __declspec(align(16)) float afEcho[128];
    unsigned int uiCursor = m_buffer.GetWriteCursor();
    unsigned int uiRead;
    if (uiCursor >= pParams->m_uiFeedbackDelay)
        uiRead = uiCursor - pParams->m_uiFeedbackDelay;
    else
        uiRead = m_buffer.GetLength() - pParams->m_uiFeedbackDelay + uiCursor;
    m_pDSP->ScaleSamples(afEcho, &m_buffer, uiRead, pParams->m_fFeedbackGain, uiSamples);
    m_pDSP->CopySamples(&m_buffer, uiCursor, pfInput, uiSamples);
    m_pDSP->AddSamples(&m_buffer, uiCursor, afEcho, uiSamples);
    if (pParams->m_fDepth != 0.0f)
        m_pDSP->ScaleAddSamples(pfOutput, afEcho, pParams->m_fDepth, uiSamples);
    m_buffer.AddWriteCursor(uiSamples);
}
