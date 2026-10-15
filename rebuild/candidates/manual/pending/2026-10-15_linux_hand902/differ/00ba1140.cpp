// CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- cull, LOD-select and queue a static mesh's sub-primitives
// Grade: DIFFER(2372v2344) -- inline-asm FtoL x2 (`fstp ; fld ; fistp dword`) unreproducible (__ftol2 stand-in), LOD loop rotated by the lane, frame 0x294 v 0x298 + slot/callee-saved register assignment drift
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA1140, 2344 bytes; vtable 0x012A50DC slot 7 (CEnginePrimitiveManagerStaticMeshesBase, rtti-vtables.tsv).
// Earlier family scans cut this row as a 28-byte prefix (skipped ledger); this is the whole body. `ret 0Ch`, ERendered 0/1/2 in eax.
// Callees: operator new 0x00BFEA1A (cdecl) + CEnginePrimitiveOcclusionInfo ctor 0x00B2FA90; the sub-primitive pool operator new
//   0x00BEB540 (__fastcall, size in ecx); CEngineSubPrimitiveBoundingBox ctor 0x00B98B10 (first site out of line, second site the same
//   ctor expanded inline: CBase ctor 0x0099A2F0, Parent/Next stores, vtable 0x012A3E84); CEngineSubPrimitiveStaticMesh ctor 0x00B9CB10;
//   CEnginePrimitiveOcclusionManager::GetOccludedFrames 0x00B2FAC0; CEngineSubPrimitiveRenderer::AddSubPrimitive 0x00B84720 (pass, sub,
//   distance, manager) and 0x00B84580 (sub, distance, alpha, flags, manager, chain); C3DMeshInfo::GetMesh 0x00A25E70, RequestLoad 0x00A23C50
//   (empty in retail); CCountedPointer<const C3DMesh2> destructor body 0x00A36EC0 (out of line on the two early returns, inline at the end);
//   CEnginePrimitiveRenderUtil::AddGeneratedObjects 0x00BF12A0 (on `this`); CEngineInternalPrimitiveStaticMesh::InitialiseSimulation
//   0x00B9DE90, IsSimulationRunning 0x00BEBBB0; CEngineLightingManager::GetLightsInSphereFast 0x00B46B90; hidden-material
//   UpdateVisibilityVector 0x00BEC8D0; CShadowRendererSceneData copy ctor 0x00B38600; operator delete 0x00BFE9BC.
// Globals: 0x01436E74 sub-primitive renderer, 0x01436E78 engine settings (+78 view, +7C mesh-effect fade scale, +9C LOD scale,
//   +A8 LOD distance factor, +BD occlusion culling), 0x01436E80 view record (+4 high detail, +2F0 -> +A4 simulation enabled),
//   0x01436E58 occlusion manager, 0x01436E60 shadow renderer (+8 enabled, +C scene data), 0x01436E9C lighting manager (+4694 shadow mode),
//   0x01436EA0 camera (+54 position). Float pool: 0.0001f 0x0129BA3C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 256.0f 0x0125B240, 255.0f 0x01230014.
// Flow: radius = mesh-info sphere radius * scale; centre = object transform * sphere centre; distance = |centre - camera|. With occlusion
//   culling on, a per-view occlusion record is created on demand and a primitive occluded for more than the manager's frame budget (or
//   without a view/record) is queued as a bounding-box test (pass 7) and reported rendered. LOD = first level whose error exceeds
//   max(distance - radius, 1e-4) / (radius * factor) * scale. A mesh that is not resident queues a bounding box and requests the load.
//   Otherwise the RenderInfo (mesh, LOD, lights) is filled, the hidden-material filter of a type-10 (from-file) primitive is fetched, the
//   shadow-scene sphere/plane test decides `shadowed`, a type-2 mesh effect fades the whole primitive (alpha scale 0..256, 0 = skip), and
//   every primitive of the mesh whose material is visible and whose faded alpha survives is queued with its render flags / render mode
//   (+ the mesh-effect passes 8/9/13 or 10/14).
// Shape notes (probed): Length() takes the vector BY VALUE and sums X*X + Z*Z + Y*Y (in-place `fld st(2) ; fmulp st(3)`); the transform
//   copies the three components to locals first (`fld x ; fld y ; fld z` then `fld st(n)` per term); `if (!(d > 1e-4f)) d = 1e-4f;` with
//   the LOD scale read into a local first gives the retail `fcom ; test ah,41h ; je` + `fdivr st(1) ; fmul st(2)`; the mesh-effect fade is
//   an if / else-if chain with `t < in || t > out` first (zero block laid out last); `(unsigned char)(flags >> 9) & 1` gives the
//   `shr ; and bl,1 ; neg bl ; sbb` select; the hidden-material data pointer is the CArray `GetData()` (`count ? first : this`).
// Remaining differences (lane gaps first): (1) the engine's inline-asm float->long rounding (`fstp [f] ; fld [f] ; fistp dword [r]`,
//   twice: alpha scale and fade) has no asm-free form (/QIfist emits `fistp qword`); modelled with volatile locals + `__ftol2`. (2) the
//   LOD loop keeps its top test in retail (`cmp edi,3 ; jae` + `jmp` back); the lane rotates it. (3) frame 0x294 v 0x298 and the slot
//   order (retail `this` +0Ch, distance +14h, shadowed byte +1Bh, alpha scale +38h; lane +10h/+18h/+17h/+1Ch), callee-saved colouring
//   (retail ebp = 0x4000 / zero / primitive / cloth, ebx = new record / occlusion manager / visibility / flags, `this` never in a register,
//   settings in edx; lane swaps ebx/ebp, parks `this` in ebp at entry, settings in ecx, keeps the mesh pointer in a register in the loop).
//   (4) plane dot-product term order (retail x,z,y; lane z,y,x) and `movzx edi,al ; and ; or ; shr` v `and eax ; or ; shr ; mov edi,eax`.
// Model: engine/CEnginePrimitiveManagerStaticMeshesBase.h has +0x08 as an unnamed pad (the manager's `Enabled` byte) and no method
//   surface, and the primitive / mesh / sub-primitive classes differ from the PDB (retail C3DPrimitive2 0xA0, RenderInfo at +0xA0,
//   12-byte CArray), so the classes are modelled locally with descriptive names.
#include <math.h>

typedef int ERendered;
typedef int EEngineRenderLayer;
typedef long ERenderMode;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline float Length(C3DVector v) { return (float)sqrt(v.X * v.X + v.Z * v.Z + v.Y * v.Y); }

struct CMatrix3x4 {
    C3DVector Row[3];
    C3DVector Position;
};
__forceinline C3DVector operator*(const CMatrix3x4& m, const C3DVector& v)
{
    float x = v.X, y = v.Y, z = v.Z;
    return C3DVector(x * m.Row[0].X + y * m.Row[1].X + z * m.Row[2].X + m.Position.X,
                     x * m.Row[0].Y + y * m.Row[1].Y + z * m.Row[2].Y + m.Position.Y,
                     x * m.Row[0].Z + y * m.Row[1].Z + z * m.Row[2].Z + m.Position.Z);
}

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere(const C3DVector& c, float r) : Center(c), Radius(r) { }
};

struct CEngineFadeDistance {
    float FadeBeginDistance;
    float MaxDrawDistance;
    CEngineFadeDistance() { }
    CEngineFadeDistance(const CEngineFadeDistance& o) : FadeBeginDistance(o.FadeBeginDistance), MaxDrawDistance(o.MaxDrawDistance) { }
};

struct CEnginePrimitiveThemeChannelDesc {
    long Channel;
    CEnginePrimitiveThemeChannelDesc(const CEnginePrimitiveThemeChannelDesc& o) : Channel(o.Channel) { }
};

// the engine's inline-asm float->long rounding helpers (`fld f ; fistp r`), written without asm
__forceinline long FtoL(float f)
{
    volatile float vf = f;
    volatile long r = (long)vf;
    return r;
}
__forceinline void FtoL(long* out, float f)
{
    long* volatile po = out;
    volatile float vf = f;
    *po = (long)vf;
}

// ---- mesh data ---------------------------------------------------------------------------------------------
struct C3DMeshMaterial {                           // 0x30-byte material record
    unsigned char _pad00[0x20];
    unsigned long Flags;                           // +0x20 (bit 0 = alpha-blended; bits 1..2 = blend kind)
    unsigned char _pad24[5];
    unsigned char Translucent;                     // +0x29
    unsigned char AlphaTest;                       // +0x2a
    unsigned char _pad2b[5];
};

struct C3DPrimitive2 {                             // retail 0xa0 bytes
    unsigned char _pad00[4];
    long          MaterialIndex;                   // +0x04
    unsigned char _pad08[0x18];
    void**        ClothFirst;                      // +0x20 CArray<CCountedPointer<C3DClothPrimitive> >
    void**        ClothLast;                       // +0x24
    unsigned char _pad28[0x78];
};

struct C3DMesh2 {
    unsigned char    _pad00[0x88];
    unsigned long    MaterialCount;                // +0x88
    C3DMeshMaterial* Materials;                    // +0x8c
    unsigned long    PrimitiveCount;               // +0x90
    C3DPrimitive2*   Primitives;                   // +0x94
    unsigned char    _pad98[0x4c];
    unsigned char    HasSimulation;                // +0xe4
};

struct CCountedMeshBlock {
    long  Count;                                   // +0
    void (__fastcall *Destroy)(void* object);      // +4 (called with ecx = +8)
    void* Object;                                  // +8
};

void __cdecl operator delete(void* p);             // 0x00BFE9BC

struct CCountedPointerMesh {                       // CCountedPointer<const C3DMesh2>
    C3DMesh2*          Object;
    CCountedMeshBlock* Block;
    CCountedPointerMesh() : Object(0), Block(0) { }
    void Release();                                // 0x00A36EC0 (the destructor's out-of-line body: release + clear)
    __forceinline void ReleaseInline()             // the same body expanded inline (its clears are dead)
    {
        if (Block != 0) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                operator delete(Block);
            }
        }
    }
};

struct C3DMeshInfo {
    unsigned char     _pad00[0x28];
    C3DVector         Center;                      // +0x28 BoundingSphere
    float             Radius;                      // +0x34
    float             SafeBoundingRadius;          // +0x38
    long              PhysicsMeshIndex;            // +0x3c
    float             LODErrors[4];                // +0x40
    unsigned long     LODDataSizes[4];             // +0x50
    bool GetMesh(CCountedPointerMesh* mesh, unsigned long lod, unsigned long flags);   // 0x00A25E70
    void RequestLoad(long priority);                                                    // 0x00A23C50
};

// ---- primitives ---------------------------------------------------------------------------------------------
struct CEnginePrimitiveOcclusionInfo {
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long Count;                                    // +0x04
    unsigned char _pad08[0x18];
    CEnginePrimitiveOcclusionInfo();               // 0x00B2FA90
};

struct CIVCountedPointerOcclusion {
    CEnginePrimitiveOcclusionInfo* Object;
    __forceinline void operator=(CEnginePrimitiveOcclusionInfo* p)
    {
        CEnginePrimitiveOcclusionInfo* old = Object;
        if (old != 0) {
            if (--old->Count == 0)
                old->DeleteThis();
            Object = 0;
        }
        Object = p;
    }
};

struct CEngineMeshEffect {
    unsigned char _pad00[8];
    long  MeshEffect;                              // +0x08 EEngineMeshEffect
    float FadeInStart;                             // +0x0c
    float FadeInEnd;                               // +0x10
    float FadeOutStart;                            // +0x14
    float FadeOutEnd;                              // +0x18
    __forceinline float GetFade(float t) const
    {
        float f;
        if (t < FadeInStart || t > FadeOutEnd)
            f = 0.0f;
        else if (t < FadeInEnd)
            f = (t - FadeInStart) / (FadeInEnd - FadeInStart);
        else if (t > FadeOutStart)
            f = (t - FadeOutEnd) / (FadeOutStart - FadeOutEnd);
        else
            f = 1.0f;
        return f;
    }
};

struct CEngineLocalLightArray;

struct CArrayVisibility {                         // CArray<unsigned char> (retail 12 bytes)
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    __forceinline bool IsEmpty() const { return m_pFirst == m_pLast; }
    __forceinline const unsigned char* GetData() const { return m_pLast - m_pFirst != 0 ? m_pFirst : (const unsigned char*)this; }
};

struct CMaterialVisibility {                       // +0xac in the type-10 static mesh
    unsigned char    _pad00[0x24];
    unsigned long    HiddenCount;                  // +0x24
    unsigned char    _pad28[4];
    CArrayVisibility Visibility;                   // +0x2c
    void UpdateVisibilityVector(C3DMesh2* mesh);   // 0x00BEC8D0
    __forceinline const unsigned char* GetVisibility(C3DMesh2* mesh)
    {
        if (HiddenCount == 0)
            return 0;
        if (Visibility.IsEmpty())
            UpdateVisibilityVector(mesh);
        return Visibility.GetData();
    }
};

struct CEngineInternalPrimitiveStaticMesh {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CEngineFadeDistance GetDynamicFadeDistance();       // slot 13 (+0x34)
    unsigned char       _pad04[4];
    long                PrimitiveType;             // +0x08
    unsigned long       EnginePrimitiveFlags;      // +0x0c
    float               AlphaMipBias;              // +0x10
    CEngineFadeDistance FadeDistance;              // +0x14
    unsigned char       _pad1c[0x0b];
    unsigned char       Alpha;                     // +0x27
    unsigned char       _pad28[0x0c];
    unsigned char       PostRenderEnable;          // +0x34
    unsigned char       _pad35[0x13];
    CIVCountedPointerOcclusion OcclusionInfo[2];   // +0x48 one per view
    C3DMeshInfo*        MeshInfo;                  // +0x50
    C3DMeshInfo*        ShadowMeshInfo;            // +0x54
    float               Scale;                     // +0x58
    unsigned char       _pad5c[0x0c];
    CEngineMeshEffect*  MeshEffect;                // +0x68
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc; // +0x6c
    CMatrix3x4          ObjectTransform;           // +0x70
    CEngineLocalLightArray* Lights;                // +0xa0 RenderInfo
    C3DMesh2*           Mesh;                      // +0xa4
    unsigned long       LOD;                       // +0xa8
    CMaterialVisibility MaterialVisibility;        // +0xac (type 10)

    void InitialiseSimulation(C3DMeshInfo* info, C3DMesh2* mesh);   // 0x00B9DE90
    bool IsSimulationRunning(unsigned long primitive);              // 0x00BEBBB0

    __forceinline CEnginePrimitiveThemeChannelDesc GetThemeChannelDesc() const { return ThemeChannelDesc; }
    __forceinline CEngineFadeDistance GetFadeDistance()
    {
        return (PostRenderEnable & 4) ? GetDynamicFadeDistance() : FadeDistance;
    }
    __forceinline unsigned char GetFadedAlpha(float distance)
    {
        if (Alpha == 0)
            return 0;
        CEngineFadeDistance fade = GetFadeDistance();
        if (distance > fade.MaxDrawDistance)
            return 0;
        if (distance < fade.FadeBeginDistance)
            return Alpha;
        long faded;
        FtoL(&faded, (distance - fade.FadeBeginDistance) * 255.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance));
        return (unsigned char)(((255 - faded) * Alpha) >> 8);
    }
};

// ---- render queue -------------------------------------------------------------------------------------------
struct CEngineSubPrimitiveChain;
struct CEnginePrimitiveManager;

struct CBase {
    CBase();                                       // 0x0099A2F0
    virtual void v00();
};

struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {
    unsigned long RenderFlags;                     // +0x04
    unsigned char Alpha;                           // +0x08
    CEngineInternalPrimitiveStaticMesh* Parent;    // +0x0c
    CEngineSubPrimitiveBase* NextSubPrimitive;     // +0x10
    CEnginePrimitiveManager* Manager;              // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveStaticMesh* parent) : Parent(parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540 (engine sub-primitive pool)
};

struct CEngineSubPrimitiveBoundingBox : CEngineSubPrimitiveBase {   // vtable 0x012A3E84
    CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveStaticMesh* parent);   // 0x00B98B10 (out of line)
    virtual void v00();
};

// retail expands the same constructor inline at the second construction site (VC7.1 inline budget)
struct CEngineSubPrimitiveBoundingBoxInline : CEngineSubPrimitiveBase {   // vtable 0x012A3E84
    __forceinline CEngineSubPrimitiveBoundingBoxInline(CEngineInternalPrimitiveStaticMesh* parent) : CEngineSubPrimitiveBase(parent) { }
    virtual void v00();
};

struct CEngineSubPrimitiveStaticMesh : CEngineSubPrimitiveBase {    // 0x24 bytes
    unsigned char _pad18[0x0c];
    CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitiveStaticMesh* parent, C3DPrimitive2* primitive, ERenderMode mode, bool cloth);   // 0x00B9CB10
};

struct CEngineSubPrimitiveRenderer {
    void AddSubPrimitive(long pass, CEngineSubPrimitiveBase* sub, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
    void AddSubPrimitive(CEngineSubPrimitiveBase* sub, float distance, unsigned char alpha, unsigned long flags,
                         CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain* chain);                           // 0x00B84580
};

struct CEnginePrimitiveOcclusionManager {
    unsigned char _pad00[0x0c];
    unsigned long MaxOccludedFrames;               // +0x0c
    unsigned long GetOccludedFrames(CEnginePrimitiveOcclusionInfo* info);   // 0x00B2FAC0
};

struct CEngineLightingManager {
    unsigned char _pad00[0x4694];
    long          ShadowMode;                      // +0x4694
    CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere& sphere, long maxLights, const CEnginePrimitiveThemeChannelDesc& theme);   // 0x00B46B90
};

struct C3DPlane { C3DVector Normal; float D; };

struct CShadowRendererSceneData {                  // 0x23c bytes
    unsigned char _pad000[0x1a4];
    C3DPlane      Planes[8];                       // +0x1a4
    long          PlaneCount;                      // +0x224
    C3DVector     SphereCentre;                    // +0x228
    float         SphereRadius;                    // +0x234
    bool          HasSphere;                       // +0x238
    CShadowRendererSceneData(const CShadowRendererSceneData& other);   // 0x00B38600
    __forceinline bool Contains(const C3DVector& centre, float radius) const
    {
        if (HasSphere) {
            float dx = centre.X - SphereCentre.X;
            float dy = centre.Y - SphereCentre.Y;
            float dz = centre.Z - SphereCentre.Z;
            float r = SphereRadius + radius;
            if (dx * dx + dy * dy + dz * dz > r * r)
                return false;
        }
        for (long i = 0; i < PlaneCount; ++i) {
            if (Planes[i].Normal.X * centre.X + Planes[i].Normal.Y * centre.Y + Planes[i].Normal.Z * centre.Z - radius > Planes[i].D)
                return false;
        }
        return true;
    }
};

struct CEngineShadowRenderer {
    unsigned char _pad00[8];
    bool          Enabled;                         // +0x08
    unsigned char _pad09[3];
    CShadowRendererSceneData SceneData;            // +0x0c
};

struct CEngineSettings {                           // 0x01436E78
    unsigned char _pad00[0x78];
    long          ViewIndex;                       // +0x78
    float         MeshEffectFadeScale;             // +0x7c
    unsigned char _pad80[0x1c];
    float         LODScale;                        // +0x9c
    unsigned char _padA0[8];
    float         LODDistanceFactor;               // +0xa8
    unsigned char _padAC[0x11];
    bool          OcclusionCulling;                // +0xbd
};

struct CEngineCameraState { unsigned char _pad00[0x54]; C3DVector Position; };   // +0x54

struct CEngineSimulationState { unsigned char _pad00[0xa4]; bool SimulationEnabled; };

struct CViewRecord {                               // 0x01436E80
    unsigned char _pad00[4];
    bool          HighDetail;                      // +0x04
    unsigned char _pad05[0x2eb];
    CEngineSimulationState* Simulation;            // +0x2f0
};

extern CEngineSubPrimitiveRenderer*      g_pSubPrimitiveRenderer;   // 0x01436E74
extern CEngineSettings*                  g_pEngineSettings;         // 0x01436E78
extern CViewRecord*                      g_pViewRecord;             // 0x01436E80
extern CEnginePrimitiveOcclusionManager* g_pOcclusionManager;       // 0x01436E58
extern CEngineShadowRenderer*            g_pShadowRenderer;         // 0x01436E60
extern CEngineLightingManager*           g_pLightingManager;        // 0x01436E9C
extern CEngineCameraState*               g_pEngineCamera;           // 0x01436EA0

struct CEnginePrimitiveManagerStaticMeshesBase {
    virtual void v00();
    unsigned char _pad04[4];
    bool          Enabled;                         // +0x08
    void AddGeneratedObjects(CEngineInternalPrimitiveStaticMesh* prim);   // 0x00BF12A0 (CEnginePrimitiveRenderUtil)
    ERendered RenderPrimitive(CEngineInternalPrimitiveStaticMesh& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive(CEngineInternalPrimitiveStaticMesh& primRef, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain)
{
    if (!Enabled)
        return 0;

    CEngineInternalPrimitiveStaticMesh* prim = &primRef;
    float radius = prim->MeshInfo->Radius * prim->Scale;
    long view = g_pEngineSettings->ViewIndex;
    C3DVector centre = prim->ObjectTransform * prim->MeshInfo->Center;
    float distance = Length(centre - g_pEngineCamera->Position);

    if (g_pEngineSettings->OcclusionCulling) {
        if (!(prim->PostRenderEnable & 2) && view != -1 && !(prim->EnginePrimitiveFlags & 0x4000)
            && prim->OcclusionInfo[view].Object == 0)
            prim->OcclusionInfo[view] = new CEnginePrimitiveOcclusionInfo;
        if (!(prim->EnginePrimitiveFlags & 0x4000)) {
            CEnginePrimitiveOcclusionInfo* occlusion;
            CEnginePrimitiveOcclusionManager* manager;
            if (view == -1 || (occlusion = prim->OcclusionInfo[view].Object) == 0
                || (manager = g_pOcclusionManager, manager->GetOccludedFrames(occlusion) > manager->MaxOccludedFrames)) {
                g_pSubPrimitiveRenderer->AddSubPrimitive(7, new CEngineSubPrimitiveBoundingBox(prim), distance, (CEnginePrimitiveManager*)this);
                return 1;
            }
        }
    }

    float lodScale = g_pEngineSettings->LODScale;
    float lodDistance = distance - radius;
    if (!(lodDistance > 0.0001f))
        lodDistance = 0.0001f;
    float lodMetric = lodDistance / (radius * g_pEngineSettings->LODDistanceFactor) * lodScale;
    unsigned long lod = 0;
    while (lod < 3) {
        if (prim->MeshInfo->LODDataSizes[lod + 1] == 0)
            break;
        if (prim->MeshInfo->LODErrors[lod + 1] > lodMetric)
            break;
        ++lod;
    }

    CCountedPointerMesh mesh;
    if (!prim->MeshInfo->GetMesh(&mesh, lod, g_pViewRecord->HighDetail ? 0x11 : 0)) {
        g_pSubPrimitiveRenderer->AddSubPrimitive(7, new CEngineSubPrimitiveBoundingBoxInline(prim), distance, (CEnginePrimitiveManager*)this);
        prim->MeshInfo->RequestLoad(1);
        mesh.Release();
        return 1;
    }
    prim->Mesh = mesh.Object;
    prim->LOD = lod;
    AddGeneratedObjects(prim);
    if (mesh.Object->HasSimulation)
        prim->InitialiseSimulation(prim->MeshInfo, mesh.Object);

    prim->Lights = g_pLightingManager->GetLightsInSphereFast(C3DBoundingSphere(centre, radius), 5, prim->GetThemeChannelDesc());

    const unsigned char* visibility = 0;
    switch (prim->PrimitiveType) {
    case 10:
        visibility = prim->MaterialVisibility.GetVisibility(mesh.Object);
        break;
    }

    bool shadowed;
    if (!(prim->EnginePrimitiveFlags & 0x1000)) {
        CShadowRendererSceneData scene(g_pShadowRenderer->SceneData);
        shadowed = scene.Contains(centre, radius);
    } else {
        shadowed = false;
    }

    volatile long alphaScale = 256;
    CEngineMeshEffect* effect = prim->MeshEffect;
    if (effect != 0 && effect->MeshEffect == 2) {
        long scale = FtoL(effect->GetFade(distance / g_pEngineSettings->MeshEffectFadeScale) * 256.0f);
        alphaScale = scale;
        if (scale == 0) {
            mesh.Release();
            return 0;
        }
    }

    for (unsigned long i = 0; i < mesh.Object->PrimitiveCount; ++i) {
        C3DPrimitive2* primitive = &mesh.Object->Primitives[i];
        long materialIndex = primitive->MaterialIndex;
        if (visibility != 0 && visibility[materialIndex] != 0)
            continue;
        C3DMeshMaterial* material = &mesh.Object->Materials[materialIndex];
        long alpha = (prim->GetFadedAlpha(distance) * alphaScale) >> 8;
        if (alpha == 0)
            continue;

        unsigned long primFlags = prim->EnginePrimitiveFlags;
        unsigned char translucent = material->Translucent;
        unsigned char alphaTest = material->AlphaTest;
        if ((primFlags & 0x100) && translucent)
            alphaTest = 1;
        unsigned long renderFlags = 0;
        if (alpha < 255)
            renderFlags = ((unsigned char)(primFlags >> 9) & 1) ? 10 : 5;
        if (alphaTest)
            renderFlags |= 2;
        else if (material->Translucent)
            renderFlags |= 5;
        if (shadowed && g_pShadowRenderer->Enabled) {
            renderFlags |= 0x200;
            if (g_pLightingManager->ShadowMode == 2)
                renderFlags |= 0x10;
        }
        if (primFlags & 0x800)
            renderFlags |= 0xc0;
        if (primFlags & 8)
            renderFlags |= 0x180;

        ERenderMode mode;
        unsigned long materialFlags = material->Flags;
        if (materialFlags & 1) {
            materialFlags &= ~1;
            if (renderFlags & 0x200) {
                if (renderFlags & 0x10) {
                    mode = (materialFlags & 4) ? 13 : 12;
                } else {
                    switch (materialFlags) {
                    case 2: case 6: mode = 11; break;
                    case 4: mode = 10; break;
                    default: mode = 9; break;
                    }
                }
            } else {
                switch (materialFlags) {
                case 2: case 6: mode = 8; break;
                case 4: mode = 7; break;
                default: mode = 4; break;
                }
            }
        } else {
            mode = 1;
        }

        bool cloth = primitive->ClothFirst != primitive->ClothLast
                  && g_pViewRecord->Simulation->SimulationEnabled
                  && prim->IsSimulationRunning(i);

        if (prim->MeshEffect != 0) {
            switch (prim->MeshEffect->MeshEffect) {
            case 0:
                g_pSubPrimitiveRenderer->AddSubPrimitive(8, new CEngineSubPrimitiveStaticMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                if (material->Translucent)
                    g_pSubPrimitiveRenderer->AddSubPrimitive(9, new CEngineSubPrimitiveStaticMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                g_pSubPrimitiveRenderer->AddSubPrimitive(13, new CEngineSubPrimitiveStaticMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                renderFlags |= 0xa0;
                break;
            case 1:
                g_pSubPrimitiveRenderer->AddSubPrimitive(10, new CEngineSubPrimitiveStaticMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                g_pSubPrimitiveRenderer->AddSubPrimitive(14, new CEngineSubPrimitiveStaticMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                renderFlags |= 0xa0;
                break;
            }
        }
        g_pSubPrimitiveRenderer->AddSubPrimitive(new CEngineSubPrimitiveStaticMesh(prim, primitive, mode, cloth), distance,
                                                 (unsigned char)alpha, renderFlags, (CEnginePrimitiveManager*)this, &chain);
    }
    mesh.ReleaseInline();
    return 2;
}
