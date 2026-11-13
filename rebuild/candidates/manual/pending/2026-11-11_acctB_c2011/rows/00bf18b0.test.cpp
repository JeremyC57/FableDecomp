// Behaviour test for 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh&,
// const CMatrix3x4&): a self-contained host model of the row's logic. Each generator with a bank index asks the mesh stats for its
// animated dummy-object position (blend states, the primitive's interpolate, the bank's animation manager), pre-multiplies that
// transform by the generated mesh's inverse transform when the generator uses its local origin (an unloaded LOD starts loading
// and skips the generator), post-multiplies it by the object transform argument and adds a static mesh.
#include <cstdio>
#include <cmath>

struct Mat { float E[12]; };
static void SetMultiply(Mat& r, const Mat& m1, const Mat& m2)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j) {
            float s = m1.E[i * 3 + 0] * m2.E[j] + m1.E[i * 3 + 1] * m2.E[3 + j] + m1.E[i * 3 + 2] * m2.E[6 + j];
            if (i == 3) s += m2.E[9 + j];
            r.E[i * 3 + j] = s;
        }
}
static void PreMultiply(Mat& t, const Mat& m) { Mat copy = t; SetMultiply(t, m, copy); }
static void PostMultiply(Mat& t, const Mat& m) { Mat copy = t; SetMultiply(t, copy, m); }
static Mat Make(float s, float tx, float ty, float tz)
{
    Mat m; for (int i = 0; i < 12; ++i) m.E[i] = 0.0f;
    m.E[0] = s; m.E[4] = s; m.E[8] = s; m.E[9] = tx; m.E[10] = ty; m.E[11] = tz; return m;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

struct MeshInfo {
    unsigned long lodDataSizes[4];
    bool lodLoaded[4];
    Mat inverse;
    int beginLoadingCalls;
    bool HasLOD(unsigned long lod) const { return lod < 4 && lodDataSizes[lod] != 0; }
    unsigned long GetBestCurrentDetailLevel() const {
        for (unsigned long lod = 0; HasLOD(lod); ++lod)
            if (lodLoaded[lod]) return lod;
        return 0xffffffff;
    }
    bool GetMesh(const Mat*& mesh, unsigned long lod, unsigned long flags) const { if (flags != 0 || !lodLoaded[lod]) return false; mesh = &inverse; return true; }
    void BeginLoading(unsigned long) { ++beginLoadingCalls; }
};
struct Generator { Mat transform; unsigned long bankIndex; bool useLocalOrigin; };
struct Dummy { Mat transform; long boneIndex; };
// The mesh stats' animated dummy position: the generator's transform scaled by the interpolate, translated by the blend states.
static int g_dummyCalls = 0;
static float g_lastInterpolate = 0.0f;
static void GetDummyObjectPositionAnimated(const Generator& g, Dummy& out, float startX, float endX, float interpolate, bool flag)
{
    ++g_dummyCalls; g_lastInterpolate = interpolate;
    out.transform = g.transform;
    out.transform.E[9] += startX + (endX - startX) * interpolate;
    out.boneIndex = flag ? 1 : 0;
}
struct Added { const MeshInfo* info; Mat transform; };
static MeshInfo g_bank[3];
static Added g_added[8];
static int g_addedCount = 0;

static void AddGeneratedObjects(bool loaded, const Generator* begin, const Generator* end, float startX, float endX, float interpolate,
                                const Mat& objectTransform)
{
    if (loaded) {
        for (const Generator* it = begin; it != end; ++it) {
            const Generator& generator = *it;
            if (generator.bankIndex) {
                MeshInfo* mesh_info = &g_bank[generator.bankIndex];
                Dummy dummy;
                GetDummyObjectPositionAnimated(generator, dummy, startX, endX, interpolate, false);
                Mat transform = dummy.transform;
                if (generator.useLocalOrigin) {
                    const Mat* mesh = 0;
                    unsigned long lod = mesh_info->GetBestCurrentDetailLevel();
                    if (lod == 0xffffffff || !mesh_info->GetMesh(mesh, lod, 0)) { mesh_info->BeginLoading(0xffffffff); continue; }
                    PreMultiply(transform, *mesh);
                }
                PostMultiply(transform, objectTransform);
                g_added[g_addedCount].info = mesh_info; g_added[g_addedCount].transform = transform; ++g_addedCount;
            }
        }
    }
}

int main()
{
    for (int i = 0; i < 3; ++i) {
        g_bank[i].beginLoadingCalls = 0; g_bank[i].inverse = Make(0.5f, 0.0f, -2.0f, 0.0f);
        for (int l = 0; l < 4; ++l) { g_bank[i].lodDataSizes[l] = 1; g_bank[i].lodLoaded[l] = false; }
    }
    g_bank[1].lodLoaded[0] = true;
    Generator gens[3];
    gens[0].transform = Make(2.0f, 1.0f, 0.0f, 0.0f); gens[0].bankIndex = 1; gens[0].useLocalOrigin = true;
    gens[1].transform = Make(1.0f, 0.0f, 0.0f, 0.0f); gens[1].bankIndex = 0; gens[1].useLocalOrigin = false;
    gens[2].transform = Make(1.0f, 0.0f, 0.0f, 4.0f); gens[2].bankIndex = 2; gens[2].useLocalOrigin = true;
    Mat object = Make(1.0f, 0.0f, 0.0f, 100.0f);
    AddGeneratedObjects(true, gens, gens + 3, 1.0f, 3.0f, 0.25f, object);
    bool ok = g_addedCount == 1 && g_dummyCalls == 2 && Near(g_lastInterpolate, 0.25f);
    // dummy: scale 2, X 1 + 1.5 = 2.5; pre (scale 0.5, ty -2): scale 1, translation (2.5, -4, 0); post: + (0, 0, 100)
    ok = ok && g_added[0].info == &g_bank[1] && Near(g_added[0].transform.E[0], 1.0f) && Near(g_added[0].transform.E[9], 2.5f)
            && Near(g_added[0].transform.E[10], -4.0f) && Near(g_added[0].transform.E[11], 100.0f);
    ok = ok && g_bank[2].beginLoadingCalls == 1 && g_bank[1].beginLoadingCalls == 0;
    if (!ok) { std::printf("00bf18b0_TEST FAIL\n"); return 1; }
    std::printf("00bf18b0_TEST PASS\n");
    return 0;
}
