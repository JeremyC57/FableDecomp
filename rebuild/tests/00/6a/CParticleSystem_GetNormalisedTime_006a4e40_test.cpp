// Behaviour test for 0x006A4E40 CParticleSystem::GetNormalisedTime: elapsed / totalTime when totalTime > 0.0f, else 0.0f (both the compare and the
// result read retail's pooled __real@00000000, 0x0122DEDC).
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include <cstdio>
struct CParticleSystem { char pad[0xb0]; float totalTime; float elapsed; };
float __fastcall CParticleSystem_GetNormalisedTime(const CParticleSystem* self);
int main()
{
    static CParticleSystem p;
    p.totalTime = 4.0f; p.elapsed = 1.0f;
    float a = CParticleSystem_GetNormalisedTime(&p);
    p.totalTime = 0.0f; p.elapsed = 3.0f;
    float b = CParticleSystem_GetNormalisedTime(&p);
    p.totalTime = 0.25f; p.elapsed = 0.125f;
    float c = CParticleSystem_GetNormalisedTime(&p);
    if (a != 0.25f || b != 0.0f || c != 0.5f) { std::printf("BAD %f %f %f\n", a, b, c); return 1; }
    std::printf("CParticleSystem_006a4e40_TEST PASS\n");
    return 0;
}
