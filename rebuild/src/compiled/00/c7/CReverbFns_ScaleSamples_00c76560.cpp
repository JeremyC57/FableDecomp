// CReverbFns::ScaleSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, float fStep, unsigned int uiSamples) -- read `count` floats out of a delay ring at `pos` under a linear gain ramp (slot 1 ScaleRampCopy), wrapping at the end of the buffer; the wrapped run starts at gain + first * step
// Leaf: ?ScaleSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76560, 148 bytes, ret 18h. Callee: virtual slot 1 (+4). Callers 0x00C758A7, 0x00C76CAA, 0x00C893E1.
// Name (NF105, family of q11557; was the descriptive CDspOps::ReadRingScaledRamp over a local model): GameCODA 2.10 CodeView public non-virtual
//   ?ScaleSamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@IMMI@Z (engine/CReverbFns.h). Two sources: (1) this body is byte-identical (148 bytes, relocations masked) to that
//   CODAApi.lib member; (2) the byte-identical SDK callers bind it at the retail call sites (CFloatEcho / CFloatLongReverb / CFloatLongReverbModule members). The
//   cyclic buffer is the SDK CFloatCyclicBuffer (m_pBase, GetLength()); the kernels are the CReverbFns virtual slots (1 ScaleSamples
//   ramp-copy, 2 ramp, 3 ScaleSamples copy, 4 in place, 5 AddSamples, 6 ScaleAddSamples ramp, 7 ScaleAddSamples).
// Shape: the tail length is a SIGNED int (plain `fild`, no 2^32 fix-up), homed in the dead `ring` argument slot for the
//   fild; `gain + first * step` is evaluated right operand first (fild ; fmul step ; fadd gain).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbFns.h"

void CReverbFns::ScaleSamples(float* pfDest, const CFloatCyclicBuffer* pBuffer, unsigned int uiOffset, float fGain, float fStep, unsigned int uiSamples)
{
    if (uiOffset + uiSamples <= pBuffer->GetLength())
        ScaleSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, fStep, uiSamples);
    else {
        int iFirst = pBuffer->GetLength() - uiOffset;
        ScaleSamples(pfDest, &pBuffer->m_pBase[uiOffset], fGain, fStep, iFirst);
        ScaleSamples(pfDest + iFirst, pBuffer->m_pBase, fGain + iFirst * fStep, fStep, uiSamples - iFirst);
    }
}