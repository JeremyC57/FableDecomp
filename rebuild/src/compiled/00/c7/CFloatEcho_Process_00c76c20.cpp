// CFloatEcho::Process(CFloatEchoParams* pParams, const CFloatEchoParams* pNewParams, const float* pfInput, float* pfOutput, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of Process while an echo parameter change is pending: read the buffer back by the old delay with the old feedback fading out and by the new delay with the new feedback fading in, write the input block at the write cursor, add the echo on top, mix it into the output with a depth ramping from old to new, advance the cursor and adopt the new record once the fade ends in this block
// Leaf: ?Process@CFloatEcho@@QAEXPAUCFloatEchoParams@@PBU2@PBMPAMII@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76C20, 381 bytes, thiscall `ret 18h`, ebp frame with `and esp,-16` (128-float block).
// Name (NF105, family of q11557; was the descriptive CReverbDelay::Blend over a local model): GameCODA 2.10 CodeView public fading
//   overload ?Process@CFloatEcho@@QAEXPAUCFloatEchoParams@@PBU2@PBMPAMII@Z. Two sources: (1) this body is byte-identical (381 bytes,
//   relocations masked) to that CODAApi.lib FloatEcho.obj member, whose relocations name the retail callees (CReverbFns::ScaleSamples
//   0x00C76560, ScaleAddSamples 0x00C767E0, CopySamples 0x00C76460, AddSamples 0x00C766F0, m_pDSP 0x0143CCDC); (2) the byte-identical
//   SDK CFloatReverbDspEngine::ProcessShortReverbInputBuffer 0x00C6A080 calls it at REL32 +0x9e, the retail call site. Layout: the
//   CodeView CFloatEcho (m_buffer +4) and CFloatEchoParams.
// Shape: the landed body with the CodeView names: the two read positions are if/else on `cursor >= delay`; the fade fraction is a
//   float local reused for the new record's (1 - fraction); virtual slot 6 ScaleAddSamples (fading) mixes.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATECHO_PROCESS
#include "engine/CFloatEchoParams.h"
#include "engine/CFloatEcho.h"

void CFloatEcho::Process(CFloatEchoParams* pParams, const CFloatEchoParams* pNewParams, const float* pfInput, float* pfOutput,
    unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    __declspec(align(16)) float afEcho[128];
    unsigned int uiCursor = m_buffer.GetWriteCursor();
    unsigned int uiRead;
    if (uiCursor >= pParams->m_uiFeedbackDelay)
        uiRead = uiCursor - pParams->m_uiFeedbackDelay;
    else
        uiRead = m_buffer.GetLength() - pParams->m_uiFeedbackDelay + uiCursor;
    unsigned int uiNewRead;
    if (uiCursor >= pNewParams->m_uiFeedbackDelay)
        uiNewRead = uiCursor - pNewParams->m_uiFeedbackDelay;
    else
        uiNewRead = m_buffer.GetLength() - pNewParams->m_uiFeedbackDelay + uiCursor;
    float fOld = (float)uiSamplesToFade * (1.0f / 512.0f);
    m_pDSP->ScaleSamples(afEcho, &m_buffer, uiRead, fOld * pParams->m_fFeedbackGain, pParams->m_fFeedbackGain * (-1.0f / 512.0f),
                         uiSamples);
    m_pDSP->ScaleAddSamples(afEcho, &m_buffer, uiNewRead, (1.0f - fOld) * pNewParams->m_fFeedbackGain,
                            pNewParams->m_fFeedbackGain * (1.0f / 512.0f), uiSamples);
    m_pDSP->CopySamples(&m_buffer, uiCursor, pfInput, uiSamples);
    m_pDSP->AddSamples(&m_buffer, uiCursor, afEcho, uiSamples);
    float fDepthStep = (pNewParams->m_fDepth - pParams->m_fDepth) * (1.0f / 512.0f);
    m_pDSP->ScaleAddSamples(pfOutput, afEcho, pParams->m_fDepth + (float)(512 - uiSamplesToFade) * fDepthStep, fDepthStep, uiSamples);
    m_buffer.AddWriteCursor(uiSamples);
    if (uiSamples >= uiSamplesToFade)
        *pParams = *pNewParams;
}
