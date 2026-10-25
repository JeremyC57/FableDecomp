// CEnvironmentSpellEffectInterface::InterpolateRadialBlurPositionParams(float t, const CRadialBlurPositionParams& from, const CRadialBlurPositionParams& to) -- blend the blur and area centres from `from` towards `to` by t where both ends track the same position kind (else take `to`'s centre); the track kinds come from `to`
// Leaf: ?InterpolateRadialBlurPositionParams@CEnvironmentSpellEffectInterface@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00885680, 352 bytes incl. int3 padding (349 of code), static `__fastcall`
//   with a hidden result pointer (ecx = result, edx = from, stack: t, to; `ret 8`), returns the result pointer in eax.
// Identity: Ego_r/FableWin PDB name set:
//   `?InterpolateRadialBlurPositionParams@CEnvironmentSpellEffectInterface@@SI?AUCRadialBlurPositionParams@1@MABU21@0@Z`
//   (the catalog label agrees). Single caller: the landed GetRenderFrameInterpolatedRadialBlurParameters 0x008857E0
//   (from = PrevFrameRadialBlurPositionParams, to = RadialBlurPositionParams). Layout (PDB, 0x20 bytes): C3DVector
//   AreaCenter +0, C3DVector BlurCenter +0xC, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos +0x18, BlurTrackPos
//   +0x1C. The PDB names a user default ctor and a user copy ctor (non-POD: the hidden result pointer goes in ecx).
// Constants: 1.0f 0x0122DED8. No callees.
// Shape: no clamp here; BlurCenter first (its track kinds compared), then AreaCenter, each `from * (1 - t) + to * t`
//   through C3DVector's inline operator* (float) / operator+ (temporaries; the right operand evaluated first, the sums
//   copied dword-wise into a named result local) or a plain copy of `to`'s centre; `return result;` is the memberwise
//   user copy constructor (the two track kinds straight from the registers that held them for the compares).
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { ENVIRONMENT_RADIAL_BLUR_TRACK_NONE };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};

class CEnvironmentSpellEffectInterface {
public:
    struct CRadialBlurPositionParams {                          // PDB, 0x20 bytes
        C3DVector                              AreaCenter;      // +0x00
        C3DVector                              BlurCenter;      // +0x0C
        ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;    // +0x18
        ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;    // +0x1C
        CRadialBlurPositionParams() { }
        CRadialBlurPositionParams(const CRadialBlurPositionParams& o)
            : AreaCenter(o.AreaCenter), BlurCenter(o.BlurCenter), AreaTrackPos(o.AreaTrackPos), BlurTrackPos(o.BlurTrackPos) { }
    };
    static CRadialBlurPositionParams __fastcall InterpolateRadialBlurPositionParams(float t, const CRadialBlurPositionParams& from,
                                                                                   const CRadialBlurPositionParams& to);
};

CEnvironmentSpellEffectInterface::CRadialBlurPositionParams __fastcall
CEnvironmentSpellEffectInterface::InterpolateRadialBlurPositionParams(float t, const CRadialBlurPositionParams& from,
                                                                     const CRadialBlurPositionParams& to)
{
    CRadialBlurPositionParams result;
    if (from.BlurTrackPos == to.BlurTrackPos)
        result.BlurCenter = from.BlurCenter * (1.0f - t) + to.BlurCenter * t;
    else
        result.BlurCenter = to.BlurCenter;
    if (from.AreaTrackPos == to.AreaTrackPos)
        result.AreaCenter = from.AreaCenter * (1.0f - t) + to.AreaCenter * t;
    else
        result.AreaCenter = to.AreaCenter;
    result.AreaTrackPos = to.AreaTrackPos;
    result.BlurTrackPos = to.BlurTrackPos;
    return result;
}