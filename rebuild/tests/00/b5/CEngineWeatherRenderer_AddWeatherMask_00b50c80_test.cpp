#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

class CWeatherMask { public: float BSphere[4]; float BBox[6]; float InvTransform[12]; float Scale; };
class CEngineWeatherRenderer { public: unsigned char _pad0[0x66c]; CWeatherMask WeatherMaskList[3]; long CurrentMask;
    void AddWeatherMask(const CWeatherMask& mask, long unused) { WeatherMaskList[CurrentMask] = mask; CurrentMask = (CurrentMask + 1) % 3; } };
int main()
{
    CEngineWeatherRenderer* r = (CEngineWeatherRenderer*)std::malloc(sizeof(CEngineWeatherRenderer)); std::memset(r, 0, sizeof(*r));
    CWeatherMask m; std::memset(&m, 0, sizeof(m)); m.Scale = 2.5f; m.BSphere[3] = 9.0f;
    r->CurrentMask = 2; r->AddWeatherMask(m, 0);
    bool ok = sizeof(CWeatherMask) == 0x5c && r->CurrentMask == 0 && r->WeatherMaskList[2].Scale == 2.5f && r->WeatherMaskList[2].BSphere[3] == 9.0f;
    m.Scale = 1.0f; r->AddWeatherMask(m, 0); ok = ok && r->CurrentMask == 1 && r->WeatherMaskList[0].Scale == 1.0f && r->WeatherMaskList[2].Scale == 2.5f;
    std::free(r);
    if (!ok) { std::printf("00b50c80_TEST FAIL\n"); return 1; }
    std::printf("00b50c80_TEST PASS\n"); return 0;
}