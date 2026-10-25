// Behaviour test for 00c334d0: AudioSystem_Create -- returns the global system when its initialiser succeeds, else 0
#include <cstdio>
struct CAudioSystem {
    unsigned long flags; const void* desc;
    bool Initialise(unsigned long f, const void* d) { flags = f; desc = d; return d != 0; }
};
static CAudioSystem g_audioSystem;
static CAudioSystem* AudioSystem_Create(unsigned long flags, const void* desc)
{
    return g_audioSystem.Initialise(flags, desc) ? &g_audioSystem : 0;
}
int main()
{
    int d = 0;
    if (AudioSystem_Create(7, &d) != &g_audioSystem || g_audioSystem.flags != 7 || g_audioSystem.desc != &d) { std::printf("00c334d0_TEST FAIL\n"); return 1; }
    if (AudioSystem_Create(0, 0) != 0) { std::printf("00c334d0_TEST FAIL\n"); return 1; }
    std::printf("00c334d0_TEST PASS\n");
    return 0;
}