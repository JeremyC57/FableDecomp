// Behaviour test for 0x00BEC680: CEngineInternalPrimitiveMeshBase::CollectStats (host model: the row's body over stub mesh info,
// mesh, materials and texture bank; std::map for the stats)
#include <cstdio>
#include <map>

enum EEnginePrimitiveStatIndex { ENGINE_PRIM_STAT_VERTICES, ENGINE_PRIM_STAT_TRIANGLES, ENGINE_PRIM_STAT_TEXTURES };

class C3DMeshMaterial { public:
    unsigned long DiffuseMapBankIndex, BumpMapBankIndex, ReflectionMapBankIndex, IlluminationMapBankIndex;
    unsigned long GetDiffuseMapBankIndex() const { return DiffuseMapBankIndex; }
    unsigned long GetBumpMapBankIndex() const { return BumpMapBankIndex; }
    unsigned long GetReflectionMapBankIndex() const { return ReflectionMapBankIndex; }
    unsigned long GetIlluminationMapBankIndex() const { return IlluminationMapBankIndex; }
};
class C3DMesh2 { public:
    unsigned long Vertices, Triangles, MaterialCount;
    C3DMeshMaterial* Materials;
    unsigned long GetVertexCount() const { return Vertices; }
    unsigned long GetTriangleCount() const { return Triangles; }
    unsigned long GetMaterialCount() const { return MaterialCount; }
    const C3DMeshMaterial* GetMaterial(unsigned long i) const { return &Materials[i]; }
};
class CGraphicDataBank { public:
    unsigned long GetEntryDataSize(unsigned long index) const { return index * 100; }
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCountedPointer() : Data(0) { }
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};
static int g_releases = 0;
class C3DMeshInfo { public:
    C3DMesh2* Mesh;                             // 0 = GetMesh fails
    CGraphicDataBank* Textures;
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags) const {
        if (lod != 0 || flags != 4 || Mesh == 0) return false;
        mesh.Data = Mesh;
        return true;
    }
    const CGraphicDataBank* PeekTextureBank() const { return Textures; }
};
typedef std::map<unsigned long, unsigned long> StatMap;
class CEngineInternalPrimitiveMeshBase { public:
    static void CollectStats(const C3DMeshInfo& mesh_info, StatMap& stats, EEnginePrimitiveStatIndex stat_index, unsigned long);
};

// the row's body
void CEngineInternalPrimitiveMeshBase::CollectStats(const C3DMeshInfo& mesh_info, StatMap& stats,
                                                    EEnginePrimitiveStatIndex stat_index, unsigned long)
{
    CCountedPointer<const C3DMesh2> mesh;
    if (!mesh_info.GetMesh(mesh, 0, 4))
        return;
    unsigned long key;
    switch (stat_index) {
    case ENGINE_PRIM_STAT_VERTICES: {
        key = (unsigned long)(size_t)&mesh_info;
        unsigned long vertex_count = mesh->GetVertexCount();
        stats[key] = vertex_count;
        break;
    }
    case ENGINE_PRIM_STAT_TRIANGLES: {
        key = (unsigned long)(size_t)&mesh_info;
        unsigned long triangle_count = mesh->GetTriangleCount();
        stats[key] = triangle_count;
        break;
    }
    case ENGINE_PRIM_STAT_TEXTURES: {
        const C3DMesh2* p_mesh = mesh.Get();
        for (unsigned long i = 0; i < p_mesh->GetMaterialCount(); ++i) {
            const C3DMeshMaterial* material = p_mesh->GetMaterial(i);
            key = material->GetDiffuseMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
            key = material->GetBumpMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
            key = material->GetReflectionMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
            key = material->GetIlluminationMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
        }
        break;
    }
    default:
        return;
    }
}

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }

int main()
{
    C3DMeshMaterial materials[2] = { { 3, 0, 7, 0 }, { 3, 5, 0, 9 } };
    C3DMesh2 mesh = { 120, 40, 2, materials };
    CGraphicDataBank bank;
    C3DMeshInfo info = { &mesh, &bank };
    unsigned long self = (unsigned long)(size_t)&info;

    StatMap v;
    CEngineInternalPrimitiveMeshBase::CollectStats(info, v, ENGINE_PRIM_STAT_VERTICES, 0);
    Check(v.size() == 1 && v[self] == 120);
    StatMap t;
    CEngineInternalPrimitiveMeshBase::CollectStats(info, t, ENGINE_PRIM_STAT_TRIANGLES, 0);
    Check(t.size() == 1 && t[self] == 40);
    StatMap x;
    CEngineInternalPrimitiveMeshBase::CollectStats(info, x, ENGINE_PRIM_STAT_TEXTURES, 0);
    Check(x.size() == 4 && x[3] == 300 && x[5] == 500 && x[7] == 700 && x[9] == 900 && x.find(0) == x.end());
    StatMap none;                                   // unknown index, or no mesh: nothing recorded
    CEngineInternalPrimitiveMeshBase::CollectStats(info, none, (EEnginePrimitiveStatIndex)3, 0);
    info.Mesh = 0;
    CEngineInternalPrimitiveMeshBase::CollectStats(info, none, ENGINE_PRIM_STAT_VERTICES, 0);
    Check(none.empty());
    if (g_fail) { std::printf("00bec680_TEST FAIL\n"); return 1; }
    std::printf("00bec680_TEST PASS\n");
    return 0;
}