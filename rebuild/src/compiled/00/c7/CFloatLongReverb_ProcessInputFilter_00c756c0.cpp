// CFloatLongReverb::ProcessInputFilter(unsigned int uiIndex, const CFloatLongReverbParams* pParams, CFloatCyclicBuffer* pBuffer, unsigned int uiSamples) -- run the long reverb's input filter `uiIndex` over one block of the shared buffer, reading at the block start minus the filter's delay and writing at the block start minus the stage's aligned offset ((uiIndex + 1) * long-reverb input length / 4 + short-reverb input length, rounded up to 4 floats); a block that would cross the buffer's end is processed in chunks that stop at whichever position wraps first
// Leaf: ?ProcessInputFilter@CFloatLongReverb@@AAEXIPBUCFloatLongReverbParams@@PAVCFloatCyclicBuffer@@I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C756C0, 341 bytes, thiscall `ret 10h`. Callers: CFloatLongReverb::Process 0x00C75A20 (+0x23) and the
//   fading Process 0x00C75AB0 (+0x4f), once per input filter (uiIndex 0..2).
// Name (NF109, q11602; was the descriptive CReverbDiffuser::UpdateGain over a local model): GameCODA 2.10 CodeView private
//   ?ProcessInputFilter@CFloatLongReverb@@AAEXIPBUCFloatLongReverbParams@@PAVCFloatCyclicBuffer@@I@Z. Two sources: (1) this body is
//   byte-identical (341 bytes, relocations masked) to that CODAApi.lib FloatLongReverb.obj release member (work/tools/sdkcmpx.py), whose
//   relocations name CFloatFilter::Process 0x00C74CE0 at both call sites; (2) the byte-identical SDK CFloatLongReverb::Process overloads
//   (retail 0x00C75A20 / 0x00C75AB0) relocate it at +0x23 / +0x4f, the retail call sites.
// Layout: the CodeView CFloatLongReverb (m_filter[3] +4, m_pRvbConstants +0x90), CFloatLongReverbParams (m_uiInputFilterDelay[3] +0,
//   m_aInputFilter[3] +0xc), CFloatCyclicBuffer (m_pBase +0, GetLength / GetWriteCursor) and CReverbConstants
//   (m_uiMaxShortReverbInputSamples +0x10, m_uiMaxLongReverbInputSamples +0x14).
// Shape: the landed body with the CodeView names (the CODAApiDbg.lib body calls GetWriteCursor / GetLength out of line in the same
//   order): both chunk lengths start at uiSamples before either wrap test.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER
#include "engine/CReverbConstants.h"
#include "engine/CFloatLongReverbParams.h"
#include "engine/CFloatLongReverb.h"

void CFloatLongReverb::ProcessInputFilter(unsigned int uiIndex, const CFloatLongReverbParams* pParams, CFloatCyclicBuffer* pBuffer,
    unsigned int uiSamples)
{
    float* pfBase = pBuffer->m_pBase;
    unsigned int uiStart = pBuffer->GetWriteCursor() >= uiSamples ? pBuffer->GetWriteCursor() - uiSamples
                                                                  : pBuffer->GetLength() - uiSamples + pBuffer->GetWriteCursor();
    unsigned int uiDelay = pParams->m_uiInputFilterDelay[uiIndex];
    unsigned int uiRead = uiStart >= uiDelay ? uiStart - uiDelay : pBuffer->GetLength() - uiDelay + uiStart;
    unsigned int uiOffset = ((uiIndex + 1) * m_pRvbConstants->m_uiMaxLongReverbInputSamples / 4
                             + m_pRvbConstants->m_uiMaxShortReverbInputSamples + 3) & ~3u;
    unsigned int uiWrite = uiStart >= uiOffset ? uiStart - uiOffset : pBuffer->GetLength() - uiOffset + uiStart;
    if (uiWrite + uiSamples < pBuffer->GetLength() && uiRead + uiSamples < pBuffer->GetLength()) {
        m_filter[uiIndex].Process(&pParams->m_aInputFilter[uiIndex], pfBase + uiRead, pfBase + uiWrite, true, uiSamples);
        return;
    }
    unsigned int uiRemaining = uiSamples;
    do {
        unsigned int uiReadChunk = uiRemaining;
        unsigned int uiChunk = uiRemaining;
        if (uiRead + uiRemaining > pBuffer->GetLength()) uiReadChunk = pBuffer->GetLength() - uiRead;
        if (uiWrite + uiRemaining > pBuffer->GetLength()) uiChunk = pBuffer->GetLength() - uiWrite;
        if (uiChunk >= uiReadChunk) uiChunk = uiReadChunk;
        m_filter[uiIndex].Process(&pParams->m_aInputFilter[uiIndex], pfBase + uiRead, pfBase + uiWrite, true, uiChunk);
        uiRead = uiRead + uiChunk < pBuffer->GetLength() ? uiRead + uiChunk : uiRead + (uiChunk - pBuffer->GetLength());
        uiWrite = uiWrite + uiChunk < pBuffer->GetLength() ? uiWrite + uiChunk : uiWrite + (uiChunk - pBuffer->GetLength());
        uiRemaining -= uiChunk;
    } while (uiRemaining);
}
