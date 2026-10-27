#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
long g_framesPerSecond_01375550 = 15;
const float g_minPositive_0129ba3c = 0.0001f;
class CHeroStatsDef { public: unsigned char _pad00[0x80]; float FatnessReductionRate; };
class CGameDefinitionManager { public: unsigned char _pad00[0xdc]; CHeroStatsDef* PHeroStatsDef; };
static CGameDefinitionManager g_defs; static CHeroStatsDef g_heroDef;
class CTCBase { public: virtual ~CTCBase() { } class CThing* ParentThing; const CGameDefinitionManager& GetDefinitionManager() const { return g_defs; } };
class CTCHeroStats : public CTCBase { public:
    long GetFatnessReducingFrequencyInFrames() const
    {
        float rate = GetDefinitionManager().PHeroStatsDef->FatnessReductionRate;
        if (rate > g_minPositive_0129ba3c) return CeilToLong(g_framesPerSecond_01375550 * rate * 60.0f);
        return g_framesPerSecond_01375550 * 2440;
    }
};
int main()
{
    g_defs.PHeroStatsDef = &g_heroDef; CTCHeroStats h;
    g_heroDef.FatnessReductionRate = 0.5f;                   // 15 * 0.5 * 60 = 450
    bool ok = h.GetFatnessReducingFrequencyInFrames() == 450;
    g_heroDef.FatnessReductionRate = 0.3333f;                // 299.97 -> 300
    ok = ok && h.GetFatnessReducingFrequencyInFrames() == 300;
    g_heroDef.FatnessReductionRate = 0.0f;                   // 15 * 2440
    ok = ok && h.GetFatnessReducingFrequencyInFrames() == 36600;
    std::printf(ok ? "0057a60a_TEST PASS\n" : "0057a60a_TEST FAIL\n");
    return ok ? 0 : 1;
}