// CReverbFns::ScaleSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, unsigned int uiSamples) -- read `count` floats out of a delay ring at `pos` scaled by `gain` (slot 3 ScaleCopy), wrapping at the end of the buffer
// Leaf: ?ScaleSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C764E0, 118 bytes, ret 14h. Callee: virtual slot 3 (+0xC). Callers 0x00C7607E, 0x00C76BA0.
// Name (NF102, q11553; was the descriptive CDspOps::ReadRingScaled over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?ScaleSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMI@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (118 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (NF99P2 NAME_FIXES evidence). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: the float gain is forwarded as a dword (`mov edx,[esp+1Ch] ; push edx`).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::ScaleSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        ScaleSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, uiSamples);
    else {
        unsigned int uiFirst = pBuffer->GetLength() - uiOffset;
        ScaleSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, uiFirst);
        ScaleSamples(pfDest + uiFirst, pBuffer->m_pBase, fGain, uiSamples - uiFirst);
    }
}