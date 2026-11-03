#include <cstdio>
// Behaviour test for CEngine::UpdateTextureMorphCache at 0x00B239C0: change the bank scene, then forward all four arguments.
static int g_scene = 0, g_morph = 0; static const void* g_morphs = 0; static bool g_flag = false; static float g_value = 0; static const void* g_indices = 0;
class CEngineTextureMorph { };
template <class T> class CArray { public: T* first; };
class CGraphicDataBank { public: void ChangeScene() { ++g_scene; } };
struct CEngineData { CGraphicDataBank* m_pGraphicBank; };
class CEngineSurfaceCompositionManager { public:
    void UpdateDiskCache(const CArray<CEngineTextureMorph>& morphs, bool flag, float value, const int* indices)
    { ++g_morph; g_morphs = &morphs; g_flag = flag; g_value = value; g_indices = indices; }
};
static CGraphicDataBank g_bank; static CEngineData g_data; static CEngineData* g_pEngineData = &g_data;
static CEngineSurfaceCompositionManager g_comp; static CEngineSurfaceCompositionManager* g_pSurfaceComposition = &g_comp;
class CEngine { public:
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs, bool flag, float value, const int* indices)
    { g_pEngineData->m_pGraphicBank->ChangeScene(); g_pSurfaceComposition->UpdateDiskCache(morphs, flag, value, indices); }
};
int main() {
    g_data.m_pGraphicBank = &g_bank;
    CEngine engine; CArray<CEngineTextureMorph> morphs; int idx = 0;
    engine.UpdateTextureMorphCache(morphs, true, 2.5f, &idx);
    if (g_scene != 1 || g_morph != 1 || g_morphs != &morphs || !g_flag || g_value < 2.4f || g_value > 2.6f || g_indices != &idx) { std::printf("00b239c0_TEST FAIL\n"); return 1; }
    std::printf("00b239c0_TEST PASS\n"); return 0;
}
