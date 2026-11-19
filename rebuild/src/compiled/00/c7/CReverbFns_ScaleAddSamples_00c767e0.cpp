// CReverbFns::ScaleAddSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, float fStep, unsigned int uiSamples) -- mix `count` floats of a delay ring at `pos` under a linear gain ramp into `dst` (slot 6 MixRamp), wrapping at the end of the buffer; the wrapped run starts at gain + first * step
// Leaf: ?ScaleAddSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C767E0, 148 bytes, ret 18h. Callee: virtual slot 6 (+0x18). Nine callers (0x00C758E1, 0x00C76CE4, 0x00C89414..0x00C89A1B).
// Name (NF105, family of q11557; was the descriptive CDspOps::MixRampFromRing over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?ScaleAddSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMMI@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (148 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (CFloatEcho / CFloatLongReverb / CFloatLongReverbModule members). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: ReadRingScaledRamp 0x00C76560 over slot 6 (signed tail length, homed in the dead `ring` slot).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::ScaleAddSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, float fStep, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        ScaleAddSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, fStep, uiSamples);
    else {
        int iFirst = pBuffer->GetLength() - uiOffset;
        ScaleAddSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, fStep, iFirst);
        ScaleAddSamples(pfDest + iFirst, pBuffer->m_pBase, fGain + iFirst * fStep, fStep, uiSamples - iFirst);
    }
}