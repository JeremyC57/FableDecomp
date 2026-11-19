// CFloatShortReverbModule::ProcessInput_MixInputs_SseOptimisation(const CFloatShortReverbModuleParams* pParams, const CFloatCyclicBuffer* pInputBuffer, float* pfBuffer, float* pfOutput, unsigned int uiSamples) -- SSE-path second half of one short-reverb module block: add the two input taps of the shared input buffer (scaled by their input gains) to the filtered block, then mix it into the module's output with the output gain
// Leaf: ?ProcessInput_MixInputs_SseOptimisation@CFloatShortReverbModule@@QAEXPBUCFloatShortReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89B60, 125 bytes, thiscall `ret 14h`.
// Name (NF105, q11513; was the descriptive CReverbFilterTap::MixHistory over a local model): GameCODA 2.10 CodeView public
//   ?ProcessInput_MixInputs_SseOptimisation@CFloatShortReverbModule@@QAEXPBUCFloatShortReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z.
//   Two sources: (1) this body is byte-identical (125 bytes, relocations masked) to that CODAApi.lib FloatShortReverbModule.obj member
//   (+0x4e CReverbFns::ScaleAddSamples 0x00C76760, m_pDSP 0x0143CCDC); (2) the byte-identical SDK CFloatShortReverb::Process
//   0x00C75CE0 calls it at REL32 +0xe5, the retail call site. Layout: CFloatShortReverbModuleParams (m_auiInputDelay[2] +0,
//   m_afInputGain[2] +8, m_fOutputGain +0x2c).
// Shape: both input offsets come from this file's descriptive inline ReadPosition (the landed CRingBuffer::Back; no retail symbol);
//   the 2-trip tap loop stays rolled (esi counter, `cmp esi,2`) and uiSamples is re-read from its slot after each call.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatShortReverbModuleParams.h"
#include "engine/CFloatShortReverbModule.h"

inline unsigned int ReadPosition(const CFloatCyclicBuffer* pBuffer, unsigned int uiCursor, unsigned int uiDelay)
{
    return uiCursor >= uiDelay ? uiCursor - uiDelay : pBuffer->GetLength() - uiDelay + uiCursor;
}

void CFloatShortReverbModule::ProcessInput_MixInputs_SseOptimisation(const CFloatShortReverbModuleParams* pParams,
    const CFloatCyclicBuffer* pInputBuffer, float* pfBuffer, float* pfOutput, unsigned int uiSamples)
{
    unsigned int uiStart = ReadPosition(pInputBuffer, pInputBuffer->GetWriteCursor(), uiSamples);
    for (unsigned int i = 0; i < 2; ++i)
        m_pDSP->ScaleAddSamples(pfBuffer, pInputBuffer, ReadPosition(pInputBuffer, uiStart, pParams->m_auiInputDelay[i]),
                                pParams->m_afInputGain[i], uiSamples);
    m_pDSP->ScaleAddSamples(pfOutput, pfBuffer, pParams->m_fOutputGain, uiSamples);
}
