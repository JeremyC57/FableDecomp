// CSoftwareVoice::ProcessSampleBlock(CAudioObject* sound, bool playing, unsigned long frames) -- render up to `frames` frames of every channel of the voice into the planar mix ring: per channel, prime the mono scratch ring with the frames carried over from the last block, read (playing) or zero-fill the rest, then resample (step/phase not unity) or copy into the mix ring; keep the scratch ring's leftover frames per channel for the next block; commit the frames written to the mix ring and return them
// Leaf: ?ProcessSampleBlock@CSoftwareVoice@@QAEKPAVCAudioObject@@_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5B480, 831 bytes, thiscall `ret 0Ch`.
// Identity (descriptive; GameCODA has no PDB / RTTI): called once (0x00C5B9F7) by slot 9 of the CSoftwareVoice vtable
//   0x012B5C60 (0x00C5B7C0) with that slot's sound argument and the frames still due in the 128-frame block, followed by the
//   landed AdvanceBuffer 0x00C5B300 on the same sound; the method name keeps the catalog's "Audio_ProcessSampleBlock".
// Callees: virtual slot 23 (+0x5C) ReadChannel 0x00C5A5B0, CAudioRingBuffer::CommitWrite 0x00C5CF00 / CommitRead
//   0x00C5CF20 / ClearChannel 0x00C5CFD0 / Flush 0x00C5CEA0 on g_monoRing (0x0143BF88) and g_mixRing (0x0143BF68),
//   CSoftwareVoice::Resample 0x00C5B030, and the sound's stdcall source hook (+0xE4).
// Layout: the voice keeps per-channel carry-over histories of 16 samples at +0x124 (stride 0x20) and their frame count at
//   +0x1E4; the 7-word buffer description +0x58 (SBufferDesc of ReadChannel / SetBuffer), the ring +0xB4, the resampler
//   +0x1E8 / +0x1EC of the landed Resample model.
// Shape (46 grades): the resampler is a two-word sub-object with an inline SourceFrames (retail reloads +0x1E8 / +0x1EC
//   for it instead of reusing the saved step); `needed = frames; if (!playing) ...`; the ring's inline ReadPointer; the
//   hook limit read into a local and the clamp written out twice (no `most` local); the restore is written m_phase first;
//   the scratch ring's queued count captured once before the carry-over copy.
#include <string.h>

class CSoundBuffer;
struct SBufferDesc {                                    // 7 words (see 0x00C5A2E0)
    unsigned long  m_state;                             // +0x00 1 = 16-bit PCM, 2 = 8-bit PCM
    unsigned long  m_numChannels;                       // +0x04
    unsigned long  m_sampleRate;                        // +0x08
    unsigned long  m_dataBytes;                         // +0x0c
    unsigned long  m_playOffset;                        // +0x10
    unsigned long  m_prefetchBytes;                     // +0x14
    unsigned char* m_pSource;                           // +0x18
};
class CAudioObject;
typedef void (__stdcall* PFN_SOURCEHOOK)(CAudioObject* sound, CSoundBuffer* buffer, SBufferDesc* desc,
                                         const unsigned char** data);
class CAudioObject {
public:
    unsigned char  _pad00[0xe4];
    PFN_SOURCEHOOK m_pfnSourceHook;                     // +0xe4
    unsigned long  m_sourceHookBytes;                   // +0xe8
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
    void Flush();                                       // 0x00C5CEA0
    void CommitWrite(unsigned long frames);             // 0x00C5CF00
    void CommitRead(unsigned long frames);              // 0x00C5CF20
    void ClearChannel(unsigned long channel, unsigned long frames);   // 0x00C5CFD0
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; }
};
extern CAudioRingBuffer g_mixRing;                      // 0x0143BF68 planar 16-bit mix ring
extern CAudioRingBuffer g_monoRing;                     // 0x0143BF88 mono scratch ring

struct SResampler {
    unsigned long m_step;                               // +0x00 resampling step, 20.12 fixed point
    unsigned long m_phase;                              // +0x04 resampling phase (fraction)
    unsigned long SourceFrames(unsigned long frames) const { return ((m_step * frames + m_phase) >> 12) + 2; }
};
class CSoftwareVoice {                                  // vtable 0x012B5C60
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void ReadChannel(CAudioRingBuffer* source, const unsigned char* data, unsigned long frames,
                             unsigned long channel);    // slot 23
    unsigned char    _pad04[8];
    CSoundBuffer*    m_pBuffer;                         // +0x0c
    unsigned char    _pad10[0x48];
    SBufferDesc      m_desc;                            // +0x58
    unsigned char    _pad74[0x40];
    CAudioRingBuffer m_ring;                            // +0xb4
    unsigned char    _padd4[0x50];
    short            m_carry[6][16];                    // +0x124 per-channel carry-over frames
    unsigned long    m_carryFrames;                     // +0x1e4
    SResampler       m_resampler;                       // +0x1e8
    void Resample(CAudioRingBuffer* out, CAudioRingBuffer* in, unsigned long frames, unsigned long channel);   // 0x00C5B030
    unsigned long ProcessSampleBlock(CAudioObject* sound, bool playing, unsigned long frames);
};

unsigned long CSoftwareVoice::ProcessSampleBlock(CAudioObject* sound, bool playing, unsigned long frames)
{
    unsigned long step = m_resampler.m_step;
    bool resampling = step != 0x1000 || m_resampler.m_phase != 0;
    unsigned long phase = m_resampler.m_phase;
    unsigned long needed = frames;
    if (!playing)
        needed = m_carryFrames;
    if (resampling) {
        needed = m_resampler.SourceFrames(needed);
        if (needed > g_monoRing.m_capacity)
            needed = g_monoRing.m_capacity;
    }
    if (playing && needed > m_carryFrames + m_ring.m_queued)
        needed = m_carryFrames + m_ring.m_queued;
    unsigned long carried = 0;
    const unsigned char* data;
    if (playing) {
        PFN_SOURCEHOOK hook = sound->m_pfnSourceHook;
        if (hook) {
            SBufferDesc desc = m_desc;
            desc.m_pSource = m_ring.ReadPointer();
            desc.m_dataBytes += m_desc.m_pSource - desc.m_pSource;
            unsigned long limit = sound->m_sourceHookBytes;
            if (desc.m_dataBytes > limit) {
                desc.m_dataBytes = limit;
                if (needed > limit / m_ring.m_frameBytes + m_carryFrames)
                    needed = limit / m_ring.m_frameBytes + m_carryFrames;
            }
            const unsigned char* hooked;
            hook(sound, m_pBuffer, &desc, &hooked);
            data = hooked;
        } else
            data = m_ring.ReadPointer();
    } else
        data = 0;
    for (unsigned long channel = 0; channel < m_desc.m_numChannels; ++channel) {
        if (m_carryFrames) {
            memcpy(g_monoRing.m_pData, m_carry[channel], m_carryFrames * 2);
            g_monoRing.CommitWrite(m_carryFrames);
        }
        if (needed > m_carryFrames) {
            if (playing)
                ReadChannel(&m_ring, data, needed - m_carryFrames, channel);
            else {
                g_monoRing.ClearChannel(0, needed - m_carryFrames);
                g_monoRing.CommitWrite(needed - m_carryFrames);
            }
        }
        if (needed > g_monoRing.m_queued)
            needed = g_monoRing.m_queued;
        if (resampling) {
            m_resampler.m_phase = phase;
            m_resampler.m_step = step;
            unsigned long most = (((needed - 1) << 12) - phase) / step;
            if (most < frames)
                frames = most;
            Resample(&g_mixRing, &g_monoRing, frames, channel);
        } else {
            unsigned char* dst = g_mixRing.m_pData + g_mixRing.m_writePos * g_mixRing.m_frameBytes
                               + g_mixRing.m_capacity * channel * 2;
            if (frames > needed)
                frames = needed;
            memcpy(dst, g_monoRing.ReadPointer(), g_monoRing.m_frameBytes * frames);
            g_monoRing.CommitRead(frames);
        }
        unsigned long left = g_monoRing.m_queued;
        if (!g_monoRing.m_pData || left) {
            if (playing) {
                memcpy(m_carry[channel], g_monoRing.ReadPointer(), left * 2);
                carried = left;
            }
            g_monoRing.Flush();
        }
    }
    m_carryFrames = carried;
    g_mixRing.CommitWrite(frames);
    return frames;
}