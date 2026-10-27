// CSoftwareVoice::SeekBytes(unsigned long bytes) -- vtable slot 14 of the GameCODA software voice: when the source ring has memory, rewind it (Reset) and skip `bytes / frameBytes` frames (CommitRead); then clear the resampler phase (+0x1EC) and the word at +0x1E4
// Leaf: ?SeekBytes@CSoftwareVoice@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5A250, 72 bytes (+ int3 padding), thiscall `ret 4`; slot 14 of the
//   CSoftwareVoice vtable 0x012B5C60 (also 0x012B5CD0 / the stream voice 0x012B5D70 / 0x012B8AE0); pure in CCodaVoice.
// Callees: CAudioRingBuffer::Reset @ 0x00C5CEB0 (this chunk) and CAudioRingBuffer::CommitRead @ 0x00C5CF20 (landed), both
//   on the ring at +0xB4.
// Name: descriptive (GameCODA, no PDB/RTTI; the manifest label `_gapscan::sub_00c5a250` is a placeholder).
// Shape: the frame size is read into a local before Reset (retail keeps it in ebx across the call: Reset lives in
//   another TU, so only a local survives); one zero register (ebp) for the pointer test and the two stores.
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
    void Reset();                                   // 0x00C5CEB0
    void CommitRead(unsigned long frames);          // 0x00C5CF20
};
class CCodaVoice {                                  // vtable 0x012B5C10, 0xB4 bytes (ctor 0x00C59BA0)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10();
    virtual void v11() = 0;
    virtual void v12();
    virtual void v13() = 0;
    virtual void SeekBytes(unsigned long bytes) = 0;   // slot 14
    unsigned char _pad04[0xb0];
};
class CSoftwareVoice : public CCodaVoice {          // vtable 0x012B5C60 (ctor 0x00C5B1A0)
public:
    virtual void SeekBytes(unsigned long bytes);    // slot 14: 0x00C5A250
    CAudioRingBuffer m_ring;                        // +0xb4
    unsigned char    _padd4[0x110];
    unsigned long    m_1e4;                         // +0x1e4
    unsigned long    m_step;                        // +0x1e8 resampling step, 20.12 fixed point
    unsigned long    m_phase;                       // +0x1ec resampling phase (fraction)
};

void CSoftwareVoice::SeekBytes(unsigned long bytes)
{
    if (m_ring.m_pData) {
        unsigned long frameBytes = m_ring.m_frameBytes;
        m_ring.Reset();
        m_ring.CommitRead(bytes / frameBytes);
    }
    m_phase = 0;
    m_1e4 = 0;
}