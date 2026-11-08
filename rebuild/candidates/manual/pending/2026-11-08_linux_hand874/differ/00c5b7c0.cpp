// CSoftwareVoice::RenderSound(CAudioObject* sound, __int64 now) -- the software voice's per-block render step (vtable slot 9): schedule the sound against the mixer clock, mix (or clear) the block through the voice's filters and resampler, advance the buffer, stop the voice when the stop time passes
// Residue: DIFFER 15 diff lines (same length): (1) retail `test dword ptr [eax+18h],0Fh` (the buffer-alignment test of NeedsConversion, also in the out-of-line copy 0x00C5AC80) -- VC7.1 3077 AND QFE 4035 narrow every `test mem32,imm8-range` to `test byte ptr` under /O1, /O2, /Ox, /Os, /G5-/G7, /arch:SSE, even through a const volatile lvalue (probes n1..n13: ~60 spellings: pointer/long/__int64/union/bitfield/reference/template/inline-mask/% 16/<<28); only /Od or /Og- keep the dword (as and+cmp, not test) -- the probe quantity is the c2 test-narrowing peephole, unreachable from C++ at /O2; (2) the 'done' frame-count add lands in eax then copies to edi where retail adds in edi directly (register choice).
// Next: (1) is a compiler quantity (park); for (2) try the carry-frames update as `m_carryFrames += frames` through a named unsigned local before the 0x80 compare.
// Leaf: ?RenderSound@CSoftwareVoice@@UAEJPAVCAudioObject@@_J@Z
class CSoundBuffer;
struct SBufferDesc {
    unsigned long  m_state;
    unsigned long  m_numChannels;
    unsigned long  m_sampleRate;
    unsigned long  m_dataBytes;
    unsigned long  m_playOffset;
    unsigned long  m_prefetchBytes;
    unsigned char* m_pSource;
};
class CSoundBuffer {
public:
    const SBufferDesc* GetDesc() const;                 // 0x00C3C4A0
};
class CAudioObject;
typedef void (__stdcall* PFN_SOURCEHOOK)(CAudioObject* sound, CSoundBuffer* buffer, SBufferDesc* desc, const unsigned char** data);
typedef void (__stdcall* PFN_MIXHOOK)(CAudioObject* sound, SBufferDesc* desc, long reserved);
class CAudioObject {
public:
    unsigned char  _pad00[0x0c];
    unsigned long  m_caps;                              // +0x0c
    unsigned long  m_flags0 : 16;                       // +0x10
    unsigned long  m_stereo : 1;                        //   bit 16
    unsigned long  m_flags17 : 15;
    unsigned char  _pad14[0xe4 - 0x14];
    PFN_SOURCEHOOK m_pfnSourceHook;                     // +0xe4
    unsigned long  m_sourceHookBytes;                   // +0xe8
    PFN_MIXHOOK    m_pfnMixHook;                        // +0xec
};
class CAudioRingBuffer {
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;
    unsigned long  m_capacity;
    unsigned long  m_frameBytes;
    unsigned long  m_channels;
    unsigned long  m_readPos;
    unsigned long  m_writePos;
    unsigned long  m_queued;
    void CopyChannel(unsigned long dstChannel, unsigned long srcChannel);   // 0x00C5CF90
    bool HasQueued() const { return m_pData != 0 && m_queued != 0; }
};
extern CAudioRingBuffer g_mixRing;                      // 0x0143BF68
extern __int64 g_performanceFrequency;                  // 0x0139CA20
extern unsigned long g_outputRate;                      // 0x0139C9E8
class CEAXManager {
public:
    unsigned char _pad[0xe8];
    long          m_version;                            // +0xe8
};
extern CEAXManager* g_pEAXManager;                      // 0x0143BB70
class CSoftwareMixer {
public:
    unsigned char _pad[4];
    bool          m_active;                             // +0x04
};
struct CCodaSystem {
    unsigned char   _pad[0x17c];
    CSoftwareMixer* m_pMixer;                           // +0x17c
};
extern CCodaSystem* g_pCodaSystem;                      // 0x0143BC2C
struct CStateVariableFilter {
    float         m_current[3];
    float         m_target[3];
    float         m_band;
    float         m_low;
    unsigned long m_rampPosition;
    void Reset();                                       // 0x00C6B720
    void FilterWithGain(short* samples, unsigned long count);   // 0x00C6B9D0
    void Filter(short* samples, unsigned long count);           // 0x00C6BAE0
};
struct SResampler {
    unsigned long m_step;
    unsigned long m_phase;
};
#pragma pack(push, 4)
class CSoftwareVoice {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual long StartSound(CAudioObject* sound, __int64 startTime);     // slot 8
    virtual long RenderSound(CAudioObject* sound, __int64 now);          // slot 9
    virtual void v10();
    virtual bool SetPositionB(unsigned long low, unsigned long high);   // slot 11 (+0x2c)
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23();
    virtual void RenderBlock(CAudioRingBuffer* ring, unsigned long channels, bool mix);   // slot 24 (+0x60)
    virtual void v25(); virtual void v26();
    virtual void EndSound(__int64 now);                                  // slot 27 (+0x6c)
    unsigned char    _pad04[8];
    CSoundBuffer*    m_pBuffer;                         // +0x0c
    __int64          m_clock;                           // +0x10
    __int64          m_stopTime;                        // +0x18
    __int64          m_positionC;                       // +0x20
    unsigned long    m_flags;                           // +0x28
    unsigned long    m_requests;                        // +0x2c
    unsigned char    _pad30[0x28];
    SBufferDesc      m_desc;                            // +0x58
    unsigned char    _pad74[0x40];
    CAudioRingBuffer m_ring;                            // +0xb4
    unsigned long    m_pendingBlocks;                   // +0xd4
    CAudioRingBuffer* m_pSource;                        // +0xd8
    CStateVariableFilter m_filters[2];                  // +0xdc
    unsigned char    _pad124[0xc0];
    unsigned long    m_carryFrames;                     // +0x1e4
    SResampler       m_resampler;                       // +0x1e8
    void ScheduleSound(CAudioObject* sound, __int64 startTime);          // 0x00C5ACB0
    void ClearMix(unsigned long frames);                                 // 0x00C5AF50
    void ApplyFilter(short* samples, unsigned long frames, unsigned long index);   // 0x00C5AFC0
    void AdvanceBuffer(CAudioObject* sound);                             // 0x00C5B300
    unsigned long ProcessSampleBlock(CAudioObject* sound, bool playing, unsigned long frames);   // 0x00C5B480
    bool IsStopPending() const { return (m_requests >> 7) & 1; }
    bool IsStopped() const { return (m_requests & 0x100) || (!m_ring.m_pData && !m_pendingBlocks); }
    bool IsBit5() const { return (m_flags >> 5) & 1; }
};
#pragma pack(pop)

void CSoftwareVoice::ApplyFilter(short* samples, unsigned long frames, unsigned long index)
{
    bool active;
    switch (index) {
    case 0: active = (m_flags & 0x60000) != 0; break;
    case 1: active = (m_flags & 0x180000) != 0; break;
    default: return;
    }
    if (!active)
        return;
    if (m_flags & 0xa0000)
        m_filters[index].Filter(samples, frames);
    else
        m_filters[index].FilterWithGain(samples, frames);
}

long CSoftwareVoice::RenderSound(CAudioObject* sound, __int64 now)
{
    bool hasSourceHook = sound->m_pfnSourceHook != 0;
    bool hasMixHook = sound->m_pfnMixHook != 0;
    ScheduleSound(sound, now);
    bool idle = IsStopped();
    bool mixed = m_carryFrames || (m_flags & 0x1e0000) || m_resampler.m_step != 0x1000 || m_resampler.m_phase != 0
        || hasMixHook;
    unsigned long done = 0;
    long result = 0;
    do {
        unsigned long frames = 0x80 - done;
        if (IsStopped() || (m_pendingBlocks && !m_carryFrames)) {
            mixed = true;
            ClearMix(frames);
            break;
        }
        bool playing;
        if (!m_pendingBlocks && m_ring.HasQueued()) {
            playing = true;
            if (m_clock > now) {
                unsigned long wait = (unsigned long)((m_clock - now) * 44100 / g_performanceFrequency) - done;
                if (wait) {
                    mixed = true;
                    playing = false;
                    if (frames > wait)
                        frames = wait;
                }
            }
        } else
            playing = false;
        if (IsStopPending()) {
            if (m_stopTime <= (__int64)(done + frames) * g_performanceFrequency / 44100 + now) {
                if (playing)
                    frames = (unsigned long)((m_stopTime - now) * 44100 / g_performanceFrequency) - done;
                SetPositionB(0, 0);
            }
        }
        if (playing) {
            const SBufferDesc* desc = m_pBuffer->GetDesc();
            if (desc->m_state != 1 || desc->m_numChannels > 1 || ((unsigned long)desc->m_pSource & 0xf)
                || m_ring.m_queued <= frames || hasSourceHook)
                mixed = true;
        } else
            mixed = true;
        if (mixed) {
            if (playing || m_carryFrames)
                frames = ProcessSampleBlock(sound, playing, frames);
            else
                ClearMix(frames);
            AdvanceBuffer(sound);
        }
        done += frames;
    } while (done < 0x80);
    if (m_flags & 0x1e0000) {
        if (idle) {
            m_filters[0].Reset();
            m_filters[1].Reset();
        } else {
            short* samples = (short*)g_mixRing.m_pData;
            if (sound->m_stereo) {
                ApplyFilter(samples, 0x80, 0);
                samples += 0x80;
                ApplyFilter(samples, 0x80, 1);
            } else {
                if (m_flags & 0xa0000)
                    m_filters[0].Filter(samples, 0x80);
                if (m_flags & 0x140000)
                    m_filters[1].FilterWithGain(samples, 0x80);
            }
        }
    }
    unsigned long channels;
    if (hasMixHook) {
        PFN_MIXHOOK hook = sound->m_pfnMixHook;
        channels = m_pSource->m_channels;
        if (m_desc.m_numChannels == 1 && channels > 1)
            for (unsigned long i = 1; i < channels; ++i)
                g_mixRing.CopyChannel(i, 0);
        SBufferDesc desc;
        desc.m_state = 1;
        desc.m_numChannels = m_pSource->m_channels;
        desc.m_sampleRate = g_outputRate;
        desc.m_dataBytes = g_mixRing.m_capacity * g_mixRing.m_frameBytes * channels;
        desc.m_playOffset = 0;
        desc.m_prefetchBytes = 0;
        desc.m_pSource = g_mixRing.m_pData;
        hook(sound, &desc, 0);
        result = 0x200;
    } else
        channels = m_desc.m_numChannels;
    CAudioRingBuffer* ring = mixed ? &g_mixRing : &m_ring;
    bool mix = false;
    CEAXManager* eax = g_pEAXManager;
    if (sound->m_caps & (((eax && eax->m_version != 1) ? 0x2000 : 0) | 0x80)) {
        if (g_pCodaSystem->m_pMixer && g_pCodaSystem->m_pMixer->m_active)
            mix = true;
    } else if (sound->m_caps & ((eax && eax->m_version == 1) ? 0x2000 : 0))
        mix = true;
    RenderBlock(ring, channels, mix);
    AdvanceBuffer(sound);
    if (!m_ring.m_pData) {
        if (m_pendingBlocks)
            --m_pendingBlocks;
        else {
            if (hasMixHook)
                return result;
            EndSound(now);
        }
        if (!m_pendingBlocks) {
            m_requests = 0;
            return result;
        }
    }
    if (IsStopped()) {
        if (!hasMixHook)
            m_requests &= ~0x100;
        return result | 8;
    }
    if (IsBit5())
        return result | 4;
    return result | 2;
}
