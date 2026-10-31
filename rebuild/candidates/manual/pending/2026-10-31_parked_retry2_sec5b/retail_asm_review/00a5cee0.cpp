// CMatrix4x4::AlignedSetInterpolate(const CMatrix4x4&, const CMatrix4x4&, float) -- this = a + (b - a) * t (SSE path when available, x87 otherwise)
// Grade: RELOCATION_MATCH 285v285 (asm oracle; SSE retail_asm pending the owner). Intrinsics best 301v285 (50 lines). Base was x87-only 136v285.
// Leaf: ?AlignedSetInterpolate@CMatrix4x4@@QAEXABV1@0M@Z
// Retail-asm: 0x00A5CF04..0x00A5CF79 `mov [esp+0Ch],ecx ; ... ; mov ecx,[esp+0Ch] ; movaps xmm0,[esp+20h] ; movaps xmm1,[esp+10h] ; shufps xmm0,xmm0,0 ; shufps xmm1,xmm1,0 ; movaps xmm2,[eax] ... mulps xmm2,xmm0 ... addps xmm2,xmm6 ... movaps [ecx+30h],xmm5` -- the hand SSE lerp of the NVectorMath/CMatrix4x4 family: this homed to a frame word and reloaded into ecx, a and b read straight from their argument slots, fixed xmm0..xmm7; the xmmintrin spelling (probe) is 301v285 with compiler-chosen registers and no this reload.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5CEE0, 285 bytes (+3 int3), `ret 0Ch`, `and esp,-16` frame. Extent from the retail bytes (no catalog length; BOUNDS_FIX.tsv).
// Name: Ego_r.pdb ?AlignedSetInterpolate@CMatrix4x4@@QAEXABV1@0M@Z (donor order AlignedTransformTransposedAssumeW then AlignedSetInterpolate; retail 0x00A5CB90 is the reloc-bound AlignedTransformTransposedAssumeW) + reloc_bindings.tsv (the caller at 0x00ADF300).
// Shape: 1 - t and t each stored as the X of an aligned C4DVector local (only X written; the asm broadcasts it). The x87 fallback reads a and b TRANSPOSED (m[r][c] = a.m[c][r] + (b.m[c][r] - a.m[c][r]) * t) -- retail walks a/b by column while it walks this by row; the SSE path is row-wise. Kept as retail computes it.
// Global: g_SSEAvailable 0x013D2880 (the landed spelling, NVectorMath_GFInterpolateVectors_00a5bef0.cpp).
class __declspec(align(16)) C4DVector {         // the SSE-aligned 4D vector (Ego_r.pdb C4DVector: X, Y, Z, W)
public:
    float X, Y, Z, W;
};
extern bool g_SSEAvailable;                        // 0x013D2880
class CMatrix4x4 {
public:
    float m[4][4];
    void AlignedSetInterpolate(const CMatrix4x4& a, const CMatrix4x4& b, float t);
};

void CMatrix4x4::AlignedSetInterpolate(const CMatrix4x4& a, const CMatrix4x4& b, float t)
{
    if (g_SSEAvailable) {
        C4DVector oneMinusT;
        oneMinusT.X = 1.0f - t;
        CMatrix4x4* self = this;
        C4DVector weight;
        weight.X = t;
        __asm {
            mov     eax, a
            mov     edx, b
            mov     ecx, self
            movaps  xmm0, oneMinusT
            movaps  xmm1, weight
            shufps  xmm0, xmm0, 0
            shufps  xmm1, xmm1, 0
            movaps  xmm2, [eax]
            movaps  xmm3, [eax+10h]
            movaps  xmm4, [eax+20h]
            movaps  xmm5, [eax+30h]
            mulps   xmm2, xmm0
            mulps   xmm3, xmm0
            mulps   xmm4, xmm0
            mulps   xmm5, xmm0
            movaps  xmm6, [edx]
            movaps  xmm7, [edx+10h]
            mulps   xmm6, xmm1
            mulps   xmm7, xmm1
            addps   xmm2, xmm6
            addps   xmm3, xmm7
            movaps  xmm6, [edx+20h]
            movaps  xmm7, [edx+30h]
            mulps   xmm6, xmm1
            mulps   xmm7, xmm1
            addps   xmm4, xmm6
            addps   xmm5, xmm7
            movaps  [ecx], xmm2
            movaps  [ecx+10h], xmm3
            movaps  [ecx+20h], xmm4
            movaps  [ecx+30h], xmm5
        }
    } else {
        for (long row = 0; row < 4; ++row) {
            for (long column = 0; column < 4; ++column) {
                float from = a.m[column][row];
                m[row][column] = from + (b.m[column][row] - from) * t;
            }
        }
    }
}
