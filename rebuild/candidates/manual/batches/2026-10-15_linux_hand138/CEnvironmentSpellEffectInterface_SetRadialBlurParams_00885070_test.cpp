// Behaviour test for 0x00885070 CEnvironmentSpellEffectInterface::SetRadialBlurParams
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CRGBFloatColour { float R, G, B, A; };
struct CRadialBlurParams { CRGBFloatColour ThresholdColour, IntensityColour; float Length, Fade, Range, Twist; };
struct CEnvironmentSpellEffectInterface {
    CRadialBlurParams RadialBlurParams, PrevFrameRadialBlurParams;
    void SetRadialBlurParams(const C3DVector& threshold, float length, const C3DVector& intensity, float fade, float range, float twist, bool interpolate)
    {
        CRadialBlurParams params;
        params.ThresholdColour.R = threshold.x; params.ThresholdColour.G = threshold.y; params.ThresholdColour.B = threshold.z; params.ThresholdColour.A = 1.0f;
        params.IntensityColour.R = intensity.x; params.IntensityColour.G = intensity.y; params.IntensityColour.B = intensity.z; params.IntensityColour.A = 1.0f;
        params.Length = length; params.Fade = fade; params.Range = range; params.Twist = twist;
        RadialBlurParams = params;
        if (!interpolate) PrevFrameRadialBlurParams = params;
    }
};
int main()
{
    CEnvironmentSpellEffectInterface s; s.PrevFrameRadialBlurParams.Length = -1.0f;
    C3DVector t = { 0.1f, 0.2f, 0.3f }, i = { 0.4f, 0.5f, 0.6f };
    s.SetRadialBlurParams(t, 2.0f, i, 3.0f, 4.0f, 5.0f, true);
    bool ok = s.RadialBlurParams.ThresholdColour.G == 0.2f && s.RadialBlurParams.ThresholdColour.A == 1.0f && s.RadialBlurParams.IntensityColour.B == 0.6f
           && s.RadialBlurParams.Length == 2.0f && s.RadialBlurParams.Fade == 3.0f && s.RadialBlurParams.Range == 4.0f && s.RadialBlurParams.Twist == 5.0f
           && s.PrevFrameRadialBlurParams.Length == -1.0f;
    s.SetRadialBlurParams(t, 7.0f, i, 3.0f, 4.0f, 5.0f, false);
    ok = ok && s.PrevFrameRadialBlurParams.Length == 7.0f && s.PrevFrameRadialBlurParams.IntensityColour.A == 1.0f;
    if (!ok) { std::printf("00885070_TEST FAIL\n"); return 1; }
    std::printf("00885070_TEST PASS\n");
    return 0;
}
