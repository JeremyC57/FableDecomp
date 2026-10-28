// Behaviour test for 0x006B1900 CAtmosProcess::MixAtmos: with a sound system, resolve the def and forward the mix (standalone copy of the logic)
#include <cstdio>
struct Sound { int calls; float mix, time; void MixAtmos(const char*, const char*, float m, float t) { ++calls; mix = m; time = t; } };
static Sound* g_pSound = 0; static int g_lookups = 0;
void MixAtmos(const char* from, const char* to, float mix, float time) { if (g_pSound) { ++g_lookups; g_pSound->MixAtmos(from, to, mix, time); } }

int main()
{
    MixAtmos("A", "B", 0.5f, 1.0f); if (g_lookups) { std::printf("006b1900_TEST FAIL\n"); return 1; }
    Sound s; s.calls = 0; g_pSound = &s; MixAtmos("A", "B", 0.25f, 2.0f);
    if (s.calls != 1 || s.mix != 0.25f || s.time != 2.0f || g_lookups != 1) { std::printf("006b1900_TEST FAIL\n"); return 1; }
    std::printf("006b1900_TEST PASS\n");
    return 0;
}