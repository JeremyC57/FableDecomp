// CAudioRingBuffer::CopyChannel(unsigned long dstChannel, unsigned long srcChannel) -- copy one whole planar channel of the ring (capacity x frameBytes bytes) over another
// Leaf: ?CopyChannel@CAudioRingBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CF90, length 49 bytes (+ int3 padding); `ret 8`.
// Callee: none (intrinsic memcpy: `rep movsd` + `rep movsb`). Caller: 0x00C5BB3D.
// Label: the manifest's "Image_CopyRow" is propagated; the planar layout is the one 0x00C5AF50 / 0x00C5B030 walk.
// Shape: one named stride local; the three operand orders of the product all give the same bytes.
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

void CAudioRingBuffer::CopyChannel(unsigned long dstChannel, unsigned long srcChannel)
{
    unsigned long stride = m_frameBytes * m_capacity;
    memcpy(m_pData + dstChannel * stride, m_pData + srcChannel * stride, stride);
}