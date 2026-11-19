// CVoice_00c54bd0::Reset() -- clear the voice's ring buffer at +0xb4 (CSalSwBuffer::Release 0x00C5CE40 (NF95 q11406; was CAudioRingBuffer::Clear)) and its current buffer at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C54BD0, length 23 bytes.
// Leaf: Reset
// Callees: CSalSwBuffer::Release 0x00C5CE40 (NF95 q11406; was CAudioRingBuffer::Clear) (the CGameflowAssistanceScript::Init label was propagated). The chunk label (CMemoryDataOutputStream::Clear) is propagated; named by address. Shape: `this` in esi, `ret`.
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "engine/CSalSwBuffer.h"   // NF95 q11406: the ring is GameCODA's CSalSwBuffer (was the local CAudioRingBuffer stand-in)

struct CVoice_00c54bd0 {                           // the libpk voice layout of CSoftwareVoice::AdvanceBuffer 0x00C5B300
    unsigned char              _pad00[0xc];
    long                       m_pBuffer;          // +0x0c
    unsigned char              _pad10[0xa4];
    CSalSwBuffer               m_ring;             // +0xb4
    void Reset();
};

void CVoice_00c54bd0::Reset()
{
    m_ring.Release();
    m_pBuffer = 0;
}