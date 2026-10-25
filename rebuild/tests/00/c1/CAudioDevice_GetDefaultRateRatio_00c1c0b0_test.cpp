// Behaviour test for 00c1c0b0: CAudioDevice::GetDefaultRateRatio -- under the audio lock, the float ratio of the settings' unsigned rate word (+0x60) to the unsigned word at +0x6C
#include <cstdio>

static int g_lockDepth = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static int g_audioLock;

struct CDefaultSettings {
    unsigned int Words[0x18];
    unsigned int Rate;
    unsigned int Words64[2];
    unsigned int Base;
    unsigned int More[0x0C];
};

static unsigned int g_rate, g_base;
static void GetDefaultSettings(CDefaultSettings* out) { out->Rate = g_rate; out->Base = g_base; }

struct CAudioDevice {
    float GetDefaultRateRatio()
    {
        CScopedLock lock(&g_audioLock);
        CDefaultSettings settings;
        GetDefaultSettings(&settings);
        return (float)settings.Rate / (float)settings.Base;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    if (sizeof(CDefaultSettings) != 0xA0) { std::printf("00c1c0b0_TEST FAIL\n"); return 1; }
    CAudioDevice d;
    g_rate = 44100; g_base = 22050;
    if (!Near(d.GetDefaultRateRatio(), 2.0f) || g_lockDepth != 0) { std::printf("00c1c0b0_TEST FAIL\n"); return 1; }
    g_rate = 0x80000000u; g_base = 0x40000000u;      // unsigned words: the 2^32 fix-up keeps them positive
    if (!Near(d.GetDefaultRateRatio(), 2.0f)) { std::printf("00c1c0b0_TEST FAIL\n"); return 1; }
    g_rate = 11025; g_base = 44100;
    if (!Near(d.GetDefaultRateRatio(), 0.25f)) { std::printf("00c1c0b0_TEST FAIL\n"); return 1; }
    std::printf("00c1c0b0_TEST PASS\n");
    return 0;
}