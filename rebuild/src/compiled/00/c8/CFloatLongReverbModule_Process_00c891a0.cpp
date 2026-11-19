// CFloatLongReverbModule::Process(const CFloatLongReverbModuleParams* pParams, const CFloatCyclicBuffer* pInputBuffer, float* pfCrossfeed, float* pfOutput, unsigned int uiSamples) -- one long-reverb module block: sum its four input taps of the shared input buffer, copy them scaled by the crossfeed gain out, scale them by the input gain, add the module's own buffer read back by its feedback delay, write the result through the feedback filter at the write cursor (tail and wrapped head run), mix it into the output with the output gain and advance the cursor
// Leaf: ?Process@CFloatLongReverbModule@@QAEXPBUCFloatLongReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C891A0, 428 bytes, thiscall `ret 14h`, ebp frame with `and esp,-16` (128-float block).
// Name (NF102, q11558; was the descriptive CReverbDiffuserStage::Process over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatLongReverbModule@@QAEXPBUCFloatLongReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z. Two sources: (1) this body
//   is byte-identical (428 bytes, relocations masked) to that CODAApi.lib member; (2) its relocations name the SDK callees at the
//   retail call targets (CReverbFns::CopySamples 0x00C763E0, AddSamples 0x00C76680, CFloatFilter::Process 0x00C74CE0) and the landed
//   callers CFloatLongReverb::Process 0x00C75A20 / 0x00C75AB0 (SDK-identical) call it. Layout: the CodeView CFloatLongReverbModule
//   (engine/CFloatLongReverbModule.h: m_Buffer +4, m_Filter +0x10, m_iMyFaderToken +0x24) and CFloatLongReverbModuleParams.
// Shape: the landed body unchanged: the module buffer's cursor update is the SDK inline CFloatCyclicBuffer::AddWriteCursor (VC7.1
//   reuses `&m_Buffer` formed for AddSamples and keeps `this` in a frame slot, so the tap loop index gets ebx as in retail); the
//   input-buffer read positions are written out (`pos >= n ? pos - n : length - n + pos`).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatLongReverbModuleParams.h"
#include "engine/CFloatLongReverbModule.h"

void CFloatLongReverbModule::Process(const CFloatLongReverbModuleParams* pParams, const CFloatCyclicBuffer* pInputBuffer,
    float* pfCrossfeed, float* pfOutput, unsigned int uiSamples)
{
    __declspec(align(16)) float afBlock[128];
    unsigned int uiStart = pInputBuffer->GetWriteCursor() >= uiSamples ? pInputBuffer->GetWriteCursor() - uiSamples
                                                                       : pInputBuffer->GetLength() - uiSamples + pInputBuffer->GetWriteCursor();
    float* pfData = m_Buffer.m_pBase;
    unsigned int uiCursor = m_Buffer.GetWriteCursor();
    unsigned int uiTap = pParams->m_auiInputDelay[0];
    m_pDSP->CopySamples(afBlock, pInputBuffer, uiStart >= uiTap ? uiStart - uiTap : pInputBuffer->GetLength() - uiTap + uiStart, uiSamples);
    for (unsigned int i = 1; i < 4; ++i)
    {
        uiTap = pParams->m_auiInputDelay[i];
        m_pDSP->AddSamples(afBlock, pInputBuffer, uiStart >= uiTap ? uiStart - uiTap : pInputBuffer->GetLength() - uiTap + uiStart, uiSamples);
    }
    m_pDSP->ScaleSamples(pfCrossfeed, afBlock, pParams->m_fCrossfeedGain, uiSamples);
    m_pDSP->ScaleSamples(afBlock, pParams->m_fInputGain, uiSamples);
    unsigned int uiDelay = pParams->m_uiFeedbackDelay;
    m_pDSP->AddSamples(afBlock, &m_Buffer, uiCursor >= uiDelay ? uiCursor - uiDelay : m_Buffer.GetLength() - uiDelay + uiCursor, uiSamples);
    unsigned int uiFirst = uiSamples;
    if (uiCursor + uiSamples > m_Buffer.GetLength())
        uiFirst = m_Buffer.GetLength() - uiCursor;
    m_Filter.Process(&pParams->m_fFeedbackFilter, afBlock, pfData + uiCursor, true, uiFirst);
    if (uiFirst < uiSamples)
        m_Filter.Process(&pParams->m_fFeedbackFilter, afBlock + uiFirst, pfData, true, uiSamples - uiFirst);
    m_pDSP->ScaleAddSamples(pfOutput, afBlock, pParams->m_fOutputGain, uiSamples);
    m_Buffer.AddWriteCursor(uiSamples);
}
