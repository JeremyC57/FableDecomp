// CFloatLongReverb::Process(CFloatLongReverbParams* pParams, const CFloatLongReverbParams* pNewParams, CFloatCyclicBuffer* pBuffer, float* pfOutput, float** apfInputs, int iToken, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of CFloatLongReverb::Process while a parameter change is pending: the input filters fade old -> new when the token is this reverb's first fader token (else run plain), the output is cleared, and each long-reverb module whose token matches fades (adopting the new module settings once the fade ends in this block) while the others run plain
// Leaf: ?Process@CFloatLongReverb@@QAEXPAUCFloatLongReverbParams@@PBU2@PAVCFloatCyclicBuffer@@PAMPAPAMHII@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75AB0, 277 bytes, thiscall `ret 20h`. Callees: CFloatLongReverb::ProcessInputFilter fading
//   0x00C75820 / plain 0x00C756C0, CFloatLongReverbModule::Process fading 0x00C89350 / plain 0x00C891A0.
// Name (NF109, q11604; was the descriptive CReverbDiffuser::ProcessRamped over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatLongReverb@@QAEXPAUCFloatLongReverbParams@@PBU2@PAVCFloatCyclicBuffer@@PAMPAPAMHII@Z (the fading overload). Two
//   sources: (1) this body is byte-identical (277 bytes, relocations masked) to that CODAApi.lib FloatLongReverb.obj release member
//   (work/tools/sdkcmpx.py), relocations +0x38 ProcessInputFilter fading 0x00C75820, +0x4f ProcessInputFilter 0x00C756C0, +0xbd module
//   Process fading 0x00C89350, +0xed module Process 0x00C891A0; (2) the byte-identical SDK CFloatReverbDspEngine::ProcessLongReverb
//   (retail 0x00C6A200) relocates it at +0x4f, the retail call site.
// Layout: the CodeView CFloatLongReverb (m_longReverbModule[2] +0x40 with m_iMyFaderToken +0x24 each, m_pRvbConstants +0x90,
//   m_iMyFaderTokenMin +0x94) and CFloatLongReverbParams (m_aLongRvb[2] +0x60, 0x3c bytes each).
// Shape: the landed body with the CodeView names. The CODAApiDbg.lib body calls CFloatLongReverbModule::Token (expanded here),
//   FloatClearSamples (its memset expanded) and memcpy for the module settings (a 15-dword `rep movsd`).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER_FADING
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESS
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS_FADING
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_TOKEN
#include "engine/CReverbConstants.h"
#include "engine/CFloatLongReverbParams.h"
#include "engine/CFloatLongReverb.h"
#include <string.h>
#pragma intrinsic(memset, memcpy)

void CFloatLongReverb::Process(CFloatLongReverbParams* pParams, const CFloatLongReverbParams* pNewParams, CFloatCyclicBuffer* pBuffer,
    float* pfOutput, float** apfInputs, int iToken, unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    if (iToken == m_iMyFaderTokenMin)
    {
        for (unsigned int i = 0; i < 3; ++i)
            ProcessInputFilter(i, pParams, pNewParams, pBuffer, uiSamples, uiSamplesToFade);
    }
    else
    {
        for (unsigned int i = 0; i < 3; ++i)
            ProcessInputFilter(i, pParams, pBuffer, uiSamples);
    }
    memset(pfOutput, 0, uiSamples * sizeof(float));
    for (unsigned int j = 0; j < 2; ++j)
    {
        unsigned int uiChannel = j % m_pRvbConstants->m_uiNumOutputs;
        if (iToken == m_longReverbModule[j].Token())
        {
            m_longReverbModule[j].Process(&pParams->m_aLongRvb[j], &pNewParams->m_aLongRvb[j], pBuffer, pfOutput, apfInputs[uiChannel],
                                          uiSamples, uiSamplesToFade);
            if (uiSamples >= uiSamplesToFade)
                memcpy(&pParams->m_aLongRvb[j], &pNewParams->m_aLongRvb[j], sizeof(pParams->m_aLongRvb[j]));
        }
        else
            m_longReverbModule[j].Process(&pParams->m_aLongRvb[j], pBuffer, pfOutput, apfInputs[uiChannel], uiSamples);
    }
}
