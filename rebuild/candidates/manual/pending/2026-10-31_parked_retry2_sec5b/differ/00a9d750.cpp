// C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector*, CMatrix4x4*, unsigned long, bool) const -- scale each bone matrix's first three rows by its morph vector (per column, or per row when the flag is clear), through SSE when it is enabled
// Grade: DIFFER(492v551) -- lane gap (original inline-assembly SSE block): retail stores the row pointer to a frame local and re-reads it into edx, loads the scale from the frame into a fixed xmm3 and rows into xmm0-2 (names re-read, fixed registers, the GOTCHAS chunk-268 signature); intrinsics schedule it differently. The x87 fallback rows also differ in commutative operand order.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D750, length 551 bytes (`ret 10h`, `and esp,-16` frame; SSE flag byte 0x013D2880;
//   Ego_r.pdb ?ApplyBoneMorphScaling@C3DAnimationManager@@ABEXPBVC3DVector@@PAVCMatrix4x4@@K_N@Z).
// Leaf: ApplyBoneMorphScaling
#include <xmmintrin.h>
extern bool g_UseSSE;                                  // 0x013D2880
struct C3DVector { float X, Y, Z; };
union CVector4SSE {
    __m128 V;
    float F[4];
};
struct CMatrix4x4 {
    float E[4][4];
    __forceinline void ScaleColumns(const C3DVector& v)
    {
        if (g_UseSSE) {
            CVector4SSE s;
            s.F[0] = v.X; s.F[1] = v.Y; s.F[2] = v.Z; s.F[3] = 0.0f;
            __m128* rows = (__m128*)E;
            float w0 = E[0][3], w1 = E[1][3], w2 = E[2][3];
            rows[0] = _mm_mul_ps(rows[0], s.V);
            rows[1] = _mm_mul_ps(rows[1], s.V);
            rows[2] = _mm_mul_ps(rows[2], s.V);
            E[0][3] = w0; E[1][3] = w1; E[2][3] = w2;
        } else {
            E[0][0] *= v.X; E[0][1] *= v.Y; E[0][2] *= v.Z;
            E[1][0] *= v.X; E[1][1] *= v.Y; E[1][2] *= v.Z;
            E[2][0] *= v.X; E[2][1] *= v.Y; E[2][2] *= v.Z;
        }
    }
    __forceinline void ScaleRows(const C3DVector& v)
    {
        if (g_UseSSE) {
            CVector4SSE s;
            s.F[0] = v.X; s.F[1] = v.Y; s.F[2] = v.Z; s.F[3] = 0.0f;
            __m128* rows = (__m128*)E;
            float w0 = E[0][3], w1 = E[1][3], w2 = E[2][3];
            rows[0] = _mm_mul_ps(rows[0], _mm_shuffle_ps(s.V, s.V, 0x00));
            rows[1] = _mm_mul_ps(rows[1], _mm_shuffle_ps(s.V, s.V, 0x55));
            rows[2] = _mm_mul_ps(rows[2], _mm_shuffle_ps(s.V, s.V, 0xaa));
            E[0][3] = w0; E[1][3] = w1; E[2][3] = w2;
        } else {
            E[0][0] *= v.X; E[0][1] *= v.X; E[0][2] *= v.X;
            E[1][0] *= v.Y; E[1][1] *= v.Y; E[1][2] *= v.Y;
            E[2][0] *= v.Z; E[2][1] *= v.Z; E[2][2] *= v.Z;
        }
    }
};
struct C3DAnimationManager {
    void ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool columns) const;
};
void C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool columns) const
{
    if (columns) {
        for (unsigned long i = 0; i < count; ++i)
            matrices[i].ScaleColumns(morphs[i]);
    } else {
        for (unsigned long i = 0; i < count; ++i)
            matrices[i].ScaleRows(morphs[i]);
    }
}
