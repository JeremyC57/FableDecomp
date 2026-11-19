// CFloatReflection::Process(const CFloatReflectionParams* pParams, CFloatCyclicBuffer* pInputBuffer, float* pfReverbInput, float* pfOutput, unsigned int uiSamples) -- one early-reflection block: add the first input tap of the shared reflections input buffer into the reverb input, read the reflection's own buffer back by its feedback delay scaled by the feedback gain, add both input taps, write the block back at the write cursor, mix it into the output with the output gain, and advance the cursor with wrap
// Leaf: ?Process@CFloatReflection@@QAEXPBUCFloatReflectionParams@@PAVCFloatCyclicBuffer@@PAM2I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75FF0, 303 bytes, thiscall `ret 14h`, ebp frame with `and esp,-16` for the
//   128-float block (`__declspec(align(16))`).
// Name (NF102, q11550; was the descriptive CCombFilter::Process over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatReflection@@QAEXPBUCFloatReflectionParams@@PAVCFloatCyclicBuffer@@PAM2I@Z. Two sources: (1) this body is
//   byte-identical (303 bytes, relocations masked) to that CODAApi.lib FloatReflection.obj member; (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessReflections 0x00C69FB0 calls it at REL32 +0x9c, the retail call site of 0x00C75FF0. Layout: the
//   CodeView CFloatReflection (engine/CFloatReflection.h: m_Buffer +4, m_iMyFaderToken +0x10) and CFloatReflectionParams.
// Callees (all on CReverbDspProcessor::m_pDSP 0x0143CCDC): CReverbFns::AddSamples 0x00C76680 (x3), ScaleSamples 0x00C764E0,
//   CopySamples 0x00C76460 (buffer form), virtual slot 7 ScaleAddSamples (+0x1C).
// Shape: the landed body unchanged; ReadPosition is this file's descriptive inline helper (no retail symbol) for
//   `cursor - delay` wrapped into the buffer.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATREFLECTION_PROCESS
#include "engine/CFloatReflectionParams.h"
#include "engine/CFloatReflection.h"

inline unsigned int ReadPosition(const CFloatCyclicBuffer* pBuffer, unsigned int uiCursor, unsigned int uiDelay)
{
    return uiCursor >= uiDelay ? uiCursor - uiDelay : pBuffer->GetLength() - uiDelay + uiCursor;
}

void CFloatReflection::Process(const CFloatReflectionParams* pParams, CFloatCyclicBuffer* pInputBuffer, float* pfReverbInput,
    float* pfOutput, unsigned int uiSamples)
{
    __declspec(align(16)) float afBlock[128];
    unsigned int uiCursor = m_Buffer.GetWriteCursor();
    unsigned int uiRead = ReadPosition(&m_Buffer, uiCursor, pParams->m_uiFeedbackDelay);
    unsigned int uiStart = ReadPosition(pInputBuffer, pInputBuffer->GetWriteCursor(), uiSamples);
    m_pDSP->AddSamples(pfReverbInput, pInputBuffer, ReadPosition(pInputBuffer, uiStart, pParams->m_auiInputDelay[0]), uiSamples);
    m_pDSP->ScaleSamples(afBlock, &m_Buffer, uiRead, pParams->m_fFeedbackGain, uiSamples);
    for (unsigned int i = 0; i < 2; ++i)
        m_pDSP->AddSamples(afBlock, pInputBuffer, ReadPosition(pInputBuffer, uiStart, pParams->m_auiInputDelay[i]), uiSamples);
    m_pDSP->CopySamples(&m_Buffer, uiCursor, afBlock, uiSamples);
    m_pDSP->ScaleAddSamples(pfOutput, afBlock, pParams->m_fOutputGain, uiSamples);
    m_Buffer.SetWriteCursor(uiCursor + uiSamples < m_Buffer.GetLength() ? uiCursor + uiSamples
                                                                        : uiCursor - m_Buffer.GetLength() + uiSamples);
}
