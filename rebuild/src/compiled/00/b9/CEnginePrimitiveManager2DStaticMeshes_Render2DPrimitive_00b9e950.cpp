// CEnginePrimitiveManager2DStaticMeshes::Render2DPrimitive(CEngineInternalPrimitiveBase&, CEngineSubPrimitiveChain&, ESubPrimitiveType) -- queue a 2D static mesh: skip it when off the viewport or outside the active 2D clip box or on a disabled layer; initialise it once, choose its LOD, start (or require) the mesh load, then fetch the mesh and queue one pool-allocated CEngineSubPrimitiveStaticMesh per sub-mesh at the layer's depth (alpha-tested materials, simulated sub-meshes and the mesh's flag bits pick the render flags and sort list)
// Leaf: Render2DPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E950, length 901 bytes (`ret 0Ch`, virtual).
#include <stddef.h>
class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveChain;
class C3DMesh2;
struct CRefInfo { long RefCount; };
struct CCountedMeshPointer {                       // CCountedPointer<const C3DMesh2>
    C3DMesh2* Object;
    CRefInfo* Info;
    CCountedMeshPointer() : Object(0), Info(0) { }
    ~CCountedMeshPointer();                        // 0x00A36EC0
};
class C3DMeshInfo { public:
    void BeginLoading(unsigned long priority, bool urgent, bool wait);   // 0x00A256F0
    void SetLoadPriority(long priority);           // 0x00A23C50
    bool IsLoaded();                               // 0x00A23E40
    bool IsBuilt();                                // 0x00A23C60
    bool GetMesh(CCountedMeshPointer& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
};
struct CMeshMaterial {                             // 0x30 bytes
    unsigned char _pad00[0x20];
    unsigned char Flags;                           // +0x20 (bit 0: sort list 0x10)
    unsigned char _pad21[8];
    bool          AlphaTest;                       // +0x29
    bool          AlphaBlend;                      // +0x2a
    unsigned char _pad2b[5];
};
struct CSubMesh {                                  // 0xa0 bytes
    unsigned char _pad00[4];
    long          MaterialIndex;                   // +0x04
    unsigned char _pad08[0x18];
    void*         SimulationStart;                 // +0x20
    void*         SimulationEnd;                   // +0x24
    unsigned char _pad28[0x78];
};
class C3DMesh2 { public:
    unsigned char  _pad00[0x8c];
    CMeshMaterial* Materials;                      // +0x8c
    unsigned long  SubMeshCount;                   // +0x90
    CSubMesh*      SubMeshes;                      // +0x94
    unsigned char  _pad98[0xe4 - 0x98];
    bool           HasSimulation;                  // +0xe4
};
class CEngineLocalLightArray;
class CEngineLightingManager { public:
    unsigned char _pad[0x44ec];
    unsigned char EmptyLightArray[0x1a4];          // +0x44ec
};
extern CEngineLightingManager* g_pLightingManager;   // 0x01436E9C

struct CEngineInternalPrimitive2DStaticMesh {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void SetLOD(float distance, bool force); // slot 33 (+0x84)
    unsigned char  _pad04[8];
    unsigned long  Flags;                          // +0x0c
    unsigned char  _pad10[0x17];
    unsigned char  Alpha;                          // +0x27
    unsigned char  _pad28[4];
    long           LODIndex;                       // +0x2c
    unsigned char  _pad30[0x20];
    C3DMeshInfo*   MeshInfo;                       // +0x50
    unsigned char  _pad54[0x4c];
    void*          LightArray;                     // +0xa0
    C3DMesh2*      CurrentMesh;                    // +0xa4
    long           CurrentLOD;                     // +0xa8
    bool           Initialised;                    // +0xac
    unsigned char  _padad[3];
    float          ScreenTLX, ScreenTLY, ScreenBRX, ScreenBRY;   // +0xb0
    long           Layer;                          // +0xc0
    void Initialise();                             // 0x00B9E120
    void InitialiseSimulation(C3DMeshInfo* info, C3DMesh2* mesh);   // 0x00B9DE90
    bool IsSimulationRunning(unsigned long subMesh);   // 0x00BEBBB0
};
struct SLODEntry { float Distance; float _pad[2]; };
extern SLODEntry g_LODTable_0143b934[];            // 0x0143B934

class CDisplayManager { public:
    unsigned char _pad[0x1f4];
    long          ViewportTLX, ViewportTLY, ViewportBRX, ViewportBRY;   // +0x1f4
};
extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

struct C2DClipInfo {
    long          Active;                          // +0x00
    long          Enabled;                         // +0x04
    unsigned char _pad08[0x10];
    float         TLX, TLY, BRX, BRY;              // +0x18
    bool          Valid;                           // +0x28
};
struct C2DClipOwner {
    unsigned char _pad0[0xc];
    C2DClipInfo   Clip;                            // +0x0c
};
class CEnginePrimitiveTypeInfo { public:
    unsigned char _pad[0x94];
    C2DClipOwner* ClipOwner;                       // +0x94
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

class CEnginePrimitiveRenderer2D { public:
    unsigned char _pad[0x320];
    bool          LayerEnabled[1];                 // +0x320
    float Layer2DToDistance(long layer);           // 0x00B4A6A0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D;   // 0x01436E7C
struct CSimulationSettings { unsigned char _pad[0xa4]; bool Enabled; };
class CEnginePrimitiveRenderer { public:
    unsigned char        _pad000[4];
    bool                 Loading;                  // +0x04
    unsigned char        _pad005[0x2f0 - 5];
    CSimulationSettings* Simulation;               // +0x2f0
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

struct CBase {
    virtual void v00();
    CBase();
};
struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {
    unsigned long                 RenderFlags;     // +0x04
    unsigned char                 Alpha;           // +0x08
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540
};
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveBase { public:   // 0x24 bytes
    unsigned char _pad0c[0x24 - 0xc];
    CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitive2DStaticMesh* primitive, CSubMesh* subMesh, long list, bool simulated);   // 0x00B9CB10
};
class CEnginePrimitiveManager;
class CEngineSubPrimitiveRenderer { public:
    unsigned char _pad[0x10];
    float         DepthBias;                       // +0x10
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

class CEnginePrimitiveManager2DStaticMeshes { public:
    void* SceneList;                               // +0x04
    bool  Enable;                                  // +0x08
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type);
    void AddGeneratedObjects(CEngineInternalPrimitive2DStaticMesh* mesh);   // 0x00BF12A0
};

void CEnginePrimitiveManager2DStaticMeshes::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type)
{
    if (!Enable)
        return;
    CEngineInternalPrimitive2DStaticMesh& mesh = (CEngineInternalPrimitive2DStaticMesh&)primitive;
    CDisplayManager* display = g_pDisplayManager_01436e1c;
    if (mesh.ScreenTLX > display->ViewportBRX || mesh.ScreenBRX < display->ViewportTLX ||
        mesh.ScreenTLY > display->ViewportBRY || mesh.ScreenBRY < display->ViewportTLY)
        return;
    C2DClipInfo& clip = g_pEnginePrimitiveTypeInfo->ClipOwner->Clip;
    if (clip.Enabled && clip.Active && clip.Valid) {
        if (mesh.ScreenTLX > clip.BRX || mesh.ScreenBRX < clip.TLX || mesh.ScreenTLY > clip.BRY || mesh.ScreenBRY < clip.TLY)
            return;
    }
    const long& layer = mesh.Layer;
    if (!g_pPrimitiveRenderer2D->LayerEnabled[layer])
        return;
    if (!mesh.Initialised)
        mesh.Initialise();
    mesh.SetLOD(g_LODTable_0143b934[mesh.LODIndex].Distance, true);
    bool urgent = (mesh.Flags >> 7) & 1;
    C3DMeshInfo* info = mesh.MeshInfo;
    if (!g_pPrimitiveRenderer_01436e80->Loading) {
        info->BeginLoading(0, false, false);
        info->SetLoadPriority(urgent ? 2 : 1);
    }
    if (!urgent && !g_pPrimitiveRenderer_01436e80->Loading) {
        C3DMeshInfo* current = mesh.MeshInfo;
        if (!current->IsLoaded())
            return;
        if (!current->IsBuilt())
            return;
    }
    CCountedMeshPointer meshPointer;
    if (mesh.MeshInfo->GetMesh(meshPointer, 0, 0x10)) {
        mesh.CurrentMesh = meshPointer.Object;
        mesh.CurrentLOD = 0;
        AddGeneratedObjects(&mesh);
        if (meshPointer.Object->HasSimulation)
            mesh.InitialiseSimulation(mesh.MeshInfo, meshPointer.Object);
        mesh.LightArray = g_pLightingManager->EmptyLightArray;
        for (unsigned long i = 0; i < meshPointer.Object->SubMeshCount; ++i) {
            CSubMesh* subMesh = &meshPointer.Object->SubMeshes[i];
            CMeshMaterial& material = meshPointer.Object->Materials[subMesh->MaterialIndex];
            bool alphaTest = material.AlphaTest;
            long alpha = mesh.Alpha;
            bool blend = material.AlphaBlend;
            if ((mesh.Flags & 0x100) && alphaTest)
                blend = true;
            unsigned long flags = 0;
            if (blend)
                flags = 2;
            else if (alphaTest)
                flags = 5;
            if (mesh.Flags & 0x800)
                flags |= 0xc0;
            if (mesh.Flags & 8)
                flags |= 0x180;
            long list = (material.Flags & 1) ? 0x10 : 0xf;
            bool simulated;
            if (subMesh->SimulationStart != subMesh->SimulationEnd && g_pPrimitiveRenderer_01436e80->Simulation->Enabled &&
                mesh.IsSimulationRunning(i))
                simulated = true;
            else
                simulated = false;
            CEngineSubPrimitiveStaticMesh* subPrimitive = new CEngineSubPrimitiveStaticMesh(&mesh, subMesh, list, simulated);
            subPrimitive->Alpha = alpha;
            subPrimitive->RenderFlags = flags;
            float depth = g_pPrimitiveRenderer2D->Layer2DToDistance(mesh.Layer);
            if (flags & 1)
                depth -= g_pEngineSubPrimitiveRenderer->DepthBias * 9.3132257e-10f;
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(type, subPrimitive, depth, (CEnginePrimitiveManager*)this);
        }
    }
}