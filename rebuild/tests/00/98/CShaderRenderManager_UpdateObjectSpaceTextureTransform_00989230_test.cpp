// Behaviour test for CShaderRenderManager::UpdateObjectSpaceTextureTransform (0x00989230): each in-use texture-transform range gets
// TextureTransforms[i] * WorldMatrix (at most four rows; ranges of 2, 4 and 6 registers), empty ranges are skipped, only the first
// TextureTransformCount ranges are visited, dirty bit 0x800 moves to 0x1000;
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
class C3DVector { public: float X, Y, Z; };
struct ALIGN16 C4DVector { float X, Y, Z, W; };
static bool GEnableSIMD;
static int g_multiplies;                                                 // matrix products formed (skipped ranges form none)
struct ALIGN16 CMatrix4x4 {
    float Data[4][4];
    float& operator()(int row, int column) { return Data[row][column]; }
    const float operator()(int row, int column) const { return Data[row][column]; }
    void AlignedSetMultiplyTransposed(const CMatrix4x4& a, const CMatrix4x4& b) { AlignedSetMultiply(b, a); }
    void AlignedPreScale(const C3DVector& scale);
    void AlignedPreScaleTransposed(const C3DVector& scale);
    void AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b)   // the header's inline member (SSE kernel / x87 loop)
    {
        ++g_multiplies;
        if (GEnableSIMD) {
            const CMatrix4x4* pa = &a;
            const CMatrix4x4* pb = &b;
            CMatrix4x4* pdst = this;
            GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);
        } else {
            CMatrix4x4& m = *this;                        // Ego_d arm + the row's FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230 reference
            const CMatrix4x4& r = b;
            for (long c1 = 0; c1 < 4; ++c1)
                for (long c2 = 0; c2 < 4; ++c2)
                    m(c1, c2) = a(c1, 0) * r(0, c2) + a(c1, 1) * r(1, c2) + a(c1, 2) * r(2, c2) + a(c1, 3) * r(3, c2);
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
class CShaderConstantRange { public: long Offset; unsigned long Count; };
class CVertexShaderConstantLayout { public: CShaderConstantRange TextureTransform[4]; };
struct Upload { long Start; unsigned long Count; float Rows[4][4]; };
class CShaderRenderManager { public:
    CMatrix4x4 WorldMatrix;
    CMatrix4x4 TextureTransforms[4];
    long TextureTransformCount;
    const CVertexShaderConstantLayout* CurrentVSConstantLayout;
    unsigned long UpdateFlags;
    Upload Uploads[4];
    int UploadCount;
    void SetVSConstants(long index, const void* data, unsigned long count)
    {
        if (count == 0)
            return;
        Upload& u = Uploads[UploadCount++];
        u.Start = index; u.Count = count;
        memcpy(u.Rows, data, count * 16);
    }
    void UpdateObjectSpaceTextureTransform()
    {
        for (unsigned long c_transform = 0; c_transform < (unsigned long)TextureTransformCount; ++c_transform) {
            const CShaderConstantRange& range = CurrentVSConstantLayout->TextureTransform[c_transform];
            if (range.Count) {
                ALIGN16 CMatrix4x4 transform;
                transform.AlignedSetMultiplyTransposed(WorldMatrix, TextureTransforms[c_transform]);
                unsigned long rows = range.Count < 4 ? range.Count : 4;
                SetVSConstants(range.Offset, &transform, rows);
            }
        }
        UpdateFlags |= 0x1000;
        UpdateFlags &= ~0x800;
    }
};
int main()
{
    static const CVertexShaderConstantLayout layout = { { { 10, 2 }, { 20, 0 }, { 30, 6 }, { 40, 4 } } };
    for (int pass = 0; pass < 4; ++pass) {
        GEnableSIMD = (pass & 1) == 0;
        static CShaderRenderManager m;
        memset(&m, 0, sizeof(m));
        Fill(m.WorldMatrix, 1);
        for (int i = 0; i < 4; ++i) Fill(m.TextureTransforms[i], 4 + i * 3);
        m.TextureTransformCount = pass < 2 ? 3 : 4;  // passes 0/1: the fourth range is not visited
        m.CurrentVSConstantLayout = &layout;
        m.UpdateFlags = 0x880;
        g_multiplies = 0;
        m.UpdateObjectSpaceTextureTransform();
        Expect("flags", pass, (float)m.UpdateFlags, (float)0x1080);
        const int n = pass < 2 ? 2 : 3;
        Expect("uploads", pass, (float)m.UploadCount, (float)n);
        Expect("products", pass, (float)g_multiplies, (float)n);
        const int idx[3] = { 0, 2, 3 };
        const long start[3] = { 10, 30, 40 };
        const unsigned long rows[3] = { 2, 4, 4 };
        for (int u = 0; u < n && u < m.UploadCount; ++u) {
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