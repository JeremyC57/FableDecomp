// CAudioSource::ComputePitchScale(const SPitchProperties* pitch, __int64 now) -- pitch vibrato: once the pitch timer has started (state 0 -> running 0x80; running or held 0x100 states compute), take the phase of a triangle wave over the 64-bit period, map it to -1..1 and set the pitch scale to 2^(wave * depth); unknown states are left alone
// Grade: DIFFER(311v311) -- one instruction: the Pitch fstp after pow is scheduled between the flags (eax) and state (ecx) reloads; retail stores after both. Tried this round: state | masked order, named float result, double result (all unchanged)
// Leaf: ?ComputePitchScale@CAudioSource@@QAEXPBUSPitchProperties@@_J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71F80, length 311 bytes (`ret 0Ch`). Sole caller: the shared property
//   update 0x00C72E00 at 0x00C72F61, which hands it the same sound-property pointer ([edi+0x20]) it passes to the landed
//   CAudioSource::SetPitchProperties 0x00C71F10 at 0x00C72F4C, so the argument is that SPitchProperties record (time +0,
//   rate +4; the depth at +8 is read here).
// Callees: __alldiv 0x00F3FC50 (period / 4, period / 2), __allrem 0x00BFEC30, _CIpow 0x00BFEB40; constants 4.0f
//   0x0123088C, 1.0f 0x0122DED8, 2.0 0x01244480.
// Names: libpk/OpenAL layer, no PDB or RTTI: the class keeps the landed CAudioSource name and model (SetPitchProperties
//   0x00C71F10: Flags +0x28, Pitch +0x3C, PitchEndTime +0x78, PitchPeriod +0x80); the method name is the catalog's
//   descriptive one and the depth member is descriptive.
// Shape: a `switch` on the state (0 falls into the 0x80/0x100 compute case after the start test; the default case breaks
//   into the shared final flags store); the pointer-free first word keeps the __int64 members at their retail offsets.
#include <math.h>
struct SPitchProperties {
    float m_time;                                        // +0x00 seconds
    float m_rate;                                        // +0x04
    float m_depth;                                       // +0x08 octaves (descriptive)
};

struct CAudioSource {
    unsigned char  _pad00[0x28];
    unsigned long  Flags;                                // +0x28 bits 7..8: pitch state (0x80 running, 0x100 held)
    unsigned char  _pad2c[0x10];
    float          Pitch;                                // +0x3C
    unsigned char  _pad40[0x38];
    __int64        PitchEndTime;                         // +0x78 the pitch timer's start
    __int64        PitchPeriod;                          // +0x80
    void ComputePitchScale(const SPitchProperties* pitch, __int64 now);
};

void CAudioSource::ComputePitchScale(const SPitchProperties* pitch, __int64 now)
{
    unsigned long flags = Flags;
    unsigned long state = flags & 0x180;
    switch (state) {
    case 0:
        if (PitchEndTime > now)
            break;
        state = 0x80;
    case 0x80:
    case 0x100: {
        __int64 period = PitchPeriod;
        __int64 phase = (period / 4 - PitchEndTime + now) % period;
        float wave;
        if (phase < period / 2)
            wave = phase * 4.0f / period - 1.0f;
        else {
            phase -= period / 2;
            wave = 1.0f - phase * 4.0f / period;
        }
        Pitch = (float)pow(2.0, (double)(wave * pitch->m_depth));
        break;
    }
    default:
        break;
    }
    Flags = (flags & ~0x180) | state;
}
