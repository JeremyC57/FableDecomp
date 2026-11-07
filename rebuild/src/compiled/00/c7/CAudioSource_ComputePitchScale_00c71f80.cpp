// CAudioSource::ComputePitchScale(const SPitchProperties* pitch, __int64 now) -- pitch vibrato: once the modulation has started (state 0 -> running 0x80; running or held 0x100 states compute), take the phase of a triangle wave over the 64-bit period, map it to -1..1 and set the pitch scale to 2^(wave * depth); unknown states are left alone
// Leaf: ?ComputePitchScale@CAudioSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71F80, length 311 bytes (`ret 0Ch`).
// Callees: __alldiv 0x00F3FC50 (period / 4, period / 2), __allrem 0x00BFEC30, _CIpow 0x00BFEB40 (through <math.h>'s C++
//          `float pow(float, float)` overload, as the landed CCodaVoice::SetVibrato 0x00C59C60); constants 4.0f 0x0123088C,
//          1.0f 0x0122DED8, 2.0 0x01244480.
// Name: descriptive (libpk / GameCODA OpenAL source, no PDB). The class is the landed CAudioSource of SetPitchProperties
//          0x00C71F10 / InitializeProperties 0x00C72AF0 / ApplySound 0x00C72E00 (which calls this row); the pitch record is the
//          sibling's SPitchProperties (m_time +0, m_rate +4) extended with the depth at +8 (the only field this body reads).
//          The two landed callers bind the record parameter as `const float*` (PBM_J); the sibling's own symbol uses the
//          struct (PBUSPitchProperties@@_J@Z): NAME_FIXES.tsv asks for the struct spelling at both call sites.
// Layout: the landed CAudioSource model: Flags +0x28 (bits 7..8 the vibrato state: 0x80 running, 0x100 held), Pitch +0x3C,
//          PitchEndTime +0x78 and PitchPeriod +0x80 (64-bit clock ticks). The pointer-free first word keeps the __int64
//          members at their retail offsets (a virtual base would pad the vptr to 8).
// Shape: a `switch` on the state (0 falls into the 0x80/0x100 compute case after the start test; the default case keeps
//          its own store + return, the other cases share the final store). The pitch is `pow(2.0f, wave * depth)` through the
//          float overload (the inline `powf` body): with a double `pow(2.0, (double)x)` VC7.1 stores the result before
//          reloading the flags and state for the shared tail; the float overload reloads them first, as retail does.
#include <math.h>
struct SPitchProperties {
    float m_time;                                        // +0x00 seconds
    float m_rate;                                        // +0x04
    float m_depth;                                       // +0x08 octaves
};
class CAudioSource {
public:
    void*         __vftable;                             // +0x00
    unsigned char _pad04[0x24];
    unsigned long Flags;                                 // +0x28 bits 7..8: modulation state (0x80 running, 0x100 held)
    unsigned char _pad2c[0x10];
    float         Pitch;                                 // +0x3c
    unsigned char _pad40[0x38];
    __int64       PitchEndTime;                          // +0x78 (the modulation start, in clock ticks)
    __int64       PitchPeriod;                           // +0x80
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
        Pitch = pow(2.0f, wave * pitch->m_depth);
        break;
    }
    default:
        Flags = (flags & ~0x180) | state;
        return;
    }
    Flags = state | (flags & ~0x180);
}