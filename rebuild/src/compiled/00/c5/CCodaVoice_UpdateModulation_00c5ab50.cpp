// CCodaVoice::UpdateModulation(const SModulationParams* mod) -- advance the second LFO one block: phase 0 waits out the start delay then enters the rising half (counter = half a period); in the rising (0x8000) / falling (0x10000) halves flip the half when the counter runs out, then multiply each modulated scale (pitch +0x88, level +0x94, the third target +0xA0) whose depth is non-zero by that half's per-block factor; the phase is written back into flag bits 15..16
// Leaf: ?UpdateModulation@CCodaVoice@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5AB50, length 294 bytes (+ int3 padding); `ret 4`.
// Caller: the software voice block update 0x00C5AED5 (desc+0x24, right after SetModulation 0x00C5A9D0 for the same
//   descriptor when its bit 0x40 is requested). No callees. Constant 0.0f @ 0x0122DEDC.
// Identity (descriptive; GameCODA has no PDB/RTTI): the twin of the landed CCodaVoice::UpdateVibrato 0x00C59890 for the
//   second LFO whose factors SetModulation 0x00C5A9D0 computes: +0x8C / +0x90 from the octave depth +8 (2^(+-2d/period)),
//   +0x98 / +0x9C from the long depth +0x10 (10^(+-k*d/period)), +0xA4 / +0xA8 from the depth +0xC, period +0xB0 from the
//   rate +4 (0.5 / rate seconds); SetModulation resets the three scales to 1.0 and the counter +0xAC to 0. The manifest's
//   "CAudioSource_UpdateRotationDamping" is propagated.
// Shape (the UpdateVibrato lesson): a switch on `m_flags & 0x18000` (0 / 0x8000 / 0x10000; the 0x18000 default writes the
//   phase straight back in its own tail copy); case 0 falls into the oscillating cases; `state = ...` comes before the
//   counter store in both arms; `!= 0.0f` is `fucompp ; test ah,44h ; jnp`.
struct SModulationParams {                              // the second LFO description (read by 0x00C5A9D0)
    float delay;                                        // +0x00 seconds
    float rate;                                         // +0x04 Hz
    float pitchDepth;                                   // +0x08 octaves (scale +0x88)
    float thirdDepth;                                   // +0x0c (scale +0xa0)
    long  levelDepth;                                   // +0x10 (scale +0x94)
};

// GameCODA voice model (no PDB/RTTI; names descriptive, the landed CCodaVoice rows). CCodaVoice = the voice base (vtable
// 0x012B5C10, 19 slots, ctor 0x00C59BA0, 0xB4 bytes): the state flags +0x28 (second LFO phase 0x18000) and the second LFO
// +0x88..+0xB0.
class CCodaVoice {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    unsigned char _pad04[0x24];
    unsigned long m_flags;                              // +0x28
    unsigned char _pad2c[0x5c];
    float m_modulationScale;                            // +0x88 second LFO pitch multiplier
    float m_modulationUp;                               // +0x8c per-block factor, rising half
    float m_modulationDown;                             // +0x90 per-block factor, falling half
    float m_levelScale;                                 // +0x94
    float m_levelUp;                                    // +0x98
    float m_levelDown;                                  // +0x9c
    float m_thirdScale;                                 // +0xa0
    float m_thirdUp;                                    // +0xa4
    float m_thirdDown;                                  // +0xa8
    long  m_modulationCounter;                          // +0xac
    long  m_modulationPeriod;                           // +0xb0 mix blocks per half cycle
    void SetModulation(const SModulationParams* mod);       // 0x00C5A9D0
    void UpdateModulation(const SModulationParams* mod);
};

void CCodaVoice::UpdateModulation(const SModulationParams* mod)
{
    unsigned long state = m_flags & 0x18000;
    switch (state) {
    case 0:
        if (m_modulationCounter-- > 0)
            break;
        state = 0x8000;
        m_modulationCounter = m_modulationPeriod / 2;
    case 0x8000:
    case 0x10000:
        if (m_modulationCounter-- <= 0) {
            state ^= 0x18000;
            m_modulationCounter = m_modulationPeriod;
        }
        if (mod->pitchDepth != 0.0f)
            m_modulationScale *= state == 0x8000 ? m_modulationUp : m_modulationDown;
        if (mod->levelDepth != 0)
            m_levelScale *= state == 0x8000 ? m_levelUp : m_levelDown;
        if (mod->thirdDepth != 0.0f)
            m_thirdScale *= state == 0x8000 ? m_thirdUp : m_thirdDown;
        break;
    }
    m_flags = (m_flags & ~0x18000) | state;
}