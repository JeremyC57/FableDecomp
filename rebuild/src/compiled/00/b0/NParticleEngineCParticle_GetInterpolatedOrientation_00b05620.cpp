// NParticleEngine::CParticle::GetInterpolatedOrientation(float t) const -- the particle's orientation part-way (t) through this step's rotation: Orientation * (the identity slerped toward OrientationChange by t)
// Leaf: ?GetInterpolatedOrientation@CParticle@NParticleEngine@@QBE?AVCQuaternion@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B05620, 495 bytes, thiscall, hidden CQuaternion result pointer + t, `ret 8`.
//   No direct callers (an emitted out-of-line copy).
// Identity (proven): Ego_r.pdb ?GetInterpolatedOrientation@CParticle@NParticleEngine@@QBE?AVCQuaternion@@M@Z -- the only
//   CParticle (or any) method returning a CQuaternion from one float; Ego_r CParticle is {vptr, Orientation +0x04,
//   OrientationChange +0x14, Angle +0x24, ...} and the row slerps the quaternion at this+0x14 (W at +0x20) from identity
//   by t (CQuaternion::SetSlerpFromIdentity 0x00AA3560 inlined) and multiplies this+4 by it through
//   CQuaternion::operator* 0x00A88B60 (landed ??DCQuaternion@@QBE?AV0@ABV0@@Z). The catalog's "ApplyPartialAnimationAlpha"
//   is propagated and wrong.
// Retail-asm: 0x00B05684 `fstp dword ptr [esp+0Ch]` / 0x00B05688 `fld dword ptr [esp+0Ch]` + 0x00B0568C `fistp dword ptr [esp+8]`
//   (and 0x00B056F6, 0x00B05760 `fistp dword ptr`) -- the engine's inline-assembly float-to-long helper inlined from
//   GFFastArcCos / GFFastSin (landed retail_asm 0x00A22530 / 0x00A147E0): VC7.1 C++ `(long)x` calls __ftol2 and /QIfist
//   (RTM 3077 and QFE 4035) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si; no C/C++ form gives the bare
//   dword fistp, so it is the original's `__asm`.
// Shape: SetSlerpFromIdentity __forceinline (plain inline stays a call); a named result local, which VC7.1 (no NRVO)
//   copies to the hidden result -- `return Orientation * change;` builds straight into it (grade 2, 466v495).
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
    void SetSlerpFromIdentity(const CQuaternion& q, float t);   // 0x00AA3560
    CQuaternion operator*(const CQuaternion& q) const;          // 0x00A88B60
};

__forceinline void CQuaternion::SetSlerpFromIdentity(const CQuaternion& q, float t)
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

namespace NParticleEngine {
class CParticle {
public:
    virtual ~CParticle();
    CQuaternion GetInterpolatedOrientation(float t) const;
    CQuaternion Orientation;                               // +0x04
    CQuaternion OrientationChange;                         // +0x14
    float Angle;                                           // +0x24
};

CQuaternion CParticle::GetInterpolatedOrientation(float t) const
{
    CQuaternion change;
    change.SetSlerpFromIdentity(OrientationChange, t);
    CQuaternion result = Orientation * change;
    return result;
}
}