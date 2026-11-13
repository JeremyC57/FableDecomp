// C3DAnimationManager::BlendAnimations(C3DKeyframe* from, const C3DKeyframe* to, unsigned long count, float t) -- blend two keyframe arrays: positions through NVectorMath::GFInterpolateVectors, rotations by the table-driven slerp (SSE: per-key weights into a pool array, finished by GFFinishPartialSlerp; else CQuaternion::SetSlerp in place), timed with the cycle counter
// Grade: RELOCATION_MATCH (chunk 3021, grade 26). The last SIB / x87 operand-order residue (0x00A9EC5A `fld [eax+edx]`, then the SSE-loop dot products 0x00A9EC40..0x00A9EC55) was the TEXT of the two table helpers: GFFastSin and GFFastArcCos both name their two table values (`float a = table[i]; float b = table[i + 1];`). See the Helper-texts lines below.
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
// Helper texts (FITTED v PROVEN):
//   GFFastSin `i -= 256; i &= 0x3ff; float a = g_CosTable[i]; float b = g_CosTable[i + 1]; return (1.0f - frac) * a + frac * b;` --
//     PROVEN-by-size in chunk 2044 (VC7.1 /Od 35i/120b v FableWin ?GFFastSin@@YIMM@Z 37i/121b, the same +2i/+1b residual as the sibling
//     GFFastCos; the matched pending rows 0x00AA0090 / 0x00A9F540 use this exact text); alone it fixes the SIB order here (grade 2).
//   GFFastArcCos `float a = g_ArcCosTable[i]; float b = g_ArcCosTable[i + 1]; return ((1.0f - frac) * a + frac * b) * 0.15915494f;` --
//     FITTED: the same named-table-value style as GFFastSin (no FableWin size evidence: its debug copy carries asserts, 119i/443b). It
//     supplies the records that put the SSE-loop dot products in retail's operand order (a dead-local ruler measured +6 / +7 units short
//     without it; with it the row still matches with +1..+3 diagnostic dead locals, so it is not on an edge). Release bytes of every
//     landed user are unchanged under this text (regraded: out-of-line GFFastArcCos 0x00A22530, CalculateSlerpWeights 0x00AA3390,
//     SetSlerpFromIdentity 0x00AA3560, SetSlerp 0x00A22590, BlendWith 0x00AA3C50, CParticle::GetInterpolatedOrientation 0x00B05620: all
//     RELOCATION_MATCH; pending 0x00AA0090 RELOCATION_MATCH), EXCEPT the pending twin 0x00A9F540 (chunk 2044), which then differs in the
//     4-term Dot order of its expanded BlendWith (0x00A9FA56..0x00A9FA87): see NOTES.md (review at landing).
//   Dot / CalculateSlerpWeights / SetSlerp: unchanged from the prior chunks (CalculateSlerpWeights' text RELOCATION_MATCHes its
//     out-of-line copy 0x00AA3390; Dot written either way is byte-neutral).
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
    float a = g_ArcCosTable[i];
    float b = g_ArcCosTable[i + 1];
    return ((1.0f - frac) * a + frac * b) * 0.15915494f;
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = g_CosTable[i];
    float b = g_CosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
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
