#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_built = 0;
class CEngineWeatherRenderer { public: unsigned char _pad0[0x5c8]; float OverridenRainStrength; float OverridenSnowStrength; bool RainOverriden; bool SnowOverriden; unsigned char _pad1[0x12f8 - 0x5d2]; float MaximumWeatherParticlePercentage;
    void BuildBoxPositionMatrix() { ++g_built; }
    void StartSnowing(float strength) {
        if ((long)(1024.0f * strength) <= 0) { SnowOverriden = false; return; }
        OverridenSnowStrength = strength < MaximumWeatherParticlePercentage ? strength : MaximumWeatherParticlePercentage;
        if (!SnowOverriden) { BuildBoxPositionMatrix(); SnowOverriden = true; } } };
int main()
{
    CEngineWeatherRenderer* r = (CEngineWeatherRenderer*)std::malloc(sizeof(CEngineWeatherRenderer)); std::memset(r, 0, sizeof(*r));
    r->MaximumWeatherParticlePercentage = 0.5f; r->SnowOverriden = false; r->OverridenSnowStrength = 9.0f;
    r->StartSnowing(0.0001f);
    bool ok = !r->SnowOverriden && g_built == 0 && r->OverridenSnowStrength == 9.0f;
    r->StartSnowing(0.25f); ok = ok && r->SnowOverriden && g_built == 1 && r->OverridenSnowStrength == 0.25f;
    r->StartSnowing(0.75f); ok = ok && r->SnowOverriden && g_built == 1 && r->OverridenSnowStrength == 0.5f;
    r->StartSnowing(-1.0f); ok = ok && !r->SnowOverriden && g_built == 1 && r->OverridenSnowStrength == 0.5f;
    std::free(r);
    if (!ok) { std::printf("00b51f00_TEST FAIL\n"); return 1; }
    std::printf("00b51f00_TEST PASS\n"); return 0;
}