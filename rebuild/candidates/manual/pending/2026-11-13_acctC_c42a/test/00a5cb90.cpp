// Behaviour test of CMatrix4x4::AlignedTransformTransposedAssumeW 0x00A5CB90: SSE path (blocks of four + x87 remainder) and the
// pure x87 path against the element formula, in place and out of place, counts 0..9; dyadic values (every product and sum exact).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORMTRANSPOSEDASSUMEW
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4TransformSSE.h"
bool g_SSEAvailable = false;                           // 0x013D2880

// the row text (self-contained link)
void CMatrix4x4::AlignedTransformTransposedAssumeW(const C4DVector* src, C4DVector* dest, unsigned long count, float w) const
{
    if (g_SSEAvailable) {
        unsigned long blocks = count >> 2;
        if (blocks != 0) {
            const CMatrix4x4* self = this;
            __declspec(align(16)) CMatrix4x4 expandedRows[4];
            __declspec(align(16)) C4DVector tempRows[4];
            __declspec(align(16)) C4DVector wwww(w, w, w, w);
            CMatrix4x4* expanded = expandedRows;
            C4DVector* temp = tempRows;
            GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww);
            dest += blocks * 4;
            src += blocks * 4;
            count &= 3;
        }
    }
    for (; count != 0; --count) {
        TransformTransposedAssumeW(*src, *dest, w);
        ++dest;
        ++src;
    }
}

static int Check(bool sse, unsigned long count, bool inPlace)
{
    __declspec(align(16)) CMatrix4x4 m;
    __declspec(align(16)) C4DVector src[10];
    __declspec(align(16)) C4DVector dst[10];
    C4DVector ref[10];
    C4DVector keep[10];
    const float w = 0.5f;
    int r, c;
    unsigned long i;
    for (r = 0; r < 4; ++r) for (c = 0; c < 4; ++c) m.Data[r][c] = (float)(r * 4 + c) * 0.25f - 1.5f;
    for (i = 0; i < 10; ++i) {
        src[i].X = (float)i * 0.5f + 1.0f; src[i].Y = 2.0f - (float)i; src[i].Z = (float)(i * i) * 0.25f; src[i].W = 77.0f;
        dst[i].X = dst[i].Y = dst[i].Z = dst[i].W = -9.0f;
        keep[i] = inPlace ? src[i] : dst[i];
        ref[i].X = m.E11 * src[i].X + m.E12 * src[i].Y + m.E13 * src[i].Z + m.E14 * w;
        ref[i].Y = m.E21 * src[i].X + m.E22 * src[i].Y + m.E23 * src[i].Z + m.E24 * w;
        ref[i].Z = m.E31 * src[i].X + m.E32 * src[i].Y + m.E33 * src[i].Z + m.E34 * w;
        ref[i].W = w;
    }
    g_SSEAvailable = sse;
    C4DVector* out = inPlace ? src : dst;
    m.AlignedTransformTransposedAssumeW(src, out, count, w);
    int bad = 0;
    for (i = 0; i < 10; ++i) {
        const C4DVector& want = i < count ? ref[i] : keep[i];
        if (memcmp(&out[i], &want, sizeof(C4DVector)) != 0) ++bad;
    }
    return bad;
}

int main()
{
    int bad = 0;
    for (unsigned long count = 0; count < 10; ++count) {
        bad += Check(false, count, false);
        bad += Check(false, count, true);
        bad += Check(true, count, false);
        bad += Check(true, count, true);
    }
    if (bad) { printf("00a5cb90_TEST FAIL %d\n", bad); return 1; }
    printf("00a5cb90_TEST PASS\n");
    return 0;
}
