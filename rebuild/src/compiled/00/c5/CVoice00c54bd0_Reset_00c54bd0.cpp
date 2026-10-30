// CVoice_00c54bd0::Reset() -- clear the voice's ring buffer at +0xb4 (CAudioRingBuffer::Clear 0x00C5CE40) and its current buffer at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C54BD0, length 23 bytes.
// Leaf: Reset
// Callees: CAudioRingBuffer::Clear 0x00C5CE40 (the CGameflowAssistanceScript::Init label was propagated). The chunk label (CMemoryDataOutputStream::Clear) is propagated; named by address. Shape: `this` in esi, `ret`.
class CAudioRingBuffer { public:                    // vtable 0x012B8FC4 (as CSoftwareVoice::AdvanceBuffer 0x00C5B300)
    void Clear();                                  // 0x00C5CE40 ?Clear@CAudioRingBuffer@@QAEXXZ
};

struct CVoice_00c54bd0 {                           // the libpk voice layout of CSoftwareVoice::AdvanceBuffer 0x00C5B300
    unsigned char              _pad00[0xc];
    long                       m_pBuffer;          // +0x0c
    unsigned char              _pad10[0xa4];
    CAudioRingBuffer           m_ring;             // +0xb4
    void Reset();
};

void CVoice_00c54bd0::Reset()
{
    m_ring.Clear();
    m_pBuffer = 0;
}