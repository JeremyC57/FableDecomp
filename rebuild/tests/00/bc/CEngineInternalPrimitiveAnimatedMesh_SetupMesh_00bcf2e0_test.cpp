// test for 0x00BCF2E0 CEngineInternalPrimitiveAnimatedMesh::SetupMesh -- LOD choice, forced LOD, per-frame caching, GetMesh failure
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class C3DMesh2 { public: int Id; };
class CCPPointerInfo { public:
    long  References;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
static int g_destroyed = 0;
static void __fastcall DestroyMesh(void*) { ++g_destroyed; }
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                free(Info);
            }
        }
    }
};
static C3DMesh2 g_meshes[4];
static unsigned long g_lastLod = 99;
class C3DMeshInfo { public:
    float         LODErrors[4];
    unsigned long LODDataSizes[4];
    bool          Fail;
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags) const
    {
        g_lastLod = lod;
        if (Fail || flags != 0)
            return false;
        mesh.Data = &g_meshes[lod];
        mesh.Info = (CCPPointerInfo*)malloc(sizeof(CCPPointerInfo));
        mesh.Info->References = 1;
        mesh.Info->Destroy = DestroyMesh;
        mesh.Info->Object = 0;
        return true;
    }
    bool HasLOD(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    float GetLODError(unsigned long lod) const { return LODErrors[lod]; }
};
class CEngine { public: long CurrentRenderFrame; };
static CEngine g_engine;
CEngine* g_pEngine_01436ea4 = &g_engine;
class CEnginePrimitiveRendererSharedData { public:
    float CharacterLODErrorTolerance;
    float LODVirtualScreenSize;
    float LODVirtualScreenSizeFOVAdjusted;
    long  LODForced;
};
class CEnginePrimitiveRenderer : public CEnginePrimitiveRendererSharedData { public: };
static CEnginePrimitiveRenderer g_renderer;
CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData = &g_renderer;
CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80 = &g_renderer;
class CEnginePrimitiveManagerMeshBase { public:
    static __forceinline unsigned long __fastcall GetAppropriateLOD(const C3DMeshInfo* meshInfo, float distance, float radius, float errorTolerance)
    {
        float distanceToSurface = distance - radius > 0.0001f ? distance - radius : 0.0001f;
        errorTolerance *= distanceToSurface / (radius * g_pPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted);
        unsigned long lod = 0;
        while (meshInfo->HasLOD(lod + 1) && !(meshInfo->GetLODError(lod + 1) > errorTolerance))
            ++lod;
        return lod;
    }
};
class CEngineRenderInfoAnimatedMesh { public:
    void*           Lights;
    const C3DMesh2* Mesh;
    unsigned long   LOD;
    float           ObjectTransform[12];
    float           Distance;
    void*           BoneTransforms;
    long            RenderFrame;
};
class CEngineInternalPrimitiveAnimatedMesh { public:
    const C3DMeshInfo*            MeshInfo;
    CEngineRenderInfoAnimatedMesh RenderInfo;
    bool SetupMesh(float distance, float radius);
};
bool CEngineInternalPrimitiveAnimatedMesh::SetupMesh(float distance, float radius)
{
    long frame = g_pEngine_01436ea4->CurrentRenderFrame;
    if (!RenderInfo.Mesh || RenderInfo.RenderFrame != frame) {
        unsigned long lod = CEnginePrimitiveManagerMeshBase::GetAppropriateLOD(MeshInfo, distance, radius, g_pPrimitiveRendererSharedData->CharacterLODErrorTolerance);
        if (g_pPrimitiveRenderer_01436e80->LODForced != -1)
            lod = g_pPrimitiveRenderer_01436e80->LODForced;
        CCountedPointer<const C3DMesh2> mesh;
        if (!MeshInfo->GetMesh(mesh, lod, 0))
            return false;
        RenderInfo.Mesh = mesh.Data;
        RenderInfo.LOD = lod;
        RenderInfo.RenderFrame = frame;
        RenderInfo.BoneTransforms = 0;
    }
    return true;
}
int main()
{
    int ok = 1;
    static C3DMeshInfo info;
    info.LODErrors[0] = 0.0f; info.LODErrors[1] = 1.0f; info.LODErrors[2] = 2.0f; info.LODErrors[3] = 4.0f;
    info.LODDataSizes[0] = info.LODDataSizes[1] = info.LODDataSizes[2] = info.LODDataSizes[3] = 100;
    info.Fail = false;
    g_renderer.CharacterLODErrorTolerance = 1.0f;
    g_renderer.LODVirtualScreenSizeFOVAdjusted = 1.0f;
    g_renderer.LODForced = -1;
    static CEngineInternalPrimitiveAnimatedMesh prim;
    prim.MeshInfo = &info;
    prim.RenderInfo.BoneTransforms = &prim;
    g_engine.CurrentRenderFrame = 7;
    // distance 3, radius 1 -> surface 2, error 1 * 2 / 1 = 2 -> LOD 2 (LOD 3 needs 4)
    if (!prim.SetupMesh(3.0f, 1.0f) || prim.RenderInfo.LOD != 2 || prim.RenderInfo.Mesh != &g_meshes[2]
        || prim.RenderInfo.RenderFrame != 7 || prim.RenderInfo.BoneTransforms != 0 || g_destroyed != 1) ok = 0;
    // same frame: cached, GetMesh not called
    g_lastLod = 99;
    if (!prim.SetupMesh(100.0f, 1.0f) || g_lastLod != 99 || prim.RenderInfo.LOD != 2) ok = 0;
    // new frame, far away: all LODs pass -> 3; missing LOD 2 data stops at 1
    g_engine.CurrentRenderFrame = 8;
    if (!prim.SetupMesh(100.0f, 1.0f) || prim.RenderInfo.LOD != 3) ok = 0;
    g_engine.CurrentRenderFrame = 9;
    info.LODDataSizes[2] = 0;
    if (!prim.SetupMesh(100.0f, 1.0f) || prim.RenderInfo.LOD != 1) ok = 0;
    // inside the sphere: clamped surface distance -> LOD 0
    g_engine.CurrentRenderFrame = 10;
    if (!prim.SetupMesh(0.5f, 1.0f) || prim.RenderInfo.LOD != 0) ok = 0;
    // forced LOD wins
    g_engine.CurrentRenderFrame = 11;
    g_renderer.LODForced = 3;
    if (!prim.SetupMesh(0.5f, 1.0f) || prim.RenderInfo.LOD != 3) ok = 0;
    // GetMesh failure: false, render info untouched
    g_engine.CurrentRenderFrame = 12;
    info.Fail = true;
    if (prim.SetupMesh(0.5f, 1.0f) || prim.RenderInfo.RenderFrame != 11) ok = 0;
    if (!ok) { printf("00bcf2e0_TEST FAIL\n"); return 1; }
    printf("00bcf2e0_TEST PASS\n");
    return 0;
}