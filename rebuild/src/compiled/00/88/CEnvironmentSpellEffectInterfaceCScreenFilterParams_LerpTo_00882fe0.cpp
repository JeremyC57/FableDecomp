// CEnvironmentSpellEffectInterface::CScreenFilterParams::LerpTo(const CScreenFilterParams& to, float t) -- clamp t to [0, 1] and return the screen-filter parameters blended from this towards `to` (the four scalars and the tint colour)
// Leaf: ?LerpTo@CScreenFilterParams@CEnvironmentSpellEffectInterface@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00882FE0, 316 bytes incl. int3 padding, thiscall with a hidden result
//   pointer (`ret 0Ch`: result, to, t), returns the result pointer in eax.
// Identity: the catalog's "InterpolateRadialBlurParams" is propagated. Ego_r/FableWin PDB name set:
//   `?LerpTo@CScreenFilterParams@CEnvironmentSpellEffectInterface@@QAE?AU12@ABU12@M@Z` (non-const). Callers: the landed
//   GetRenderFrameInterpolatedScreenFilterParameters 0x008859F0 (on PrevFrameScreenFilterParams) and the
//   screen-filter process 0x00880E22 / 0x00881993. Layout (PDB, 0x20 bytes): Alpha, Saturation, Contrast, Brightness,
//   CRGBFloatColour Tint (+0x10).
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8. No callees.
// Shape: the clamp is `if (t < 0) t = 0; else if (t > 1) t = 1;` on t's own argument slot; the scalars are written
//   straight into a named result local, the tint goes through CRGBFloatColour's inline operator* (float) and operator+
//   (temporaries; the sum copied dword-wise into result.Tint), and `return result;` is the implicit bitwise copy
//   (8-dword `rep movsd`; VC7.1 has no NRVO).
class CRGBFloatColour {
public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};

class CEnvironmentSpellEffectInterface {
public:
    struct CScreenFilterParams {                    // PDB, 0x20 bytes
        float           Alpha;                      // +0x00
        float           Saturation;                 // +0x04
        float           Contrast;                   // +0x08
        float           Brightness;                 // +0x0C
        CRGBFloatColour Tint;                       // +0x10
        CScreenFilterParams LerpTo(const CScreenFilterParams& to, float t);
    };
};

CEnvironmentSpellEffectInterface::CScreenFilterParams
CEnvironmentSpellEffectInterface::CScreenFilterParams::LerpTo(const CScreenFilterParams& to, float t)
{
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    float s = 1.0f - t;
    CScreenFilterParams result;
    result.Alpha = Alpha * s + to.Alpha * t;
    result.Saturation = Saturation * s + to.Saturation * t;
    result.Contrast = Contrast * s + to.Contrast * t;
    result.Brightness = Brightness * s + to.Brightness * t;
    result.Tint = Tint * s + to.Tint * t;
    return result;
}