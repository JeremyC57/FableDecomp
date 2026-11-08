// CMatrix4x4AlignedSSE.h -- the hand-written SSE kernels of the CMatrix4x4 Aligned* members (16-byte rows), isolated as
// macros. Each macro names the caller's variables directly (an `__asm` block reads a C local or parameter from its frame
// slot), which is retail's shape: the pointer is homed to a frame word by the C++ around the block and re-read into a fixed
// register (`mov [esp],ecx ; ... ; mov edx,[esp]`), the operands sit in fixed xmm registers, and no register allocation
// crosses the block. A `__forceinline` function would copy each operand into a parameter slot of its own; VC7.1 then
// re-reads the scale through a pointer (`mov eax,s ; movaps xmm3,[eax]`), which retail never does (see chunk 748 NOTES).
// Retail-asm-helper: GF_SSE_MATRIX_PRESCALE_TRANSPOSED, GF_SSE_MATRIX_PRESCALE, GF_SSE_MATRIX_MULTIPLY, AlignedSetMultiply
// Retail-asm: 0x00A9D5C9 `mov edx,[esp]` + 0x00A9D5CC `movaps xmm3,[esp+10h]` + 0x00A9D5DC `mulps xmm0,xmm3`
//   (CMatrix4x4::AlignedPreScaleTransposed), 0x00A9D4EA `shufps xmm4,xmm4,55h` (CMatrix4x4::AlignedPreScale), and the same
//   blocks expanded in C3DAnimationManager::ApplyBoneMorphScaling 0x00A9D7C1 / 0x00A9D8C5, GF_SSE_MATRIX_MULTIPLY: CShaderRenderManager 0x009892A9 `movaps xmm0,[edx]`. VC7.1 RTM 3077 intrinsics
//   (_mm_load_ps / _mm_mul_ps / _mm_shuffle_ps / _mm_store_ps, every spelling in the chunk 748 table, /arch:SSE and SSE2)
//   keep `this` in ecx, pick their own xmm registers and interleave the stores; no C++ form re-reads the matrix pointer
//   from a frame word.
// Semantics: rows 0..2 of `matrix` (16-byte aligned, four floats each) are multiplied element-wise by the aligned
//   four-float `scale` (PRESCALE_TRANSPOSED: row r *= (s0, s1, s2, s3); PRESCALE: row r *= s[r] broadcast). Row 3 is not
//   touched. Each product is one IEEE single multiply (round to nearest), identical in SSE and in the portable C++ (the
//   exact 48-bit product of two floats is rounded once either way; x87 precision control does not change a single
//   rounding of an exact product).
// Clobbers: edx, xmm0..xmm5. The caller saves anything it needs (retail saves the fourth column around the block).
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable; tests in tests/portable/test_CMatrix4x4AlignedSSE.cpp.
#ifndef FABLE_ENGINE_CMATRIX4X4ALIGNEDSSE_H
#define FABLE_ENGINE_CMATRIX4X4ALIGNEDSSE_H
#include "fable_compiler.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void AlignedRowsTimesVector(float* rows, const float* scale)
    {
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                rows[r * 4 + c] = rows[r * 4 + c] * scale[c];
    }
    inline void AlignedRowsTimesBroadcast(float* rows, const float* scale)
    {
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                rows[r * 4 + c] = rows[r * 4 + c] * scale[r];
    }
    // dst = a * b (row vectors: dst row r = sum over k of a[r][k] * b row k), summed in the kernel's order
    // ((a0*b0 + a1*b1) + (a2*b2 + a3*b3)); a and b are read in full before dst is written (dst may alias neither).
    inline void AlignedMatrixMultiply(float* dst, const float* a, const float* b)
    {
        for (int r = 0; r < 4; ++r) {
            float row[4];
            for (int c = 0; c < 4; ++c) {
                float p0 = a[r * 4 + 0] * b[0 * 4 + c];
                float p1 = a[r * 4 + 1] * b[1 * 4 + c];
                float p2 = a[r * 4 + 2] * b[2 * 4 + c];
                float p3 = a[r * 4 + 3] * b[3 * 4 + c];
                float s01 = p0 + p1;
                float s23 = p2 + p3;
                row[c] = s01 + s23;
            }
            for (int c = 0; c < 4; ++c)
                dst[r * 4 + c] = row[c];
        }
    }
}
#define FABLE_PORTABLE_SSE_MATRIX_MULTIPLY(dst, a, b) \
    fable_portable::AlignedMatrixMultiply((float*)(dst), (const float*)(a), (const float*)(b))
#define FABLE_PORTABLE_SSE_PRESCALE_TRANSPOSED(matrix, scale) \
    fable_portable::AlignedRowsTimesVector((float*)(matrix), (const float*)&(scale))
#define FABLE_PORTABLE_SSE_PRESCALE(matrix, scale) \
    fable_portable::AlignedRowsTimesBroadcast((float*)(matrix), (const float*)&(scale))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_MATRIX_PRESCALE_TRANSPOSED(matrix, scale) FABLE_PORTABLE_SSE_PRESCALE_TRANSPOSED(matrix, scale)
#define GF_SSE_MATRIX_PRESCALE(matrix, scale) FABLE_PORTABLE_SSE_PRESCALE(matrix, scale)
#define GF_SSE_MATRIX_MULTIPLY(dst, a, b) FABLE_PORTABLE_SSE_MATRIX_MULTIPLY(dst, a, b)
#else
// dst, a, b: pointer or reference variables (16-byte-aligned 4x4 matrices); dst = a * b. Clobbers eax, edx, edi, xmm0..xmm7.
#define GF_SSE_MATRIX_MULTIPLY(dst, a, b) \
    __asm mov eax, a \
    __asm mov edx, b \
    __asm mov edi, dst \
    __asm movaps xmm0, [edx] \
    __asm movaps xmm1, [edx+10h] \
    __asm movaps xmm2, [edx+20h] \
    __asm movaps xmm3, [edx+30h] \
    __asm movaps xmm4, [eax] \
    __asm movaps xmm5, xmm4 \
    __asm movaps xmm6, xmm4 \
    __asm movaps xmm7, xmm4 \
    __asm shufps xmm4, xmm4, 00h \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm mulps xmm4, xmm0 \
    __asm mulps xmm5, xmm1 \
    __asm mulps xmm6, xmm2 \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi], xmm4 \
    __asm movaps xmm4, [eax+10h] \
    __asm movaps xmm5, xmm4 \
    __asm movaps xmm6, xmm4 \
    __asm movaps xmm7, xmm4 \
    __asm shufps xmm4, xmm4, 00h \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm mulps xmm4, xmm0 \
    __asm mulps xmm5, xmm1 \
    __asm mulps xmm6, xmm2 \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi+10h], xmm4 \
    __asm movaps xmm4, [eax+20h] \
    __asm movaps xmm5, xmm4 \
    __asm movaps xmm6, xmm4 \
    __asm movaps xmm7, xmm4 \
    __asm shufps xmm4, xmm4, 00h \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm mulps xmm4, xmm0 \
    __asm mulps xmm5, xmm1 \
    __asm mulps xmm6, xmm2 \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi+20h], xmm4 \
    __asm movaps xmm4, [eax+30h] \
    __asm movaps xmm5, xmm4 \
    __asm movaps xmm6, xmm4 \
    __asm movaps xmm7, xmm4 \
    __asm shufps xmm4, xmm4, 00h \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm mulps xmm4, xmm0 \
    __asm mulps xmm5, xmm1 \
    __asm mulps xmm6, xmm2 \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi+30h], xmm4
// matrix: a pointer local (or parameter) holding the 16-byte-aligned rows; scale: an aligned four-float local.
#define GF_SSE_MATRIX_PRESCALE_TRANSPOSED(matrix, scale) \
    __asm mov edx, matrix \
    __asm movaps xmm3, scale \
    __asm movaps xmm0, [edx] \
    __asm movaps xmm1, [edx+10h] \
    __asm movaps xmm2, [edx+20h] \
    __asm mulps xmm0, xmm3 \
    __asm mulps xmm1, xmm3 \
    __asm mulps xmm2, xmm3 \
    __asm movaps [edx], xmm0 \
    __asm movaps [edx+10h], xmm1 \
    __asm movaps [edx+20h], xmm2
#define GF_SSE_MATRIX_PRESCALE(matrix, scale) \
    __asm mov edx, matrix \
    __asm movaps xmm3, scale \
    __asm movaps xmm0, [edx] \
    __asm movaps xmm1, [edx+10h] \
    __asm movaps xmm2, [edx+20h] \
    __asm movaps xmm4, xmm3 \
    __asm movaps xmm5, xmm3 \
    __asm shufps xmm3, xmm3, 00h \
    __asm shufps xmm4, xmm4, 55h \
    __asm shufps xmm5, xmm5, 0AAh \
    __asm mulps xmm0, xmm3 \
    __asm mulps xmm1, xmm4 \
    __asm mulps xmm2, xmm5 \
    __asm movaps [edx], xmm0 \
    __asm movaps [edx+10h], xmm1 \
    __asm movaps [edx+20h], xmm2
#endif

#endif  // FABLE_ENGINE_CMATRIX4X4ALIGNEDSSE_H

// CMatrix4x4::AlignedSetMultiply (FableWin ?AlignedSetMultiply@CMatrix4x4@@QAEXABV1@0@Z, always inlined in retail: 0x00988E80,
// 0x00989230, 0x00AE1320, 0x00AE29D0): this = a * b. Defined here, next to its kernel, so a row that calls it is seen as calling
// a retail-asm helper. The SSE arm homes the three pointers in locals (retail's `mov [esp+2Ch],eax ; ... ; mov eax,[esp+2Ch]`);
// the x87 arm is plain C++ (its term order is the open residue of those rows, chunk 748 NOTES).
// Outside the include guard: engine/CMatrix4x4.h includes this header again once the class is complete.
#if defined(FABLE_RETAIL_CMATRIX4X4) && defined(FABLE_ENGINE_CMATRIX4X4_H) && !defined(FABLE_CMATRIX4X4_ALIGNEDSETMULTIPLY_DEFINED)
#define FABLE_CMATRIX4X4_ALIGNEDSETMULTIPLY_DEFINED
extern bool g_SSEAvailable;                                              // 0x013D2880 ?g_SSEAvailable@@3_NA
FABLE_FORCEINLINE void CMatrix4x4::AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b)
{
    if (g_SSEAvailable) {
        const CMatrix4x4* pa = &a;
        const CMatrix4x4* pb = &b;
        CMatrix4x4* pdst = this;
        GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);
    } else {
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                float v = a.Data[r][0] * b.Data[0][c];
                v += a.Data[r][1] * b.Data[1][c];
                v += a.Data[r][2] * b.Data[2][c];
                v += a.Data[r][3] * b.Data[3][c];
                Data[r][c] = v;
            }
        }
    }
}
#endif
