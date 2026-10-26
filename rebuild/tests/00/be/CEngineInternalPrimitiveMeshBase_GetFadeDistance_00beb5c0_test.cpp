#include <cstdio>
struct SFadeSettings { float FadeDistanceScale, MinDrawDistance, MaxDrawDistance; };
static SFadeSettings g_settings;
static SFadeSettings* g_pFadeSettings = &g_settings;
class CEngineFadeDistance { public:
    float FadeBeginDistance, MaxDrawDistance;
    CEngineFadeDistance() { }
    CEngineFadeDistance(float b, float m) : FadeBeginDistance(b), MaxDrawDistance(m) { }
};
struct CMesh {
    CEngineFadeDistance FadeDistance;
    unsigned char Enable : 1; unsigned char NonPersistent : 1; unsigned char OverrideFadeDistance : 1;
    CEngineFadeDistance GetFadeDistance() const
    {
        CEngineFadeDistance result;
        if (OverrideFadeDistance) {
            float scale = g_pFadeSettings->FadeDistanceScale;
            result.FadeBeginDistance = scale * FadeDistance.FadeBeginDistance;
            result.MaxDrawDistance = scale * FadeDistance.MaxDrawDistance;
            float minDraw = g_pFadeSettings->MinDrawDistance;
            float maxDraw = g_pFadeSettings->MaxDrawDistance;
            if (result.MaxDrawDistance < minDraw) {
                result.FadeBeginDistance *= minDraw / result.MaxDrawDistance;
                result.MaxDrawDistance = minDraw;
            } else if (result.MaxDrawDistance > maxDraw) {
                result.FadeBeginDistance *= maxDraw / result.MaxDrawDistance;
                result.MaxDrawDistance = maxDraw;
            }
        } else {
            result = FadeDistance;
        }
        return result;
    }
};
int main()
{
    g_settings.FadeDistanceScale = 2.0f; g_settings.MinDrawDistance = 40.0f; g_settings.MaxDrawDistance = 100.0f;
    CMesh m; m.Enable = 1; m.NonPersistent = 0; m.OverrideFadeDistance = 0; m.FadeDistance = CEngineFadeDistance(5.0f, 10.0f);
    CEngineFadeDistance d = m.GetFadeDistance();
    if (d.FadeBeginDistance != 5.0f || d.MaxDrawDistance != 10.0f) { std::printf("00beb5c0_TEST FAIL\n"); return 1; }
    m.OverrideFadeDistance = 1;                       // scaled 10/20 -> clamped up to 40 (ratio 2)
    d = m.GetFadeDistance();
    if (d.FadeBeginDistance != 20.0f || d.MaxDrawDistance != 40.0f) { std::printf("00beb5c0_TEST FAIL\n"); return 1; }
    m.FadeDistance = CEngineFadeDistance(50.0f, 100.0f);   // scaled 100/200 -> clamped down to 100 (ratio 0.5)
    d = m.GetFadeDistance();
    if (d.FadeBeginDistance != 50.0f || d.MaxDrawDistance != 100.0f) { std::printf("00beb5c0_TEST FAIL\n"); return 1; }
    m.FadeDistance = CEngineFadeDistance(10.0f, 30.0f);    // scaled 20/60 -> inside the range
    d = m.GetFadeDistance();
    if (d.FadeBeginDistance != 20.0f || d.MaxDrawDistance != 60.0f) { std::printf("00beb5c0_TEST FAIL\n"); return 1; }
    std::printf("00beb5c0_TEST PASS\n");
    return 0;
}