// CEnginePrimitiveManager2DAnimatedMeshes::Render2DPrimitive(CEngineInternalPrimitiveBase&, CEngineSubPrimitiveChain&, ESubPrimitiveType) -- queue a 2D animated mesh: the 2D static-mesh culling and load sequence (0x00B9E950), then SetupMesh2D, the LOD transform stored into the render info, generated objects, simulation start, and one pool-allocated CEngineSubPrimitiveAnimatedMesh per sub-mesh whose material is not hidden by the primitive's visibility vector (type 0x26 only)
// Leaf: Render2DPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD1F70, length 1004 bytes (`ret 0Ch`, virtual).
#include <stddef.h>
struct CEngineInternalPrimitiveBase;
struct CEngineSubPrimitiveChain;
struct C3DMesh2;
struct CRefInfo { long RefCount; };
struct CCountedMeshPointer {                       // CCountedPointer<const C3DMesh2>
    C3DMesh2* Object;
    CRefInfo* Info;
    CCountedMeshPointer() : Object(0), Info(0) { }
    ~CCountedMeshPointer();                        // 0x00A36EC0
};
struct C3DMeshInfo {
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
struct C3DMesh2 {
    unsigned char  _pad00[0x8c];
    CMeshMaterial* Materials;                      // +0x8c
    unsigned long  SubMeshCount;                   // +0x90
    CSubMesh*      SubMeshes;                      // +0x94
    unsigned char  _pad98[0xe4 - 0x98];
    bool           HasSimulation;                  // +0xe4
};
struct CEngineLocalLightArray;
struct CEngineLightingManager {
    unsigned char _pad[0x44ec];
    unsigned char EmptyLightArray[0x1a4];          // +0x44ec
};
extern CEngineLightingManager* g_pLightingManager;   // 0x01436E9C

struct CMatrix3x4 { float m[12]; };
struct C3DVector { float X, Y, Z; };
struct CByteVector {                               // STLport vector<unsigned char>, 12 bytes
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned char* GetData() { return _M_finish - _M_start ? _M_start : (unsigned char*)this; }
};
struct CEngineMeshVisibility {
    unsigned char _pad00[0x24];
    void*         Def;                             // +0x24
    unsigned char _pad28[4];
    CByteVector   HiddenMaterials;                 // +0x2c
    void UpdateVisibilityVector(C3DMesh2* mesh);   // 0x00BEC8D0
};
struct CEngineInternalPrimitive2DAnimatedMesh {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual void GetRenderData(C3DVector* out);   // slot 15 (+0x3c)
    virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CMatrix3x4 GetLODTransform(float distance, bool force);   // slot 33 (+0x84)
    virtual void InitialiseSimulation(C3DMeshInfo* info, C3DMesh2* mesh);   // slot 34 (+0x88)
    unsigned char  _pad04[4];
    long           Type;                           // +0x08
    unsigned long  Flags;                          // +0x0c
    unsigned char  _pad10[0x17];
    unsigned char  Alpha;                          // +0x27
    unsigned char  _pad28[4];
    long           LODIndex;                       // +0x2c
    unsigned char  _pad30[0x20];
    C3DMeshInfo*   MeshInfo;                       // +0x50
    unsigned char  _pad54[0x8c];
    void*          LightArray;                     // +0xe0 RenderInfo.Lights
    C3DMesh2*      CurrentMesh;                    // +0xe4 RenderInfo.Mesh
    unsigned char  _pade8[4];
    CMatrix3x4     Transform;                      // +0xec
    unsigned char  _pad11c[0xc];
    bool           Initialised;                    // +0x128
    unsigned char  _pad129[3];
    long           Layer;                          // +0x12c
    float          ScreenTLX, ScreenTLY, ScreenBRX, ScreenBRY;   // +0x130
    unsigned char  _pad140[0x10];
    CEngineMeshVisibility Visibility;              // +0x150
    void Initialise();                             // 0x00BCF4F0
    bool SetupMesh2D();                            // 0x00BCF430
    bool IsSimulationRunning(unsigned long subMesh);   // 0x00BEBBB0
};
struct SLODEntry { float Distance; float _pad[2]; };
extern SLODEntry g_LODTable_0143b934[];            // 0x0143B934

struct CDisplayManager {
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
struct CEnginePrimitiveTypeInfo {
    unsigned char _pad[0x94];
    C2DClipOwner* ClipOwner;                       // +0x94
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

struct CEnginePrimitiveRenderer2D {
    unsigned char _pad[0x320];
    bool          LayerEnabled[1];                 // +0x320
    float Layer2DToDistance(long layer);           // 0x00B4A6A0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D;   // 0x01436E7C
struct CSimulationSettings { unsigned char _pad[0xa4]; bool Enabled; };
struct CEnginePrimitiveRenderer {
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
struct CEngineSubPrimitiveAnimatedMesh : CEngineSubPrimitiveBase {   // 0x24 bytes
    unsigned char _pad0c[0x24 - 0xc];
    CEngineSubPrimitiveAnimatedMesh(CEngineInternalPrimitive2DAnimatedMesh* primitive, CSubMesh* subMesh, long list, bool simulated);   // 0x00BCE740
};
struct CEnginePrimitiveManager;
struct CEngineSubPrimitiveRenderer {
    unsigned char _pad[0x10];
    float         DepthBias;                       // +0x10
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

struct CEnginePrimitiveManager2DAnimatedMeshes {
    void* SceneList;                               // +0x04
    bool  Enable;                                  // +0x08
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type);
    void AddGeneratedObjects(CEngineInternalPrimitive2DAnimatedMesh* mesh, CMatrix3x4* transform);   // 0x00BF18B0
};

void CEnginePrimitiveManager2DAnimatedMeshes::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type)
{
    if (!Enable)
        return;
    CEngineInternalPrimitive2DAnimatedMesh& mesh = (CEngineInternalPrimitive2DAnimatedMesh&)primitive;
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
    CMatrix3x4 transform = mesh.GetLODTransform(g_LODTable_0143b934[mesh.LODIndex].Distance, true);
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
    if (!mesh.SetupMesh2D())
        return;
    C3DMesh2* data = mesh.CurrentMesh;
    mesh.Transform = transform;
    AddGeneratedObjects(&mesh, &transform);
    if (data->HasSimulation)
        mesh.InitialiseSimulation(mesh.MeshInfo, data);
    mesh.LightArray = g_pLightingManager->EmptyLightArray;
    unsigned char* hidden = 0;
    const long& primitiveType = mesh.Type;
    if (primitiveType == 0x26) {
        CEngineMeshVisibility& visibility = mesh.Visibility;
        if (!visibility.Def)
            hidden = 0;
        else {
            if (visibility.HiddenMaterials.empty())
                visibility.UpdateVisibilityVector(data);
            hidden = visibility.HiddenMaterials.GetData();
        }
    }
    C3DVector renderData;
    mesh.GetRenderData(&renderData);
    unsigned long count = data->SubMeshCount;
    for (unsigned long i = 0; i < count; ++i) {
        CSubMesh* subMesh = &data->SubMeshes[i];
        long materialIndex = subMesh->MaterialIndex;
        if (hidden && hidden[materialIndex])
            continue;
        CMeshMaterial& material = data->Materials[materialIndex];
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
        CEngineSubPrimitiveAnimatedMesh* subPrimitive = new CEngineSubPrimitiveAnimatedMesh(&mesh, subMesh, list, simulated);
        subPrimitive->Alpha = alpha;
        subPrimitive->RenderFlags = flags;
        float depth = g_pPrimitiveRenderer2D->Layer2DToDistance(mesh.Layer);
        if (flags & 1)
            depth -= g_pEngineSubPrimitiveRenderer->DepthBias * 9.3132257e-10f;
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(type, subPrimitive, depth, (CEnginePrimitiveManager*)this);
    }
}