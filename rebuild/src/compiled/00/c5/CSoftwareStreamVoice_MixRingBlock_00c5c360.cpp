// CSoftwareStreamVoice::MixRingBlock() -- mix the block at the voice's own ring read cursor (planar 16-bit, one plane per channel) into the CODA mix buffer with the per-speaker Q15 levels; when new target levels are pending, ramp to them across the 128-sample block (step = (target - level) / 128 per speaker) and adopt them
// Leaf: ?MixRingBlock@CSoftwareStreamVoice@@QAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5C360, 1184 bytes (one function, ends `ret` at 0x00C5C7F4), thiscall, /GS (the
// six-word step array; cookie 0x0139C8A8, check 0x00BFE9F9).
// Identity: its only caller 0x00C5CA00 (a stream-voice method, ret 0Ch) copies one 0x100-byte channel plane into the ring
// at +0x1F0 and calls this after the last plane (`index == m_numChannels - 1`). Class: the landed CSoftwareStreamVoice
// (vtable 0x012B5D70): Setup 0x00C5C1B0 resets +0x228 (pending flag) and the six Q15 levels +0x22A; MixMono 0x00C5C800 is
// the same ramp for the voice's single gain. The ring at +0x1F0 is a CAudioRingBuffer (landed 0x00C5CE00.. model; read
// pointer = data +0x1F4 + read position +0x204 * frame bytes +0x1FC); +0x5C is the channel count. Descriptive method name
// (GameCODA, no PDB/RTTI).
// Callees (on g_pCodaSystem->+0x58, the CMixBuffer accumulator, ctor 0x00C591F0): MixMono16Ramped 0x00C58210 / MixMono16
// 0x00C581D0, MixStereo16Ramped 0x00C58420 / MixStereo16 0x00C58370, MixQuad16Ramped 0x00C58880 / 0x00C58740 (landed as
// CUnprovenAudioBuffer_00C591F0::RouteShort4: same class, the un-ramped quad twin; NAME_FIXES.tsv), MixSurround16Ramped
// 0x00C58CE0 / MixSurround16 0x00C58AB0.
// Shape: the per-speaker step is `(target - level) / 128` on zero-extended words (`cdq ; and edx,7Fh ; add ; sar 7`) into
// a short[6] local; every ramped arm ends with the same six level := target word copies, which VC7.1 cross-jumps into one
// tail.

class CMixBuffer {                                  // GameCODA block accumulator (ctor 0x00C591F0)
public:
    void MixMono16(const short* src, const unsigned short* gains);                                       // 0x00C581D0
    void MixMono16Ramped(const short* src, const unsigned short* gains, const short* steps);             // 0x00C58210
    void MixStereo16(const short* src, const unsigned short* gains);                                     // 0x00C58370
    void MixStereo16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps);  // 0x00C58420
    void MixQuad16(const short* src, const unsigned short* gains);                                       // 0x00C58740
    void MixQuad16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps);    // 0x00C58880
    void MixSurround16(const short* src, const unsigned short* gains);                                   // 0x00C58AB0
    void MixSurround16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps); // 0x00C58CE0
};
struct CCodaSystem {
    unsigned char _pad[0x58];
    CMixBuffer*   m_pMixBuffer;                     // +0x58
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
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; }
};

class CSoftwareStreamVoice {                        // vtable 0x012B5D70, 0x250 bytes (only what this row touches)
public:
    unsigned char    _pad00[0x5c];
    unsigned long    m_numChannels;                 // +0x5c
    unsigned char    _pad60[0x1f0 - 0x60];
    CAudioRingBuffer m_ring;                        // +0x1f0 .. +0x20f
    float            m_channelGain[6];              // +0x210
    bool             m_levelRampPending;            // +0x228
    unsigned char    _pad229;
    unsigned short   m_channelLevel[6];             // +0x22a Q15
    unsigned short   m_channelTarget[6];            // +0x236 Q15
    void MixRingBlock();
};

void CSoftwareStreamVoice::MixRingBlock()
{
    const short* src = (const short*)m_ring.ReadPointer();
    CMixBuffer* mix = g_pCodaSystem->m_pMixBuffer;
    short steps[6];
    if (m_numChannels == 1) {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixMono16Ramped(src, m_channelLevel, steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixMono16(src, m_channelLevel);
        }
    } else if (m_numChannels == 2) {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixStereo16Ramped(src, m_channelLevel, (const unsigned short*)steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixStereo16(src, m_channelLevel);
        }
    } else if (m_numChannels == 4) {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixQuad16Ramped(src, m_channelLevel, (const unsigned short*)steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixQuad16(src, m_channelLevel);
        }
    } else {
        if (m_levelRampPending) {
            m_levelRampPending = false;
            for (int i = 0; i < 6; ++i)
                steps[i] = (short)(((int)m_channelTarget[i] - (int)m_channelLevel[i]) / 128);
            mix->MixSurround16Ramped(src, m_channelLevel, (const unsigned short*)steps);
            for (int j = 0; j < 6; ++j)
                m_channelLevel[j] = m_channelTarget[j];
        } else {
            mix->MixSurround16(src, m_channelLevel);
        }
    }
}