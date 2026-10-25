// CAudioRingBuffer::Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels, unsigned long readPos, unsigned long queued) -- point the ring at caller-owned sample memory: capacity in frames, bytes per frame, channel count, read cursor and queued frames; the write cursor follows as (read + queued) mod capacity
// Leaf: ?Init@CAudioRingBuffer@@QAEXPAEKKKKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CE00, thiscall `ret 18h`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 ""Register" 0xC5CE00 = CAudioRingBuffer::Init"):
//   - `this` is the GameCODA ring buffer (vtable 0x012B8FC4, ctor 0x00C5D0A0) whose seven words +4..+0x1c this body fills,
//     the layout of agent441's landed ring rows (SetCapacity 0x00C5CE60, CommitWrite 0x00C5CF00, CommitRead 0x00C5CF20);
//   - the callers are the software voice constructor 0x00C5B1A0, which sets up the shared scratch rings 0x0143BF88 (one
//     channel over 0x0143CBC0) and 0x0143BF68 (six channels over 0x0143C5C0), 0x80 frames of 2 bytes, cursors 0.
//   The landed CAudioRingBuffer00c5ce00 row had +0 as a data word: it is the vtable pointer. Descriptive names
//   (GameCODA: no PDB/RTTI), agent441's field names.
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CAudioRingBuffer : public CPKObject {             // vtable 0x012B8FC4, 0x20 bytes
public:
    unsigned char* m_pData;                             // +0x04 sample memory
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c bytes per frame
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14 read cursor (frames)
    unsigned long  m_writePos;                          // +0x18 write cursor (frames)
    unsigned long  m_queued;                            // +0x1c queued frames
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long readPos, unsigned long queued);
};

void CAudioRingBuffer::Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
                            unsigned long readPos, unsigned long queued)
{
    m_pData = data;
    m_capacity = capacity;
    m_frameBytes = frameBytes;
    m_channels = channels;
    m_readPos = readPos;
    m_queued = queued;
    m_writePos = (m_readPos + m_queued) % m_capacity;
}
