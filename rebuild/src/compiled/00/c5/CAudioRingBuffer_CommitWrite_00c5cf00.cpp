// CAudioRingBuffer::CommitWrite(unsigned long frames) -- account for `frames` newly written frames: bump the queued count and advance the write cursor, wrapping once at the capacity
// Leaf: ?CommitWrite@CAudioRingBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CF00, length 32 bytes; `ret 4`.
// Callers: 0x00C5A689, 0x00C5AFA8 (the voice's ClearMix on the global mix ring 0x0143BF68), 0x00C5B611, 0x00C5B669,
// 0x00C5B7AC. No callees.
// Label: the manifest's "Timer_Advance" is propagated.
// Shape: `m_queued += frames` is the memory-destination `add`; the advanced cursor stays in eax for the wrap test.
// The GameCODA streaming ring-buffer cursor (vtable 0x012B8FC4, base vtable 0x012B5868; ctor 0x00C5D0A0, Init
// 0x00C5CE00 landed as `CAudioRingBuffer00c5ce00`): sample memory +4, capacity in frames +8, bytes per frame +0xC,
// channels +0x10, read cursor +0x14, write cursor +0x18, queued frames +0x1C. One sits at +0xB4 of every software
// voice; the global mix rings at 0x0143BF68 / 0x0143BF88 are planar (channel c at data + c * capacity * 2).
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
    void SetCapacity(unsigned long bytes, bool keepQueued);
    void CommitWrite(unsigned long frames);
    void CommitRead(unsigned long frames);
    void CopyChannel(unsigned long dstChannel, unsigned long srcChannel);
    void ClearChannel(unsigned long channel, unsigned long frames);
    void CopyFrom(const CAudioRingBuffer* src, unsigned long frames);
};

void CAudioRingBuffer::CommitWrite(unsigned long frames)
{
    m_queued += frames;
    m_writePos += frames;
    if (m_writePos >= m_capacity)
        m_writePos -= m_capacity;
}