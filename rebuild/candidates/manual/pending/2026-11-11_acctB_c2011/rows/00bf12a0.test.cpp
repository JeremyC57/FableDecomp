// Behaviour test for 0x00BF12A0 CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh&):
// a self-contained host model of the row's logic (mesh generators of a loaded mesh -> one static mesh each, the generator's
// transform pre-multiplied by the generated mesh's inverse transform when it uses its local origin, then post-multiplied by
// the primitive's object transform; an unloaded LOD starts loading and skips the generator).
#include <cstdio>
#include <cmath>

struct Mat { float E[12]; };                       // E11..E43, rows of three
static void SetMultiply(Mat& r, const Mat& m1, const Mat& m2)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j) {
            float s = m1.E[i * 3 + 0] * m2.E[0 * 3 + j] + m1.E[i * 3 + 1] * m2.E[1 * 3 + j] + m1.E[i * 3 + 2] * m2.E[2 * 3 + j];
            if (i == 3) s += m2.E[9 + j];
            r.E[i * 3 + j] = s;
        }
}
static void PreMultiply(Mat& t, const Mat& m) { Mat copy = t; SetMultiply(t, m, copy); }
static void PostMultiply(Mat& t, const Mat& m) { Mat copy = t; SetMultiply(t, copy, m); }

struct MeshInfo {
    bool hasMeshData;
    unsigned long lodDataSizes[4];
    bool lodLoaded[4];
    Mat inverse;                                  // C3DMesh2::InverseTransform of the fetched LOD
    int beginLoadingCalls;
    unsigned long lastBeginLoadingLod;
    bool HasLOD(unsigned long lod) const { return lod < 4 && lodDataSizes[lod] != 0; }
    unsigned long GetBestCurrentDetailLevel() const {
        if (hasMeshData)
            for (unsigned long lod = 0; HasLOD(lod); ++lod)
                if (lodLoaded[lod]) return lod;
        return 0xffffffff;
    }
    bool GetMesh(const Mat*& mesh, unsigned long lod, unsigned long) const { if (!lodLoaded[lod]) return false; mesh = &inverse; return true; }
    void BeginLoading(unsigned long lod) { ++beginLoadingCalls; lastBeginLoadingLod = lod; }
};
struct Generator { Mat transform; unsigned long bankIndex; bool useLocalOrigin; };
struct Added { const MeshInfo* info; const MeshInfo* shadow; Mat transform; float scale; };

static MeshInfo g_bank[4];
static Added g_added[8];
static int g_addedCount = 0;

static void AddGeneratedObjects(bool loaded, const Generator* begin, const Generator* end, const Mat& objectTransform, float scale)
{
    if (loaded) {
        for (const Generator* it = begin; it != end; ++it) {
            if (it->bankIndex) {
                MeshInfo* mesh_info = &g_bank[it->bankIndex];
                Mat transform = it->transform;
                if (it->useLocalOrigin) {
                    const Mat* mesh = 0;
                    unsigned long lod = mesh_info->GetBestCurrentDetailLevel();
                    if (lod == 0xffffffff || !mesh_info->GetMesh(mesh, lod, 4)) {
                        mesh_info->BeginLoading(0xffffffff);
                        continue;
                    }
                    PreMultiply(transform, *mesh);
                }
                PostMultiply(transform, objectTransform);
                Added& a = g_added[g_addedCount++];
                a.info = mesh_info; a.shadow = mesh_info; a.transform = transform; a.scale = scale;
            }
        }
    }
}

static Mat Make(float s, float tx, float ty, float tz)
{
    Mat m; for (int i = 0; i < 12; ++i) m.E[i] = 0.0f;
    m.E[0] = s; m.E[4] = s; m.E[8] = s; m.E[9] = tx; m.E[10] = ty; m.E[11] = tz; return m;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    for (int i = 0; i < 4; ++i) {
        g_bank[i].hasMeshData = true; g_bank[i].beginLoadingCalls = 0; g_bank[i].lastBeginLoadingLod = 0;
        for (int l = 0; l < 4; ++l) { g_bank[i].lodDataSizes[l] = 1; g_bank[i].lodLoaded[l] = false; }
        g_bank[i].inverse = Make(1.0f, -1.0f, 0.0f, 0.0f);
    }
    g_bank[1].lodLoaded[2] = true;                 // bank 1: LOD 2 loaded (LODs 0, 1 not yet)
    g_bank[3].lodDataSizes[1] = 0;                 // bank 3: LOD 1 missing and LOD 0 not loaded -> none
    g_bank[3].lodLoaded[2] = true;
    Generator gens[4];
    gens[0].transform = Make(2.0f, 1.0f, 2.0f, 3.0f); gens[0].bankIndex = 1; gens[0].useLocalOrigin = true;
    gens[1].transform = Make(1.0f, 5.0f, 0.0f, 0.0f); gens[1].bankIndex = 0; gens[1].useLocalOrigin = false;   // skipped
    gens[2].transform = Make(1.0f, 0.0f, 7.0f, 0.0f); gens[2].bankIndex = 2; gens[2].useLocalOrigin = false;
    gens[3].transform = Make(1.0f, 0.0f, 0.0f, 9.0f); gens[3].bankIndex = 3; gens[3].useLocalOrigin = true;    // loads, skipped
    Mat object = Make(3.0f, 10.0f, 20.0f, 30.0f);
    AddGeneratedObjects(false, gens, gens + 4, object, 1.5f);
    bool ok = g_addedCount == 0;
    AddGeneratedObjects(true, gens, gens + 4, object, 1.5f);
    ok = ok && g_addedCount == 2;
    ok = ok && g_added[0].info == &g_bank[1] && g_added[0].shadow == &g_bank[1] && Near(g_added[0].scale, 1.5f);
    // (inverse * gen) * object: x scale 2*3 = 6, translation ((-1)*2 + 1, 2, 3) * 3 + (10, 20, 30) = (7, 26, 39)
    ok = ok && Near(g_added[0].transform.E[0], 6.0f) && Near(g_added[0].transform.E[9], 7.0f)
            && Near(g_added[0].transform.E[10], 26.0f) && Near(g_added[0].transform.E[11], 39.0f);
    ok = ok && g_added[1].info == &g_bank[2] && Near(g_added[1].transform.E[10], 41.0f) && Near(g_added[1].transform.E[4], 3.0f);
    ok = ok && g_bank[3].beginLoadingCalls == 1 && g_bank[3].lastBeginLoadingLod == 0xffffffff && g_bank[1].beginLoadingCalls == 0;
    if (!ok) { std::printf("00bf12a0_TEST FAIL\n"); return 1; }
    std::printf("00bf12a0_TEST PASS\n");
    return 0;
}
