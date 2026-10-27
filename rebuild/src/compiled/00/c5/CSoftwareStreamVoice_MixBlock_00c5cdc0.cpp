// CSoftwareStreamVoice::MixBlock(CSoftwareMixer* mixer, CAudioRingBuffer* ring, unsigned long channels) -- vtable slot 25 of the GameCODA software stream voice: hand the block at the ring's read cursor to the software mixer, through MixMono for one channel, else Mix with the channel count
// Leaf: ?MixBlock@CSoftwareStreamVoice@@UAEXPAVCSoftwareMixer@@PAVCAudioRingBuffer@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CDC0, 56 bytes (+ int3 padding), thiscall `ret 0Ch` (the mixer
//   argument is unused: the callees fetch g_pCodaSystem->m_pMixer themselves); slot 25 of the stream voice vtable
//   0x012B5D70 (CSoftwareVoice's slot 25 0x00C3F580 is an empty `ret 0Ch`).
// Callees: CSoftwareStreamVoice::MixMono @ 0x00C5C800 / Mix @ 0x00C5C880 (landed; their headers name this row as the
//   "slot-25 mix entry"). Caller: CSoftwareVoice slot 24 0x00C5B0F0 passes (g_pCodaSystem->m_pMixer, ring, channels).
// Name: descriptive (GameCODA, no PDB/RTTI; the manifest label `_gapscan::sub_00c5cdc0` is a placeholder).
// Shape: the read pointer `pData + readPos * frameBytes` is an inline ring accessor, computed in each arm.
class CSoftwareMixer;
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
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; }
};
class CSoftwareStreamVoice {                        // vtable 0x012B5D70 (only the slots this row needs)
public:
    virtual void v00();
    virtual void MixBlock(CSoftwareMixer* mixer, CAudioRingBuffer* ring, unsigned long channels);   // slot 25 (index not modelled)
    unsigned char  _pad04[0x244];
    bool           m_gainRampPending;               // +0x248
    unsigned short m_gain;                          // +0x24a
    unsigned short m_targetGain;                    // +0x24c
    void MixMono(const short* src);                 // 0x00C5C800
    void Mix(const short* src, unsigned long channels);   // 0x00C5C880
};

void CSoftwareStreamVoice::MixBlock(CSoftwareMixer* mixer, CAudioRingBuffer* ring, unsigned long channels)
{
    if (channels == 1)
        MixMono((const short*)ring->ReadPointer());
    else
        Mix((const short*)ring->ReadPointer(), channels);
}