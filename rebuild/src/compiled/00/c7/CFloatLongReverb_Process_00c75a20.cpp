// CFloatLongReverb::Process(const CFloatLongReverbParams* pParams, CFloatCyclicBuffer* pBuffer, float* pfOutput, float** apfInputs, unsigned int uiSamples) -- run the long reverb over one block: the three input filters on the shared buffer, then clear the output and mix both long-reverb modules into it, each fed by input channel (module % outputs)
// Leaf: ?Process@CFloatLongReverb@@QAEXPBUCFloatLongReverbParams@@PAVCFloatCyclicBuffer@@PAMPAPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75A20, 132 bytes, thiscall `ret 14h`. Callees: CFloatLongReverb::ProcessInputFilter 0x00C756C0
//   (three times), CFloatLongReverbModule::Process 0x00C891A0 (twice).
// Name (NF109, q11603; was the descriptive CReverbDiffuser::Process over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatLongReverb@@QAEXPBUCFloatLongReverbParams@@PAVCFloatCyclicBuffer@@PAMPAPAMI@Z. Two sources: (1) this body is
//   byte-identical (132 bytes, relocations masked) to that CODAApi.lib FloatLongReverb.obj release member (work/tools/sdkcmpx.py), whose
//   relocations name ProcessInputFilter 0x00C756C0 (+0x23) and CFloatLongReverbModule::Process 0x00C891A0 (+0x6d); (2) the byte-identical
//   SDK CFloatReverbDspEngine::ProcessLongReverb (retail 0x00C6A200) relocates it at +0x76, the retail call site.
// Layout: the CodeView CFloatLongReverb (m_longReverbModule[2] +0x40, stride 0x28; m_pRvbConstants +0x90 with m_uiNumOutputs +4) and
//   CFloatLongReverbParams (m_aLongRvb[2] +0x60, stride 0x3c).
// Shape: the landed body with the CodeView names. The CODAApiDbg.lib body clears the output with FloatClearSamples; the release expands
//   its memset (`rep stosd` with ecx = uiSamples).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESS
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS
#include "engine/CReverbConstants.h"
#include "engine/CFloatLongReverbParams.h"
#include "engine/CFloatLongReverb.h"
#include <string.h>
#pragma intrinsic(memset)

void CFloatLongReverb::Process(const CFloatLongReverbParams* pParams, CFloatCyclicBuffer* pBuffer, float* pfOutput, float** apfInputs,
    unsigned int uiSamples)
{
    for (unsigned int i = 0; i < 3; ++i)
        ProcessInputFilter(i, pParams, pBuffer, uiSamples);
    memset(pfOutput, 0, uiSamples * sizeof(float));
    for (unsigned int j = 0; j < 2; ++j)
        m_longReverbModule[j].Process(&pParams->m_aLongRvb[j], pBuffer, pfOutput, apfInputs[j % m_pRvbConstants->m_uiNumOutputs], uiSamples);
}
