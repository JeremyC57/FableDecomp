// CFloatLongReverbModule::Process(CFloatLongReverbModuleParams* pParams, const CFloatLongReverbModuleParams* pNewParams, const CFloatCyclicBuffer* pInputBuffer, float* pfCrossfeed, float* pfOutput, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of Process while a module parameter change is pending: sum the four input taps of the old record fading out (uiSamplesToFade/512 down by 1/512 a sample) and of the new record fading in, copy it to the crossfeed output and scale it with gains ramping from old to new, add the old and new feedback delays of the module's own buffer the same way, mix it into the output, cross-fade the feedback filter (split at the buffer's end), advance the write cursor and adopt the new record once the fade ends in this block
// Leaf: ?Process@CFloatLongReverbModule@@QAEXPAUCFloatLongReverbModuleParams@@PBU2@PBVCFloatCyclicBuffer@@PAM3II@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89350, 850 bytes (padded to 880), thiscall `ret 1Ch`, ebp frame with `and esp,-16` (the
//   aligned float[128] scratch block).
// Name (NF105, family of q11492; was the descriptive CReverbDiffuserStage::ProcessRamped over a local model): GameCODA 2.10 CodeView
//   public ?Process@CFloatLongReverbModule@@QAEXPAUCFloatLongReverbModuleParams@@PBU2@PBVCFloatCyclicBuffer@@PAM3II@Z (the fading
//   overload). Two sources: (1) this body is byte-identical (850 bytes, relocations masked) to that CODAApi.lib FloatLongReverbModule.obj
//   member; (2) the byte-identical SDK CFloatLongReverb::Process fading overload 0x00C75AB0 calls it at REL32 +0xbd, the retail call
//   site of 0x00C89350. Layout: the CodeView CFloatLongReverbModule (engine/CFloatLongReverbModule.h: m_Buffer +4, m_Filter +0x10) and
//   CFloatLongReverbModuleParams (m_auiInputDelay +0, m_fInputGain +0x10, m_fCrossfeedGain +0x14, m_fFeedbackFilter +0x18,
//   m_uiFeedbackDelay +0x34, m_fOutputGain +0x38).
// Callees (on CReverbDspProcessor::m_pDSP 0x0143CCDC): CReverbFns::ScaleSamples 0x00C76560 and ScaleAddSamples 0x00C767E0 (x9, the
//   cyclic-buffer fading forms), virtual slots 1 ScaleSamples (x2, fading copy) and 6 ScaleAddSamples (fading); CFloatFilter::Process
//   0x00C74D30 (the fading overload). Constants: 1/512 0x012A3AF4, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (unsigned fix-up); the +-1/512
//   steps are pushed as immediates (0x3B000000 / 0xBB000000).
// Shape: the landed body with the CodeView names: the fade fraction converts through its own float (`float fFade =
//   (float)uiSamplesToFade;` then `* (1/512)`: VC7.1 then schedules the fmul after the first tap compare as retail); the other weight
//   is a stored `1 - fOld`; the input-gain step is a named local; the tap loop reads both records through one pointer plus their
//   distance (VC7.1 strength reduction).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS_FADING
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatLongReverbModuleParams.h"
#include "engine/CFloatLongReverbModule.h"

void CFloatLongReverbModule::Process(CFloatLongReverbModuleParams* pParams, const CFloatLongReverbModuleParams* pNewParams,
    const CFloatCyclicBuffer* pInputBuffer, float* pfCrossfeed, float* pfOutput, unsigned int uiSamples,
    unsigned int uiSamplesToFade)
{
    __declspec(align(16)) float afBlock[128];
    unsigned int uiStart = pInputBuffer->GetWriteCursor() >= uiSamples ? pInputBuffer->GetWriteCursor() - uiSamples
                                                                       : pInputBuffer->GetLength() - uiSamples + pInputBuffer->GetWriteCursor();
    float* pfData = m_Buffer.m_pBase;
    unsigned int uiCursor = m_Buffer.GetWriteCursor();
    float fFade = (float)uiSamplesToFade;
    float fOld = fFade * (1.0f / 512.0f);
    float fNew = 1.0f - fOld;
    unsigned int uiTap = pParams->m_auiInputDelay[0];
    m_pDSP->ScaleSamples(afBlock, pInputBuffer, uiStart >= uiTap ? uiStart - uiTap : pInputBuffer->GetLength() - uiTap + uiStart,
                         fOld, -1.0f / 512.0f, uiSamples);
    uiTap = pNewParams->m_auiInputDelay[0];
    m_pDSP->ScaleAddSamples(afBlock, pInputBuffer, uiStart >= uiTap ? uiStart - uiTap : pInputBuffer->GetLength() - uiTap + uiStart,
                            fNew, 1.0f / 512.0f, uiSamples);
    for (unsigned int i = 1; i < 4; ++i) {
        uiTap = pParams->m_auiInputDelay[i];
        m_pDSP->ScaleAddSamples(afBlock, pInputBuffer, uiStart >= uiTap ? uiStart - uiTap : pInputBuffer->GetLength() - uiTap + uiStart,
                                fOld, -1.0f / 512.0f, uiSamples);
        uiTap = pNewParams->m_auiInputDelay[i];
        m_pDSP->ScaleAddSamples(afBlock, pInputBuffer, uiStart >= uiTap ? uiStart - uiTap : pInputBuffer->GetLength() - uiTap + uiStart,
                                fNew, 1.0f / 512.0f, uiSamples);
    }
    m_pDSP->ScaleSamples(pfCrossfeed, afBlock, fOld * pParams->m_fCrossfeedGain + fNew * pNewParams->m_fCrossfeedGain,
                         (pNewParams->m_fCrossfeedGain - pParams->m_fCrossfeedGain) * (1.0f / 512.0f), uiSamples);
    float fInputGainStep = (pNewParams->m_fInputGain - pParams->m_fInputGain) * (1.0f / 512.0f);
    m_pDSP->ScaleSamples(afBlock, afBlock, fOld * pParams->m_fInputGain + fNew * pNewParams->m_fInputGain, fInputGainStep, uiSamples);
    unsigned int uiDelay = pParams->m_uiFeedbackDelay;
    m_pDSP->ScaleAddSamples(afBlock, &m_Buffer, uiCursor >= uiDelay ? uiCursor - uiDelay : m_Buffer.GetLength() - uiDelay + uiCursor,
                            fOld, -1.0f / 512.0f, uiSamples);
    uiDelay = pNewParams->m_uiFeedbackDelay;
    m_pDSP->ScaleAddSamples(afBlock, &m_Buffer, uiCursor >= uiDelay ? uiCursor - uiDelay : m_Buffer.GetLength() - uiDelay + uiCursor,
                            fNew, 1.0f / 512.0f, uiSamples);
    m_pDSP->ScaleAddSamples(pfOutput, afBlock, fOld * pParams->m_fOutputGain + fNew * pNewParams->m_fOutputGain,
                            (pNewParams->m_fOutputGain - pParams->m_fOutputGain) * (1.0f / 512.0f), uiSamples);
    unsigned int uiFirst = uiSamples;
    if (uiCursor + uiSamples > m_Buffer.GetLength())
        uiFirst = m_Buffer.GetLength() - uiCursor;
    m_Filter.Process(&pParams->m_fFeedbackFilter, &pNewParams->m_fFeedbackFilter, afBlock, pfData + uiCursor, true, uiFirst, uiSamplesToFade);
    if (uiFirst < uiSamples)
        m_Filter.Process(&pParams->m_fFeedbackFilter, &pNewParams->m_fFeedbackFilter, afBlock + uiFirst, pfData, true,
                         uiSamples - uiFirst, uiSamplesToFade);
    m_Buffer.AddWriteCursor(uiSamples);
    if (uiSamples >= uiSamplesToFade)
        *pParams = *pNewParams;
}
