// CFloatFilter::Process(CFloatFilterParams* pParams, const CFloatFilterParams* pNewParams, const float* pfSrc, float* pfDest, bool bInputValid, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of CFloatFilter::Process while a parameter change is pending: run the block through the new settings into an aligned scratch block (second history pair) and through the old settings into the output (first pair), fade the old gain out and the new gain in (gain/512 per sample from the remaining fraction), add the scratch block in, and adopt the new settings and history once the fade ends in this block
// Leaf: ?Process@CFloatFilter@@QAEXPAVCFloatFilterParams@@PBV2@PBMPAM_NII@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C74D30, 297 bytes, thiscall `ret 1Ch`, `and esp,-16` frame (the 128-float scratch block is
//   16-byte aligned for the SIMD CReverbFns).
// Name (NF109, q11601; was the descriptive CDspGain::Crossfade over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatFilter@@QAEXPAVCFloatFilterParams@@PBV2@PBMPAM_NII@Z (the fading overload). Two sources: (1) this body is
//   byte-identical (297 bytes, relocations masked) to that CODAApi.lib FloatFilter.obj release member (work/tools/sdkcmpx.py), whose
//   relocations name ApplyFilter 0x00C74510 (twice) and m_pDSP 0x0143CCDC; (2) the byte-identical SDK callers name it at the retail call
//   sites: CFloatReverbDspEngine::ProcessReflectionsInputBuffer 0x00C69C60, the fading CFloatLongReverb::ProcessInputFilter 0x00C75820
//   (+0xf6) and the fading CFloatLongReverbModule::Process 0x00C89350.
// Layout: the CodeView CFloatFilter (m_fHistoryData[4] +4: the old settings' pair +4, the new settings' pair +0xc) and
//   CFloatFilterParams (m_fGain +0, m_fA[3] +4, m_fB[3] +0x10; copied whole, 7 dwords, when the fade ends).
// Callees (CodeView order of the CODAApiDbg.lib body): BeginFading when uiSamplesToFade == 512, ApplyFilter (new, then old),
//   CReverbFns::ScaleSamples(float*, float, float, unsigned int) twice, AddSamples(float*, const float*, unsigned int), then
//   `*pParams = *pNewParams` and FloatCopySamples(m_fHistoryData, m_fHistoryData + 2, 2).
// Shape: the landed body with the CodeView names (grades: 3; a plain memcpy of the two floats loads both before storing). The release expands the private BeginFading (its out-of-line copy is the uncatalogued
//   0x00C74500: two 1.0f stores to +0xc / +0x10, written out here) and FloatCopySamples (file-local inline helper:
//   through its pointer parameters the two-float memcpy stores the first float before loading the second). The fraction is (float)uiSamplesToFade * 1/512 (the Dbg body divides by 512.0f).
//   The CODAApiDbg.lib body reads the params' members directly (header opt-in FABLE_RETAIL_CFLOATFILTERPARAMS_FRIEND_CFLOATFILTER).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATFILTERPARAMS_FRIEND_CFLOATFILTER
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatFilter.h"
#include <string.h>
#pragma intrinsic(memcpy)

// GameCODA 2.10 FloatCopySamples: the CODAApiDbg.lib body calls the C helper _FloatCopySamples (asserts, then memcpy); no header carries
// it, so it is a file-local inline here (descriptive placement, SDK name).
static inline void FloatCopySamples(float* pfDest, const float* pfSrc, unsigned int uiSamples)
{
    memcpy(pfDest, pfSrc, uiSamples * sizeof(float));
}

void CFloatFilter::Process(CFloatFilterParams* pParams, const CFloatFilterParams* pNewParams, const float* pfSrc, float* pfDest,
    bool bInputValid, unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    __declspec(align(16)) float afBlock[128];
    if (uiSamplesToFade == 512) {
        m_fHistoryData[2] = 1.0f;
        m_fHistoryData[3] = 1.0f;
    }
    ApplyFilter(pNewParams->m_fA, pNewParams->m_fB, &m_fHistoryData[2], pfSrc, afBlock, bInputValid, uiSamples);
    ApplyFilter(pParams->m_fA, pParams->m_fB, m_fHistoryData, pfSrc, pfDest, bInputValid, uiSamples);
    float fOld = (float)uiSamplesToFade * (1.0f / 512.0f);
    m_pDSP->ScaleSamples(pfDest, fOld * pParams->m_fGain, pParams->m_fGain * (-1.0f / 512.0f), uiSamples);
    m_pDSP->ScaleSamples(afBlock, (1.0f - fOld) * pNewParams->m_fGain, pNewParams->m_fGain * (1.0f / 512.0f), uiSamples);
    m_pDSP->AddSamples(pfDest, afBlock, uiSamples);
    if (uiSamples >= uiSamplesToFade) {
        *pParams = *pNewParams;
        FloatCopySamples(m_fHistoryData, &m_fHistoryData[2], 2);
    }
}
