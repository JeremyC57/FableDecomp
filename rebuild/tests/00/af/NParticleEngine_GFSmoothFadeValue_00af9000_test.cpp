#include <cstdio>
#include <math.h>
namespace NParticleEngine { float __fastcall GFSmoothFadeValue(float t); }
float g_CosTable[1025];
__forceinline long FloatToLong(float value) { long result;
    __asm fld value
    __asm fistp result
    return result;
}
float __fastcall NParticleEngine::GFSmoothFadeValue(float t)
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = FloatToLong(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}
int main()
{
    for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)cos(k * 6.283185307179586 / 1024.0);
    using NParticleEngine::GFSmoothFadeValue;
    float a = GFSmoothFadeValue(0.0f), b = GFSmoothFadeValue(0.5f), c = GFSmoothFadeValue(1.0f), d = GFSmoothFadeValue(0.25f);
    bool ok = fabs(a) < 0.01f && fabs(b - 0.5f) < 0.01f && fabs(c - 1.0f) < 0.01f && d > 0.1f && d < 0.2f;   // cosine ease-in-out
    std::printf(ok ? "00af9000_TEST PASS\n" : "00af9000_TEST FAIL\n");
    return ok ? 0 : 1;
}