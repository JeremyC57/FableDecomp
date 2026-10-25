// CAudioRingBuffer::SetCapacity(unsigned long bytes, bool keepQueued) -- resize the ring to `bytes / frameBytes` frames; when asked to keep the queued data, grow or shrink the queued count by the capacity change and re-derive the write cursor modulo the new capacity
// Leaf: ?SetCapacity@CAudioRingBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CE60, length 54 bytes (+ int3 padding); `ret 8`.
// Callers: the software voice's CommitBuffer 0x00C5A3C0 (three sites, `keepQueued` = 1). No callees.
// Label: the manifest's "Audio_ComputeTimeOffset" is propagated; the class is the ring cursor of the landed Init
// 0x00C5CE00 (same +8/+0xC/+0x14/+0x18/+0x1C fields).
// Shape: the unsigned quotient is kept in esi for the second `div`; `m_queued += capacity - m_capacity` gives the
// `sub eax,edx ; add edx,eax` pair.
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

void CAudioRingBuffer::SetCapacity(unsigned long bytes, bool keepQueued)
{
    unsigned long capacity = bytes / m_frameBytes;
    if (keepQueued) {
        m_queued += capacity - m_capacity;
        m_writePos = (m_readPos + m_queued) % capacity;
    }
    m_capacity = capacity;
}