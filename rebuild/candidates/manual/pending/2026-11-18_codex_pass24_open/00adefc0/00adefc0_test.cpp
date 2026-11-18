#include <stdio.h>
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#define FABLE_DUAL_PATH_TEST
#include "engine/CMatrix4x4AlignedSSE.h"
#ifdef _MSC_VER
#define FABLE_MATRIX_LOCAL_ALIGNMENT __declspec(align(16))
#else
#define FABLE_MATRIX_LOCAL_ALIGNMENT
#endif
bool GEnableSIMD = false;
struct CMatrix4x4 {
    float Data[4][4];
    void AlignedPreMultiplyTransposed(const CMatrix4x4&);
};

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

int main()
{
    FABLE_MATRIX_LOCAL_ALIGNMENT CMatrix4x4 left, right, x87, simd;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) {
        left.Data[r][c] = (float)(r*4+c-5);
        right.Data[r][c] = (float)((r+c*5)%7-3);
    }
    x87 = left;
    simd = left;
    x87.AlignedPreMultiplyTransposed(right);
    GEnableSIMD = true;
    simd.AlignedPreMultiplyTransposed(right);
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) {
        float expected = 0.0f;
        for (int k = 0; k < 4; ++k) expected += left.Data[r][k]*right.Data[k][c];
        if (x87.Data[r][c] != expected || simd.Data[r][c] != expected)
            return 1;
    }
    puts("00adefc0_TEST PASS");
    return 0;
}
