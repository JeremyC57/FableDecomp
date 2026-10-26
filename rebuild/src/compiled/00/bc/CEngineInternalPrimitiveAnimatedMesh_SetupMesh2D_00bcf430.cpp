// CEngineInternalPrimitiveAnimatedMesh::SetupMesh2D() -- once per render frame, fetch LOD 0 of the mesh (flag 0x10) from the mesh info into the render info, clearing the LOD and bone transforms and stamping the frame; false when the mesh info has none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCF430, 191 bytes (+ int3 padding), bool in al.
// Callees: C3DMeshInfo::GetMesh(CCountedPointer<const C3DMesh2>*, unsigned long lod, unsigned long flags) 0x00A25E70 (bool),
//   operator delete 0x00BFE9BC (the counted pointer's info block); the engine singleton 0x01436EA4 (CurrentRenderFrame at retail
//   +0x88; engine/CEngine.h +0x90, the retail CEngine sits 8 bytes below the PDB there, cf. 0x00B23A90).
// Layout (retail): CEngineInternalPrimitiveMeshBase::MeshInfo +0x50; RenderInfo (CEngineRenderInfoAnimatedMesh, 4 bytes below
//   the PDB's +0xe4) Lights +0xe0, Mesh +0xe4, LOD +0xe8, BoneTransforms +0x120, RenderFrame +0x124.
// Shape: the counted pointer is a local declared after the early `return true` (that exit skips its release); its inline
//   dtor releases the info block {References, Destroy(__fastcall, Object), Object} and is duplicated on both later exits; the
//   render info takes the raw mesh pointer (no add-ref).
// Layout (supersedes any local-model note above): engine/CEngine.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CEngine CEngineDonor
#include "engine/CEngine.h"
#undef CEngine
void __cdecl operator delete(void* block);             // 0x00BFE9BC

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

class C3DMeshInfo { public:
    bool GetMesh(CCountedPointer<const C3DMesh2>* mesh, unsigned long lod, unsigned long flags);   // 0x00A25E70
};

class CEngine : public CEngineDonor {
 public:
};
extern CEngine* g_pEngine_01436ea4;                             // 0x01436EA4

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
    C3DMeshInfo*                  MeshInfo;            // +0x50
    unsigned char                 _pad54[0xe0 - 0x54];
    CEngineRenderInfoAnimatedMesh RenderInfo;          // +0xe0
    bool SetupMesh2D();
};

bool CEngineInternalPrimitiveAnimatedMesh::SetupMesh2D()
{
    long frame = g_pEngine_01436ea4->CurrentRenderFrame;
    if (RenderInfo.Mesh && RenderInfo.RenderFrame == frame)
        return true;
    CCountedPointer<const C3DMesh2> mesh;
    if (!MeshInfo->GetMesh(&mesh, 0, 0x10))
        return false;
    RenderInfo.Mesh = mesh.Data;
    RenderInfo.LOD = 0;
    RenderInfo.RenderFrame = frame;
    RenderInfo.BoneTransforms = 0;
    return true;
}