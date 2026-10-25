// Behaviour test for 00c35ec0: AudioSystem_SetOutputFlags -- error -3 for each invalid combination, the flags are applied regardless
#include <cstdio>
struct CAudioSystem {
    int errors; long lastError; unsigned int applied;
    void SetError(long code) { ++errors; lastError = code; }
    void SetOutputFlags(unsigned int f) { applied = f; }
};
static CAudioSystem g_audioSystem;
static void AudioSystem_SetOutputFlags(unsigned int flags)
{
    if (flags & 0xFFF8C000u) g_audioSystem.SetError(-3);
    if ((flags & 0x2000) && (flags & 0x80)) g_audioSystem.SetError(-3);
    if ((flags & 0x2000) && (flags & 0x1000)) g_audioSystem.SetError(-3);
    g_audioSystem.SetOutputFlags(flags);
}
int main()
{
    AudioSystem_SetOutputFlags(0x2001);
    if (g_audioSystem.errors != 0 || g_audioSystem.applied != 0x2001) { std::printf("00c35ec0_TEST FAIL\n"); return 1; }
    AudioSystem_SetOutputFlags(0x3080);
    if (g_audioSystem.errors != 2 || g_audioSystem.lastError != -3 || g_audioSystem.applied != 0x3080) { std::printf("00c35ec0_TEST FAIL\n"); return 1; }
    AudioSystem_SetOutputFlags(0x00080000);
    if (g_audioSystem.errors != 3) { std::printf("00c35ec0_TEST FAIL\n"); return 1; }
    AudioSystem_SetOutputFlags(0x1080);
    if (g_audioSystem.errors != 3) { std::printf("00c35ec0_TEST FAIL\n"); return 1; }
    std::printf("00c35ec0_TEST PASS\n");
    return 0;
}