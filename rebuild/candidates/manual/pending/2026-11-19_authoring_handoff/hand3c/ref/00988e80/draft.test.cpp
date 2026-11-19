// Behaviour test for CShaderRenderManager::UpdateLocalisedSphereMapping (0x00988E80): with room for two constants the normalised first and
// negated second rows of ViewMatrix * WorldMatrix go up as (v * 0.5, 0.5); dirty bit 0x8000 is cleared either way;
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
#include <math.h>
static unsigned long GFastOneOverSqrtTable[128];
static void BuildTable()          // the 8-bit mantissa of 1/sqrt at the middle of each (exponent parity, 6 mantissa bits) cell
{
    for (unsigned long i = 0; i < 128; ++i) {
        const unsigned long e = (i & 64) ? 127 : 128;
        unsigned long bits = (e << 23) | ((i & 63) << 17) | (1u << 16);
        float x; memcpy(&x, &bits, 4);
        const unsigned long E = (0x17c - e) >> 1;
        const double m = (1.0 / sqrt((double)x)) / ldexp(1.0, (int)E - 127);
        GFastOneOverSqrtTable[i] = (unsigned long)((m - 1.0) * 256.0) & 0xff;
    }
}
static float FastOneOverSqrt(float value)
{
    unsigned int bits; memcpy(&bits, &value, 4);
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000) | ((unsigned int)GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r; memcpy(&r, &estimate, 4);
    return (3.0f - r * r * value) * r * 0.5f;
}
struct CShaderConstantRange { long Offset; unsigned long Count; };
struct CVertexShaderConstantLayout { CShaderConstantRange SphereMapMatrix; };
struct CShaderRenderManager {
    CMatrix4x4 WorldMatrix, ViewMatrix;
    const CVertexShaderConstantLayout* CurrentVSConstantLayout;
    unsigned long UpdateFlags;
    long UploadStart; unsigned long UploadCount; C4DVector Upload[2];
    void SetVSConstants(long start, const void* data, unsigned long count)
    {
        if (count != 0) { UploadStart = start; UploadCount = count; memcpy(Upload, data, count * 16); }
    }
    void UpdateLocalisedSphereMapping()
    {
        if (CurrentVSConstantLayout->SphereMapMatrix.Count >= 2) {
            ALIGN16 CMatrix4x4 transform;
            transform.AlignedSetMultiply(ViewMatrix, WorldMatrix);
            C4DVector axes[2];
            const float (&t)[4][4] = transform.Data;
            float s = FastOneOverSqrt(t[0][0] * t[0][0] + t[0][1] * t[0][1] + t[0][2] * t[0][2]);
            axes[0].X = t[0][0] * s * 0.5f; axes[0].Y = t[0][1] * s * 0.5f; axes[0].Z = t[0][2] * s * 0.5f; axes[0].W = 0.5f;
            float x = -t[1][0], y = -t[1][1], z = -t[1][2];
            s = FastOneOverSqrt(z * z + x * x + y * y);
            axes[1].X = x * s * 0.5f; axes[1].Y = y * s * 0.5f; axes[1].Z = z * s * 0.5f; axes[1].W = 0.5f;
            SetVSConstants(CurrentVSConstantLayout->SphereMapMatrix.Offset, axes, 2);
        }
        UpdateFlags &= ~0x8000;
    }
};
int main()
{
    BuildTable();
    C4DVector first[2];
    for (int pass = 0; pass < 3; ++pass) {
        g_SSEAvailable = pass == 0;
        const CVertexShaderConstantLayout layout = { { 12, pass == 2 ? 1u : 2u } };
        static CShaderRenderManager m;
        memset(&m, 0, sizeof(m));
        Fill(m.WorldMatrix, 2); Fill(m.ViewMatrix, 5);
        m.CurrentVSConstantLayout = &layout;
        m.UpdateFlags = 0x18001;
        m.UpdateLocalisedSphereMapping();
        Expect("flags", pass, (float)m.UpdateFlags, (float)0x10001);
        if (pass == 2) { Expect("no upload", pass, (float)m.UploadCount, 0.0f); continue; }
        Expect("start", pass, (float)m.UploadStart, 12.0f);
        Expect("count", pass, (float)m.UploadCount, 2.0f);
        float want[4][4];
        RefMul(want, m.ViewMatrix, m.WorldMatrix);
        for (int a = 0; a < 2; ++a) {
            const float sign = a ? -1.0f : 1.0f;
            const double len = sqrt((double)want[a][0] * want[a][0] + (double)want[a][1] * want[a][1] + (double)want[a][2] * want[a][2]);
            const float* got = &m.Upload[a].X;
            for (int c = 0; c < 3; ++c)
                if (fabs(got[c] - sign * want[a][c] / len * 0.5) > 1e-4) { printf("  pass %d axis %d [%d] %.9g\n", pass, a, c, got[c]); ++g_bad; }
            Expect("W", pass, m.Upload[a].W, 0.5f);
        }
        if (pass == 0) memcpy(first, m.Upload, sizeof(first));
        else for (int a = 0; a < 2; ++a) {      // the SSE and x87 multiplies give the same (exact) matrix
            Expect("sse==x87 X", pass, m.Upload[a].X, first[a].X); Expect("sse==x87 Y", pass, m.Upload[a].Y, first[a].Y);
            Expect("sse==x87 Z", pass, m.Upload[a].Z, first[a].Z);
        }
    }
    printf(g_bad ? "00988e80_TEST FAIL\n" : "00988e80_TEST PASS\n");
    return g_bad ? 1 : 0;
}
