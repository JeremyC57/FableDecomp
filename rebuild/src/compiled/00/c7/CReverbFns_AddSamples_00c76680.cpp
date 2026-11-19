// CReverbFns::AddSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, unsigned int uiSamples) -- add `count` floats of a delay ring starting at `pos` into `dst` (slot 5 Add), wrapping at the end of the buffer
// Leaf: ?AddSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@II@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76680, 104 bytes, ret 10h. Callee: virtual slot 5 (+0x14). Callers 0x00C7605D, 0x00C760B0, 0x00C8923B, 0x00C892AC.
// Name (NF102, q11552; was the descriptive CDspOps::AddFromRing over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?AddSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@II@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (104 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (NF99P2 NAME_FIXES evidence). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: the ring's data pointer is loaded once before the branch (no float argument competes for edx).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::AddSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        AddSamples(pfDest, &pBuffer->m_pBase[uiOffset], uiSamples);
    else {
        unsigned int uiFirst = pBuffer->GetLength() - uiOffset;
        AddSamples(pfDest, &pBuffer->m_pBase[uiOffset], uiFirst);
        AddSamples(pfDest + uiFirst, pBuffer->m_pBase, uiSamples - uiFirst);
    }
}