// CSoftwareStreamVoice::MixMono(const short* src) -- hand one mono 16-bit block to the software mixer at the voice's gain; when a new target gain is pending, ramp to it across the 128-sample block (step = (target - gain) / 128) and adopt it
// Leaf: ?MixMono@CSoftwareStreamVoice@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5C800, length 124 bytes (+ int3 padding); `ret 4`.
// Callees: CSoftwareMixer::MixMonoRamped 0x00C6AC90 / MixMono 0x00C6AC60 on g_pCodaSystem->m_pMixer.
// Caller: the voice's slot-25 mix entry 0x00C5CDC0 (channels == 1, the block at the ring's read cursor).
// Label: the manifest's "CTCCarryable_OnCarryableKill" is propagated; GameCODA has no PDB/RTTI, names are
// descriptive (from the mixer kernels and the 0x80-sample block).
// Shape: the signed `/ 128` of the zero-extended words is `cdq ; and edx,7Fh ; add ; sar 7`; the 16-bit gain is
// pushed straight from `cx` (an `unsigned short` parameter) and the pending flag cleared between the pushes.
// The GameCODA software mixer (held at +0x17C of the CODA system object g_pCodaSystem, 0x0143BC2C): 16-bit block
// kernels with a Q15 gain, optionally ramped by `step` per sample (bodies 0x00C6AC60..0x00C6AD10).
class CSoftwareMixer {
public:
    void MixMono(const short* src, unsigned short gain);                                        // 0x00C6AC60
    void MixMonoRamped(const short* src, unsigned short gain, short step);                      // 0x00C6AC90
    void Mix(const short* src, unsigned long channels, unsigned short gain);                    // 0x00C6ACC0
    void MixRamped(const short* src, unsigned long channels, unsigned short gain, short step);  // 0x00C6AD10
};
struct CCodaSystem {
    unsigned char   _pad[0x17c];
    CSoftwareMixer* m_pMixer;           // +0x17c
};
extern CCodaSystem* g_pCodaSystem;      // 0x0143BC2C

// The software voice with its own second ring at +0x1F0 (vtable 0x012B5D70, 0x250 bytes; ctor 0x00C3F760 over the
// CSoftwareVoice constructor 0x00C5B1A0): only the gain fields these rows touch are modelled.
class CSoftwareStreamVoice {
public:
    unsigned char  _pad[0x248];
    bool           m_gainRampPending;   // +0x248 a new target gain is waiting
    unsigned short m_gain;              // +0x24a current Q15 gain
    unsigned short m_targetGain;        // +0x24c
    void MixMono(const short* src);
    void Mix(const short* src, unsigned long channels);
};

void CSoftwareStreamVoice::MixMono(const short* src)
{
    if (m_gainRampPending) {
        m_gainRampPending = false;
        g_pCodaSystem->m_pMixer->MixMonoRamped(src, m_gain, (m_targetGain - m_gain) / 128);
        m_gain = m_targetGain;
    } else
        g_pCodaSystem->m_pMixer->MixMono(src, m_gain);
}