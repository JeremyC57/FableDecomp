// Behaviour test for 00c1c030: CAudioDevice::GetDefaultRate -- under the audio lock, read the library default settings block and return its rate word (+0x60)
#include <cstdio>

static int g_lockDepth = 0, g_maxDepth = 0, g_settingsCalls = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_lockDepth; if (g_lockDepth > g_maxDepth) g_maxDepth = g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static int g_audioLock;

struct CDefaultSettings {
    unsigned int Words[0x18];
    unsigned int Rate;
    unsigned int More[0x0F];
};

static void GetDefaultSettings(CDefaultSettings* out)
{
    ++g_settingsCalls;
    if (g_lockDepth != 1) return;              // must be called under the lock
    for (int i = 0; i < 0x18; ++i) out->Words[i] = 0xAAAAAAAAu;
    out->Rate = 44100;
    for (int i = 0; i < 0x0F; ++i) out->More[i] = 0x55555555u;
}

struct CAudioDevice {
    unsigned int GetDefaultRate()
    {
        CScopedLock lock(&g_audioLock);
        CDefaultSettings settings;
        GetDefaultSettings(&settings);
        return settings.Rate;
    }
};

int main()
{
    if (sizeof(CDefaultSettings) != 0xA0) { std::printf("00c1c030_TEST FAIL\n"); return 1; }
    CAudioDevice d;
    if (d.GetDefaultRate() != 44100 || g_settingsCalls != 1 || g_lockDepth != 0 || g_maxDepth != 1) {
        std::printf("00c1c030_TEST FAIL\n"); return 1;
    }
    std::printf("00c1c030_TEST PASS\n");
    return 0;
}