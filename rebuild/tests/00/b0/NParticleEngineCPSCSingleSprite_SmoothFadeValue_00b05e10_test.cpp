// Behaviour test for NParticleEngine::CPSCSingleSprite::SmoothFadeValue (0x00B05E10): the cosine-table fade rises from 0 at t = 0 through
// 0.5 at t = 0.5 to 1 at t = 1 (0.5 - 0.5 * cos(pi * t) read from the interpolated 1024-step table, index rounded by GFFloatToLongNear).
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongNear.h"
static float g_CosTable[1025];
struct Fade {
    float SmoothFadeValue(float t) const
    {
        t = t * 0.5f;
        t = t * 1024.0f;
        long i = GFFloatToLongNear(t - 0.5f);
        float frac = t - (float)i;
        i &= 0x3ff;
        return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
    }
};
int main()
{
    for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)cos(k * 6.283185307179586 / 1024.0);
    Fade f;
    float a = f.SmoothFadeValue(0.0f), b = f.SmoothFadeValue(0.5f), c = f.SmoothFadeValue(1.0f), d = f.SmoothFadeValue(0.25f);
    bool ok = fabs(a) < 0.01f && fabs(b - 0.5f) < 0.01f && fabs(c - 1.0f) < 0.01f && d > 0.1f && d < 0.2f;
    printf(ok ? "00b05e10_TEST PASS\n" : "00b05e10_TEST FAIL\n");
    return ok ? 0 : 1;
}