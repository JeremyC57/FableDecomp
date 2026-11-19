// CFloatShortReverb::Process(CFloatShortReverbParams* pParams, const CFloatShortReverbParams* pNewParams, const CFloatCyclicBuffer* pInputBuffer, float* pfFeedback, float** apfOutputs, int iFaderToken, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of the short reverb's block while a parameter change is pending: each of the four modules whose fader token is the fading one runs its fading ProcessInput (old -> new record), the others the plain one, each into its own 128-float block and output channel (i % outputs); then each module writes its crossfed block from the four blocks, the feedback block and its row of the default crossfeed matrix
// Leaf: ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBU2@PBVCFloatCyclicBuffer@@PAMPAPAMHII@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75E90, 221 bytes, thiscall `ret 20h`, ebp frame with `and esp,-16` (four 128-float blocks).
// Name (NF105, q11518; was the descriptive CReverbTapBank::Process over a local model): GameCODA 2.10 CodeView public fading overload
//   ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBU2@PBVCFloatCyclicBuffer@@PAMPAPAMHII@Z. Two sources: (1) this body
//   is byte-identical (221 bytes, relocations masked) to that CODAApi.lib FloatShortReverb.obj member, whose relocations name the retail
//   callees (+0x73 the fading CFloatShortReverbModule::ProcessInput 0x00C89850, +0x8a ProcessInput 0x00C89750, +0xad
//   sc_fDefaultShortReverbCrossfeedGainMatrix 0x012BF088, +0xc7 ProcessCrossfeed 0x00C89AA0); (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessShortReverb 0x00C6A180 calls it at REL32 +0x4f, the retail call site. Layout: the CodeView
//   CFloatShortReverb (m_shortReverbModule[4] +4, stride 0x28; m_uiNumOutputs +0xa4) and CFloatShortReverbParams (m_aShortRvb[4], 0x30 each).
// Shape: the landed body with the CodeView names: `i % m_uiNumOutputs` is a named local computed above the per-module branch; the
//   module's token is the SDK inline CFloatShortReverbModule::Token (the Dbg build calls it).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATSHORTREVERB_PROCESSFADE
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT_FADING
#include "engine/CFloatShortReverbParams.h"
#include "engine/CFloatShortReverb.h"
extern "C" const float sc_fDefaultShortReverbCrossfeedGainMatrix[4][4];   // 0x012BF088

void CFloatShortReverb::Process(CFloatShortReverbParams* pParams, const CFloatShortReverbParams* pNewParams,
    const CFloatCyclicBuffer* pInputBuffer, float* pfFeedback, float** apfOutputs, int iFaderToken, unsigned int uiSamples,
    unsigned int uiSamplesToFade)
{
    __declspec(align(16)) float afBuffer[4][128];
    for (unsigned int i = 0; i < 4; ++i) {
        unsigned int uiOutput = i % m_uiNumOutputs;
        if (iFaderToken == m_shortReverbModule[i].Token())
            m_shortReverbModule[i].ProcessInput(&pParams->m_aShortRvb[i], &pNewParams->m_aShortRvb[i], pInputBuffer, afBuffer[i],
                                                apfOutputs[uiOutput], uiSamples, uiSamplesToFade);
        else
            m_shortReverbModule[i].ProcessInput(&pParams->m_aShortRvb[i], pInputBuffer, afBuffer[i], apfOutputs[uiOutput], uiSamples);
    }
    for (unsigned int j = 0; j < 4; ++j)
        m_shortReverbModule[j].ProcessCrossfeed(afBuffer[0], pfFeedback, sc_fDefaultShortReverbCrossfeedGainMatrix[j], uiSamples);
}
