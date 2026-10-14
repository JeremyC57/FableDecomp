#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

struct CEnvironment { unsigned char _pad0[0x1c0]; float m_snowStrength; float m_rainStrength; float m_mistStrength;
    long IsEnvironmentRequiringWeatherMasks() { return m_rainStrength > 1.0e-4f || m_snowStrength > 1.0e-4f || m_mistStrength > 1.0e-4f; } };
int main()
{
    CEnvironment e; std::memset(&e, 0, sizeof(e));
    bool ok = e.IsEnvironmentRequiringWeatherMasks() == 0;
    e.m_rainStrength = 0.5f; ok = ok && e.IsEnvironmentRequiringWeatherMasks() == 1; e.m_rainStrength = 0.0f;
    e.m_snowStrength = 0.5f; ok = ok && e.IsEnvironmentRequiringWeatherMasks() == 1; e.m_snowStrength = 0.00005f;
    ok = ok && e.IsEnvironmentRequiringWeatherMasks() == 0;
    e.m_mistStrength = 0.001f; ok = ok && e.IsEnvironmentRequiringWeatherMasks() == 1;
    if (!ok) { std::printf("006b2c00_TEST FAIL\n"); return 1; }
    std::printf("006b2c00_TEST PASS\n"); return 0;
}