// CSoftwareVoice::RenderBlock(CAudioRingBuffer* ring, unsigned long channels, bool mix) -- vtable slot 24 of the GameCODA software voice: optionally hand the block to the system mixer (slot 25), run slot 26 for every channel of the voice's source ring (the destination channel is 0 for a mono output, else the same channel), then consume one 128-frame block of `ring`
// Leaf: ?RenderBlock@CSoftwareVoice@@UAEXPAVCAudioRingBuffer@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5B0F0, 116 bytes (+ int3 padding), thiscall `ret 0Ch`; slot 24 of the
//   software voice vtables 0x012B5C60 / 0x012B5CD0 / 0x012B5D70.
// Callees: virtual slot 25 (+0x64) MixBlock (0x00C3F580 empty here, 0x00C5CDC0 in the stream voice) with
//   g_pCodaSystem->m_pMixer (0x0143BC2C, +0x17C as in the landed 0x00C5C800); virtual slot 26 (+0x68); the landed
//   CAudioRingBuffer::CommitRead @ 0x00C5CF20 on `ring` with 0x80 frames.
// Name: descriptive (GameCODA, no PDB/RTTI; the manifest label `_gapscan::sub_00c5b0f0` is a placeholder). +0xD8 is the
//   pointer the software voice's slot 1 0x00C5A1D0 clears; its +0x10 is read as a channel count (a ring's m_channels).
// Shape: `channels == 1 ? 0 : i` is `dec ; neg ; sbb ; and`; the source ring's channel count is re-read every pass.
class CSoftwareMixer;
struct CCodaSystem {
    unsigned char   _pad[0x17c];
    CSoftwareMixer* m_pMixer;                       // +0x17c
};
extern CCodaSystem* g_pCodaSystem;                  // 0x0143BC2C

class CAudioRingBuffer {                            // vtable 0x012B8FC4 (the landed 0x00C5CE00.. rows)
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;                         // +0x04
    unsigned long  m_capacity;                      // +0x08 frames
    unsigned long  m_frameBytes;                    // +0x0c
    unsigned long  m_channels;                      // +0x10
    unsigned long  m_readPos;                       // +0x14
    unsigned long  m_writePos;                      // +0x18
    unsigned long  m_queued;                        // +0x1c
    void CommitRead(unsigned long frames);          // 0x00C5CF20
};
class CSoftwareVoice {                              // vtable 0x012B5C60 (ctor 0x00C5B1A0)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void RenderBlock(CAudioRingBuffer* ring, unsigned long channels, bool mix);              // slot 24
    virtual void MixBlock(CSoftwareMixer* mixer, CAudioRingBuffer* ring, unsigned long channels);   // slot 25
    virtual void RenderChannel(CAudioRingBuffer* ring, unsigned long channel, unsigned long target); // slot 26
    unsigned char     _pad04[0xd4];
    CAudioRingBuffer* m_pSource;                    // +0xd8
};

void CSoftwareVoice::RenderBlock(CAudioRingBuffer* ring, unsigned long channels, bool mix)
{
    if (mix)
        MixBlock(g_pCodaSystem->m_pMixer, ring, channels);
    for (unsigned long i = 0; i < m_pSource->m_channels; ++i)
        RenderChannel(ring, i, channels == 1 ? 0 : i);
    ring->CommitRead(0x80);
}