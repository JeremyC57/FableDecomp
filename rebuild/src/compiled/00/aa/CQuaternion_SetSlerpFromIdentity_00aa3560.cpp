// CQuaternion::SetSlerpFromIdentity(const CQuaternion& q, float t) -- sets this to the spherical lerp from the identity rotation to q at t (q scaled by the target weight, the identity weight added to W; plain lerp weights within 0.05 of identity, q's sign folded in when W < 0)
// Leaf: ?SetSlerpFromIdentity@CQuaternion@@QAEXABV1@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA3560, 447 bytes, thiscall, `ret 8`. No direct callers (an emitted
//   out-of-line copy; CParticle::GetInterpolatedOrientation 0x00B05620 inlines it).
// Identity (proven): FableWin.pdb ?SetSlerpFromIdentity@CQuaternion@@QAEXABV1@M@Z (thiscall, const CQuaternion&, float --
//   the row's `ret 8` and its single quaternion argument); the cosine is q.W alone (q dotted with the identity), the
//   weight computation is CalculateSlerpWeights' (0x00AA3390, the next-door FableWin name), and the result is
//   q * scale1 + (0, 0, 0, scale0). The catalog's "ApplyPartialAnimationAlpha" is propagated and wrong.
// Retail-asm: 0x00AA35CD `fstp dword ptr [esp+0Ch]` / 0x00AA35D1 `fld dword ptr [esp+0Ch]` + 0x00AA35D5 `fistp dword ptr [esp+4]` (and the two
//   GFFastSin `fistp dword ptr` at 0x00AA3640 / 0x00AA36A9) -- the engine's inline-assembly float-to-long helper inlined
//   from GFFastArcCos / GFFastSin (landed retail_asm 0x00A22530 / 0x00A147E0): VC7.1 C++ `(long)x` calls __ftol2 and
//   /QIfist (RTM 3077 and QFE 4035) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si; no C/C++ form gives
//   the bare dword fistp, so it is the original's `__asm`.
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
    void SetSlerpFromIdentity(const CQuaternion& q, float t);
};

void CQuaternion::SetSlerpFromIdentity(const CQuaternion& q, float t)
{
    float cosom = q.W;
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
    X = scale1 * q.X;
    Y = scale1 * q.Y;
    Z = scale1 * q.Z;
    W = scale1 * q.W + scale0;
}