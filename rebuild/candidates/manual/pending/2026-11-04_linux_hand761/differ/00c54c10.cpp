// CAudioSource::ApplyParameters(const SSourceParams* params, unsigned long dirty) -- push a source's dirty parameters into its software voice: resampler step, ring flush, 3D direction to azimuth/elevation, the two one-pole filters, the 16-bit volume target (when a volume scaler exists), the pan gains, the stream's mix parameters, and the two state-variable filters
// Grade: DIFFER(651v651) -- float multiply leaf order: VC7.1 canonicalises the stream-gain chain to [edi+134h],[edi+130h],[esi+94h] whatever the source order, retail keeps [esi+94h] first; explicit parentheses keep the order but move the x87 ops against the integer argument set-up (dl 3, best)
// Leaf: ?ApplyParameters@CAudioSource@@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54C10, 651 bytes, thiscall `ret 8`. Constant 32767.0f (0x0129C820), 2^32 (0x0122DCB4).
// Retail-asm: 0x00C54D03 `fld dword ptr [esp+14h]` + 0x00C54D07 `fistp dword ptr [esp+18h]` -- libpk's inline-assembly float-to-long
//   (the shared libpk/FtoL.h): the volume product homed in the dead `params` slot and reloaded, a bare dword fistp into the dead
//   `dirty` slot; every C++ spelling (NOTES.md table) calls __ftol2, emits fistp qword or cvt(t)ss2si.
// Identity: libpk software voice parameter push (descriptive names; no PDB/RTTI); caller the CSoftwareVoice block update
//   (0x00C5AF0A: `push dirty ; push params-or-null ; mov ecx,voice`). The catalog's "C3DMeshFileXBoundingBoxChunk_..." is propagated.
// Callees: CSoftwareVoice::UpdateStep 0x00C5A500, CAudioRingBuffer::Clear 0x00C5CE40, ComputeAzimuthElevation 0x00C599C0,
//   COnePoleLowPass::SetLevel 0x00C6C000 (x2), MillibelsToAmplitude_00c54ae0 0x00C54AE0 (x2), Audio_ComputeFrequencyResponse
//   0x00C6BE60 (catalog name), stream slot 4 (+0x10), CStateVariableFilter::SetParameters 0x00C6BC80 / SetParametersWithGain
//   0x00C6BD00 / Reset 0x00C6B720.
// Shape found: `(m_flags >> 6) & 1` through a bool local; the muted/unmuted amplitude as two calls in an if/else (retail pushes
//   in each arm and shares the call); the volume chain parenthesised left-deep. ~40 attempts (LOG.tsv).
#include "libpk/FtoL.h"
#include "engine/C3DVector.h"

struct SFilterParameters;
struct SGainFilterParameters;

struct SVoiceDesc {                                     // embedded at source parameters +0x14
    unsigned long m_flags;                              // +0x00
    float m_level;                                      // +0x04 linear
    unsigned char _pad08[4];
    float m_pitch;                                      // +0x0c
    const SFilterParameters* m_pFilter;                 // +0x10
    const SGainFilterParameters* m_pGainFilter;         // +0x14
};

struct SSourceParams {
    unsigned char _pad00[0x0c];
    unsigned long m_flags;                              // +0x0c
    unsigned char _pad10[4];
    SVoiceDesc m_voice;                                 // +0x14
    unsigned char _pad2c[0x120 - 0x2c];
    C3DVector m_direction;                              // +0x120 listener-relative
    float m_panScale;                                   // +0x12c
    float m_gainScale;                                  // +0x130
    float m_gainScale2;                                 // +0x134
    float m_pitch;                                      // +0x138
    long m_level;                                       // +0x13c millibels
    long m_directLevel;                                 // +0x140 millibels
    unsigned long m_directReference;                    // +0x144 Hz
    long m_volume;                                      // +0x148 millibels
    long m_effectLevel;                                 // +0x14c millibels
    unsigned long m_effectReference;                    // +0x150 Hz
    float m_spread;                                     // +0x154
};

class CAudioRingBuffer {
public:
    virtual void v00();
    unsigned char _pad04[0x24];
    void Clear();                                       // 0x00C5CE40
};

struct CStateVariableFilter {                           // 0x24 bytes
    float m_state[9];
    void Reset();                                                                  // 0x00C6B720
    void SetParameters(const SFilterParameters* parameters, float scale);          // 0x00C6BC80
    void SetParametersWithGain(const SGainFilterParameters* parameters, float scale);   // 0x00C6BD00
};

struct COnePoleLowPass {                                // 0x0c bytes
    long m_feedback;
    long m_input;
    long m_history;
    void SetLevel(long levelMb, float referenceFrequency);                         // 0x00C6C000
};

struct CDecodedStream {
    virtual void v00();
    virtual void Rewind();
    virtual void Write();
    virtual void v03();
    virtual void SetParameters(float azimuth, float elevation, float pan, float gain, float* gains, bool mono, bool force);   // slot 4
};

struct CVolumeScaler;
struct CAudioMaster {                                   // 0x0143BC2C
    unsigned char _pad[0x17c];
    CVolumeScaler* m_pScaler;                           // +0x17c
};
extern CAudioMaster* g_pAudioMaster;                    // 0x0143BC2C

class CEAXManager {
public:
    unsigned char _pad[0xe8];
    long m_version;                                     // +0xe8
};
extern CEAXManager* g_pEAXManager;                      // 0x0143BB70

extern float g_f_0143bbe8;                              // 0x0143BBE8
extern float g_f_0143bbf4;                              // 0x0143BBF4

float __cdecl MillibelsToAmplitude_00c54ae0(long millibels);                                  // 0x00C54AE0
void __cdecl ComputeAzimuthElevation(float* azimuth, float* elevation, const C3DVector* direction);   // 0x00C599C0
void __cdecl Audio_ComputeFrequencyResponse(float azimuth, float elevation, float pan, float spread, float* gains);   // 0x00C6BE60

class CSoftwareVoice {
public:
    void* __vftable;                                    // +0x00
    unsigned char _pad04[8];
    long m_pending;                                     // +0x0c
    unsigned char _pad10[0x18];
    unsigned long m_flags;                              // +0x28
    unsigned long m_format;                             // +0x2c
    float m_masterLevel;                                // +0x30
    unsigned char _pad34[0x48];
    float m_filterScale;                                // +0x7c
    unsigned char _pad80[0x14];
    float m_levelScale;                                 // +0x94
    unsigned char _pad98[8];
    float m_filterDepth;                                // +0xa0
    unsigned char _pada4[0x10];
    CAudioRingBuffer m_ring;                            // +0xb4
    CStateVariableFilter m_filters[2];                  // +0xdc
    unsigned char _pad124[0xec];
    void UpdateStep(float pitch, const SVoiceDesc* desc);   // 0x00C5A500
};

class CAudioSource : public CSoftwareVoice {
public:
    float m_azimuth;                                    // +0x210
    float m_elevation;                                  // +0x214
    float m_gains[2];                                   // +0x218
    CDecodedStream* m_pStream;                          // +0x220
    COnePoleLowPass m_directFilter;                     // +0x224
    COnePoleLowPass m_effectFilter;                     // +0x230
    short m_volume;                                     // +0x23c
    short m_targetVolume;                               // +0x23e
    bool m_bVolumeRamp;                                 // +0x240
    void ApplyParameters(const SSourceParams* params, unsigned long dirty);
};

void CAudioSource::ApplyParameters(const SSourceParams* params, unsigned long dirty)
{
    const SVoiceDesc* voice = &params->m_voice;
    if (dirty & 0x10074)
        UpdateStep(params->m_pitch * voice->m_pitch, voice);
    bool flush = (m_flags >> 6) & 1;
    if (flush) {
        m_ring.Clear();
        m_pending = 0;
    }
    if (dirty & 0x8000)
        ComputeAzimuthElevation(&m_azimuth, &m_elevation, &params->m_direction);
    if (dirty & 0x400000)
        m_directFilter.SetLevel(params->m_directLevel, (float)params->m_directReference);
    if (g_pAudioMaster->m_pScaler) {
        float amplitude;
        if (g_pEAXManager && g_pEAXManager->m_version != 1 && (params->m_flags & 0x2000))
            amplitude = MillibelsToAmplitude_00c54ae0(0);
        else
            amplitude = MillibelsToAmplitude_00c54ae0(params->m_volume);
        short target = (short)FtoL(((amplitude * m_levelScale) * voice->m_level) * m_masterLevel * 32767.0f);
        if (target != m_targetVolume) {
            m_targetVolume = target;
            m_bVolumeRamp = true;
        }
        if (dirty & 0x1000000)
            m_effectFilter.SetLevel(params->m_effectLevel, (float)params->m_effectReference);
    }
    if ((dirty & 0x800000) && (m_format & 0x3c00) != 0x2000)
        Audio_ComputeFrequencyResponse(m_azimuth, m_elevation, g_f_0143bbe8 * params->m_panScale, g_f_0143bbf4 * params->m_spread, m_gains);
    float gain = MillibelsToAmplitude_00c54ae0(params->m_level) * m_levelScale * params->m_gainScale2 * params->m_gainScale * voice->m_level * m_masterLevel;
    m_pStream->SetParameters(m_azimuth, m_elevation, g_f_0143bbe8 * params->m_panScale, gain, m_gains, !(m_flags & 1), false);
    if (dirty & 0x1d0) {
        m_flags &= ~0x1e0000;
        float scale = m_filterDepth * m_filterScale;
        if (voice->m_pFilter) {
            m_filters[0].SetParameters(voice->m_pFilter, scale);
            m_flags |= 0x20000;
        } else
            m_filters[0].Reset();
        if (voice->m_pGainFilter) {
            m_filters[1].SetParametersWithGain(voice->m_pGainFilter, scale);
            m_flags |= 0x100000;
        } else
            m_filters[1].Reset();
    }
}
