// Behaviour test for 0x00ADEFC0 CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4&): this = this * matrix (row vectors), through
// an aligned copy of *this, so the product reads the old value of this even though it writes this. The row's own text over the shared
// engine/CMatrix4x4.h + engine/CMatrix4x4AlignedSSE.h (FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD), run on both arms: the SSE kernel
// (GF_SSE_MATRIX_MULTIPLY; its C++ twin under FABLE_PORTABLE) and the x87 arm, checked against an independent product, against each
// other, and (FABLE_DUAL_PATH_TEST) against fable_portable::AlignedMatrixMultiply. Dyadic inputs: every product and sum is exact.
#include <stdio.h>
#if !defined(_MSC_VER)
#define FABLE_PORTABLE
#endif
#define FABLE_DUAL_PATH_TEST
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_DEFINITION
#include "engine/CMatrix4x4.h"
#if defined(_MSC_VER)
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
bool GEnableSIMD = false;

void CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4& matrix)
{
    ALIGN16 CMatrix4x4 m;
    m = *this;
    AlignedSetMultiplyTransposed(matrix, m);
}

static int g_bad;
static void Expect(const char* what, int pass, int r, int c, float got, float want)
{
    if (got != want) { printf("  pass %d %s [%d][%d]: %g, expected %g\n", pass, what, r, c, got, want); ++g_bad; }
}
static float Dy(int i) { return (float)(((i * 7) % 13) - 6) * 0.25f; }

int main()
{
    ALIGN16 static CMatrix4x4 left, right, result[2];
    for (int i = 0; i < 16; ++i) { left.Data[i / 4][i % 4] = Dy(i); right.Data[i / 4][i % 4] = Dy(i + 5); }
    right.Data[0][1] += 1.0f;                                  // asymmetric, so this * matrix != matrix * this
    for (int pass = 0; pass < 2; ++pass) {
        GEnableSIMD = pass == 1;
        result[pass] = left;
        result[pass].AlignedPreMultiplyTransposed(right);
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c) {
                float want = 0.0f;
                for (int k = 0; k < 4; ++k) want += left.Data[r][k] * right.Data[k][c];
                Expect("this*matrix", pass, r, c, result[pass].Data[r][c], want);
            }
    }
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) Expect("x87 == SSE", 2, r, c, result[0].Data[r][c], result[1].Data[r][c]);
    ALIGN16 static CMatrix4x4 twin;
    fable_portable::AlignedMatrixMultiply(&twin.E11, &left.E11, &right.E11);
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) Expect("twin", 3, r, c, result[1].Data[r][c], twin.Data[r][c]);
    printf(g_bad ? "00adefc0_TEST FAIL\n" : "00adefc0_TEST PASS\n");
    return g_bad ? 1 : 0;
}