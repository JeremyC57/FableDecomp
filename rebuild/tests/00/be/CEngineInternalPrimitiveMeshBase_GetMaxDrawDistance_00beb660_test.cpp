#include <cstdio>
// behaviour copy of CEngineInternalPrimitiveMeshBase::GetMaxDrawDistance @ 0x00BEB660
struct SFadeSettings { float FadeDistanceScale, MinDrawDistance, MaxDrawDistance; };
static SFadeSettings g_settings;
static SFadeSettings* g_pFadeSettings = &g_settings;
class CEngineFadeDistance { public: float FadeBeginDistance, MaxDrawDistance; };
struct CPrimitiveModel {
    CEngineFadeDistance FadeDistance;
    unsigned char Enable : 1;
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    float GetMaxDrawDistance() const
    {
        float distance = FadeDistance.MaxDrawDistance;
        if (OverrideFadeDistance) {
            float maxDraw = distance * g_pFadeSettings->FadeDistanceScale;
            float minLimit = g_pFadeSettings->MinDrawDistance;
            float maxLimit = g_pFadeSettings->MaxDrawDistance;
            if (maxDraw < minLimit)
                return minLimit;
            if (maxDraw > maxLimit)
                return maxLimit;
            return maxDraw;
        }
        return distance;
    }
};
static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    CPrimitiveModel p;
    p.FadeDistance.FadeBeginDistance = 10.0f;
    p.FadeDistance.MaxDrawDistance = 100.0f;
    p.Enable = 1; p.NonPersistent = 0; p.OverrideFadeDistance = 0;
    g_settings.FadeDistanceScale = 2.0f; g_settings.MinDrawDistance = 50.0f; g_settings.MaxDrawDistance = 150.0f;
    int ok = 1;
    if (!Near(p.GetMaxDrawDistance(), 100.0f)) ok = 0;      // no override: the stored value
    p.OverrideFadeDistance = 1;
    if (!Near(p.GetMaxDrawDistance(), 150.0f)) ok = 0;      // 200 clamped to the max
    g_settings.FadeDistanceScale = 0.25f;
    if (!Near(p.GetMaxDrawDistance(), 50.0f)) ok = 0;       // 25 clamped to the min
    g_settings.FadeDistanceScale = 1.2f;
    if (!Near(p.GetMaxDrawDistance(), 120.0f)) ok = 0;      // in range
    if (!ok) { std::printf("00beb660_TEST FAIL\n"); return 1; }
    std::printf("00beb660_TEST PASS\n");
    return 0;
}