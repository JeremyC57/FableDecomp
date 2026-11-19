// CFloatReverbDspEngine::ProcessReflections(float** apfOutputs, unsigned int uiSamples, float* pfReverbInput) -- the early-reflection stage of one block: clear the reverb input block, then run the four reflections over the shared reflections input buffer (the one whose fader token is fading runs its fading Process, old -> new record), each adding its direct tap into the reverb input and mixing its output into output channel i % outputs
// Leaf: ?ProcessReflections@CFloatReverbDspEngine@@AAEXPAPAMIPAM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C69FB0, 197 bytes, thiscall `ret 0Ch`.
// Name (NF105, q11520; was the descriptive CReverbProcessor::ProcessCombs over a local model): GameCODA 2.10 CodeView private
//   ?ProcessReflections@CFloatReverbDspEngine@@AAEXPAPAMIPAM@Z. Two sources: (1) this body is byte-identical (197 bytes, relocations
//   masked) to that CODAApi.lib FloatReverbDspEngine.obj member, whose relocations name the retail callees (+0x80 the fading
//   CFloatReflection::Process 0x00C76120, +0x9c CFloatReflection::Process 0x00C75FF0); (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessReverb 0x00C6A5D0 calls it at REL32 +0x46, the retail call site. Layout: the CodeView CReverbDspEngine
//   (m_pvParams +4, m_pvParamsNew +8, m_bIsFading +0x14, m_iFaderToken +0x18, m_iSamplesToFade +0x1c) and CFloatReverbDspEngine
//   (m_ReverbConstants.m_uiNumOutputs +0x2c, m_reflectionsInputBuffer +0x54, m_reflection[4] +0x1f8); CFloatReverbParameters::m_Ref +0x38.
// Shape: the landed body with the CodeView names: both parameter pointers are read into locals before the memset (`rep stosd` of
//   uiSamples dwords; the SDK Dbg build calls _FloatClearSamples there); the per-reflection new record address is strength-reduced to
//   `params + (new - params)`; the output index wraps with an unsigned `>=`; the token is the SDK inline CFloatReflection::Token.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREFLECTION_PROCESS
#define FABLE_RETAIL_CFLOATREFLECTION_PROCESS_FADING
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB
#include <string.h>
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbDspEngine.h"

void CFloatReverbDspEngine::ProcessReflections(float** apfOutputs, unsigned int uiSamples, float* pfReverbInput)
{
    CFloatReverbParameters* pParams = static_cast<CFloatReverbParameters*>(m_pvParams);
    CFloatReverbParameters* pNewParams = static_cast<CFloatReverbParameters*>(m_pvParamsNew);
    memset(pfReverbInput, 0, uiSamples * sizeof(float));
    unsigned int uiOutput = 0;
    for (int i = 0; i < 4; ++i) {
        if (m_bIsFading && m_iFaderToken == m_reflection[i].Token())
            m_reflection[i].Process(&pParams->m_Ref[i], &pNewParams->m_Ref[i], &m_reflectionsInputBuffer, pfReverbInput,
                                    apfOutputs[uiOutput], uiSamples, m_iSamplesToFade);
        else
            m_reflection[i].Process(&pParams->m_Ref[i], &m_reflectionsInputBuffer, pfReverbInput, apfOutputs[uiOutput], uiSamples);
        if (++uiOutput >= m_ReverbConstants.m_uiNumOutputs)
            uiOutput = 0;
    }
}
