// CFloatFilter::Process(const CFloatFilterParams* pParams, const float* pfSrc, float* pfDest, bool bInputValid, unsigned int uiSamples) -- run one block through the filter's biquad (ApplyFilter on the params' A / B coefficients and this filter's history), then scale the block by the params' gain unless it is exactly 1.0
// Leaf: ?Process@CFloatFilter@@QAEXPBVCFloatFilterParams@@PBMPAM_NI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C74CE0, 73 bytes, thiscall `ret 14h`. Callees: CFloatFilter::ApplyFilter 0x00C74510 (private),
//   then CReverbFns slot ScaleSamples(float*, float, unsigned int) through CReverbDspProcessor::m_pDSP 0x0143CCDC.
// Name (NF109, q11600; was the descriptive CDspGain::Apply over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatFilter@@QAEXPBVCFloatFilterParams@@PBMPAM_NI@Z. Two sources: (1) this body is byte-identical (73 bytes,
//   relocations masked) to that CODAApi.lib FloatFilter.obj release member (work/tools/sdkcmpx.py), whose relocations name
//   ApplyFilter 0x00C74510 and m_pDSP 0x0143CCDC; (2) the byte-identical SDK callers name it at the retail call sites: the release
//   CFloatLongReverb::ProcessInputFilter 0x00C756C0 (+0x99 / +0x10d), CFloatReverbDspEngine::ProcessReflectionsInputBuffer 0x00C69C60,
//   CFloatShortReverbModule::ProcessInput 0x00C89750 and CFloatLongReverbModule::Process 0x00C891A0.
// Layout: the CodeView CFloatFilter (m_fHistoryData[4] +4) and CFloatFilterParams (m_fGain +0, m_fA[3] +4, m_fB[3] +0x10).
// Shape: the landed body with the CodeView names: `m_fGain != 1.0f` compiles to an integer compare with 0x3F800000 (the gain is
//   forwarded as a dword to ScaleSamples). The CODAApiDbg.lib body reads the params' members directly (header opt-in
//   FABLE_RETAIL_CFLOATFILTERPARAMS_FRIEND_CFLOATFILTER).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATFILTERPARAMS_FRIEND_CFLOATFILTER
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatFilter.h"

void CFloatFilter::Process(const CFloatFilterParams* pParams, const float* pfSrc, float* pfDest, bool bInputValid, unsigned int uiSamples)
{
    ApplyFilter(pParams->m_fA, pParams->m_fB, m_fHistoryData, pfSrc, pfDest, bInputValid, uiSamples);
    if (pParams->m_fGain != 1.0f)
        m_pDSP->ScaleSamples(pfDest, pParams->m_fGain, uiSamples);
}
