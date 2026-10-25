// CAudioRingBuffer::CopyFrom(const CAudioRingBuffer* src, unsigned long frames) -- copy `frames` frames of every channel of another planar ring, from its read cursor to this ring's write cursor
// Leaf: ?CopyFrom@CAudioRingBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D010, length 141 bytes (+ int3 padding); `ret 8`.
// Callee: none (intrinsic memcpy per channel). No direct caller or pointer reference in the image.
// Label: the manifest's "ImageData_CopyRows" is propagated.
// Shape: the five set-up locals must be written in exactly this order (source pointer, destination pointer, source
// stride, destination stride, byte count -- 1 of the 120 orders matches; the others move the `src` loads or the push
// order); both strides use THIS ring's frame size; the loop index lives in the dead `src` slot and the byte count in
// the dead `frames` slot.
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

void CAudioRingBuffer::CopyFrom(const CAudioRingBuffer* src, unsigned long frames)
{
    const unsigned char* from = src->m_pData + src->m_readPos * src->m_frameBytes;
    unsigned char* to = m_pData + m_writePos * m_frameBytes;
    unsigned long fromStride = src->m_capacity * m_frameBytes;
    unsigned long toStride = m_capacity * m_frameBytes;
    unsigned long bytes = m_frameBytes * frames;
    for (unsigned long c = 0; c < src->m_channels; ++c) {
        memcpy(to, from, bytes);
        from += fromStride;
        to += toStride;
    }
}