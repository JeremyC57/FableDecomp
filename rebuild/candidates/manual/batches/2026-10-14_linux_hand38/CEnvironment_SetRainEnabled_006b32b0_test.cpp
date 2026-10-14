#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_calls = 0; static float g_value = 5.0f;
struct IWeatherOverride { virtual void v00() { } virtual void SetRainOverride(float strength) { ++g_calls; g_value = strength; } };
struct CEngineRenderers { unsigned char _pad[0x2c]; IWeatherOverride* m_pWeather; };
struct CEngineGlobals { unsigned char _pad[0x28]; CEngineRenderers* m_pRenderers; };
static CEngineGlobals* g_pEngineGlobals = 0;
static inline IWeatherOverride* GetWeatherOverride() { return g_pEngineGlobals->m_pRenderers->m_pWeather; }
struct CEnvironment { unsigned char _pad0[0x45]; bool RainEnabled;
    void SetRainEnabled(bool enabled) { if (enabled) GetWeatherOverride()->SetRainOverride(-1.0f); else GetWeatherOverride()->SetRainOverride(0.0f); RainEnabled = enabled; } };
int main()
{
    IWeatherOverride w; CEngineRenderers rr; rr.m_pWeather = &w; CEngineGlobals g; g.m_pRenderers = &rr; g_pEngineGlobals = &g;
    CEnvironment e; e.RainEnabled = false;
    e.SetRainEnabled(true); bool ok = g_calls == 1 && g_value == -1.0f && e.RainEnabled;
    e.SetRainEnabled(false); ok = ok && g_calls == 2 && g_value == 0.0f && !e.RainEnabled;
    if (!ok) { std::printf("006b32b0_TEST FAIL\n"); return 1; }
    std::printf("006b32b0_TEST PASS\n"); return 0;
}
