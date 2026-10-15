// CEnvironmentSpellEffectInterface::SetRadialBlurParams(const C3DVector& threshold, float length, const C3DVector& intensity, float fade, float range, float twist, bool interpolate) -- build a radial-blur parameter block (opaque threshold and intensity colours) and store it as the current parameters, and also as the previous frame's when not interpolating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00885070, 149 bytes (+ int3 padding).
// Callees: none. Constant: 0x3F800000 = 1.0f (both colour alphas, stored as immediates).
// Layout (Ego_r.pdb, matches retail): CEnvironmentSpellEffectInterface { CEnvironment* Environment +0; CRadialBlurParams
//   RadialBlurParams +4 (48 bytes: ThresholdColour, IntensityColour, Length, Fade, Range, Twist); ...;
//   CRadialBlurParams PrevFrameRadialBlurParams +0x84 }. Modelled locally (no generated header for the class).
// Shape: the block is built in a 0x30-byte frame local and copied twice with `rep movsd` (12 dwords) -- a POD
//   assignment; the bool is tested once, before the first copy (the flags survive `rep movsd`).
struct C3DVector { float x, y, z; };

struct CRGBFloatColour { float R, G, B, A; };

struct CRadialBlurParams {
    CRGBFloatColour ThresholdColour;                   // +0x00
    CRGBFloatColour IntensityColour;                   // +0x10
    float           Length;                            // +0x20
    float           Fade;                              // +0x24
    float           Range;                             // +0x28
    float           Twist;                             // +0x2c
};

struct CEnvironment;

struct CEnvironmentSpellEffectInterface {
    CEnvironment*     Environment;                     // +0x00
    CRadialBlurParams RadialBlurParams;                // +0x04
    unsigned char     _pad34[0x84 - 0x34];             // position, displacement and screen-filter params
    CRadialBlurParams PrevFrameRadialBlurParams;       // +0x84
    void SetRadialBlurParams(const C3DVector& threshold, float length, const C3DVector& intensity,
                             float fade, float range, float twist, bool interpolate);
};

void CEnvironmentSpellEffectInterface::SetRadialBlurParams(const C3DVector& threshold, float length, const C3DVector& intensity,
                                                           float fade, float range, float twist, bool interpolate)
{
    CRadialBlurParams params;
    params.ThresholdColour.R = threshold.x;
    params.ThresholdColour.G = threshold.y;
    params.ThresholdColour.B = threshold.z;
    params.ThresholdColour.A = 1.0f;
    params.IntensityColour.R = intensity.x;
    params.IntensityColour.G = intensity.y;
    params.IntensityColour.B = intensity.z;
    params.IntensityColour.A = 1.0f;
    params.Length = length;
    params.Fade = fade;
    params.Range = range;
    params.Twist = twist;
    RadialBlurParams = params;
    if (!interpolate)
        PrevFrameRadialBlurParams = params;
}