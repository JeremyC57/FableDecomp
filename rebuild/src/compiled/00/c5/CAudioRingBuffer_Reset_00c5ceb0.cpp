// CAudioRingBuffer::Reset() -- rewind the GameCODA ring cursor: read and write positions to 0 and the frame count at +0x1C back to the full capacity
// Leaf: ?Reset@CAudioRingBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CEB0, 15 bytes, thiscall, plain `ret`.
// Caller: 0x00C5A270 (CSoftwareVoice slot 14, 0x00C5A250) on the voice's ring at +0xB4, just before CommitRead
//   0x00C5CF20. The ring model is the landed CAudioRingBuffer (0x00C5CE00..0x00C5D0A0): capacity +8, read +0x14,
//   write +0x18, queued +0x1C.
// Label correction: the manifest's "CThingSearchTools::CThingSearchTools" is propagated across the byte-identical body
//   (GameCODA has no PDB); the method name is descriptive.
// Shape: one zero register for the two cursor stores, then the capacity copy.
class CAudioRingBuffer {
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;             // +0x04
    unsigned long  m_capacity;          // +0x08 frames
    unsigned long  m_frameBytes;        // +0x0c
    unsigned long  m_channels;          // +0x10
    unsigned long  m_readPos;           // +0x14
    unsigned long  m_writePos;          // +0x18
    unsigned long  m_queued;            // +0x1c
    void Reset();
};

void CAudioRingBuffer::Reset()
{
    m_readPos = 0;
    m_writePos = 0;
    m_queued = m_capacity;
}