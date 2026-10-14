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
struct CEngineWeatherRenderer { unsigned char _pad0[0x5c8]; float OverridenRainStrength; float OverridenSnowStrength; bool RainOverriden; bool SnowOverriden; unsigned char _pad1[0x12f8 - 0x5d2]; float MaximumWeatherParticlePercentage;
    void BuildBoxPositionMatrix() { ++g_built; }
    void StartRaining(float strength) {
        if ((long)(128.0f * strength) <= 0) { RainOverriden = false; return; }
        OverridenRainStrength = strength < MaximumWeatherParticlePercentage ? strength : MaximumWeatherParticlePercentage;
        if (!RainOverriden) { BuildBoxPositionMatrix(); RainOverriden = true; } } };
int main()
{
    CEngineWeatherRenderer* r = (CEngineWeatherRenderer*)std::malloc(sizeof(CEngineWeatherRenderer)); std::memset(r, 0, sizeof(*r));
    r->MaximumWeatherParticlePercentage = 0.5f; r->RainOverriden = false; r->OverridenRainStrength = 9.0f;
    r->StartRaining(0.0001f);
    bool ok = !r->RainOverriden && g_built == 0 && r->OverridenRainStrength == 9.0f;
    r->StartRaining(0.25f); ok = ok && r->RainOverriden && g_built == 1 && r->OverridenRainStrength == 0.25f;
    r->StartRaining(0.75f); ok = ok && r->RainOverriden && g_built == 1 && r->OverridenRainStrength == 0.5f;
    r->StartRaining(-1.0f); ok = ok && !r->RainOverriden && g_built == 1 && r->OverridenRainStrength == 0.5f;
    std::free(r);
    if (!ok) { std::printf("00b51ea0_TEST FAIL\n"); return 1; }
    std::printf("00b51ea0_TEST PASS\n"); return 0;
}
