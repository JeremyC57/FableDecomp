// CReverbFns::AddSamples(CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, const float* pfSrc, unsigned int uiSamples) -- add `count` floats of `src` into a delay ring starting at `pos` (slot 5 Add), wrapping at the end of the buffer
// Leaf: ?AddSamples@CReverbFns@@QAEXPAVCFloatCyclicBuffer@@IPBMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C766F0, 107 bytes, ret 10h. Callee: virtual slot 5 (+0x14). Callers 0x00C76BC5, 0x00C76D09.
// Name (NF105, family of q11557; was the descriptive CDspOps::AddToRing over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?AddSamples@CReverbFns@@QAEXPAVCFloatCyclicBuffer@@IPBMI@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (107 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (CFloatEcho / CFloatLongReverb / CFloatLongReverbModule members). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: the mirror of AddFromRing 0x00C76680.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::AddSamples(CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, const float* pfSrc, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        AddSamples(&pBuffer->m_pBase[uiOffset], pfSrc, uiSamples);
    else {
        unsigned int uiFirst = pBuffer->GetLength() - uiOffset;
        AddSamples(&pBuffer->m_pBase[uiOffset], pfSrc, uiFirst);
        AddSamples(pBuffer->m_pBase, pfSrc + uiFirst, uiSamples - uiFirst);
    }
}