// CReverbFns::CopySamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, unsigned int uiSamples) -- copy `count` floats out of a delay ring starting at `pos`, wrapping at the end of the buffer
// Leaf: ?CopySamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@II@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C763E0, 116 bytes, ret 10h. No callees (memcpy intrinsic); `this` is unused. Callers 0x00C891FF, 0x00C89B58.
// Name (NF102, q11555; was the descriptive CDspOps::ReadRing over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?CopySamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@II@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (116 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (NF99P2 NAME_FIXES evidence). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: memcpy of `count * sizeof(float)` (a plain variable) collapses to `rep movsd` with ecx = count; the
//   wrapped runs keep the dword+byte form.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::CopySamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        memcpy(pfDest, &pBuffer->m_pBase[uiOffset], uiSamples * sizeof(float));
    else {
        unsigned int uiFirst = pBuffer->GetLength() - uiOffset;
        memcpy(pfDest, &pBuffer->m_pBase[uiOffset], uiFirst * sizeof(float));
        memcpy(pfDest + uiFirst, pBuffer->m_pBase, (uiSamples - uiFirst) * sizeof(float));
    }
}