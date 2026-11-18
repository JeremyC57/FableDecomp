// CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4&) -- multiply the transposed matrices in place
// Grade: DIFFER(472v471) -- The existing shared SSE kernel matches. The x87 fallback still differs in the ordering of the four products and the first two addends.
// Leaf: ?AlignedPreMultiplyTransposed@CMatrix4x4@@QAEXABV1@@Z
// Retail: 0x00ADEFC0, 471 bytes, thiscall.
// Retail-asm: 0x00ADEFEA through 0x00ADF0E7 form the GF_SSE_MATRIX_MULTIPLY kernel, including aligned movaps and the four shufps lanes.
#if !defined(_MSC_VER)
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_DEFINITION
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"

#ifdef _MSC_VER
#define FABLE_MATRIX_LOCAL_ALIGNMENT __declspec(align(16))
#else
#define FABLE_MATRIX_LOCAL_ALIGNMENT
#endif

void CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4& matrix)
{
    FABLE_MATRIX_LOCAL_ALIGNMENT CMatrix4x4 m;
    m = *this;
    if (GEnableSIMD) {
        const float* src1 = &m.Data[0][0];
        const float* src2 = &matrix.Data[0][0];
        float* dest = &Data[0][0];
        GF_SSE_MATRIX_MULTIPLY(dest, src1, src2);
    } else {
        for (long c1 = 0; c1 < 4; ++c1) {
            const float* a = m.Data[c1];
            for (long c2 = 0; c2 < 4; ++c2) {
                Data[c1][c2] = a[0] * matrix.Data[0][c2] + a[1] * matrix.Data[1][c2]
                             + a[2] * matrix.Data[2][c2] + a[3] * matrix.Data[3][c2];
            }
        }
    }
}
