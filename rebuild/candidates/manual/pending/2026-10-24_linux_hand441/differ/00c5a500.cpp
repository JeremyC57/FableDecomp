// CSoftwareVoice::UpdateStep(float pitch, const SVoiceDesc* desc) -- recompute the resampler step: the pitch times the active modulators (pitch envelope, vibrato, second pitch LFO), times source rate / output rate, clamped below at 0.01, in 20.12 fixed point (x 4096, rounded through the retail inline-asm `fistp`), capped at 0xF000
// Grade: DIFFER(171v171) -- retail_asm candidate: `fstp [esp+4] ; fld [esp+4] ; fistp dword [esp+8]` (0x00C5A57C..0x00C5A584) is the inline-asm ROUND helper homed in the dead `pitch` slot; this C++ reference rounds through __ftol2 calls instead
// Leaf: ?UpdateStep@CSoftwareVoice@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5A500, length 171 bytes (+ int3 padding); `ret 8`.
// Globals: the output rate 0x0139C9E8, 2^32 0x0122DCB4, 0.01f 0x01230A5C, 4096.0f 0x012B8FC0, 0.5f 0x0122F59C.
// Callers: 0x00C54C37, 0x00C5CA7C. No callees.
// Label: the manifest's "CSound_ComputeEffectiveVolume" is propagated; descriptive names (GameCODA, no PDB).
// Shape: each modulator multiplies only when the description carries it (`mov edx,[eax+1Ch] ; test ; je ; fmul`);
// both rates are unsigned conversions (`fild ; jge ; fadd 2^32`); `< 0.01f` is `fcom ; test ah,5 ; jp`; the
// unfolded `+ 0.5f - 0.5f` precedes the ROUND; the cap stores first then re-stores 0xF000 (`cmp ; mov ; jbe`).
// C++ stand-in for the retail inline-asm rounding helper (`fld value ; fistp result`, round to nearest even): the
// retail row cannot come from C++ (VC7.1 `(long)x` calls __ftol2, /QIfist emits `fistp qword`).
static long RoundToNearest(float value)
{
    float down = (float)(long)value;
    if (value < 0.0f && down != value)
        down -= 1.0f;
    float frac = value - down;
    long result = (long)down;
    if (frac > 0.5f || (frac == 0.5f && (result & 1)))
        ++result;
    return result;
}

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

extern unsigned long g_outputRate;                      // 0x0139C9E8

void CSoftwareVoice::UpdateStep(float pitch, const SVoiceDesc* desc)
{
    if (desc->m_pPitchEnvelope)
        pitch *= m_pitchEnvLevel;
    if (desc->m_pVibrato)
        pitch *= m_pitchScale;
    if (desc->m_pModulation)
        pitch *= m_modulationScale;
    float ratio = (float)m_sampleRate * pitch / (float)g_outputRate;
    if (ratio < 0.01f)
        ratio = 0.01f;
    long step = RoundToNearest(ratio * 4096.0f + 0.5f - 0.5f);
    if (step < 0)
        ++step;
    m_step = step;
    if (m_step > 0xf000)
        m_step = 0xf000;
}
