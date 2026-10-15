// CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) -- cull, set up and queue an animated mesh's sub-primitives
// Grade: DIFFER(2121v2116) -- inline-asm FtoL (`fstp ; fld ; fistp dword [edx]`) unreproducible (__ftol2 stand-in), radius `fld [eax+34h]` hoisted first in retail, frame 0x290 v 0x294 + slot/callee-saved register drift
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD71B0, 2116 bytes; vtable 0x012A78DC slot 7 (CEnginePrimitiveManagerAnimatedMeshesBase, rtti-vtables.tsv).
// Earlier family scans cut this row as a 28-byte prefix (skipped ledger); this is the whole body. `ret 0Ch`, ERendered 0/1/2 in eax.
// The twin of CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive (0x00BA1140, parked beside it) with the animated differences:
//   the object transform is refreshed first through virtual slot 33 CalcInterpolatedObjectTransform(transform, time, true) 0x00BCEF80
//   (time = the 12-byte interpolation-set table at 0x0143B934 indexed by the primitive's +0x2C set); the cull radius is
//   (|sphere centre| + sphere radius) * scale + view-record padding (+0x23C); the distance is also stored in RenderInfo.Distance (+0x11C);
//   LOD/mesh selection is CEngineInternalPrimitiveAnimatedMesh::SetupMesh(distance, radius) 0x00BCF2E0 (no counted pointer); the mesh is
//   RenderInfo.Mesh (+0xE4); AddGeneratedObjects 0x00BF18B0 also takes the transform; InitialiseSimulation is virtual slot 34 0x00BCFCB0;
//   the hidden-material filter exists on primitive types 11 (+0x128) and 38 (+0x150); there is no mesh-effect alpha scale; the primitive
//   count is cached before the loop; sub-primitives are CEngineSubPrimitiveAnimatedMesh (ctor 0x00BCE740); after the loop the bone
//   hierarchy is drawn when settings +0x25B is set (RenderDebugBoneHierarchy 0x00BD6810 on `this`).
// Other callees/globals as in 0x00BA1140: operator new 0x00BFEA1A, occlusion record ctor 0x00B2FA90, sub-primitive pool new 0x00BEB540,
//   bounding-box ctor 0x00B98B10 (first site; second site inlined: CBase 0x0099A2F0 + vtable 0x012A3E84), GetOccludedFrames 0x00B2FAC0,
//   AddSubPrimitive 0x00B84720 / 0x00B84580, RequestLoad 0x00A23C50, IsSimulationRunning 0x00BEBBB0, GetLightsInSphereFast 0x00B46B90,
//   UpdateVisibilityVector 0x00BEC8D0, CShadowRendererSceneData copy ctor 0x00B38600; globals 0x01436E58/60/74/78/80/9C/A0.
// Shape notes: the hidden-material type test is an if / else-if chain (11 first, then 38: `mov eax,[edi+8] ; cmp eax,0Bh ; jne ; ...
//   cmp eax,26h`), each arm a static_cast to the derived primitive; the rest as documented in 0x00BA1140.
// Remaining differences: (1) the inline-asm float->long rounding of the fade alpha (`fstp [f] ; fld [f] ; mov edx,[po] ;
//   fistp dword [edx]`) has no asm-free form (volatile locals + `__ftol2` stand-in). (2) retail loads the sphere radius first
//   (`fld [eax+34h]` before the centre, `fadd st(1)` then `fstp st(0)`); no source form tried (by value / by reference / local / member
//   helper / copied sphere) reproduces the hoist, and the transform terms then start with y*m10 instead of z*m20. (3) frame 0x290 v
//   0x294 and slot order (retail `this` +0Ch, radius +10h, shadowed +1Bh); callee-saved colouring (retail ebx = view / visibility,
//   ebp = new record / mesh pointer kept across the loop, esi = transform then material/mode; lane swaps ebx/ebp and spills the mesh
//   pointer). (4) plane dot-product term order and `movzx esi,al ; and ; or ; shr` v `and eax ; or ; shr ; mov esi,eax`.
// Model: no engine header for the manager; the primitive classes are modelled locally (retail RenderInfo sits 4 bytes below the PDB:
//   Lights +0xE0, Mesh +0xE4, LOD +0xE8, ObjectTransform +0xEC, Distance +0x11C).
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

struct CMaterialVisibility {                       // hidden-material filter of the mesh-from-file primitives
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

struct SInterpolationSetTime { float Time; float _4; float _8; };   // 12-byte records
extern SInterpolationSetTime g_InterpolationSetTimes[];             // 0x0143B934

struct CEngineInternalPrimitiveAnimatedMesh {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CEngineFadeDistance GetDynamicFadeDistance();       // slot 13 (+0x34)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& transform, float time, bool cache);   // slot 33 (+0x84), 0x00BCEF80
    virtual void InitialiseSimulation(const C3DMeshInfo* info, const C3DMesh2& mesh);              // slot 34 (+0x88), 0x00BCFCB0
    unsigned char       _pad04[4];
    long                PrimitiveType;             // +0x08
    unsigned long       EnginePrimitiveFlags;      // +0x0c
    float               AlphaMipBias;              // +0x10
    CEngineFadeDistance FadeDistance;              // +0x14
    unsigned char       _pad1c[0x0b];
    unsigned char       Alpha;                     // +0x27
    unsigned char       _pad28[4];
    long                InterpolationSet;          // +0x2c
    unsigned char       _pad30[4];
    unsigned char       PostRenderEnable;          // +0x34
    unsigned char       _pad35[0x13];
    CIVCountedPointerOcclusion OcclusionInfo[2];   // +0x48 one per view
    C3DMeshInfo*        MeshInfo;                  // +0x50
    C3DMeshInfo*        ShadowMeshInfo;            // +0x54
    float               Scale;                     // +0x58
    unsigned char       _pad5c[0x0c];
    CEngineMeshEffect*  MeshEffect;                // +0x68
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc; // +0x6c
    unsigned char       _pad70[0x70];              // +0x70 Anim / WorldPos / Orientation / Cache
    CEngineLocalLightArray* Lights;                // +0xe0 RenderInfo (retail 4 bytes below the PDB)
    C3DMesh2*           Mesh;                      // +0xe4
    unsigned long       LOD;                       // +0xe8
    CMatrix3x4          ObjectTransform;           // +0xec
    float               Distance;                  // +0x11c
    void*               BoneTransforms;            // +0x120
    long                RenderFrame;               // +0x124

    bool SetupMesh(float distance, float radius);                   // 0x00BCF2E0
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

struct CEngineInternalPrimitiveAnimatedMeshFromFile : CEngineInternalPrimitiveAnimatedMesh {   // primitive type 11
    CMaterialVisibility MaterialVisibility;        // +0x128
};

struct CEngineInternalPrimitiveAnimatedMeshType38 : CEngineInternalPrimitiveAnimatedMesh {     // primitive type 38
    unsigned char       _pad128[0x28];
    CMaterialVisibility MaterialVisibility;        // +0x150
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
    CEngineInternalPrimitiveAnimatedMesh* Parent;  // +0x0c
    CEngineSubPrimitiveBase* NextSubPrimitive;     // +0x10
    CEnginePrimitiveManager* Manager;              // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveAnimatedMesh* parent) : Parent(parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540 (engine sub-primitive pool)
};

struct CEngineSubPrimitiveBoundingBox : CEngineSubPrimitiveBase {   // vtable 0x012A3E84
    CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveAnimatedMesh* parent);   // 0x00B98B10 (out of line)
    virtual void v00();
};

// retail expands the same constructor inline at the second construction site (VC7.1 inline budget)
struct CEngineSubPrimitiveBoundingBoxInline : CEngineSubPrimitiveBase {   // vtable 0x012A3E84
    __forceinline CEngineSubPrimitiveBoundingBoxInline(CEngineInternalPrimitiveAnimatedMesh* parent) : CEngineSubPrimitiveBase(parent) { }
    virtual void v00();
};

struct CEngineSubPrimitiveAnimatedMesh : CEngineSubPrimitiveBase {  // 0x24 bytes
    unsigned char _pad18[0x0c];
    CEngineSubPrimitiveAnimatedMesh(CEngineInternalPrimitiveAnimatedMesh* parent, C3DPrimitive2* primitive, ERenderMode mode, bool cloth);   // 0x00BCE740
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
    unsigned char _pad80[0x3d];
    bool          OcclusionCulling;                // +0xbd
    unsigned char _padBE[0x19d];
    bool          RenderDebugBones;                // +0x25b
};

struct CEngineCameraState { unsigned char _pad00[0x54]; C3DVector Position; };   // +0x54

struct CEngineSimulationState { unsigned char _pad00[0xa4]; bool SimulationEnabled; };

struct CViewRecord {                               // 0x01436E80
    unsigned char _pad00[4];
    bool          HighDetail;                      // +0x04
    unsigned char _pad05[0x237];
    float         BoundingRadiusPadding;           // +0x23c
    unsigned char _pad240[0xb0];
    CEngineSimulationState* Simulation;            // +0x2f0
};

extern CEngineSubPrimitiveRenderer*      g_pSubPrimitiveRenderer;   // 0x01436E74
extern CEngineSettings*                  g_pEngineSettings;         // 0x01436E78
extern CViewRecord*                      g_pViewRecord;             // 0x01436E80
extern CEnginePrimitiveOcclusionManager* g_pOcclusionManager;       // 0x01436E58
extern CEngineShadowRenderer*            g_pShadowRenderer;         // 0x01436E60
extern CEngineLightingManager*           g_pLightingManager;        // 0x01436E9C
extern CEngineCameraState*               g_pEngineCamera;           // 0x01436EA0

struct CEnginePrimitiveManagerAnimatedMeshesBase {
    virtual void v00();
    unsigned char _pad04[4];
    bool          Enabled;                         // +0x08
    void AddGeneratedObjects(CEngineInternalPrimitiveAnimatedMesh* prim, CMatrix3x4* transform);   // 0x00BF18B0 (CEnginePrimitiveRenderUtil)
    void RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh* prim);                     // 0x00BD6810
    ERendered RenderPrimitive(CEngineInternalPrimitiveAnimatedMesh& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive(CEngineInternalPrimitiveAnimatedMesh& primRef, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain)
{
    if (!Enabled)
        return 0;

    CEngineInternalPrimitiveAnimatedMesh* prim = &primRef;
    prim->CalcInterpolatedObjectTransform(prim->ObjectTransform, g_InterpolationSetTimes[prim->InterpolationSet].Time, true);
    float radius = (Length(prim->MeshInfo->Center) + prim->MeshInfo->Radius) * prim->Scale + g_pViewRecord->BoundingRadiusPadding;
    C3DVector centre = prim->ObjectTransform * prim->MeshInfo->Center;
    long view = g_pEngineSettings->ViewIndex;
    float distance = Length(centre - g_pEngineCamera->Position);
    prim->Distance = distance;

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

    if (!prim->SetupMesh(distance, radius)) {
        g_pSubPrimitiveRenderer->AddSubPrimitive(7, new CEngineSubPrimitiveBoundingBoxInline(prim), distance, (CEnginePrimitiveManager*)this);
        prim->MeshInfo->RequestLoad(1);
        return 1;
    }
    C3DMesh2* mesh = prim->Mesh;
    AddGeneratedObjects(prim, &prim->ObjectTransform);
    if (mesh->HasSimulation)
        prim->InitialiseSimulation(prim->MeshInfo, *mesh);

    prim->Lights = g_pLightingManager->GetLightsInSphereFast(C3DBoundingSphere(centre, radius), 5, prim->GetThemeChannelDesc());

    const unsigned char* visibility = 0;
    if (prim->PrimitiveType == 11)
        visibility = static_cast<CEngineInternalPrimitiveAnimatedMeshFromFile*>(prim)->MaterialVisibility.GetVisibility(mesh);
    else if (prim->PrimitiveType == 38)
        visibility = static_cast<CEngineInternalPrimitiveAnimatedMeshType38*>(prim)->MaterialVisibility.GetVisibility(mesh);

    bool shadowed;
    if (!(prim->EnginePrimitiveFlags & 0x1000)) {
        CShadowRendererSceneData scene(g_pShadowRenderer->SceneData);
        shadowed = scene.Contains(centre, radius);
    } else {
        shadowed = false;
    }

    unsigned long count = mesh->PrimitiveCount;
    for (unsigned long i = 0; i < count; ++i) {
        C3DPrimitive2* primitive = &mesh->Primitives[i];
        long materialIndex = primitive->MaterialIndex;
        if (visibility != 0 && visibility[materialIndex] != 0)
            continue;
        C3DMeshMaterial* material = &mesh->Materials[materialIndex];
        long alpha = prim->GetFadedAlpha(distance);
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
                g_pSubPrimitiveRenderer->AddSubPrimitive(8, new CEngineSubPrimitiveAnimatedMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                if (material->Translucent)
                    g_pSubPrimitiveRenderer->AddSubPrimitive(9, new CEngineSubPrimitiveAnimatedMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                g_pSubPrimitiveRenderer->AddSubPrimitive(13, new CEngineSubPrimitiveAnimatedMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                renderFlags |= 0xa0;
                break;
            case 1:
                g_pSubPrimitiveRenderer->AddSubPrimitive(10, new CEngineSubPrimitiveAnimatedMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                g_pSubPrimitiveRenderer->AddSubPrimitive(14, new CEngineSubPrimitiveAnimatedMesh(prim, primitive, mode, cloth), distance, (CEnginePrimitiveManager*)this);
                renderFlags |= 0xa0;
                break;
            }
        }
        g_pSubPrimitiveRenderer->AddSubPrimitive(new CEngineSubPrimitiveAnimatedMesh(prim, primitive, mode, cloth), distance,
                                                 (unsigned char)alpha, renderFlags, (CEnginePrimitiveManager*)this, &chain);
    }
    if (g_pEngineSettings->RenderDebugBones)
        RenderDebugBoneHierarchy(prim);
    return 2;
}
