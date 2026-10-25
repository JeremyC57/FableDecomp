// CEnvironmentSpellEffectInterface::InterpolateRadialBlurParams(float t, const CRadialBlurParams& from, const CRadialBlurParams& to) -- clamp t to [0, 1] and return the radial-blur parameters blended from `from` towards `to` (four scalars, then the threshold and intensity colours)
// Leaf: ?InterpolateRadialBlurParams@CEnvironmentSpellEffectInterface@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008854A0, 480 bytes incl. int3 padding, static `__fastcall` with a hidden
//   result pointer (ecx = result, edx = from, stack: t, to; `ret 8`), returns the result pointer in eax.
// Identity: Ego_r/FableWin PDB name set: `?InterpolateRadialBlurParams@CEnvironmentSpellEffectInterface@@SI?AUCRadialBlurParams@1@MABU21@0@Z`
//   (the catalog label agrees). Single caller: the landed GetRenderFrameInterpolatedRadialBlurParameters 0x008857E0
//   (from = PrevFrameRadialBlurParams, to = RadialBlurParams). Layout (PDB, 0x30 bytes): CRGBFloatColour
//   ThresholdColour +0, CRGBFloatColour IntensityColour +0x10, float Length +0x20, Fade +0x24, Range +0x28, Twist +0x2C.
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8. No callees.
// Shape: the clamp is `if (t < 0) t = 0; else if (t > 1) t = 1;` on t's own argument slot; Fade, Length, Range and Twist
//   (in that order) are written straight into a named result local, the colours go through CRGBFloatColour's inline
//   operator* (float) and operator+ (temporaries, the sums copied dword-wise), and `return result;` is the implicit
//   bitwise copy (12-dword `rep movsd`; VC7.1 has no NRVO).
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
    struct CRadialBlurParams {                      // PDB, 0x30 bytes
        CRGBFloatColour ThresholdColour;            // +0x00
        CRGBFloatColour IntensityColour;            // +0x10
        float           Length;                     // +0x20
        float           Fade;                       // +0x24
        float           Range;                      // +0x28
        float           Twist;                      // +0x2C
        CRadialBlurParams() { }                     // PDB user default ctor (non-POD: hidden result pointer in ecx)
    };
    static CRadialBlurParams __fastcall InterpolateRadialBlurParams(float t, const CRadialBlurParams& from,
                                                                   const CRadialBlurParams& to);
};

CEnvironmentSpellEffectInterface::CRadialBlurParams __fastcall
CEnvironmentSpellEffectInterface::InterpolateRadialBlurParams(float t, const CRadialBlurParams& from, const CRadialBlurParams& to)
{
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    CRadialBlurParams result;
    result.Fade = from.Fade * (1.0f - t) + to.Fade * t;
    result.Length = from.Length * (1.0f - t) + to.Length * t;
    result.Range = from.Range * (1.0f - t) + to.Range * t;
    result.Twist = from.Twist * (1.0f - t) + to.Twist * t;
    result.ThresholdColour = from.ThresholdColour * (1.0f - t) + to.ThresholdColour * t;
    result.IntensityColour = from.IntensityColour * (1.0f - t) + to.IntensityColour * t;
    return result;
}