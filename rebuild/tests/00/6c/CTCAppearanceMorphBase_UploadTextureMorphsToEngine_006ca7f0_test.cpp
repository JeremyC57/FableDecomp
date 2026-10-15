#include <cstdio>
#include <cstring>
#include <math.h>

struct CLongSet { int n; };
struct CEngineTextureMorphSet { int id; };
struct CMeshMorphSet { CEngineTextureMorphSet OriginalMeshTextureMorphs; };
static int g_uploads = 0; static const CEngineTextureMorphSet* g_morphs = 0; static bool g_flag = false; static float g_amount = 0.0f; static const CLongSet* g_ids = 0;
struct CIEngine {
    void UploadTextureMorphs(const CEngineTextureMorphSet& m, bool flag, float amount, const CLongSet* ids) { ++g_uploads; g_morphs = &m; g_flag = flag; g_amount = amount; g_ids = ids; }
};
static bool g_bTextureMorphsEnabled = true;
struct CTCAppearanceMorphBase {
    CMeshMorphSet m_set; int m_updates;
    void UpdateMorphs() { ++m_updates; }
    CMeshMorphSet* GetMeshTextureMorphs() { return &m_set; }
    void UploadTextureMorphsToEngine(CIEngine& engine, const CLongSet* ids, bool flag, float amount)
    {
        if (g_bTextureMorphsEnabled) {
            UpdateMorphs();
            const CEngineTextureMorphSet& morphs = GetMeshTextureMorphs()->OriginalMeshTextureMorphs;
            engine.UploadTextureMorphs(morphs, flag, amount, ids);
        }
    }
};
int main()
{
    CTCAppearanceMorphBase m; m.m_updates = 0; m.m_set.OriginalMeshTextureMorphs.id = 8;
    CIEngine engine; CLongSet ids; ids.n = 3;
    m.UploadTextureMorphsToEngine(engine, &ids, true, 0.5f);
    if (g_uploads != 1 || m.m_updates != 1 || g_morphs != &m.m_set.OriginalMeshTextureMorphs || !g_flag || g_amount != 0.5f || g_ids != &ids) { std::printf("006ca7f0_TEST FAIL\n"); return 1; }
    g_bTextureMorphsEnabled = false; m.UploadTextureMorphsToEngine(engine, 0, false, 1.0f);
    if (g_uploads != 1 || m.m_updates != 1) { std::printf("006ca7f0_TEST FAIL\n"); return 1; }
    std::printf("006ca7f0_TEST PASS\n");
    return 0;
}