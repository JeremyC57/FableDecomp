// CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const C3DVector& thresholdColour, float length, const C3DVector& intensityColour, float fade, float range, float twist) -- build a radial-blur parameter block (opaque colours from the two vectors) and forward to the block overload
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the parameter block stores Fade before Length (statement order), so VC7.1 loads Fade into eax and Length into edx as retail
// Leaf: ?RadialBlurFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@@ABVC3DVector@@M1MMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8550, length 123 bytes (+ int3 padding), `ret 20h`.
// Callees: CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float, ScreenEffectPriority, const CRadialBlurParams&)
//   @ 0x006B84E0 (landed; `this` in ecx passed straight through). Callers: the six-float overload 0x006B8730 (landed),
//   CTCBulletTime::PlayCreatureOnHitEffect 0x0079C750 and CGameScriptInterface::RadialBlurFadeTo 0x008900D0. The catalog
//   label "vector<pair<long,CCountedPointer<CContainmentVolume>>>::_Ucopy" is propagated; the Ego_r.pdb name set holds
//   ?RadialBlurFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@NScreenEffect@@ABVC3DVector@@M1MMM@Z.
// Layout (Ego_r.pdb): CEnvironmentSpellEffectInterface::CRadialBlurParams { CRGBFloatColour ThresholdColour +0x00,
//   IntensityColour +0x10, float Length +0x20, Fade +0x24, Range +0x28, Twist +0x2c } = 0x30 bytes, a 0x30-byte frame local.
// Tried: in-place ctor init list (123v123, colours first), assignment from CRGBFloatColour temporaries (185v123), by-value colour ctor (187v123).
// Shape: the four scalars are stored first, then the two vectors copied dword by dword (intensity, then threshold), the
//   three argument pushes, and only then the two 1.0f alpha immediates.
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