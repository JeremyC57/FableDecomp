// CFloatReverbDspEngine::ProcessLongReverb(float** apfOutputs, unsigned int uiSamples) -- run the long reverb on this block: fade it (current -> new parameters, at the fade position) when the engine's fader token falls in the long reverb's token range, else run the plain Process; both read the reverb buffer and the long-reverb crossfeed block and mix into the outputs
// Leaf: ?ProcessLongReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6A200, 128 bytes, `ret 8`.
// Name (NF105, q11522; was the descriptive CReverbProcessor::ProcessStage144 over local models): GameCODA 2.10 CodeView private
//   ?ProcessLongReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z. Two sources: (1) this body is byte-identical (128 bytes, relocations masked)
//   to that CODAApi.lib FloatReverbDspEngine.obj member, whose relocations name the retail callees (+0x4f the fading CFloatLongReverb::Process
//   0x00C75AB0, +0x76 CFloatLongReverb::Process 0x00C75A20); (2) the byte-identical SDK CFloatReverbDspEngine::ProcessReverb 0x00C6A5D0
//   calls it at REL32 +0x65, the retail call site. Layout: m_pvParams +4 / m_pvParamsNew +8 (CFloatReverbParameters, m_LongReverb +0x148),
//   m_iFaderToken +0x18, m_iSamplesToFade +0x1c, m_reverbBuffer +0x60, m_LongReverb +0x144, m_pfLongRvbXfeed +0x288.
// Shape: the twin of ProcessShortReverb 0x00C6A180: the long-reverb pointer is a named local formed first, then both parameter
//   pointers are read into locals before the test; MatchToken is the SDK header inline (`>=` then `<`; the Dbg body calls it).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESS
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbDspEngine.h"

void CFloatReverbDspEngine::ProcessLongReverb(float** apfOutputs, unsigned int uiSamples)
{
    CFloatLongReverb* pLongReverb = &m_LongReverb;
    CFloatReverbParameters* pParams = static_cast<CFloatReverbParameters*>(m_pvParams);
    CFloatReverbParameters* pParamsNew = static_cast<CFloatReverbParameters*>(m_pvParamsNew);
    if (pLongReverb->MatchToken(m_iFaderToken))
        pLongReverb->Process(&pParams->m_LongReverb, &pParamsNew->m_LongReverb, &m_reverbBuffer, m_pfLongRvbXfeed, apfOutputs,
                              m_iFaderToken, uiSamples, m_iSamplesToFade);
    else
        pLongReverb->Process(&pParams->m_LongReverb, &m_reverbBuffer, m_pfLongRvbXfeed, apfOutputs, uiSamples);
}
