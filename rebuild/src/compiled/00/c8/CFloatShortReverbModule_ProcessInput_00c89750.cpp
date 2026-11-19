// CFloatShortReverbModule::ProcessInput(const CFloatShortReverbModuleParams* pParams, const CFloatCyclicBuffer* pInputBuffer, float* pfBuffer, float* pfOutput, unsigned int uiSamples) -- x87-path block of one short-reverb module: clear the block, run the module's own buffer (tail and wrapped head) through its filter into it, add the two input taps of the shared input buffer scaled by their input gains, and mix the block into the module's output with the output gain
// Leaf: ?ProcessInput@CFloatShortReverbModule@@QAEXPBUCFloatShortReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89750, 255 bytes, thiscall `ret 14h`. The block clear is the memset intrinsic (`rep stosd`
//   of uiSamples dwords); the input start lives in the dead `uiSamples` argument slot, &m_Filter in a dead argument slot.
// Name (NF105, q11514; was the descriptive CReverbFilterTap::Process over a local model): GameCODA 2.10 CodeView public
//   ?ProcessInput@CFloatShortReverbModule@@QAEXPBUCFloatShortReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z. Two sources: (1) this
//   body is byte-identical (255 bytes, relocations masked) to that CODAApi.lib FloatShortReverbModule.obj member, whose relocations name
//   the retail callees (+0x75 / +0x93 CFloatFilter::Process 0x00C74CE0, +0xd5 CReverbFns::ScaleAddSamples 0x00C76760, m_pDSP
//   0x0143CCDC); (2) the byte-identical SDK CFloatShortReverb::Process 0x00C75CE0 (REL32 +0x14d) and its fading overload 0x00C75E90
//   (+0x8a) call it at the retail call sites. Layout: the CodeView CFloatShortReverbModule (m_Filter +4, m_Buffer +0x18) and
//   CFloatShortReverbModuleParams (m_auiInputDelay[2] +0, m_afInputGain[2] +8, m_fFilter +0x10, m_fOutputGain +0x2c).
// Shape: the landed body with the CodeView names; ReadPosition is this file's descriptive inline helper (no retail symbol) for
//   `cursor - delay` wrapped into the buffer (the landed CRingBuffer::Back). The SDK Dbg build clears the block with _FloatClearSamples;
//   the release expands a memset there. Virtual slot 7 ScaleAddSamples (+0x1C) mixes.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include <string.h>
#include "engine/CFloatFilterParams.h"
#include "engine/CFloatShortReverbModuleParams.h"
#include "engine/CFloatShortReverbModule.h"

inline unsigned int ReadPosition(const CFloatCyclicBuffer* pBuffer, unsigned int uiCursor, unsigned int uiDelay)
{
    return uiCursor >= uiDelay ? uiCursor - uiDelay : pBuffer->GetLength() - uiDelay + uiCursor;
}

void CFloatShortReverbModule::ProcessInput(const CFloatShortReverbModuleParams* pParams, const CFloatCyclicBuffer* pInputBuffer,
    float* pfBuffer, float* pfOutput, unsigned int uiSamples)
{
    float* pfData = m_Buffer.m_pBase;
    unsigned int uiCursor = m_Buffer.GetWriteCursor();
    unsigned int uiStart = ReadPosition(pInputBuffer, pInputBuffer->GetWriteCursor(), uiSamples);
    memset(pfBuffer, 0, uiSamples * sizeof(float));
    unsigned int uiFirst = uiSamples;
    if (uiCursor + uiSamples > m_Buffer.GetLength())
        uiFirst = m_Buffer.GetLength() - uiCursor;
    m_Filter.Process(&pParams->m_fFilter, pfData + uiCursor, pfBuffer, true, uiFirst);
    if (uiFirst < uiSamples)
        m_Filter.Process(&pParams->m_fFilter, pfData, pfBuffer + uiFirst, true, uiSamples - uiFirst);
    for (unsigned int i = 0; i < 2; ++i)
        m_pDSP->ScaleAddSamples(pfBuffer, pInputBuffer, ReadPosition(pInputBuffer, uiStart, pParams->m_auiInputDelay[i]),
                                pParams->m_afInputGain[i], uiSamples);
    m_pDSP->ScaleAddSamples(pfOutput, pfBuffer, pParams->m_fOutputGain, uiSamples);
}
