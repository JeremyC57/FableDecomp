// CReverbFns::ScaleAddSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, unsigned int uiSamples) -- mix `count` floats of a delay ring at `pos` scaled by `gain` into `dst` (slot 7 Mix), wrapping at the end of the buffer
// Leaf: ?ScaleAddSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76760, 118 bytes, ret 14h. Callee: virtual slot 7 (+0x1C). Callers 0x00C89824, 0x00C89BAD.
// Name (NF102, q11554; was the descriptive CDspOps::MixFromRing over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?ScaleAddSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMI@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (118 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (NF99P2 NAME_FIXES evidence). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: ReadRingScaled 0x00C764E0 over slot 7.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::ScaleAddSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        ScaleAddSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, uiSamples);
    else {
        unsigned int uiFirst = pBuffer->GetLength() - uiOffset;
        ScaleAddSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, uiFirst);
        ScaleAddSamples(pfDest + uiFirst, pBuffer->m_pBase, fGain, uiSamples - uiFirst);
    }
}