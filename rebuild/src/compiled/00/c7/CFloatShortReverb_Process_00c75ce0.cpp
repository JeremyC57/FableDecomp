// CFloatShortReverb::Process(CFloatShortReverbParams* pParams, const CFloatCyclicBuffer* pInputBuffer, float* pfFeedback, float** apfOutputs, unsigned int uiSamples) -- one block of the short reverb's four modules: on the SSE ops table each module reads its feedback data into a scratch block, one four-wide second-order filter (CReverbFnsSSE::ApplyFilterOrder2_quad) runs over all four blocks and each module then mixes its inputs into its output channel; on the other tables each module runs its whole ProcessInput; finally every module cross-feeds the four blocks through its row of the default crossfeed matrix
// Leaf: ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBVCFloatCyclicBuffer@@PAMPAPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75CE0, 432 bytes, thiscall, `ret 14h`, 16-byte aligned ebp frame (0x834 bytes).
// Name (NF99 draft, ported by NF105; q11492 + q11415; was the descriptive CReverbStage94::Process over local models): byte-identical (432 bytes, relocations
//   masked; hand4d b11 NAME_FIXES) to GameCODA 2.10 CODAApi.lib FloatShortReverb.obj
//   ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBVCFloatCyclicBuffer@@PAMPAPAMI@Z + the SDK FloatReverbDspEngine.obj
//   ?ProcessShortReverb@CFloatReverbDspEngine@@ REL32 +0x76 to it, retail 0x00C6A1F5 in 0x00C6A180 (128 bytes = the SDK length).
// Callees (the release relocations, SDK spellings): CReverbDspProcessor::m_pDSP 0x0143CCDC (slot 0 Type; eReverb_SSE = 1),
//   CFloatShortReverbModule::ProcessInput_GetFeedbackData_SseOptimisation 0x00C89B40, CReverbFnsSSE::ApplyFilterOrder2_quad 0x00C88E60
//   (q11415: was the stand-in CDspOps::FilterGain4; B's landed toolchain_library row), ProcessInput_MixInputs_SseOptimisation 0x00C89B60,
//   ProcessInput 0x00C89750, ProcessCrossfeed 0x00C89AA0; the crossfeed matrix 0x012BF088 is the SDK
//   `_sc_fDefaultShortReverbCrossfeedGainMatrix` (NF99P2 q11516 respelled it in both landed binders 0x00C75CE0 / 0x00C75E90). Statement order: the CODAApiDbg body
//   (GetFilterHistory / GetFeedbackData per module, ApplyFilterOrder2_quad, MixInputs per output, else ProcessInput, then ProcessCrossfeed).
// Members: the CodeView names on the shared headers (m_shortReverbModule[4] +0x04 stride 0x28, m_uiNumOutputs +0xA4).
// Shape: the TU of the fading CFloatShortReverb::Process 0x00C75E90 (four 128-float blocks in one 16-byte aligned local,
//   `j % m_uiNumOutputs` named above each call); the SIMD path also fills two four-pointer arrays (filter parameters, tap gain states).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATSHORTREVERB_PROCESS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include "engine/CFloatShortReverbParams.h"
#include "engine/CFloatShortReverb.h"
#include "engine/CReverbFnsSSE.h"
extern "C" const float sc_fDefaultShortReverbCrossfeedGainMatrix[4][4];   // 0x012BF088

void CFloatShortReverb::Process(CFloatShortReverbParams* pParams, const CFloatCyclicBuffer* pInputBuffer, float* pfFeedback,
                                float** apfOutputs, unsigned int uiSamples)
{
    __declspec(align(16)) float afBuffer[4][128];
    if (m_pDSP->Type() == CReverbFns::eReverb_SSE) {
        CFloatFilterParams* aFilterParams[4];
        float* apHistory[4];
        for (unsigned int i = 0; i < 4; ++i) {
            apHistory[i] = m_shortReverbModule[i].GetFilterHistory();
            aFilterParams[i] = &pParams->m_aShortRvb[i].m_fFilter;
            m_shortReverbModule[i].ProcessInput_GetFeedbackData_SseOptimisation(afBuffer[i], uiSamples);
        }
        static_cast<CReverbFnsSSE*>(m_pDSP)->ApplyFilterOrder2_quad(afBuffer[0], afBuffer[0], apHistory, aFilterParams, uiSamples);
        for (unsigned int j = 0; j < 4; ++j) {
            unsigned int uiOutput = j % m_uiNumOutputs;
            m_shortReverbModule[j].ProcessInput_MixInputs_SseOptimisation(&pParams->m_aShortRvb[j], pInputBuffer, afBuffer[j],
                                                                          apfOutputs[uiOutput], uiSamples);
        }
    } else {
        for (unsigned int j = 0; j < 4; ++j) {
            unsigned int uiOutput = j % m_uiNumOutputs;
            m_shortReverbModule[j].ProcessInput(&pParams->m_aShortRvb[j], pInputBuffer, afBuffer[j], apfOutputs[uiOutput], uiSamples);
        }
    }
    for (unsigned int k = 0; k < 4; ++k)
        m_shortReverbModule[k].ProcessCrossfeed(afBuffer[0], pfFeedback, sc_fDefaultShortReverbCrossfeedGainMatrix[k], uiSamples);
}
