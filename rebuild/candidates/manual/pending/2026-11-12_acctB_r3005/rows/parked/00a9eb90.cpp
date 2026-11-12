// C3DAnimationManager::BlendAnimations(C3DKeyframe* from, const C3DKeyframe* to, unsigned long count, float t) -- blend two keyframe arrays: positions through NVectorMath::GFInterpolateVectors, rotations by the table-driven slerp (SSE: per-key weights into a pool array, finished by GFFinishPartialSlerp; else CQuaternion::SetSlerp in place), timed with the cycle counter
// Grade: DIFFER(1337v1337) -- one SIB operand order: 0x00A9EC5A retail `fld dword ptr [eax+edx]` (d9 04 10), ours `fld dword ptr [edx+eax]` (d9 04 02); every other byte equal. Chunk 3005 measured the mechanism (c2 hooks, NOTES.md): the address operands are ordered by their symbol records' ids MOD 1024 (records come from a pool of 32-record chunks with sequential ids); the to-IV temp has id 1021 and the (from - to) offset temp 1064 -> 40, so the offset sorts first; retail needs the SSE IVs pushed past 1024 by one more chunk of records allocated between the SSE-loop inlining and loop strength reduction, while the scalar-loop orders stay (see NOTES.md for the sweep tables); no natural spelling found
// Leaf: ?BlendAnimations@C3DAnimationManager@@AAEXPAVC3DKeyframe@@PBV2@KM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9EB90, 1337 bytes, thiscall, `ret 10h`.
// Identity: Ego_r.pdb ?BlendAnimations@C3DAnimationManager@@AAEXPAVC3DKeyframe@@PBV2@KM@Z (private; four call sites in the landed
//   CombineBaseAnimations / CombineBlendedBaseAnimations rows bind 0x00A9EB90 under this spelling); `ret 10h` = four stack arguments.
// Callees: NVectorMath::GFInterpolateVectors(C4DVector*, unsigned long, const C4DVector*, unsigned long, unsigned long, float)
//   0x00A5B870 and NVectorMath::GFFinishPartialSlerp 0x00A5C2B0 (FableWin.pdb spellings, landed), CSimpleMemoryPool::Allocate 0x00A5C910
//   (through the inline FableWin ??$AllocateType@VCQuaternion@@@CSimpleMemoryPool@@QAEPAVCQuaternion@@K@Z).
// Globals: g_SSEAvailable 0x013D2880, the arc-cosine / cosine tables 0x013CB530 / 0x013CD550 (landed spellings), the two 64-bit
//   blend statistics 0x013D2D88 (cycles) / 0x013D2D90 (keyframes) (descriptive names; no PDB data symbol on this machine).
// Layout: engine/C3DAnimationManager.h (MemoryPool +0x14); C3DKeyframe 0x30 (Rotation +0, Position +0x10).
// Retail-asm: 0x00A9EB99 `xor eax,eax ; xor edx,edx ; rdtsc` + the two stores (and 0x00A9F066) -- engine/GFGetCycleCount.h (VC7.1 has
//   no rdtsc intrinsic); 0x00A9ECCC / 0x00A9ED34 / 0x00A9ED9D (and the scalar loop's) `fistp dword ptr` -- engine/GFFloatToLongNear.h
//   inside GFFastArcCos / GFFastSin (as in the landed C3DKeyframe::BlendWith 0x00AA3C50 and CQuaternion::CalculateSlerpWeights
//   0x00AA3390). This row carries no asm of its own.
#include "fable_compiler.h"
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CQUATERNION_SLERP
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimationManager.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFGetCycleCount.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
extern bool g_SSEAvailable;             // 0x013D2880
extern unsigned __int64 g_BlendAnimationsCycles;       // 0x013D2D88 (descriptive)
extern unsigned __int64 g_BlendAnimationsKeyframes;    // 0x013D2D90 (descriptive)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
namespace NVectorMath {
void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                          unsigned long count, float t);                                         // 0x00A5B870
void GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride,
                          const CQuaternion* weights, unsigned long weightStride, unsigned long count);   // 0x00A5C2B0
}
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
inline float CQuaternion::Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
FABLE_FORCEINLINE void FABLE_FASTCALL CQuaternion::CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = to.Dot(from);
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
FABLE_FORCEINLINE void CQuaternion::SetSlerp(const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = to.Dot(from);
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
    q.X = scale0 * from.X + scale1 * to.X;
    q.Y = scale0 * from.Y + scale1 * to.Y;
    q.Z = scale0 * from.Z + scale1 * to.Z;
    q.W = scale0 * from.W + scale1 * to.W;
    *this = q;
}

void C3DAnimationManager::BlendAnimations(C3DKeyframe* from, const C3DKeyframe* to, unsigned long count, float t)
{
    __int64 start;
    GF_GET_CYCLE_COUNT(start);
    NVectorMath::GFInterpolateVectors(&from->Position, sizeof(C3DKeyframe), &to->Position, sizeof(C3DKeyframe), count, t);
    if (g_SSEAvailable) {
        CQuaternion* weights = MemoryPool.AllocateType<CQuaternion>(count);
        for (unsigned long i = 0; i < count; ++i)
            CQuaternion::CalculateSlerpWeights(weights[i], from[i].Rotation, to[i].Rotation, t);
        NVectorMath::GFFinishPartialSlerp(&from->Rotation, sizeof(C3DKeyframe), &to->Rotation, sizeof(C3DKeyframe), weights, sizeof(CQuaternion), count);
    } else {
        for (unsigned long i = 0; i < count; ++i) {
            CQuaternion& rotation = from[i].Rotation;
            rotation.SetSlerp(rotation, to[i].Rotation, t);
        }
    }
    __int64 end;
    GF_GET_CYCLE_COUNT(end);
    g_BlendAnimationsCycles += end - start;
    g_BlendAnimationsKeyframes += count;
}
