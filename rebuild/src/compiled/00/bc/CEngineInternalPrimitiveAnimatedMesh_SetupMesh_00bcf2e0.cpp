// CEngineInternalPrimitiveAnimatedMesh::SetupMesh(float distance, float radius) -- once per render frame, pick the mesh LOD for this distance and radius (the forced renderer LOD when set) and fetch it from the mesh info into the render info, clearing the bone transforms and stamping the frame; false when the mesh info has no mesh for it
// Leaf: ?SetupMesh@CEngineInternalPrimitiveAnimatedMesh@@QAE_NMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCF2E0, length 323 bytes, `ret 8`, bool in al.
// Name: FableWin.pdb `?SetupMesh@CEngineInternalPrimitiveAnimatedMesh@@QAE_NMM@Z` (catalog label SetupMesh agrees; sibling SetupMesh2D
//   0x00BCF430 landed). Parameter names are descriptive (the PDB name set carries none).
// Callees: C3DMeshInfo::GetMesh 0x00A25E70 (FableWin + Ego_r `?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z`:
//   const, counted pointer BY REFERENCE), operator delete 0x00BFE9BC (the counted pointer's info block).
// Inlines (FableWin.pdb names): CEnginePrimitiveManagerMeshBase::GetAppropriateLOD (`?GetAppropriateLOD@CEnginePrimitiveManagerMeshBase@@
//   SIKPBVC3DMeshInfo@@MMM@Z`, public static __fastcall -- no out-of-line copy in Fable.exe), C3DMeshInfo::HasLOD(unsigned long) const,
//   C3DMeshInfo::GetLODError(unsigned long) const. HasLOD carries the `lod < 4` bound (the loop's `cmp edx,3 ; jae` top test, the loop
//   left unrotated; without the bound VC7.1 rotates it); the tolerance parameter is scaled in place (`errorTolerance *=`), which keeps
//   it enregistered from the first instruction as in retail; the distance clamp is a `>` ternary (`test ah,41h ; je`).
// Globals: the engine 0x01436EA4 (CurrentRenderFrame +0x88, engine/CEngine.h); the renderer shared data 0x01436E78
//   (CharacterLODErrorTolerance +0xA0, LODVirtualScreenSizeFOVAdjusted +0xA8 -- Ego_r.pdb CEnginePrimitiveRendererSharedData offsets,
//   as the landed FrameStart 0x00B32660 reads them); the renderer 0x01436E80 (LODForced +0x264, inherited from the shared data). The
//   pointer spellings are descriptive (see NOTES). Constant 0.0001f @ 0x0129BA3C (a pooled literal).
// Layout (retail): CEngineInternalPrimitiveMeshBase::MeshInfo +0x50; RenderInfo (CEngineRenderInfoAnimatedMesh, 4 bytes below the
//   Ego_r +0xe4) Mesh +0xe4, LOD +0xe8, BoneTransforms +0x120, RenderFrame +0x124 (as the landed SetupMesh2D); C3DMeshInfo
//   LODErrors +0x40, LODDataSizes +0x50 (engine/C3DMeshInfo.h).
void __cdecl operator delete(void* block);             // 0x00BFE9BC
#define CEngine CEngineDonor
#include "engine/CEngine.h"
#undef CEngine
#define CEnginePrimitiveManagerMeshBase CEnginePrimitiveManagerMeshBaseHeader
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#undef CEnginePrimitiveManagerMeshBase
#define C3DMeshInfo C3DMeshInfoDonor
#include "engine/C3DMeshInfo.h"
#undef C3DMeshInfo


class C3DMesh2;
class CCPPointerInfo { public:
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
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
                operator delete(Info);
            }
        }
    }
};
class C3DMeshInfo : public C3DMeshInfoDonor { public:
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
    bool HasLOD(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    float GetLODError(unsigned long lod) const { return LODErrors[lod]; }
};
class CEngine : public CEngineDonor { public: };
extern CEngine* g_pEngine_01436ea4;                             // 0x01436EA4
class CEnginePrimitiveRendererSharedData { public:
    unsigned char _pad00[0xa0];
    float         CharacterLODErrorTolerance;          // +0xa0
    float         LODVirtualScreenSize;                // +0xa4
    float         LODVirtualScreenSizeFOVAdjusted;     // +0xa8
    unsigned char _padac[0x264 - 0xac];
    long          LODForced;                           // +0x264
};
class CEnginePrimitiveRenderer : public CEnginePrimitiveRendererSharedData { public: };
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData;   // 0x01436E78
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;              // 0x01436E80
class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManagerMeshBaseHeader { public:   // engine/CEnginePrimitiveManagerMeshBase.h layout
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
class CEngineRenderInfoAnimatedMesh { public:                 // retail +0xe0 in the primitive
    void*           Lights;                            // +0x00
    const C3DMesh2* Mesh;                              // +0x04
    unsigned long   LOD;                               // +0x08
    float           ObjectTransform[12];               // +0x0c
    float           Distance;                          // +0x3c
    void*           BoneTransforms;                    // +0x40
    long            RenderFrame;                       // +0x44
};
class CEngineInternalPrimitiveAnimatedMesh { public:
    unsigned char                 _pad00[0x50];
    const C3DMeshInfo*            MeshInfo;            // +0x50
    unsigned char                 _pad54[0xe0 - 0x54];
    CEngineRenderInfoAnimatedMesh RenderInfo;          // +0xe0
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