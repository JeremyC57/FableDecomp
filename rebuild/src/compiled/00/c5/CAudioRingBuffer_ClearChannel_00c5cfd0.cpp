// CAudioRingBuffer::ClearChannel(unsigned long channel, unsigned long frames) -- zero `frames` frames of one planar channel starting at the write cursor
// Leaf: ?ClearChannel@CAudioRingBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CFD0, length 58 bytes (+ int3 padding); `ret 8`.
// Callee: none (intrinsic memset: `rep stosd` + `rep stosb`). Caller: 0x00C5B654.
// Label: the manifest's "Image_FillRow" is propagated.
// Shape: `(m_capacity * channel + m_writePos) * m_frameBytes` (`this` moves to edx to free ecx for the count).
#include <string.h>

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

void CAudioRingBuffer::ClearChannel(unsigned long channel, unsigned long frames)
{
    memset(m_pData + (m_capacity * channel + m_writePos) * m_frameBytes, 0, m_frameBytes * frames);
}