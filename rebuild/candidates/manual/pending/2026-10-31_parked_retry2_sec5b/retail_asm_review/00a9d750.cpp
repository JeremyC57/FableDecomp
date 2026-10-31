// C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector*, CMatrix4x4*, unsigned long, bool) const -- scale each bone matrix's first three rows by its morph vector (per column, or per row when the flag is clear), through SSE when it is enabled
// Grade: DIFFER(551v551) 34 diff lines (asm oracle, not landable under the brief; base intrinsics 492v551, 104) -- the inlined PostScale-style __asm (mov edx,self ; fixed xmm0..xmm5) gives retail's length; residue: store scheduling of the translation copies and the x87 fallback's commutative operand order
// Retail-asm: 0x00A9D7A1..0x00A9D7E5 `mov [esp+10h],edx ; ... ; mov edx,[esp+10h] ; movaps xmm3,[esp+20h] ; movaps xmm0,[edx] ; movaps xmm1,[edx+10h] ; movaps xmm2,[edx+20h] ; mulps xmm0,xmm3 ...` -- the matrix pointer homed to a frame slot and reloaded into edx inside each inlined helper (an __asm naming this), fixed xmm registers.
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
            s.F[0] = v.X; s.F[1] = v.Y; s.F[2] = v.Z;
            CMatrix4x4* self = this;
            float w0 = E[0][3], w1 = E[1][3], w2 = E[2][3];
            s.F[3] = 0.0f;
            __asm {
                mov     edx, self
                movaps  xmm3, s
                movaps  xmm0, [edx]
                movaps  xmm1, [edx+10h]
                movaps  xmm2, [edx+20h]
                mulps   xmm0, xmm3
                mulps   xmm1, xmm3
                mulps   xmm2, xmm3
                movaps  [edx], xmm0
                movaps  [edx+10h], xmm1
                movaps  [edx+20h], xmm2
            }
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
            s.F[0] = v.X; s.F[1] = v.Y; s.F[2] = v.Z;
            CMatrix4x4* self = this;
            float w0 = E[0][3], w1 = E[1][3], w2 = E[2][3];
            s.F[3] = 0.0f;
            __asm {
                mov     edx, self
                movaps  xmm3, s
                movaps  xmm0, [edx]
                movaps  xmm1, [edx+10h]
                movaps  xmm2, [edx+20h]
                movaps  xmm4, xmm3
                movaps  xmm5, xmm3
                shufps  xmm3, xmm3, 0
                shufps  xmm4, xmm4, 55h
                shufps  xmm5, xmm5, 0AAh
                mulps   xmm0, xmm3
                mulps   xmm1, xmm4
                mulps   xmm2, xmm5
                movaps  [edx], xmm0
                movaps  [edx+10h], xmm1
                movaps  [edx+20h], xmm2
            }
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
