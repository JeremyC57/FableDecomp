// CFloatReverbDspEngine::ProcessShortReverb(float** apfOutputs, unsigned int uiSamples) -- run the short reverb on this block: cross-fade it (current -> new parameters, at the fade position) when the engine's fader token is one of the short reverb's, else process it with the current parameters
// Leaf: ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6A180, 128 bytes, `ret 8`.
// Name (NF99 draft, ported by NF105; q11493; was the descriptive CReverbProcessor::ProcessStage94 over local models): byte-identical (128 bytes, relocations
//   masked; hand4d b11 audit) to GameCODA 2.10 CODAApi.lib FloatReverbDspEngine.obj private
//   ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z + the SDK ?ProcessReverb@CFloatReverbDspEngine@@ REL32 +0x5c to it, retail
//   call 0x00C6A62B in 0x00C6A5D0 (ProcessReverb, ported with this row).
// Callees: CFloatShortReverb::Process (fading overload) 0x00C75E90 (REL32 +0x4f) and CFloatShortReverb::Process 0x00C75CE0 (+0x76),
//   thiscall on m_ShortReverb (+0x94); MatchToken is the header inline (the CODAApiDbg body calls it out of line).
// Members: the CodeView names on the shared headers: m_pvParams +0x04 / m_pvParamsNew +0x08 (the CFloatReverbParameters blocks;
//   m_ShortReverb parameters at +0x88), m_iFaderToken +0x18, m_iSamplesToFade +0x1c, m_reverbBuffer +0x60, m_ShortReverb +0x94,
//   m_pfLongRvbXfeed +0x288. The two parameter locals follow the Dbg frame (m_pvParams read first); their names are descriptive.
// Shape: the short-reverb pointer is a named local formed first (`lea ecx,[eax+94h]`, `this` moves to eax), then both parameter
//   pointers are read into locals (esi/edi) before the test; MatchToken is `>=` then `<` (`jl` / `jge` to the plain Process arm).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATSHORTREVERB_PROCESS
#define FABLE_RETAIL_CFLOATSHORTREVERB_PROCESSFADE
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbDspEngine.h"

void CFloatReverbDspEngine::ProcessShortReverb(float** apfOutputs, unsigned int uiSamples)
{
    CFloatShortReverb* pShortReverb = &m_ShortReverb;
    CFloatReverbParameters* pParams = static_cast<CFloatReverbParameters*>(m_pvParams);
    CFloatReverbParameters* pParamsNew = static_cast<CFloatReverbParameters*>(m_pvParamsNew);
    if (pShortReverb->MatchToken(m_iFaderToken))
        pShortReverb->Process(&pParams->m_ShortReverb, &pParamsNew->m_ShortReverb, &m_reverbBuffer, m_pfLongRvbXfeed, apfOutputs,
                              m_iFaderToken, uiSamples, m_iSamplesToFade);
    else
        pShortReverb->Process(&pParams->m_ShortReverb, &m_reverbBuffer, m_pfLongRvbXfeed, apfOutputs, uiSamples);
}
