// CAudioSource::ComputePitchScale(const SPitchModulation* modulation, __int64 position) -- pitch vibrato: once the modulation has started (state 0 -> running 0x80; running or held 0x100 states compute), take the phase of a triangle wave over the 64-bit period, map it to -1..1 and set the pitch scale to 2^(wave * depth); unknown states are left alone
// Grade: DIFFER(311v311) -- scheduling only: after the pow call retail reloads the saved flags (eax) before the state (ecx), ours the state first; tried both operand orders of the final `(flags & ~0x180) | state` store
// Leaf: ?ComputePitchScale@CAudioSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71F80, length 311 bytes (`ret 0Ch`).
// Callees: __alldiv 0x00F3FC50 (period / 4, period / 2), __allrem 0x00BFEC30, _CIpow 0x00BFEB40; constants 4.0f
//          0x0123088C, 1.0f 0x0122DED8, 2.0 0x01244480.
// Layout (local; no PDB members at these offsets): flags +0x28 (bits 7..8 the modulation state), pitch scale +0x3c,
//          modulation start +0x78 and period +0x80 (64-bit sample positions); the modulation record's depth at +8.
// Shape: a `switch` on the state (0 falls into the 0x80/0x100 compute case after the start test; the default case keeps
//          its own store + return, the other cases share the final store); the pointer-free first word keeps the
//          __int64 members at their retail offsets (a virtual base would pad the vptr to 8).
#include <math.h>
struct SPitchModulation {
    unsigned char _pad[8];
    float Depth;                                 // +0x08 (octaves)
};
class CAudioSource {
public:
    void*         __vftable;                     // +0x00
    unsigned char _pad04[0x24];
    unsigned long m_flags;                       // +0x28 bits 7..8: modulation state (0x80 running, 0x100 held)
    unsigned char _pad2c[0x10];
    float         m_pitchScale;                  // +0x3c
    unsigned char _pad40[0x38];
    __int64       m_modulationStart;             // +0x78
    __int64       m_modulationPeriod;            // +0x80
    void ComputePitchScale(const SPitchModulation* modulation, __int64 position);
};

void CAudioSource::ComputePitchScale(const SPitchModulation* modulation, __int64 position)
{
    unsigned long flags = m_flags;
    unsigned long state = flags & 0x180;
    switch (state) {
    case 0:
        if (m_modulationStart > position)
            break;
        state = 0x80;
    case 0x80:
    case 0x100: {
        __int64 period = m_modulationPeriod;
        __int64 phase = (period / 4 - m_modulationStart + position) % period;
        float wave;
        if (phase < period / 2)
            wave = phase * 4.0f / period - 1.0f;
        else {
            phase -= period / 2;
            wave = 1.0f - phase * 4.0f / period;
        }
        m_pitchScale = (float)pow(2.0, (double)(wave * modulation->Depth));
        break;
    }
    default:
        m_flags = (flags & ~0x180) | state;
        return;
    }
    m_flags = state | (flags & ~0x180);
}
