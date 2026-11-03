// CAudioRingBuffer::Flush() -- empty the GameCODA ring: read and write cursors (+0x14, +0x18) and the queued count (+0x1C) to 0
// Leaf: ?Flush@CAudioRingBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5CEA0, 12 bytes, thiscall, plain `ret`.
// Identity (landing 2026-11-03g, chunk 736 NAME_FIXES): CSoftwareVoice::Reset 0x00C5A2A0 calls it on the two shared scratch
//   rings ?g_monoRing@@3VCAudioRingBuffer@@A (0x0143BF88) and ?g_mixRing@@3VCAudioRingBuffer@@A (0x0143BF68); the ring model
//   is the landed CAudioRingBuffer (0x00C5CE00..0x00C5D0A0: read +0x14, write +0x18, queued +0x1C). Descriptive name (GameCODA
//   has no PDB / RTTI): Clear (0x00C5CE40) and Reset (0x00C5CEB0) are the siblings' names. Was `NScript::CV_MurderTwistScript::
//   Init_b` (a propagated quest-script label).
class CAudioRingBuffer {                                // vtable 0x012B8FC4, 0x20 bytes
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    void Flush();                                       // 0x00C5CEA0
};

void CAudioRingBuffer::Flush()
{
    m_readPos = 0;
    m_writePos = 0;
    m_queued = 0;
}
