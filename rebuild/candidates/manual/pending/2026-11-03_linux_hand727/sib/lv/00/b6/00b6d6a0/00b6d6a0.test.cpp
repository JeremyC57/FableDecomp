#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

struct CEffectQualitySettings { unsigned char _pad[0x70]; bool HighendWater; };
class CEngineWaterRenderer { public: unsigned char _pad0[0x274]; bool HighendWaterEnabled; bool HighendWaterSupported;
    bool SetEffectQuality(const CEffectQualitySettings* quality) { HighendWaterEnabled = quality->HighendWater && HighendWaterSupported; return true; } };
int main()
{
    CEngineWaterRenderer r; CEffectQualitySettings q; bool ok = true;
    r.HighendWaterSupported = true; q.HighendWater = true; r.HighendWaterEnabled = false; ok = ok && r.SetEffectQuality(&q) && r.HighendWaterEnabled;
    q.HighendWater = false; ok = ok && r.SetEffectQuality(&q) && !r.HighendWaterEnabled;
    q.HighendWater = true; r.HighendWaterSupported = false; ok = ok && r.SetEffectQuality(&q) && !r.HighendWaterEnabled;
    if (!ok) { std::printf("00b6d6a0_TEST FAIL\n"); return 1; }
    std::printf("00b6d6a0_TEST PASS\n"); return 0;
}