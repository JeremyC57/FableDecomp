// Behaviour test for C3DAnimationManager::ApplyBoneMorphScaling (0x00A9D750): each bone matrix scaled by its morph vector, per column when
// transposed (AlignedPreScaleTransposed) and per row otherwise (AlignedPreScale);
// both the SSE path (the shared kernel macro, its C++ twin under -DFABLE_PORTABLE) and the x87 path are run on dyadic
// inputs, so they agree bit for bit. A model of the row over engine/ helper headers (collaborators are stubs).
#include <stdio.h>
#include <string.h>
#include "engine/CMatrix4x4AlignedSSE.h"
#if defined(_MSC_VER)
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
struct C3DVector { float X, Y, Z; };
struct ALIGN16 C4DVector { float X, Y, Z, W; };
static bool g_SSEAvailable;
struct ALIGN16 CMatrix4x4 {
    float Data[4][4];
    void AlignedPreScale(const C3DVector& scale);
    void AlignedPreScaleTransposed(const C3DVector& scale);
    void AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b)   // the header's inline member (SSE kernel / x87 loop)
    {
        if (g_SSEAvailable) {
            const CMatrix4x4* pa = &a;
            const CMatrix4x4* pb = &b;
            CMatrix4x4* pdst = this;
            GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);
        } else {
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c) {
                    float v = a.Data[r][0] * b.Data[0][c];
                    v += a.Data[r][1] * b.Data[1][c];
                    v += a.Data[r][2] * b.Data[2][c];
                    v += a.Data[r][3] * b.Data[3][c];
                    Data[r][c] = v;
                }
        }
    }
};
static float Dy(int i) { return (float)(((i * 7) % 13) - 6) * 0.25f; }          // dyadic, products and sums exact
static void Fill(CMatrix4x4& m, int seed) { for (int i = 0; i < 16; ++i) m.Data[i / 4][i % 4] = Dy(i + seed); }
static inline void RefMul(float out[4][4], const CMatrix4x4& a, const CMatrix4x4& b)
{
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float v = 0.0f;
            for (int k = 0; k < 4; ++k) v += a.Data[r][k] * b.Data[k][c];
            out[r][c] = v;
        }
}
static int g_bad;
static void Expect(const char* what, int pass, float got, float want)
{
    if (got != want) { printf("  pass %d %s: %.9g, expected %.9g\n", pass, what, got, want); ++g_bad; }
}
void CMatrix4x4::AlignedPreScale(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        ALIGN16 C4DVector s;
        s.X = scale.X; s.Y = scale.Y; s.Z = scale.Z;
        s.W = 0.0f;
        CMatrix4x4* self = this;
        float t0 = Data[0][3], t1 = Data[1][3], t2 = Data[2][3];
        GF_SSE_MATRIX_PRESCALE(self, s);
        Data[0][3] = t0; Data[1][3] = t1; Data[2][3] = t2;
        return;
    }
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) Data[r][c] *= (&scale.X)[r];
}
void CMatrix4x4::AlignedPreScaleTransposed(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        ALIGN16 C4DVector s;
        s.X = scale.X; s.Y = scale.Y; s.Z = scale.Z;
        s.W = 0.0f;
        CMatrix4x4* self = this;
        float t0 = Data[0][3], t1 = Data[1][3], t2 = Data[2][3];
        GF_SSE_MATRIX_PRESCALE_TRANSPOSED(self, s);
        Data[0][3] = t0; Data[1][3] = t1; Data[2][3] = t2;
        return;
    }
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) Data[r][c] *= (&scale.X)[c];
}
struct C3DAnimationManager {
    void ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool transposed) const
    {
        if (transposed) {
            for (unsigned long i = 0; i < count; ++i) matrices[i].AlignedPreScaleTransposed(morphs[i]);
        } else {
            for (unsigned long i = 0; i < count; ++i) matrices[i].AlignedPreScale(morphs[i]);
        }
    }
};
int main()
{
    const C3DVector morphs[3] = { { 2.0f, 0.5f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.25f, 4.0f, -2.0f } };
    for (int pass = 0; pass < 4; ++pass) {
        g_SSEAvailable = (pass & 1) == 0;
        const bool transposed = (pass & 2) != 0;
        CMatrix4x4 m[3], in[3];
        for (int i = 0; i < 3; ++i) { Fill(m[i], i * 5); in[i] = m[i]; }
        C3DAnimationManager manager;
        manager.ApplyBoneMorphScaling(morphs, m, 3, transposed);
        for (int i = 0; i < 3; ++i)
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c) {
                    const float k = (r < 3 && c < 3) ? (&morphs[i].X)[transposed ? c : r] : 1.0f;
                    Expect("m", pass, m[i].Data[r][c], in[i].Data[r][c] * k);
                }
    }
    printf(g_bad ? "00a9d750_TEST FAIL\n" : "00a9d750_TEST PASS\n");
    return g_bad ? 1 : 0;
}
