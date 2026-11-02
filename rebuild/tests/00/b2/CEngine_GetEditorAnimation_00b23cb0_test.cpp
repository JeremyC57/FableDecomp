// Test: CEngine::GetEditorAnimation(name) sets the animation up in the resource manager and peeks it from mesh bank 1 by the returned index.
#include <cstdio>
class CWideString { public: int Key; };
class C3DAnimationInfo { public: int Id; };
static C3DAnimationInfo g_infos[4];
static const CWideString* g_setupName = 0;
class CMeshDataBank { public:
    int Bank;
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const { return Bank == 1 ? &g_infos[index] : 0; }
};
class CEngineResourceManager { public:
    CMeshDataBank* MeshBanks[3];
    unsigned long SetupEditorAnimation(const CWideString& name) { g_setupName = &name; return (unsigned long)name.Key; }
};
static CEngineResourceManager* g_pResourceManager = 0;
class CEngine { public:
    virtual ~CEngine() { }
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString& name) const;
};
const C3DAnimationInfo* CEngine::GetEditorAnimation(const CWideString& name) const
{
    unsigned long index = g_pResourceManager->SetupEditorAnimation(name);
    const CMeshDataBank* bank = g_pResourceManager->MeshBanks[1];
    return bank->PeekAnimation(index);
}
int main()
{
    CMeshDataBank b0, b1, b2;
    b0.Bank = 0; b1.Bank = 1; b2.Bank = 2;
    CEngineResourceManager manager;
    manager.MeshBanks[0] = &b0; manager.MeshBanks[1] = &b1; manager.MeshBanks[2] = &b2;
    g_pResourceManager = &manager;
    CWideString name; name.Key = 2;
    CEngine engine;
    const C3DAnimationInfo* info = engine.GetEditorAnimation(name);
    if (info != &g_infos[2] || g_setupName != &name) { std::printf("00b23cb0_TEST FAIL\n"); return 1; }
    std::printf("00b23cb0_TEST PASS\n");
    return 0;
}