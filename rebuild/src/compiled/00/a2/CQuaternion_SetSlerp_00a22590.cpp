// CQuaternion::SetSlerp(const CQuaternion& a, const CQuaternion& b, float t) -- sets this to the spherical lerp from a to b at t (b's sign folded in when the dot product is negative; plain lerp weights within 0.05 of parallel)
// Leaf: ?SetSlerp@CQuaternion@@QAEXABV1@0M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A22590, 514 bytes, thiscall, `ret 0Ch`. No direct callers (an emitted
//   out-of-line copy kept in the CRightHandedSet TU).
// Identity (proven): FableWin.pdb ?SetSlerp@CQuaternion@@QAEXABV1@0M@Z sits in the CRightHandedSet TU between
//   GetAngleXZ/Interpolate and MakePerpendicular (with CQuaternion::Dot), and retail 0x00A22590 sits in the same TU
//   (GFFastArcCos 0x00A22530 before, the CRightHandedSet(const C3DAngles&) ctor 0x00A229E0 after); the row is thiscall with
//   two quaternion references and a float (`ret 0Ch`), a four-float dot product, and the landed slerp-weight body of
//   CQuaternion::CalculateSlerpWeights 0x00AA3390 / SetSlerpFromIdentity 0x00AA3560 (same globals), storing
//   scale0 * a + scale1 * b into this. The catalog's "ApplyPartialAnimationAlpha" is propagated and wrong.
// Retail-asm: 0x00A2261C `fstp dword ptr [esp+8]` / 0x00A22620 `fld dword ptr [esp+8]` + 0x00A22624 `fistp dword ptr [esp+4]` (and the two
//   GFFastSin `fistp dword ptr [esp+1Ch]` at 0x00A22692 / 0x00A226FC) -- the engine's inline-assembly float-to-long helper inlined
//   from GFFastArcCos / GFFastSin (landed retail_asm 0x00A22530 / 0x00A147E0): VC7.1 C++ `(long)x` calls __ftol2 and
//   /QIfist emits `fistp qword` into an 8-byte slot; no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Globals: as CalculateSlerpWeights 0x00AA3390 (tables 0x013CB530 / 0x013CD550, 0.0f, 1.0f, 0.05f, 512.5f, 0.5f,
//   1/(2pi), 1024.0f).
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
__forceinline float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = FloatToLong(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

class CQuaternion {
public:
    float X;                                               // +0x0
    float Y;                                               // +0x4
    float Z;                                               // +0x8
    float W;                                               // +0xC
    void SetSlerp(const CQuaternion& a, const CQuaternion& b, float t);
};

void CQuaternion::SetSlerp(const CQuaternion& a, const CQuaternion& b, float t)
{
    float cosom = a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0;
    float scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    }
    else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    X = scale0 * a.X + scale1 * b.X;
    Y = scale0 * a.Y + scale1 * b.Y;
    Z = scale0 * a.Z + scale1 * b.Z;
    W = scale0 * a.W + scale1 * b.W;
}