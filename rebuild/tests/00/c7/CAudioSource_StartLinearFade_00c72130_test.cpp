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

void CAudioSource::StartLinearFade(__int64 now, float duration)
{
    FadeTicks = (__int64)(g_AudioClockRate * duration);
    float length = (float)FadeTicks;
    if (length != 0.0f) {
        FadeRate = 1.0f / length;
        FadeEndTime = (__int64)((1.0f - FadeGain) / FadeRate) + now;
    } else {
        FadeRate = 1.0f;
        FadeEndTime = now;
    }
}
static int fail() { std::printf("00c72130_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
__int64 g_AudioClockRate = 1024;
int main()
{
    CAudioSource s;
    s.FadeGain = 0.5f; s.FadeRate = 0.0f; s.FadeTicks = 0; s.FadeEndTime = 0;
    s.StartLinearFade(1000, 2.0f);
    if (s.FadeTicks != 2048 || !nearly(s.FadeRate * 2048.0f, 1.0f) || s.FadeEndTime != 1000 + 1024) return fail();
    s.FadeGain = 0.0f;
    s.StartLinearFade(10, 0.5f);
    if (s.FadeTicks != 512 || s.FadeEndTime != 10 + 512) return fail();
    s.StartLinearFade(77, 0.0f);                              // zero length: jump straight to the end
    if (s.FadeTicks != 0 || s.FadeRate != 1.0f || s.FadeEndTime != 77) return fail();
    std::printf("00c72130_TEST PASS\n");
    return 0;
}