#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
extern __int64 g_AudioClockRate;                         // 0x0139CA20 (ticks per second)

struct SPitchProperties {
    float m_time;                                        // +0x00 seconds
    float m_rate;                                        // +0x04
};

struct CAudioSource {
    unsigned char  _pad00[0x28];
    unsigned long  Flags;                                // +0x28
    unsigned char  _pad2c[4];
    float          FadeGain;                             // +0x30
    float          FadeRate;                             // +0x34 (gain change per tick)
    unsigned char  _pad38[4];
    float          Pitch;                                // +0x3C
    unsigned char  _pad40[0x18];
    unsigned long  SourceId;                             // +0x58
    unsigned char  _pad5c[0xc];
    __int64        FadeEndTime;                          // +0x68
    __int64        FadeTicks;                            // +0x70
    __int64        PitchEndTime;                         // +0x78
    __int64        PitchPeriod;                          // +0x80
    void SetPitchProperties(const SPitchProperties* pitch, __int64 now);
    void StartExponentialFade(__int64 now, float duration);
    void StartLinearFade(__int64 now, float duration);
};

void CAudioSource::SetPitchProperties(const SPitchProperties* pitch, __int64 now)
{
    Flags &= ~0x180;
    if (pitch) {
        PitchPeriod = (__int64)(g_AudioClockRate / pitch->m_rate);
        if (PitchPeriod == 0)
            PitchPeriod = 1;
    } else
        Flags &= ~0x180;
    Pitch = 1.0f;
    PitchEndTime = now;
}
static int fail() { std::printf("00c71f10_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
__int64 g_AudioClockRate = 1000000;
int main()
{
    CAudioSource s;
    s.Flags = 0x1ff; s.Pitch = 3.0f; s.PitchPeriod = 55; s.PitchEndTime = 0;
    SPitchProperties p; p.m_time = 0.5f; p.m_rate = 4.0f;
    s.SetPitchProperties(&p, 12345);
    if (s.Flags != 0x7f || s.PitchPeriod != 250000 || s.Pitch != 1.0f || s.PitchEndTime != 12345) return fail();
    p.m_rate = 4.0e7f;                                        // shorter than a tick: clamped to one
    s.SetPitchProperties(&p, 99);
    if (s.PitchPeriod != 1 || s.PitchEndTime != 99) return fail();
    s.Flags = 0x180; s.PitchPeriod = 42; s.Pitch = 0.25f;
    s.SetPitchProperties(0, 7);                               // no pitch: flags cleared, period kept
    if (s.Flags != 0 || s.PitchPeriod != 42 || s.Pitch != 1.0f || s.PitchEndTime != 7) return fail();
    std::printf("00c71f10_TEST PASS\n");
    return 0;
}