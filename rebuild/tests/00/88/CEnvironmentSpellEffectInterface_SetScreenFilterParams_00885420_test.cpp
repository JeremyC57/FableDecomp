// Behaviour test for 0x00885420 CEnvironmentSpellEffectInterface::SetScreenFilterParams
#include <cstdio>
class CRGBFloatColour { public: float R, G, B, A; };
struct CScreenFilterParams { float Alpha, Saturation, Contrast, Brightness; CRGBFloatColour Tint; };
class CEnvironmentSpellEffectInterface { public:
    CScreenFilterParams ScreenFilterParams, PrevFrameScreenFilterParams;
    void SetScreenFilterParams(float alpha, float saturation, float contrast, float brightness, const CRGBFloatColour& tint, bool interpolate)
    {
        CScreenFilterParams params;
        params.Alpha = alpha; params.Saturation = saturation; params.Contrast = contrast; params.Brightness = brightness; params.Tint = tint;
        ScreenFilterParams = params;
        if (!interpolate) PrevFrameScreenFilterParams = params;
    }
};
int main()
{
    CEnvironmentSpellEffectInterface s; s.PrevFrameScreenFilterParams.Alpha = -1.0f;
    CRGBFloatColour tint = { 0.25f, 0.5f, 0.75f, 1.0f };
    s.SetScreenFilterParams(0.5f, 1.5f, 2.5f, 3.5f, tint, true);
    bool ok = s.ScreenFilterParams.Alpha == 0.5f && s.ScreenFilterParams.Brightness == 3.5f && s.ScreenFilterParams.Tint.B == 0.75f && s.PrevFrameScreenFilterParams.Alpha == -1.0f;
    s.SetScreenFilterParams(0.75f, 1.5f, 2.5f, 3.5f, tint, false);
    ok = ok && s.PrevFrameScreenFilterParams.Alpha == 0.75f && s.PrevFrameScreenFilterParams.Tint.G == 0.5f;
    if (!ok) { std::printf("00885420_TEST FAIL\n"); return 1; }
    std::printf("00885420_TEST PASS\n");
    return 0;
}