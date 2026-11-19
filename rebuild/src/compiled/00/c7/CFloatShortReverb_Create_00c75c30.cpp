// CFloatShortReverb::Create(unsigned int uiNumOutputs, const CFloatShortReverbConstants* pConstants, void*& pvMemory, int* piBytes, int& iToken) -- two-pass layout of the short reverb: with memory, keep the output count; record the first fader token, create the four modules from their buffer lengths (one token each) and record the end token
// Leaf: ?Create@CFloatShortReverb@@QAEXIPBUCFloatShortReverbConstants@@AAPAXPAHAAH@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75C30, 106 bytes, thiscall `ret 14h`; CFloatShortReverbModule::Create 0x00C896C0 at +0x44.
// Name (NF105, family of q11549; was the descriptive CReverbTapBank::Allocate over a local model): GameCODA 2.10 CodeView public
//   ?Create@CFloatShortReverb@@QAEXIPBUCFloatShortReverbConstants@@AAPAXPAHAAH@Z. Two sources: (1) this body is byte-identical (106
//   bytes, relocations masked) to that CODAApi.lib FloatShortReverb.obj member (REL32 +0x44 ?Create@CFloatShortReverbModule@@QAEXIAAPAXPAHH@Z
//   = 0x00C896C0); (2) the byte-identical SDK CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0xe2, the retail
//   call site. Layout: the CodeView CFloatShortReverb (m_shortReverbModule[4] +4, m_uiNumOutputs +0xa4, m_iMyFaderTokenMin +0xa8,
//   m_iMyFaderTokenMax +0xac) and CFloatShortReverbConstants (uiBufferLength).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_CREATE
#define FABLE_RETAIL_CFLOATSHORTREVERB_CREATE
#include "engine/CFloatShortReverbConstants.h"
#include "engine/CFloatShortReverb.h"

void CFloatShortReverb::Create(unsigned int uiNumOutputs, const CFloatShortReverbConstants* pConstants, void*& pvMemory, int* piBytes,
    int& iToken)
{
    if (pvMemory)
        m_uiNumOutputs = uiNumOutputs;
    m_iMyFaderTokenMin = iToken;
    for (unsigned int i = 0; i < 4; ++i) {
        m_shortReverbModule[i].Create(pConstants[i].uiBufferLength, pvMemory, piBytes, iToken);
        ++iToken;
    }
    m_iMyFaderTokenMax = iToken;
}
