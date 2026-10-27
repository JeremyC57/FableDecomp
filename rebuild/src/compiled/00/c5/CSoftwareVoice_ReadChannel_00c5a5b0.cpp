// CSoftwareVoice::ReadChannel(CAudioRingBuffer* source, const unsigned char* data, unsigned long frames, unsigned long channel) -- de-interleave one channel of the current buffer's samples into the mono scratch ring 0x0143BF88 as 16-bit PCM (state 1: 16-bit copy, a straight memcpy for mono; state 2: unsigned 8-bit widened by (s - 0x80) << 8; other states write nothing), commit the frames to the scratch ring, and after the last channel commit them as read from the source ring
// Leaf: ?ReadChannel@CSoftwareVoice@@UAEXPAVCAudioRingBuffer@@PBEKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5A5B0, 250 bytes, thiscall `ret 0x10`.
// Identity (descriptive method name; GameCODA has no PDB or RTTI): slot 23 of the CSoftwareVoice vtable 0x012B5C60
//   (also 0x012B8AE0 + 0x5C); `this` is only used for the buffer at +0x0C, as in SetBuffer 0x00C5A2E0. The format test
//   (1 = 16-bit, 2 = 8-bit) is the one SetBuffer uses for the sample size (2 / 1).
// Callees: CSoundBuffer::GetDesc 0x00C3C4A0, CAudioRingBuffer::CommitWrite 0x00C5CF00 on g_monoRing (0x0143BF88, the
//   spelling of the landed ctor 0x00C5B1A0), CAudioRingBuffer::CommitRead 0x00C5CF20 on `source`.
// Shape: the write offset is `m_writePos * m_frameBytes` (the other order swaps the two loads, hidden by the relocation
//   mask, caught by RELOC_IDENTITY); the 8-bit sample is read as `signed char` (retail `movsx`); the channel count is re-read through the const
//   description each strided iteration (the 16-bit store may alias it); the mono 16-bit arm is the memcpy intrinsic.
#include <string.h>

struct SBufferDesc {                                    // 7 words (see 0x00C5A2E0)
    unsigned long  m_state;                             // +0x00 1 = 16-bit PCM, 2 = 8-bit PCM
    unsigned long  m_numChannels;                       // +0x04
    unsigned long  m_sampleRate;                        // +0x08
    unsigned long  m_dataBytes;                         // +0x0c
    unsigned long  m_playOffset;                        // +0x10
    unsigned long  m_prefetchBytes;                     // +0x14
    unsigned char* m_pSource;                           // +0x18
};
class CSoundBuffer {
public:
    const SBufferDesc* GetDesc() const;                 // 0x00C3C4A0
};
class CAudioRingBuffer {                                // vtable 0x012B8FC4
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    void CommitWrite(unsigned long frames);             // 0x00C5CF00
    void CommitRead(unsigned long frames);              // 0x00C5CF20
};
extern CAudioRingBuffer g_monoRing;                     // 0x0143BF88 mono scratch ring

class CSoftwareVoice {                                  // vtable 0x012B5C60
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void ReadChannel(CAudioRingBuffer* source, const unsigned char* data, unsigned long frames,
                             unsigned long channel);    // slot 23
    unsigned char _pad04[8];
    CSoundBuffer* m_pBuffer;                            // +0x0c
};

void CSoftwareVoice::ReadChannel(CAudioRingBuffer* source, const unsigned char* data, unsigned long frames,
                                 unsigned long channel)
{
    const SBufferDesc* desc = m_pBuffer->GetDesc();
    short* dst = (short*)(g_monoRing.m_pData + g_monoRing.m_writePos * g_monoRing.m_frameBytes);
    switch (desc->m_state) {
    case 1: {
        const short* src = (const short*)data + channel;
        if (desc->m_numChannels == 1) {
            memcpy(dst, src, frames * 2);
        } else {
            for (unsigned long i = 0; i < frames; i++) {
                dst[i] = *src;
                src += desc->m_numChannels;
            }
        }
        break;
    }
    case 2: {
        const signed char* src = (const signed char*)data + channel;
        if (desc->m_numChannels == 1) {
            for (unsigned long i = 0; i < frames; i++)
                dst[i] = (short)((src[i] - 0x80) << 8);
        } else {
            for (unsigned long i = 0; i < frames; i++)
                dst[i] = (short)((src[desc->m_numChannels * i] - 0x80) << 8);
        }
        break;
    }
    }
    g_monoRing.CommitWrite(frames);
    if (channel == desc->m_numChannels - 1)
        source->CommitRead(frames);
}