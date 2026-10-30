// Behaviour test for CAudioObject::SetMillibels (0x00C42FB0): a changed level is stored, its amplitude cached
// (stand-in converter) and dirty bit 2 raised; an unchanged level touches nothing.
#include <cstdio>
static int g_calls = 0;
static float Convert(long mb) { g_calls++; return mb <= -10000 ? 0.0f : 1.0f + mb * 0.001f; }
struct CAudioObject {
    unsigned char pad[0x14]; unsigned long DirtyFlags; float Amplitude; long Millibels;
    void SetMillibels(long millibels) {
        if (Millibels != millibels) { Millibels = millibels; Amplitude = Convert(millibels); DirtyFlags |= 2; }
    }
};
int main()
{
    CAudioObject o; o.DirtyFlags = 1; o.Amplitude = 7.0f; o.Millibels = 0;
    o.SetMillibels(0);
    if (g_calls != 0 || o.DirtyFlags != 1 || o.Amplitude != 7.0f) { std::printf("00c42fb0_TEST FAIL\n"); return 1; }
    o.SetMillibels(-10000);
    if (g_calls != 1 || o.DirtyFlags != 3 || o.Amplitude != 0.0f || o.Millibels != -10000) { std::printf("00c42fb0_TEST FAIL\n"); return 1; }
    std::printf("00c42fb0_TEST PASS\n"); return 0;
}