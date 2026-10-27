// CSoftwareVoice::SetBuffer(CSoundBuffer* buffer, bool resume) -- attach a sound buffer to the software voice: copy its 7-word description into the voice (+0x58), work out the sample size (2 for state 1, 1 for state 2, else slot 19), point the source ring (+0xB4) at the playable region (from the play offset or loop start when resuming the same buffer, up to the prefetch size when resuming, else the whole data or the loop end), then run the base CCodaVoice::SetBuffer
// Leaf: ?SetBuffer@CSoftwareVoice@@UAEXPAVCSoundBuffer@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5A2E0, 222 bytes, thiscall `ret 8`.
// Identity (descriptive method name; GameCODA has no PDB or RTTI): slot 18 of the CSoftwareVoice vtable 0x012B5C60 (also
//   0x012B5CD0 / the stream voice 0x012B5D70 / 0x012B8AE0 at +0x48), overriding CCodaVoice's slot 18 0x00C59880 which it
//   calls directly at the end; the ring and description fields are those of the landed SizeRing 0x00C5A3C0 (same
//   `state 1..2` test, slots 20 GetLoopStart / 21 GetLoopEnd). Catalog gapscan name is a placeholder.
// Callees: CSoundBuffer::GetDesc 0x00C3C4A0 and CAudioRingBuffer::Init 0x00C5CE00 (bound spellings), CCodaVoice::SetBuffer
//   0x00C59880 (descriptive), virtual slots 19 (+0x4C), 20 (+0x50), 21 (+0x54).
// Shape: the description copy is the implicit 28-byte struct assignment (`rep movsd`, 7); the sample-size switch keeps
//   the default (virtual) arm on the fall-through.
struct SBufferDesc {                                    // 7 words copied into the voice
    unsigned long  m_state;                             // +0x00 playback state / sample format
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
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long readPos, unsigned long queued);   // 0x00C5CE00
};
class CCodaVoice {                                      // vtable 0x012B5C10, 0xB4 bytes
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17();
    virtual void SetBuffer(CSoundBuffer* buffer, bool resume);   // slot 18 (0x00C59880)
    unsigned char _pad04[8];
    CSoundBuffer* m_pBuffer;                            // +0x0c current buffer
    unsigned char _pad10[0x48];
    SBufferDesc   m_desc;                               // +0x58
    unsigned char _pad74[0x40];
};
class CSoftwareVoice : public CCodaVoice {              // vtable 0x012B5C60
public:
    virtual void SetBuffer(CSoundBuffer* buffer, bool resume);   // slot 18 (0x00C5A2E0)
    virtual unsigned long GetSampleBytes();             // slot 19 (+0x4c)
    virtual unsigned long GetLoopStart();               // slot 20 (+0x50)
    virtual unsigned long GetLoopEnd();                 // slot 21 (+0x54)
    CAudioRingBuffer m_ring;                            // +0xb4
};

void CSoftwareVoice::SetBuffer(CSoundBuffer* buffer, bool resume)
{
    m_desc = *buffer->GetDesc();
    unsigned long sampleBytes;
    switch (m_desc.m_state) {
    case 1:
        sampleBytes = 2;
        break;
    case 2:
        sampleBytes = 1;
        break;
    default:
        sampleBytes = GetSampleBytes();
        break;
    }
    unsigned long start;
    if (buffer == m_pBuffer && resume) {
        start = m_desc.m_playOffset;
        if (start == 0) {
            if (m_desc.m_state > 0 && m_desc.m_state <= 2)
                start = 0;
            else
                start = GetLoopStart();
        }
    } else {
        start = 0;
    }
    unsigned long end;
    if (resume && m_desc.m_prefetchBytes != 0)
        end = m_desc.m_prefetchBytes;
    else if (m_desc.m_state > 0 && m_desc.m_state <= 2)
        end = m_desc.m_dataBytes;
    else
        end = GetLoopEnd();
    unsigned long channels = m_desc.m_numChannels;
    unsigned long frameBytes = channels * sampleBytes;
    unsigned long frames = (end - start) / frameBytes;
    m_ring.Init(m_desc.m_pSource + start, frames, frameBytes, channels, 0, frames);
    CCodaVoice::SetBuffer(buffer, resume);
}