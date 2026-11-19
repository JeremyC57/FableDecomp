// CReverbFns::CopySamples(CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, const float* pfSrc, unsigned int uiSamples) -- copy `count` floats into a delay ring starting at `pos`, wrapping at the end of the buffer
// Leaf: ?CopySamples@CReverbFns@@QAEXPAVCFloatCyclicBuffer@@IPBMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76460, 113 bytes, ret 10h. No callees (memcpy intrinsic); `this` is unused. Callers 0x00C6A158, 0x00C75958, 0x00C760D4, 0x00C76BB2, 0x00C76CF6.
// Name (NF102, q11556; was the descriptive CDspOps::WriteRing over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?CopySamples@CReverbFns@@QAEXPAVCFloatCyclicBuffer@@IPBMI@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (113 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (NF99P2 NAME_FIXES evidence). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: the mirror of ReadRing 0x00C763E0.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::CopySamples(CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, const float* pfSrc, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        memcpy(&pBuffer->m_pBase[uiOffset], pfSrc, uiSamples * sizeof(float));
    else {
        unsigned int uiFirst = pBuffer->GetLength() - uiOffset;
        memcpy(&pBuffer->m_pBase[uiOffset], pfSrc, uiFirst * sizeof(float));
        memcpy(pBuffer->m_pBase, pfSrc + uiFirst, (uiSamples - uiFirst) * sizeof(float));
    }
}