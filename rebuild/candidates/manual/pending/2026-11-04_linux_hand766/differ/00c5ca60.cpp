// CSoftwareVoice::ApplyParameters(const CSampleBuffer* buffer, unsigned long changed) -- apply a buffer's changed voice parameters
// Grade: DIFFER(831v858) -- register allocation: retail keeps buffer in ebx and changed in its argument slot, shrink-wraps edi/ebp around the level/pan blocks; ours keeps changed in ebx and pushes all four at entry
// Leaf: ?ApplyParameters@CSoftwareVoice@@QAEXPBVCSampleBuffer@@K@Z
// Flags: none
#include "libpk/FtoL.h"
extern "C" double __cdecl exp(double x);
#pragma intrinsic(exp)
// Same-TU bodies (context, not graded): retail keeps the loop counter in ecx and the entry pointer in edx across both
// calls, which VC7.1 does only when it has seen the callees' bodies.
#pragma auto_inline(off)
long __cdecl SpeakerToChannelIndex(unsigned long speaker)          // 0x00C5C980 (the landed body)
{
    switch (speaker) {
    case 0x01: return 0;
    case 0x02: return 1;
    case 0x10: return 2;
    case 0x20: return 3;
    case 0x04: return 4;
    case 0x08: return 5;
    }
    return -1;
}
static float s_lastAmplitude;
float __cdecl MillibelsToAmplitude_00c5c130(long millibels)         // 0x00C5C130 (10^(mB/2000); retail computes it in inline asm)
{
    long offset = millibels + 10000;
    if (offset <= 0)
        return 0.0f;
    float amplitude = (float)exp((offset - 10000) * 0.0005f * 2.30258509f);
    s_lastAmplitude = amplitude;
    return amplitude;
}
#pragma auto_inline(on)
void __cdecl ComputePanGains(float pan, float* left, float* right);  // 0x00C59960

struct SFilterParameters;
struct SGainFilterParameters;
struct SSpeakerLevel {
    unsigned long speaker;                              // +0x00
    long          level;                                // +0x04 millibels (or a raw gain)
};
struct SSpeakerLevels {
    long           count;                               // +0x00
    SSpeakerLevel* entries;                             // +0x04
};
struct SVoiceDesc {
    unsigned long m_flags;                              // +0x00
    float         m_gain;                               // +0x04
    unsigned long m_x08;                                // +0x08
    float         m_pitch;                              // +0x0c
    const SFilterParameters*     m_pFilter;             // +0x10
    const SGainFilterParameters* m_pGainFilter;         // +0x14
    unsigned char _pad18[0x10];
};
struct SSampleInfo {
    float m_pan;                                        // +0x00
    unsigned long m_x04;                                // +0x04
    long  m_level;                                      // +0x08 millibels
    const SSpeakerLevels* m_pGains;                     // +0x0c raw speaker gains
    const SSpeakerLevels* m_pLevels;                    // +0x10 speaker levels (millibels)
    const SFilterParameters*     m_pFilter;             // +0x14
    const SGainFilterParameters* m_pGainFilter;         // +0x18
};
class CCodaBuffer {
public:
    virtual void v00();
    unsigned char _pad04[0xe];
    unsigned char m_bufferFlags;                        // +0x12
    unsigned char _pad13;
};
class CSampleBuffer : public CCodaBuffer {
public:
    SVoiceDesc    m_desc;                               // +0x14
    unsigned char _pad3c[0xfc - 0x3c];
    SSampleInfo   m_info;                               // +0xfc
};
class CAudioRingBuffer {
public:
    void Clear();                                       // 0x00C5CE40
    unsigned char _body[0x28];
};
class CStateVariableFilter {
public:
    void SetParameters(const SFilterParameters* parameters, float scale);            // 0x00C6BC80
    void SetParametersWithGain(const SGainFilterParameters* parameters, float scale); // 0x00C6BD00
    void Reset();                                                                     // 0x00C6B720
    unsigned char _body[0x24];
};
class CSoftwareVoice {
public:
    virtual void v00();
    unsigned char _pad04[8];
    long          m_x0c;                                // +0x0c
    unsigned char _pad10[0x18];
    unsigned long m_flags;                              // +0x28
    unsigned long m_mixFlags;                           // +0x2c
    float         m_envLevel;                           // +0x30
    unsigned char _pad34[0x7c - 0x34];
    float         m_pitchEnvSecondScale;                // +0x7c
    unsigned char _pad80[0x94 - 0x80];
    float         m_modulationLevelScale;               // +0x94
    unsigned char _pad98[8];
    float         m_modulationThirdScale;               // +0xa0
    unsigned char _pada4[0xb4 - 0xa4];
    CAudioRingBuffer m_ring;                            // +0xb4
    CStateVariableFilter m_filter0;                     // +0xdc
    CStateVariableFilter m_filter1;                     // +0x100
    unsigned char _pad124[0x210 - 0x124];
    float         m_speakerGains[6];                    // +0x210
    bool          m_volumeDirty;                        // +0x228
    unsigned char _pad229;
    short         m_volume[6];                          // +0x22a
    short         m_targetVolume[6];                    // +0x236
    unsigned char _pad242[2];
    long          m_level;                              // +0x244
    bool          m_masterDirty;                        // +0x248
    unsigned char _pad249;
    short         m_masterVolume;                       // +0x24a
    short         m_targetMasterVolume;                 // +0x24c
    bool IsEnvelopeDone() const { return (m_flags >> 6) & 1; }
    void UpdateStep(float pitch, const SVoiceDesc* desc);   // 0x00C5A500
    void ApplyParameters(const CSampleBuffer* buffer, unsigned long changed);
};

void CSoftwareVoice::ApplyParameters(const CSampleBuffer* buffer, unsigned long changed)
{
    const SVoiceDesc* desc = &buffer->m_desc;
    if (changed & 0x74)
        UpdateStep(desc->m_pitch, desc);
    if (IsEnvelopeDone()) {
        m_ring.Clear();
        m_x0c = 0;
    }
    if (buffer->m_info.m_pLevels && ((changed & 0x400) || ((changed & 0x48) && (m_mixFlags & 0x40000)))) {
        for (long i = 0; i < buffer->m_info.m_pLevels->count; ++i) {
            const SSpeakerLevel& entry = buffer->m_info.m_pLevels->entries[i];
            long channel = SpeakerToChannelIndex(entry.speaker);
            m_speakerGains[channel] = MillibelsToAmplitude_00c5c130(entry.level);
        }
        m_mixFlags = (m_mixFlags & ~0x70000) | 0x40000;
    }
    if (buffer->m_info.m_pGains && ((changed & 0x800) || ((changed & 0x48) && (m_mixFlags & 0x20000)))) {
        for (long i = 0; i < buffer->m_info.m_pGains->count; ++i) {
            const SSpeakerLevel& entry = buffer->m_info.m_pGains->entries[i];
            long channel = SpeakerToChannelIndex(entry.speaker);
            *(long*)&m_speakerGains[channel] = entry.level;
        }
        m_mixFlags = (m_mixFlags & ~0x70000) | 0x20000;
    }
    if ((changed & 0x202) || ((changed & 0x48) && (m_mixFlags & 0x10000))) {
        ComputePanGains(buffer->m_info.m_pan, &m_speakerGains[0], &m_speakerGains[1]);
        m_speakerGains[0] *= buffer->m_desc.m_gain;
        m_speakerGains[1] *= buffer->m_desc.m_gain;
        ComputePanGains(buffer->m_info.m_pan, &m_speakerGains[2], &m_speakerGains[3]);
        m_speakerGains[2] *= buffer->m_desc.m_gain;
        m_speakerGains[3] *= buffer->m_desc.m_gain;
        m_mixFlags = (m_mixFlags & ~0x70000) | 0x10000;
    }
    if (changed & 0xe4a) {
        float level = m_modulationLevelScale * m_envLevel;
        for (long i = 0; i < 6; ++i) {
            unsigned long volume = FtoL(level * m_speakerGains[i] * 32767.0f - 0.5f);
            if ((long)volume < 0)
                ++volume;
            if (volume > 0x7fff)
                volume = 0x7fff;
            if (m_flags & 1)
                m_volume[i] = (short)volume;
            else {
                m_targetVolume[i] = (short)volume;
                if (m_volume[i] != (short)volume)
                    m_volumeDirty = true;
            }
        }
    }
    if (changed & 0x11d0) {
        m_flags &= ~0x1e0000;
        float scale = m_modulationThirdScale * m_pitchEnvSecondScale;
        if (buffer->m_bufferFlags & 1) {
            if (buffer->m_desc.m_pFilter) {
                m_filter0.SetParameters(buffer->m_desc.m_pFilter, scale);
                m_flags |= 0x20000;
            } else if (buffer->m_desc.m_pGainFilter) {
                m_filter0.SetParametersWithGain(buffer->m_desc.m_pGainFilter, scale);
                m_flags |= 0x40000;
            } else
                m_filter0.Reset();
            if (buffer->m_info.m_pFilter) {
                m_filter1.SetParameters(buffer->m_info.m_pFilter, scale);
                m_flags |= 0x80000;
            } else if (buffer->m_info.m_pGainFilter) {
                m_filter1.SetParametersWithGain(buffer->m_info.m_pGainFilter, scale);
                m_flags |= 0x100000;
            } else
                m_filter1.Reset();
        } else {
            if (buffer->m_desc.m_pFilter) {
                m_filter0.SetParameters(buffer->m_desc.m_pFilter, scale);
                m_flags |= 0x20000;
            } else
                m_filter0.Reset();
            if (buffer->m_desc.m_pGainFilter) {
                m_filter1.SetParametersWithGain(buffer->m_desc.m_pGainFilter, scale);
                m_flags |= 0x100000;
            } else
                m_filter1.Reset();
        }
    }
    if ((changed & 0x4000) && buffer->m_info.m_level != m_level) {
        m_level = buffer->m_info.m_level;
        long volume = FtoL(MillibelsToAmplitude_00c5c130(m_level) * 32767.0f);
        if (m_flags & 1)
            m_masterVolume = (short)volume;
        else {
            m_targetMasterVolume = (short)volume;
            m_masterDirty = true;
        }
    }
}
