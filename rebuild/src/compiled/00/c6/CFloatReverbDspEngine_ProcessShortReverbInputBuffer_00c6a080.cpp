// CFloatReverbDspEngine::ProcessShortReverbInputBuffer(unsigned int uiSamples, float* pfInput) -- the reverb input stage of one block: run the reverb input through the pitch shifter into a 128-float block, then through the echo in place (each fading old -> new parameters when its fader token is the fading one), diffuse it, write it into the reverb buffer at the write cursor and advance the cursor
// Leaf: ?ProcessShortReverbInputBuffer@CFloatReverbDspEngine@@AAEXIPAM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6A080, 252 bytes, thiscall `ret 8`, ebp frame with `and esp,-16` (128-float block).
// Name (NF105, q11521; was the descriptive BufferState::ProcessBlock over local models): GameCODA 2.10 CodeView private
//   ?ProcessShortReverbInputBuffer@CFloatReverbDspEngine@@AAEXIPAM@Z. Two sources: (1) this body is byte-identical (252 bytes,
//   relocations masked) to that CODAApi.lib FloatReverbDspEngine.obj member, whose relocations name the retail callees (+0x4d / +0x67
//   CFloatPitch::Process 0x00C770B0 / 0x00C76E90, +0x9e / +0xaf CFloatEcho::Process 0x00C76C20 / 0x00C76B60, +0xc0
//   CFloatDiffuser::Process 0x00C75140, +0xd9 CReverbFns::CopySamples 0x00C76460, m_pDSP 0x0143CCDC); (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessReverb 0x00C6A5D0 calls it at REL32 +0x53, the retail call site. Layout: the CodeView
//   CFloatReverbDspEngine (m_reverbBuffer +0x60, m_Pitch +0x1e0, m_echo +0x248, m_diffuser +0x25c) and CFloatReverbParameters
//   (m_PitchShift +0x220, m_Echo +0x228).
// Shape: the landed body with the CodeView names (both parameter pointers read first; no m_bIsFading test, as in the SDK Dbg body:
//   Token() compared with m_iFaderToken); the tokens are the SDK inlines CFloatPitch::Token / CFloatEcho::Token and the cursor advance
//   CFloatCyclicBuffer::AddWriteCursor (the Dbg build calls them out of line).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATPITCH_PROCESS
#define FABLE_RETAIL_CFLOATECHO_PROCESS
#define FABLE_RETAIL_CFLOATECHO_TOKEN
#define FABLE_RETAIL_CFLOATDIFFUSER_PROCESS
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbDspEngine.h"

void CFloatReverbDspEngine::ProcessShortReverbInputBuffer(unsigned int uiSamples, float* pfInput)
{
    __declspec(align(16)) float afBuffer[128];
    CFloatReverbParameters* pParams = static_cast<CFloatReverbParameters*>(m_pvParams);
    CFloatReverbParameters* pNewParams = static_cast<CFloatReverbParameters*>(m_pvParamsNew);
    if (m_iFaderToken == m_Pitch.Token())
        m_Pitch.Process(&pParams->m_PitchShift, &pNewParams->m_PitchShift, pfInput, afBuffer, uiSamples, m_iSamplesToFade);
    else
        m_Pitch.Process(&pParams->m_PitchShift, pfInput, afBuffer, uiSamples);
    if (m_iFaderToken == m_echo.Token())
        m_echo.Process(&pParams->m_Echo, &pNewParams->m_Echo, afBuffer, afBuffer, uiSamples, m_iSamplesToFade);
    else
        m_echo.Process(&pParams->m_Echo, afBuffer, afBuffer, uiSamples);
    m_diffuser.Process(afBuffer, uiSamples);
    m_pDSP->CopySamples(&m_reverbBuffer, m_reverbBuffer.GetWriteCursor(), afBuffer, uiSamples);
    m_reverbBuffer.AddWriteCursor(uiSamples);
}
