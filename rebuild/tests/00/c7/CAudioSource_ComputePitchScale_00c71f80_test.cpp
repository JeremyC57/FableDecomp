// Behaviour test for CAudioSource::ComputePitchScale (0x00C71F80): triangle-wave vibrato phase -> pitch scale 2^(wave * depth).
#include <cstdio>
#include <math.h>
typedef long long int64;
struct SPitchProperties {
    float m_time;
    float m_rate;
    float m_depth;
};
class CAudioSource {
public:
    void*         __vftable;
    unsigned char _pad04[0x24];
    unsigned long Flags;
    unsigned char _pad2c[0x10];
    float         Pitch;
    unsigned char _pad40[0x38];
    int64         PitchEndTime;
    int64         PitchPeriod;
    void ComputePitchScale(const SPitchProperties* pitch, int64 now);
};
void CAudioSource::ComputePitchScale(const SPitchProperties* pitch, int64 now)
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
        int64 period = PitchPeriod;
        int64 phase = (period / 4 - PitchEndTime + now) % period;
        float wave;
        if (phase < period / 2)
            wave = phase * 4.0f / period - 1.0f;
        else {
            phase -= period / 2;
            wave = 1.0f - phase * 4.0f / period;
        }
        Pitch = (float)pow(2.0f, wave * pitch->m_depth);
        break;
    }
    default:
        Flags = (flags & ~0x180) | state;
        return;
    }
    Flags = state | (flags & ~0x180);
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    CAudioSource s;
    SPitchProperties p;
    p.m_time = 0.0f; p.m_rate = 2.0f; p.m_depth = 1.0f;
    bool ok = true;
    // not started yet: state 0 and the start lies ahead -> nothing computed, state bits stay clear
    s.Flags = 0x0F; s.Pitch = 5.0f; s.PitchEndTime = 1000; s.PitchPeriod = 400;
    s.ComputePitchScale(&p, 999);
    if (s.Flags != 0x0F || !Near(s.Pitch, 5.0f)) ok = false;
    // starts now: phase = period/4 -> wave 0 -> pitch 1, state becomes running (0x80)
    s.Flags = 0x0F; s.Pitch = 5.0f;
    s.ComputePitchScale(&p, 1000);
    if (s.Flags != 0x8F || !Near(s.Pitch, 1.0f)) ok = false;
    // running, a quarter period later: phase = period/2 -> wave 1 -> pitch 2^depth
    s.Flags = 0x80 | 0x201; s.Pitch = 0.0f;
    s.ComputePitchScale(&p, 1100);
    if (s.Flags != (0x80 | 0x201) || !Near(s.Pitch, 2.0f)) ok = false;
    // held (0x100), three quarters later: phase = 0 -> wave -1 -> pitch 2^-depth; the other flag bits survive
    s.Flags = 0x100 | 0x40; s.Pitch = 0.0f; p.m_depth = 2.0f;
    s.ComputePitchScale(&p, 1300);
    if (s.Flags != (0x100 | 0x40) || !Near(s.Pitch, 0.25f)) ok = false;
    // unknown state (both bits): left alone, pitch untouched
    s.Flags = 0x180 | 0x3; s.Pitch = 7.0f;
    s.ComputePitchScale(&p, 1300);
    if (s.Flags != (0x180 | 0x3) || !Near(s.Pitch, 7.0f)) ok = false;
    if (!ok) { std::printf("00c71f80_TEST FAIL\n"); return 1; }
    std::printf("00c71f80_TEST PASS\n");
    return 0;
}