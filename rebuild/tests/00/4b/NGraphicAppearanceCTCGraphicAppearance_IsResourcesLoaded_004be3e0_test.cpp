// Behaviour test for CTCGraphicAppearance::IsResourcesLoaded (0x004BE3E0): every component's meshes are gathered; the result is
// false when any mesh is not loaded, and loading is begun for each missing mesh only when asked.
#include <cstdio>
#include <vector>
#include <list>
struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh : 1; };
class C3DMeshInfo { public:
    bool Loaded; mutable int BeginCount;
    bool IsLoaded() const { return Loaded; }
    void BeginLoading(unsigned long, bool, bool) const { ++BeginCount; }
};
class CMeshDataBank { public: C3DMeshInfo Infos[8]; const C3DMeshInfo* PeekMeshInfo(unsigned long i) const { return &Infos[i]; } };
static CMeshDataBank Bank;
static CMeshDataBank* g_pMeshDataBank = &Bank;
struct CAGraphicAppearanceComponent {
    std::vector<long> Meshes;
    virtual ~CAGraphicAppearanceComponent() {}
    virtual void GetMeshesUsed(std::vector<CMeshInfo>& meshes) const
    { for (size_t i = 0; i < Meshes.size(); ++i) { CMeshInfo m; m.Index = Meshes[i]; m.SizeX = 0; m.PrimaryMesh = 0; meshes.push_back(m); } }
};
struct CTCGraphicAppearance {
    std::list<CAGraphicAppearanceComponent*> Components;
    void GetAllMeshes(std::vector<CMeshInfo>& meshes) const
    { meshes.reserve(4); for (std::list<CAGraphicAppearanceComponent*>::const_iterator it = Components.begin(); it != Components.end(); ++it) (*it)->GetMeshesUsed(meshes); }
    bool IsResourcesLoadedForMesh(const CMeshInfo& mesh, bool begin_loading) const
    {
        const C3DMeshInfo* mesh_info = g_pMeshDataBank->PeekMeshInfo(mesh.Index);
        bool loaded = mesh_info->IsLoaded();
        if (!loaded && begin_loading) mesh_info->BeginLoading((unsigned long)-1, false, false);
        return loaded;
    }
    bool IsResourcesLoaded(bool begin_loading) const
    {
        std::vector<CMeshInfo> meshes;
        GetAllMeshes(meshes);
        bool loaded = true;
        for (std::vector<CMeshInfo>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
            if (!IsResourcesLoadedForMesh(*it, begin_loading)) loaded = false;
        return loaded;
    }
};
int main()
{
    for (int i = 0; i < 8; ++i) { Bank.Infos[i].Loaded = true; Bank.Infos[i].BeginCount = 0; }
    CAGraphicAppearanceComponent a, b; a.Meshes.push_back(1); a.Meshes.push_back(2); b.Meshes.push_back(3);
    CTCGraphicAppearance app; app.Components.push_back(&a); app.Components.push_back(&b);
    if (!app.IsResourcesLoaded(true)) { std::printf("004be3e0_TEST FAIL\n"); return 1; }
    Bank.Infos[2].Loaded = false; Bank.Infos[3].Loaded = false;
    if (app.IsResourcesLoaded(false) || Bank.Infos[2].BeginCount != 0) { std::printf("004be3e0_TEST FAIL\n"); return 1; }
    if (app.IsResourcesLoaded(true) || Bank.Infos[2].BeginCount != 1 || Bank.Infos[3].BeginCount != 1 || Bank.Infos[1].BeginCount != 0)
        { std::printf("004be3e0_TEST FAIL\n"); return 1; }
    std::printf("004be3e0_TEST PASS\n");
    return 0;
}