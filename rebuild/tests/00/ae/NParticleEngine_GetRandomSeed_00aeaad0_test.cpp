#include <cstdio>
// Behaviour test for NParticleEngine::GetRandomSeed (0x00AEAAD0): the stand-in returns a reference to a seed word.
namespace NParticleEngine {
static unsigned long g_seed = 12345;
unsigned long& __fastcall GetRandomSeed() { return g_seed; }
}
int main()
{
    unsigned long& s = NParticleEngine::GetRandomSeed();
    s = 777;
    if (&s == &NParticleEngine::GetRandomSeed() && NParticleEngine::GetRandomSeed() == 777) { std::printf("AUTO_TINY_00aeaad0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00aeaad0_TEST FAIL\n");
    return 1;
}
