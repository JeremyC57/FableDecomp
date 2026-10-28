// CQuaternion::CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& a, const CQuaternion& b, float t) -- spherical-lerp weights from a to b at t: weights.X for a, weights.Y for b (sign-flipped when the quaternions point apart), plain lerp weights when they are within 0.05 of parallel
// Leaf: ?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA3390, 462 bytes, static __fastcall (weights in ecx, a in edx, b and t on
//   the stack), `ret 8`. No direct callers (an emitted out-of-line copy; its body is inlined at its users).
// Identity (proven): FableWin.pdb ?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z -- the only static CQuaternion
//   function taking (CQuaternion&, const CQuaternion&, const CQuaternion&, float); the body is the textbook slerp weight
//   pair (cos from the 4-term dot, |cos|, acos/sin from the engine's GFFastArcCos 0x00A22530 / GFFastSin 0x00A147E0
//   tables, inlined) and writes only the two weights, as the name says. Its siblings 0x00AA3560 (SetSlerpFromIdentity)
//   and 0x00B05620 (CParticle::GetInterpolatedOrientation, which inlines SetSlerpFromIdentity) carry the same body.
//   The catalog's "ApplyPartialAnimationAlpha" is propagated and wrong.
// Retail-asm: 0x00AA3416 `fstp dword ptr [esp+0Ch]` / 0x00AA341A `fld dword ptr [esp+0Ch]` + 0x00AA341E `fistp dword ptr [esp+4]`
//   (and 0x00AA3489, 0x00AA34F0 `fistp dword ptr`) -- the engine's inline-assembly float-to-long helper inlined from
//   GFFastArcCos / GFFastSin (landed retail_asm 0x00A22530 / 0x00A147E0): VC7.1 C++ `(long)x` calls __ftol2 and /QIfist
//   (RTM 3077 and QFE 4035) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si; no C/C++ form gives the bare
//   dword fistp, so it is the original's `__asm`.
// Globals: arc-cosine table 0x013CB530, cosine table 0x013CD550 (landed names); 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.05f
//   0x0125A15C, 512.5f 0x0129CD1C, 0.5f 0x0122F59C, 1/(2pi) 0x01237F08, 1024.0f 0x01230010.
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
    static void __fastcall CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& a, const CQuaternion& b, float t);
};

void __fastcall CQuaternion::CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& a, const CQuaternion& b, float t)
{
    float cosom = a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        weights.X = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        weights.Y = GFFastSin(omega * t) * (1.0f / sinom);
    }
    else {
        weights.X = 1.0f - t;
        weights.Y = t;
    }
    weights.Y = sign * weights.Y;
}