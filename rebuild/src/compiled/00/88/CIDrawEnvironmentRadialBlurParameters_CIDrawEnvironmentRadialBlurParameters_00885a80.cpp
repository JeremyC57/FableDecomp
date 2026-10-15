// CIDrawEnvironmentRadialBlurParameters::CIDrawEnvironmentRadialBlurParameters(const CIDrawEnvironmentRadialBlurParameters& other) -- memberwise copy of the radial-blur draw parameters (two centres, two colours, four scalars, two track modes)
// Leaf: ??0CIDrawEnvironmentRadialBlurParameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00885A80, length 139 bytes, `ret 4`, returns `this`.
// Label: the catalog's CPreTransposedBoneMatrix::Set is propagated. The only caller (0x008858E5) builds its by-value
// result in the hidden return slot with this function, right after interpolating the CRadialBlurPositionParams of
// CEnvironmentSpellEffectInterface; the 0x50-byte layout is Ego_r.pdb CIDrawEnvironmentRadialBlurParameters.
// Layout: BlurCenter +0x00, AreaCenter +0x0c (C3DVector), ThresholdColour +0x18, IntensityColour +0x28
// (CRGBFloatColour), Length/Fade/Range/Twist +0x38..+0x44, BlurTrackPos +0x48, AreaTrackPos +0x4c.
// Shape: an out-of-line memberwise copy constructor. C3DVector has a memberwise USER copy constructor (its three
// words copy straight from the source); CRGBFloatColour copies IMPLICITLY, which VC forms through re-based address
// registers (`lea edx,[ecx+18h] ; lea esi,[eax+18h]`). The scalars are plain dword moves.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

struct CRGBFloatColour {                        // implicit copy constructor
    float R, G, B, A;
};

enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { RADIAL_BLUR_TRACK_NONE };

struct CIDrawEnvironmentRadialBlurParameters {
    C3DVector       BlurCenter;                 // +0x00
    C3DVector       AreaCenter;                 // +0x0c
    CRGBFloatColour ThresholdColour;            // +0x18
    CRGBFloatColour IntensityColour;            // +0x28
    float           Length;                     // +0x38
    float           Fade;                       // +0x3c
    float           Range;                      // +0x40
    float           Twist;                      // +0x44
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;   // +0x48
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;   // +0x4c
    CIDrawEnvironmentRadialBlurParameters(const CIDrawEnvironmentRadialBlurParameters& other);
};

CIDrawEnvironmentRadialBlurParameters::CIDrawEnvironmentRadialBlurParameters(const CIDrawEnvironmentRadialBlurParameters& other)
    : BlurCenter(other.BlurCenter)
    , AreaCenter(other.AreaCenter)
    , ThresholdColour(other.ThresholdColour)
    , IntensityColour(other.IntensityColour)
    , Length(other.Length)
    , Fade(other.Fade)
    , Range(other.Range)
    , Twist(other.Twist)
    , BlurTrackPos(other.BlurTrackPos)
    , AreaTrackPos(other.AreaTrackPos)
{
}