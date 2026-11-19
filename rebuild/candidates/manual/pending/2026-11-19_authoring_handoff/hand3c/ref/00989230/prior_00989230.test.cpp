// Behaviour test for CShaderRenderManager::UpdateObjectSpaceTextureTransform (0x00989230): each in-use texture-transform range gets
// TextureTransforms[i] * WorldMatrix (at most four rows), empty ranges are skipped, dirty bit 0x800 moves to 0x1000;
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
struct CShaderConstantRange { long Offset; unsigned long Count; };
struct CVertexShaderConstantLayout { CShaderConstantRange TextureTransform[4]; };
struct Upload { long Start; unsigned long Count; float Rows[4][4]; };
struct CShaderRenderManager {
    CMatrix4x4 WorldMatrix;
    CMatrix4x4 TextureTransforms[4];
    long TextureTransformCount;
    const CVertexShaderConstantLayout* CurrentVSConstantLayout;
    unsigned long UpdateFlags;
    Upload Uploads[4];
    int UploadCount;
    void SetVSConstants(long start, const void* data, unsigned long count)
    {
        if (count != 0) {
            Upload& u = Uploads[UploadCount++];
            u.Start = start; u.Count = count;
            memcpy(u.Rows, data, count * 16);
        }
    }
    void UpdateObjectSpaceTextureTransform()
    {
        for (unsigned long i = 0; i < (unsigned long)TextureTransformCount; ++i) {
            const CShaderConstantRange& range = CurrentVSConstantLayout->TextureTransform[i];
            if (range.Count) {
                ALIGN16 CMatrix4x4 transform;
                transform.AlignedSetMultiply(TextureTransforms[i], WorldMatrix);
                SetVSConstants(range.Offset, &transform, range.Count < 4 ? range.Count : 4);
            }
        }
        UpdateFlags = (UpdateFlags & ~0x800) | 0x1000;
    }
};
int main()
{
    static const CVertexShaderConstantLayout layout = { { { 10, 2 }, { 20, 0 }, { 30, 6 }, { 40, 4 } } };
    for (int pass = 0; pass < 2; ++pass) {
        g_SSEAvailable = pass == 0;
        static CShaderRenderManager m;
        memset(&m, 0, sizeof(m));
        Fill(m.WorldMatrix, 1);
        for (int i = 0; i < 4; ++i) Fill(m.TextureTransforms[i], 4 + i * 3);
        m.TextureTransformCount = 3;                 // the fourth range is not visited
        m.CurrentVSConstantLayout = &layout;
        m.UpdateFlags = 0x880;
        m.UpdateObjectSpaceTextureTransform();
        Expect("flags", pass, (float)m.UpdateFlags, (float)0x1080);
        Expect("uploads", pass, (float)m.UploadCount, 2.0f);
        const int idx[2] = { 0, 2 };
        const long start[2] = { 10, 30 };
        const unsigned long rows[2] = { 2, 4 };
        for (int u = 0; u < 2 && u < m.UploadCount; ++u) {
            float want[4][4];
            RefMul(want, m.TextureTransforms[idx[u]], m.WorldMatrix);
            Expect("start", pass, (float)m.Uploads[u].Start, (float)start[u]);
            Expect("count", pass, (float)m.Uploads[u].Count, (float)rows[u]);
            for (unsigned long r = 0; r < rows[u]; ++r)
                for (int c = 0; c < 4; ++c) Expect("row", pass, m.Uploads[u].Rows[r][c], want[r][c]);
        }
    }
    printf(g_bad ? "00989230_TEST FAIL\n" : "00989230_TEST PASS\n");
    return g_bad ? 1 : 0;
}
