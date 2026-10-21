// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters(float t) -- the screen-filter parameters for the render frame: the previous frame's filter interpolated towards the current one by t
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008859F0, length 119 bytes.
// Callees: CScreenFilterParams::Interpolate(const CScreenFilterParams& to, float t) @ 0x00882FE0 (thiscall on PrevFrameScreenFilterParams, hidden
// result pointer; it clamps t to [0, 1]; manifest label "InterpolateRadialBlurParams" is propagated).
// Label correction: the manifest's "CEngineInternalPrimitive2DStaticMesh::GetObjectTransform" is a propagated name. The body sits next to
// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedDisplacementParameters (0x00885900), is called from CEnvironment::RenderUpdate
// (0x006B4889, on the interface pointer at +0x1960) and reads the PDB CEnvironmentSpellEffectInterface members ScreenFilterParams (+0x64)
// and PrevFrameScreenFilterParams (+0xe4), both CScreenFilterParams (Alpha, Saturation, Contrast, Brightness, Tint = 0x20 bytes).
// Shape: `ret 8` = hidden result + t; the call's result is a named copy-initialised local (RVO into a 0x20-byte frame slot at
// the bottom of the frame), assigned member-wise (user operator=, read from fixed frame offsets) into a second named local,
// which goes to the caller's buffer as an 8-dword `rep movsd` (implicit bitwise copy constructor). Assigning the call result
// directly copies it through the returned pointer in eax instead.
struct CRGBFloatColour {
    float R, G, B, A;
};

struct CScreenFilterParams {
    float           Alpha;                   // +0x00
    float           Saturation;              // +0x04
    float           Contrast;                // +0x08
    float           Brightness;              // +0x0c
    CRGBFloatColour Tint;                    // +0x10
    CScreenFilterParams& operator=(const CScreenFilterParams& o)
    {
        Alpha = o.Alpha;
        Saturation = o.Saturation;
        Contrast = o.Contrast;
        Brightness = o.Brightness;
        Tint = o.Tint;
        return *this;
    }
    CScreenFilterParams Interpolate(const CScreenFilterParams& to, float t) const;   // 0x00882FE0
};

struct CEnvironmentSpellEffectInterface {
    unsigned char       _pad00[0x64];        // Environment, RadialBlurParams, RadialBlurPositionParams, DisplacementParams
    CScreenFilterParams ScreenFilterParams;          // +0x64
    unsigned char       _pad84[0xe4 - 0x84];        // PrevFrameRadialBlurParams, PrevFrameRadialBlurPositionParams, PrevFrameDisplacementParams
    CScreenFilterParams PrevFrameScreenFilterParams; // +0xe4
    CScreenFilterParams GetRenderFrameInterpolatedScreenFilterParameters(float t) const;
};

CScreenFilterParams CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters(float t) const
{
    CScreenFilterParams interpolated = PrevFrameScreenFilterParams.Interpolate(ScreenFilterParams, t);
    CScreenFilterParams params;
    params = interpolated;
    return params;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static float Lerp(float a, float b, float t) { return a + (b - a) * t; }
CScreenFilterParams CScreenFilterParams::Interpolate(const CScreenFilterParams& to, float t) const
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    CScreenFilterParams r;
    r.Alpha = Lerp(Alpha, to.Alpha, t);
    r.Saturation = Lerp(Saturation, to.Saturation, t);
    r.Contrast = Lerp(Contrast, to.Contrast, t);
    r.Brightness = Lerp(Brightness, to.Brightness, t);
    r.Tint.R = Lerp(Tint.R, to.Tint.R, t);
    r.Tint.G = Lerp(Tint.G, to.Tint.G, t);
    r.Tint.B = Lerp(Tint.B, to.Tint.B, t);
    r.Tint.A = Lerp(Tint.A, to.Tint.A, t);
    return r;
}
int main()
{
    static CEnvironmentSpellEffectInterface ei;
    memset(&ei, 0, sizeof(ei));
    CScreenFilterParams& cur = ei.ScreenFilterParams;
    CScreenFilterParams& prev = ei.PrevFrameScreenFilterParams;
    cur.Alpha = 1.0f; cur.Saturation = 2.0f; cur.Contrast = 4.0f; cur.Brightness = 8.0f;
    cur.Tint.R = 1.0f; cur.Tint.G = 0.5f; cur.Tint.B = 0.25f; cur.Tint.A = 1.0f;
    prev.Alpha = 0.0f; prev.Saturation = 0.0f; prev.Contrast = 2.0f; prev.Brightness = 4.0f;
    prev.Tint.R = 0.0f; prev.Tint.G = 0.0f; prev.Tint.B = 0.75f; prev.Tint.A = 0.0f;
    CScreenFilterParams r = ei.GetRenderFrameInterpolatedScreenFilterParameters(0.5f);
    CHECK(r.Alpha == 0.5f && r.Saturation == 1.0f && r.Contrast == 3.0f && r.Brightness == 6.0f);
    CHECK(r.Tint.R == 0.5f && r.Tint.G == 0.25f && r.Tint.B == 0.5f && r.Tint.A == 0.5f);
    r = ei.GetRenderFrameInterpolatedScreenFilterParameters(2.0f);
    CHECK(r.Alpha == 1.0f && r.Brightness == 8.0f && r.Tint.B == 0.25f);
    r = ei.GetRenderFrameInterpolatedScreenFilterParameters(-1.0f);
    CHECK(r.Alpha == 0.0f && r.Contrast == 2.0f && r.Tint.B == 0.75f);
    if (fails == 0) printf("008859f0_TEST PASS\n");
    return fails;
}