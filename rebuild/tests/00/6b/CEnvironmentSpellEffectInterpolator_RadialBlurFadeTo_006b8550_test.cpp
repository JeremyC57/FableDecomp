// Behaviour test for 0x006B8550 CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(duration, priority, threshold,
// length, intensity, fade, range, twist): builds the radial-blur block (opaque colours from the vectors, the four
// scalars) and forwards it with the same duration and priority, returning the block overload's result.
#include <stdio.h>
class C3DVector { public:
    float X, Y, Z;
};

class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};

enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_DEFAULT = 0 };

// CEnvironmentSpellEffectInterface::CRadialBlurParams (0x30 bytes, PDB layout Ego_r.pdb)
struct CRadialBlurParams {
    CRGBFloatColour ThresholdColour;   // +0x00
    CRGBFloatColour IntensityColour;   // +0x10
    float           Length;            // +0x20
    float           Fade;              // +0x24
    float           Range;             // +0x28
    float           Twist;             // +0x2c
    CRadialBlurParams() { }
};

class CEnvironmentSpellEffectInterpolator { public:
    void* RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const CRadialBlurParams& params);   // 0x006B84E0
    void* RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const C3DVector& thresholdColour, float length,
                           const C3DVector& intensityColour, float fade, float range, float twist);
};

void* CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const C3DVector& thresholdColour, float length,
                                                           const C3DVector& intensityColour, float fade, float range, float twist)
{
    CRadialBlurParams params;
    params.Fade = fade;
    params.Length = length;
    params.Range = range;
    params.Twist = twist;
    params.IntensityColour.R = intensityColour.X;
    params.IntensityColour.G = intensityColour.Y;
    params.IntensityColour.B = intensityColour.Z;
    params.IntensityColour.A = 1.0f;
    params.ThresholdColour.R = thresholdColour.X;
    params.ThresholdColour.G = thresholdColour.Y;
    params.ThresholdColour.B = thresholdColour.Z;
    params.ThresholdColour.A = 1.0f;
    return RadialBlurFadeTo(duration, priority, params);
}

static CRadialBlurParams g_seen;
static float g_duration;
static int g_priority;
static int g_token;
void* CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const CRadialBlurParams& params)
{
    g_seen = params; g_duration = duration; g_priority = (int)priority;
    return &g_token;
}

int main()
{
    int ok = 1;
    CEnvironmentSpellEffectInterpolator fx;
    C3DVector threshold = { 0.1f, 0.2f, 0.3f };
    C3DVector intensity = { 0.7f, 0.8f, 0.9f };
    void* r = fx.RadialBlurFadeTo(2.5f, (ScreenEffectPriority)3, threshold, 11.0f, intensity, 12.0f, 13.0f, 14.0f);
    if (r != &g_token || g_duration != 2.5f || g_priority != 3) ok = 0;
    if (g_seen.Length != 11.0f || g_seen.Fade != 12.0f || g_seen.Range != 13.0f || g_seen.Twist != 14.0f) ok = 0;
    if (g_seen.ThresholdColour.R != 0.1f || g_seen.ThresholdColour.G != 0.2f || g_seen.ThresholdColour.B != 0.3f
        || g_seen.ThresholdColour.A != 1.0f) ok = 0;
    if (g_seen.IntensityColour.R != 0.7f || g_seen.IntensityColour.G != 0.8f || g_seen.IntensityColour.B != 0.9f
        || g_seen.IntensityColour.A != 1.0f) ok = 0;
    printf("006b8550_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}