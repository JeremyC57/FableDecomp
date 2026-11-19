// CFloatLongReverb::ProcessInputFilter(unsigned int uiIndex, CFloatLongReverbParams* pParams, const CFloatLongReverbParams* pNewParams, CFloatCyclicBuffer* pBuffer, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of the long reverb's input-filter stage `uiIndex` while a parameter change is pending: read the shared buffer back by the old filter delay fading out and by the new one fading in, run the block through the input filter fading old -> new, write it back into the buffer at this stage's offset (index-scaled share of the long-reverb input length past the short-reverb input), and adopt the new delay once the fade ends in this block
// Leaf: ?ProcessInputFilter@CFloatLongReverb@@AAEXIPAUCFloatLongReverbParams@@PBU2@PAVCFloatCyclicBuffer@@II@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75820, 343 bytes, thiscall `ret 18h`, ebp frame with `and esp,-16` (128-float block).
// Name (NF105, family of q11557; was the descriptive CReverbDiffuser::UpdateGainRamped over a local model): GameCODA 2.10 CodeView
//   private ?ProcessInputFilter@CFloatLongReverb@@AAEXIPAUCFloatLongReverbParams@@PBU2@PAVCFloatCyclicBuffer@@II@Z (the fading
//   overload). Two sources: (1) this body is byte-identical (343 bytes, relocations masked) to that CODAApi.lib FloatLongReverb.obj
//   member, whose relocations name the retail callees (+0x88 CReverbFns::ScaleSamples 0x00C76560, +0xc2 ScaleAddSamples 0x00C767E0,
//   +0xf6 CFloatFilter::Process 0x00C74D30, +0x139 CopySamples 0x00C76460, m_pDSP 0x0143CCDC); (2) the byte-identical SDK
//   CFloatLongReverb::Process fading overload 0x00C75AB0 calls it at REL32 +0x38, the retail call site. Layout: the CodeView
//   CFloatLongReverb (m_filter[3] +4, m_pRvbConstants +0x90), CFloatLongReverbParams (m_uiInputFilterDelay[3] +0, m_aInputFilter[3]
//   +0xc) and CReverbConstants (m_uiMaxShortReverbInputSamples +0x10, m_uiMaxLongReverbInputSamples +0x14).
// Shape: the landed body with the CodeView names: the unsigned-to-float conversion is a named local (the inline
//   `(float)uiSamplesToFade * k` schedules the fmul straight after the 2^32 correction; retail places it after the first compare).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER_FADING
#include "engine/CReverbConstants.h"
#include "engine/CFloatLongReverbParams.h"
#include "engine/CFloatLongReverb.h"

void CFloatLongReverb::ProcessInputFilter(unsigned int uiIndex, CFloatLongReverbParams* pParams, const CFloatLongReverbParams* pNewParams,
    CFloatCyclicBuffer* pBuffer, unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    __declspec(align(16)) float afBlock[128];
    unsigned int uiStart = pBuffer->GetWriteCursor() >= uiSamples ? pBuffer->GetWriteCursor() - uiSamples
                                                                  : pBuffer->GetLength() - uiSamples + pBuffer->GetWriteCursor();
    float fFade = (float)uiSamplesToFade;
    float fOld = fFade * (1.0f / 512.0f);
    float fNew = 1.0f - fOld;
    unsigned int uiDelay = pParams->m_uiInputFilterDelay[uiIndex];
    m_pDSP->ScaleSamples(afBlock, pBuffer, uiStart >= uiDelay ? uiStart - uiDelay : pBuffer->GetLength() - uiDelay + uiStart, fOld,
                         -1.0f / 512.0f, uiSamples);
    uiDelay = pNewParams->m_uiInputFilterDelay[uiIndex];
    m_pDSP->ScaleAddSamples(afBlock, pBuffer, uiStart >= uiDelay ? uiStart - uiDelay : pBuffer->GetLength() - uiDelay + uiStart, fNew,
                            1.0f / 512.0f, uiSamples);
    m_filter[uiIndex].Process(&pParams->m_aInputFilter[uiIndex], &pNewParams->m_aInputFilter[uiIndex], afBlock, afBlock, true, uiSamples,
                              uiSamplesToFade);
    unsigned int uiOffset = (uiIndex + 1) * m_pRvbConstants->m_uiMaxLongReverbInputSamples / 4 + m_pRvbConstants->m_uiMaxShortReverbInputSamples;
    m_pDSP->CopySamples(pBuffer, uiStart >= uiOffset ? uiStart - uiOffset : pBuffer->GetLength() - uiOffset + uiStart, afBlock, uiSamples);
    if (uiSamples >= uiSamplesToFade)
        pParams->m_uiInputFilterDelay[uiIndex] = pNewParams->m_uiInputFilterDelay[uiIndex];
}
