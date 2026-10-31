// CSoftwareVoice::ClearMix(unsigned long frames) -- write `frames` frames of silence for every channel of the voice into the global planar 16-bit mix ring at its write cursor, commit them, and restart the resampler phase
// Leaf: ?ClearMix@CSoftwareVoice@@
// Grade: RELOCATION_MATCH (wave-2 sec6: the RELOC_IDENTITY reject was the multiply operand order, not three globals --
//   written m_frameBytes * m_writePos VC7.1 loaded writePos first, so the masked bytes matched with swapped field
//   addends; m_writePos * m_frameBytes gives retail's frameBytes-then-writePos loads)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5AF50, length 108 bytes (+ int3 padding); `ret 4`.
// Callee: CAudioRingBuffer::CommitWrite @ 0x00C5CF00 on g_mixRing @ 0x0143BF68 (pData 0x0143BF6C, capacity
// 0x0143BF70, frame bytes 0x0143BF74, write cursor 0x0143BF80). Callers: 0x00C5BA37 / 0x00C5BA50 (block update).
// Label: the manifest's "RenderTarget_ClearBuffer" is propagated; descriptive name (GameCODA, no PDB).
// Shape: the channel start is `pData + frameBytes * writePos`; each channel is an inline /Oi `memset` (`rep stosd` +
// `rep stosb` of `frames * 2` bytes, recomputed per pass) and the next channel is `capacity` shorts on.
#include <string.h>

// GameCODA voice model (no PDB/RTTI; names descriptive). CCodaVoice = the voice base (vtable 0x012B5C10, 19 slots,
// ctor 0x00C59BA0, 0xB4 bytes): the state flags +0x28 (envelope stage bits 0x02..0x40, vibrato phase 0x180, pitch
// envelope 0x7E00, second pitch LFO 0x18000, filter bits 0x60000/0x180000), the amplitude envelope +0x30..+0x38, the vibrato
// +0x3C..+0x4C, the pitch envelope +0x74..+0x84 and the second pitch LFO +0x88..+0xB0. CSoftwareVoice (vtable 0x012B5C60,
// 28 slots, ctor 0x00C5B1A0) adds the source ring +0xB4, two biquad filters +0xDC and the resampler +0x1E8/+0x1EC;
// CSoftwareStreamVoice (vtable 0x012B5D70) overrides slot 22 (SetParameters).
long __cdecl SecondsToMixBlocks(float seconds);    // 0x00C59B00 (seconds -> mix blocks of the output rate, retail inline asm)

struct SEnvelopeParams {                                // amplitude / pitch envelope description
    float delay;                                        // +0x00
    float attack;                                       // +0x04
    float hold;                                         // +0x08
    float decay;                                        // +0x0c
    float sustain;                                      // +0x10
    float release;                                      // +0x14
};
struct SVibratoParams {                                 // vibrato description
    float delay;                                        // +0x00
    float rate;                                         // +0x04 Hz
    float depth;                                        // +0x08 octaves
};
struct SModulationParams {                              // the second pitch LFO (read by 0x00C5A9D0)
    float delay;                                        // +0x00
};
struct SVoiceDesc {
    unsigned long m_flags;                              // +0x00 requested capabilities
    unsigned char _pad04[0xc];
    unsigned long m_filter0;                            // +0x10
    unsigned long m_filter1;                            // +0x14
    const SEnvelopeParams* m_pEnvelope;                 // +0x18
    const SEnvelopeParams* m_pPitchEnvelope;            // +0x1c
    const SVibratoParams* m_pVibrato;                   // +0x20
    const SModulationParams* m_pModulation;             // +0x24
};
class CCodaBuffer {                                     // a sound buffer handed to a voice
public:
    virtual void v00();
    virtual bool IsStream();                            // slot 1
    unsigned char _pad04[8];
    unsigned long m_formatFlags;                        // +0x0c
    unsigned char _pad10[2];
    unsigned char m_bufferFlags;                        // +0x12 (bit 0: tested by the stream voice)
    unsigned char _pad13;
};
struct SSampleInfo {
    unsigned char _pad00[0x14];
    unsigned long m_filter0;                            // +0x14
    unsigned long m_filter1;                            // +0x18
};
class CSampleBuffer : public CCodaBuffer {              // a non-stream buffer: its sample header at +0xFC
public:
    unsigned char _pad14[0xe8];
    SSampleInfo m_info;                                 // +0xfc
};
class CAudioRingBuffer {                                // vtable 0x012B8FC4 (the landed 0x00C5CE60.. rows)
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    void SetCapacity(unsigned long bytes, bool keepQueued);   // 0x00C5CE60
    void CommitWrite(unsigned long frames);                   // 0x00C5CF00
    void CommitRead(unsigned long frames);                    // 0x00C5CF20
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; }
};
extern CAudioRingBuffer g_mixRing;                      // 0x0143BF68 (the planar 16-bit mix ring)
class CVoiceFilter {                                    // 0x24 bytes (ctor 0x00C6B9A0)
public:
    float m_coef[3];                                    // +0x00 current coefficients
    float m_target[3];                                  // +0x0c target coefficients
    long  m_history[3];                                 // +0x18
    void Reset();                                       // 0x00C6B720
    void Process(short* samples, unsigned long frames);         // 0x00C6B9D0
    void ProcessRamped(short* samples, unsigned long frames);   // 0x00C6BAE0
};
class CCodaVoice {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    unsigned char _pad04[0x24];
    unsigned long m_flags;                              // +0x28
    unsigned long m_x2c;                                // +0x2c (0x80000000 after the ctor)
    float m_envLevel;                                   // +0x30
    float m_envDelta;                                   // +0x34 per-block step / factor
    long  m_envCounter;                                 // +0x38 mix blocks left in the stage
    float m_pitchScale;                                 // +0x3c vibrato pitch multiplier
    float m_vibratoUp;                                  // +0x40 per-block factor, rising half
    float m_vibratoDown;                                // +0x44 per-block factor, falling half
    long  m_vibratoCounter;                             // +0x48
    long  m_vibratoPeriod;                              // +0x4c mix blocks per half cycle
    unsigned char _pad50[8];
    unsigned long m_state;                              // +0x58 playback state
    unsigned long m_numChannels;                        // +0x5c
    unsigned long m_sampleRate;                         // +0x60 source sample rate
    unsigned long m_dataBytes;                          // +0x64
    unsigned long m_playOffset;                         // +0x68
    unsigned long m_prefetchBytes;                      // +0x6c
    unsigned char* m_pSource;                           // +0x70
    float m_pitchEnvLevel;                              // +0x74
    unsigned char _pad78[4];
    float m_pitchEnvScale;                              // +0x7c
    unsigned char _pad80[4];
    long  m_pitchEnvCounter;                            // +0x84
    float m_modulationScale;                            // +0x88 second pitch LFO multiplier
    unsigned char _pad8c[0x20];
    long  m_modulationCounter;                          // +0xac
    unsigned char _padb0[4];
    void StartEnvelope(const SEnvelopeParams* env);                         // 0x00C59C20
    bool UpdateEnvelopeStages(const SEnvelopeParams* env, bool recompute);  // 0x00C59E20
    bool UpdateEnvelope(const SEnvelopeParams* env, bool recompute);        // 0x00C5A480
    void SetVibrato(const SVibratoParams* vib);                             // 0x00C59C60
    void UpdateVibrato(const SVibratoParams* vib);                          // 0x00C59890
    void StartVibrato(const SVibratoParams* vib);                           // 0x00C59FC0
    void SetModulation(const SModulationParams* mod);                       // 0x00C5A9D0
    void StartPitchEnvelope(const SEnvelopeParams* env)                     // inline in 0x00C5BD00
    {
        m_flags &= ~0x7e20;
        if (env)
            m_pitchEnvCounter = SecondsToMixBlocks(env->delay);
        m_pitchEnvLevel = 1.0f;
        m_pitchEnvScale = 1.0f;
    }
    void StartModulation(const SModulationParams* mod)                      // inline in 0x00C5BD00
    {
        SetModulation(mod);
        m_flags &= ~0x18000;
        if (mod)
            m_modulationCounter = SecondsToMixBlocks(mod->delay);
    }
};
class CSoftwareVoice : public CCodaVoice {
public:
    virtual void v19();
    virtual unsigned long GetLoopStart();               // slot 20 (+0x50)
    virtual unsigned long GetLoopEnd();                 // slot 21 (+0x54)
    virtual unsigned long SetParameters(CCodaBuffer* buffer, const SVoiceDesc* desc);   // slot 22, 0x00C5BD00
    CAudioRingBuffer m_ring;                            // +0xb4 (the voice's ring object spans 0x28 bytes)
    unsigned char _padd4[8];
    CVoiceFilter m_filters[2];                          // +0xdc
    unsigned char _pad124[0xc4];
    unsigned long m_step;                               // +0x1e8 resampling step, 20.12 fixed point
    unsigned long m_phase;                              // +0x1ec resampling phase (fraction)
    void SizeRing(bool streaming);                                          // 0x00C5A3C0
    void ClearMix(unsigned long frames);                                    // 0x00C5AF50
    void ApplyFilter(short* samples, unsigned long frames, unsigned long index);   // 0x00C5AFC0
    void Resample(CAudioRingBuffer* out, CAudioRingBuffer* in, unsigned long frames, unsigned long channel);   // 0x00C5B030
    void UpdateStep(float pitch, const SVoiceDesc* desc);                   // 0x00C5A500
};
class CSoftwareStreamVoice : public CSoftwareVoice {
public:
    virtual unsigned long SetParameters(CCodaBuffer* buffer, const SVoiceDesc* desc);   // slot 22, 0x00C5C2D0
};

void CSoftwareVoice::ClearMix(unsigned long frames)
{
    short* p = (short*)(g_mixRing.m_pData + g_mixRing.m_writePos * g_mixRing.m_frameBytes);
    for (unsigned long c = 0; c < m_numChannels; ++c) {
        memset(p, 0, frames * sizeof(short));
        p += g_mixRing.m_capacity;
    }
    g_mixRing.CommitWrite(frames);
    m_phase = 0;
}