// Behaviour test for CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately (0x00BA6DC0): which mesh primitives get a
// stack sub-primitive and are rendered (triangles present, material has a diffuse map, not hidden by the morph info).
#include <cstdio>

class C3DMeshMaterial { public: unsigned long DiffuseMapBankIndex; };
class C3DPrimitive2 { public: long MaterialIndex; unsigned long TriangleCount; };
class C3DMesh2 { public: C3DMeshMaterial* Materials; unsigned long PrimitiveCount; C3DPrimitive2* Primitives; };

class CEngineInternalPrimitiveBase { public: virtual ~CEngineInternalPrimitiveBase() {} long Type; };
class CEngineSubPrimitiveBase {
public:
    virtual ~CEngineSubPrimitiveBase() {}
    unsigned long                 RenderFlags;
    CEngineInternalPrimitiveBase* Parent;
    CEngineSubPrimitiveBase*      NextSubPrimitive;
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
};
class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {
public:
    C3DPrimitive2* MeshPrimitive;
    long           RenderMode;
    bool           IsCloth;
    CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase& parent, C3DPrimitive2* p, long mode)
        : CEngineSubPrimitiveBase(parent), MeshPrimitive(p), RenderMode(mode), IsCloth(false) { }
};
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveMeshBase {
public:
    CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitiveBase& parent, C3DPrimitive2* p, long mode)
        : CEngineSubPrimitiveMeshBase(parent, p, mode) { }
};

static int g_initialised;
class CEngineInternalPrimitiveMorphInfo {
public:
    unsigned int   HiddenMaterialNameCount;
    unsigned char* HiddenStart;
    unsigned char* HiddenFinish;
    unsigned char  Storage[8];
    void InitialiseHiddenMaterials(const C3DMesh2&) { ++g_initialised; HiddenStart = Storage; HiddenFinish = Storage + 4; }
    const unsigned char* GetHiddenMaterials(const C3DMesh2& mesh)
    {
        if (HiddenMaterialNameCount == 0)
            return 0;
        if (HiddenStart == HiddenFinish)
            InitialiseHiddenMaterials(mesh);
        return (HiddenFinish - HiddenStart) ? HiddenStart : (const unsigned char*)this;
    }
};
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveBase { public: C3DMesh2* Mesh; };
class CEngineInternalPrimitiveMorphedStaticMesh : public CEngineInternalPrimitiveStaticMesh { public: CEngineInternalPrimitiveMorphInfo Morph; };

static long g_rendered[8];
static int g_count;
static bool g_bad;
class CEnginePrimitiveManagerStaticMeshesBase {
public:
    void RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh& sub, long pass)
    {
        if (sub.RenderFlags != 2 || sub.RenderMode != 7 || pass != 3 || sub.IsCloth || sub.NextSubPrimitive != 0) g_bad = true;
        g_rendered[g_count++] = sub.MeshPrimitive->MaterialIndex;
    }
    void DrawImmediately(CEngineInternalPrimitiveStaticMesh& primitive, long mode, long pass)
    {
        C3DMesh2* mesh = primitive.Mesh;
        const unsigned char* hidden = 0;
        if (primitive.Type == 10)
            hidden = static_cast<CEngineInternalPrimitiveMorphedStaticMesh&>(primitive).Morph.GetHiddenMaterials(*mesh);
        for (unsigned long i = 0; i < mesh->PrimitiveCount; ++i) {
            C3DPrimitive2* meshPrimitive = &mesh->Primitives[i];
            if (meshPrimitive->TriangleCount == 0)
                continue;
            C3DMeshMaterial* material = &mesh->Materials[meshPrimitive->MaterialIndex];
            if (hidden && hidden[meshPrimitive->MaterialIndex])
                continue;
            if (material->DiffuseMapBankIndex == 0)
                continue;
            CEngineSubPrimitiveStaticMesh subPrimitive(primitive, meshPrimitive, mode);
            subPrimitive.RenderFlags = 2;
            RenderMeshPrimitive(subPrimitive, pass);
        }
    }
};

int main()
{
    C3DMeshMaterial materials[4] = { { 1 }, { 1 }, { 0 }, { 1 } };
    C3DPrimitive2 prims[5] = { { 0, 10 }, { 1, 10 }, { 2, 10 }, { 3, 0 }, { 3, 4 } };
    C3DMesh2 mesh = { materials, 5, prims };
    CEnginePrimitiveManagerStaticMeshesBase manager;

    CEngineInternalPrimitiveStaticMesh plain;              // not morphed: 0, 1, 3 (2 has no diffuse map, the first 3 no triangles)
    plain.Type = 1; plain.Mesh = &mesh;
    manager.DrawImmediately(plain, 7, 3);
    bool ok = g_count == 3 && g_rendered[0] == 0 && g_rendered[1] == 1 && g_rendered[2] == 3;

    CEngineInternalPrimitiveMorphedStaticMesh morphed;     // morphed, material 1 hidden: 0, 3
    morphed.Type = 10; morphed.Mesh = &mesh;
    morphed.Morph.HiddenMaterialNameCount = 1;
    morphed.Morph.HiddenStart = morphed.Morph.HiddenFinish = 0;
    morphed.Morph.Storage[0] = 0; morphed.Morph.Storage[1] = 1; morphed.Morph.Storage[2] = 0; morphed.Morph.Storage[3] = 0;
    g_count = 0;
    manager.DrawImmediately(morphed, 7, 3);
    ok = ok && g_initialised == 1 && g_count == 2 && g_rendered[0] == 0 && g_rendered[1] == 3;

    morphed.Morph.HiddenMaterialNameCount = 0;             // morphed, nothing hidden: no initialisation, 0, 1, 3
    g_count = 0;
    manager.DrawImmediately(morphed, 7, 3);
    ok = ok && g_initialised == 1 && g_count == 3;

    if (!ok || g_bad) {
        std::printf("00ba6dc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ba6dc0_TEST PASS\n");
    return 0;
}