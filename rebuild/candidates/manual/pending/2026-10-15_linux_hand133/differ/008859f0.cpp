// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters(float t) -- the screen-filter parameters for the render frame: the previous frame's filter interpolated towards the current one by t
// Grade: DIFFER(112v119) -- the middle copy: the lane copies the returned temporary through the returned pointer in eax (`mov ecx,[eax] ... add eax,10h` for the Tint member) and puts the named local BELOW the temporary (`lea eax,[esp+30h]` for the hidden result); retail reads the temporary from fixed frame offsets ([esp+8]..[esp+24h], temporary at the frame bottom) with no re-based Tint. Tried: copy-init (elided, 55 bytes), POD assignment (elided), user copy ctor + out-param assignment (elided), user member-wise operator= + implicit copy (112v119).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008859F0, length 119 bytes.
// Callees: CScreenFilterParams::Interpolate(const CScreenFilterParams& to, float t) @ 0x00882FE0 (thiscall on PrevFrameScreenFilterParams, hidden
// result pointer; it clamps t to [0, 1]; manifest label "InterpolateRadialBlurParams" is propagated).
// Label correction: the manifest's "CEngineInternalPrimitive2DStaticMesh::GetObjectTransform" is a propagated name. The body sits next to
// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedDisplacementParameters (0x00885900), is called from CEnvironment::RenderUpdate
// (0x006B4889, on the interface pointer at +0x1960) and reads the PDB CEnvironmentSpellEffectInterface members ScreenFilterParams (+0x64)
// and PrevFrameScreenFilterParams (+0xe4), both CScreenFilterParams (Alpha, Saturation, Contrast, Brightness, Tint = 0x20 bytes).
// Shape: `ret 8` = hidden result + t; the call's result lands in a 0x20-byte frame temporary (at the bottom of the frame), is copied
// dword by dword (interleaved ecx/eax/edx, read from fixed frame offsets) into a second 0x20-byte local, and that local goes to the
// caller's buffer as an 8-dword `rep movsd` (an implicit bitwise copy). A user member-wise operator= plus the implicit copy constructor
// is the closest model: copy-initialisation, plain POD assignment and an out-parameter all let the lane elide the middle copy.
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
    CScreenFilterParams params;
    params = PrevFrameScreenFilterParams.Interpolate(ScreenFilterParams, t);
    return params;
}
