// C3DKeyframe::BlendWith(const C3DKeyframe& other, float t) -- blend this keyframe towards another: the rotation by the engine's table-driven slerp (shortest arc, plain lerp within 0.05 of parallel), the position linearly
// Leaf: ?BlendWith@C3DKeyframe@@QAEXABV1@M@Z
// Flags: none
// Retail-asm: 0x00AA3CDE + 0x00AA3D49 + 0x00AA3DB2 `fistp dword ptr` -- engine/GFFloatToLongNear.h (GFFloatToLongNear inside the
//   landed GFFastArcCos 0x00A22530 / GFFastSin bodies: the index minus 0.5 stored, reloaded, bare dword fistp). VC7.1 `(long)x`
//   calls __ftol2, `/QIfist` stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (NOTES.md attempts table).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA3C50, length 671 bytes (ret 8). No callees.
// Identity: FableWin.pdb ?BlendWith@C3DKeyframe@@QAEXABV1@M@Z (thiscall, `ret 8`, the keyframe updated in place; the old draft's
//   "CRightHandedSet::Interpolate" is Ego_r's by-value const ?Interpolate@CRightHandedSet@@QBE?AV1@ABV1@M@Z, which does not fit).
// Data: the arc-cosine table 0x013CB530 and the cosine table 0x013CD550 (GFFastArcCos / GFFastSin).
// Layout: Ego_r.pdb C3DKeyframe (0x30) = Rotation (CQuaternion) +0, Position (C4DVector) +0x10, Scaling +0x20 (FableWin GetRotation / GetPosition accessors).
// Shape: the cosine is the inline FableWin CQuaternion::Dot taken before the sign weight is declared (declared after it, VC7.1
//   keeps the dot on the x87 stack with `fst` where retail stores and reloads it); the position is the inline
//   C4DVector::Interpolate (engine/C4DVector.h retail opt-in: (v - *this) * t + *this through three temporaries).
#include "fable_compiler.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#include "engine/GFFloatToLongNear.h"
FABLE_FORCEINLINE float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
class CQuaternion { public: float X, Y, Z, W;
    float Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }   // FableWin ?Dot@CQuaternion@@QBEMABV1@@Z
};
#define FABLE_RETAIL_C4DVECTOR
#include "engine/C4DVector.h"
class C3DKeyframe {
public:
    CQuaternion Rotation;                 // +0x00
    C4DVector   Position;                 // +0x10
    C4DVector   Scaling;                  // +0x20
    void BlendWith(const C3DKeyframe& other, float t);
};

void C3DKeyframe::BlendWith(const C3DKeyframe& other, float t)
{
    float cosom = other.Rotation.Dot(Rotation);
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0, scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    } else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    CQuaternion q;
    q.X = scale1 * other.Rotation.X + scale0 * Rotation.X;
    q.Y = scale1 * other.Rotation.Y + scale0 * Rotation.Y;
    q.Z = scale1 * other.Rotation.Z + scale0 * Rotation.Z;
    q.W = scale0 * Rotation.W + scale1 * other.Rotation.W;
    Rotation = q;
    Position = Position.Interpolate(other.Position, t);
}