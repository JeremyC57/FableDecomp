// CAudioRingBuffer::Clear() -- reset a GameCODA ring to its empty, unallocated state: the data pointer, capacity, frame size, channel count, cursors and queued count (+0x04..+0x1C) to 0
// Leaf: ?Clear@CAudioRingBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5CE40, thiscall, plain `ret`.
// Identity (landing 2026-11-03g, chunk 736 NAME_FIXES): the bound spelling (CSoftwareVoice::AdvanceBuffer 0x00C5B300 binds
//   ?Clear@CAudioRingBuffer@@QAEXXZ); CSoftwareVoice::Stop 0x00C5A1F0 calls it on the voice's ring at +0xB4. The ring model is
//   the landed CAudioRingBuffer (0x00C5CE00..0x00C5D0A0). Descriptive name (GameCODA has no PDB / RTTI). Was the free
//   `__fastcall CGameflowAssistanceScript_Init` (a propagated quest-script label).
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
    void Clear();                                       // 0x00C5CE40
};

void CAudioRingBuffer::Clear()
{
    m_pData = 0;
    m_capacity = 0;
    m_frameBytes = 0;
    m_channels = 0;
    m_readPos = 0;
    m_writePos = 0;
    m_queued = 0;
}
