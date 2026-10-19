#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
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
void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct C3DMesh2;
struct CCPPointerInfo {
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
template <class T> struct CCountedPointer {
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

struct C3DMeshInfo {
    bool GetMesh(CCountedPointer<const C3DMesh2>* mesh, unsigned long lod, unsigned long flags);   // 0x00A25E70
};

struct CEngine {
    unsigned char _pad[0x88];
    long          CurrentRenderFrame;                  // +0x88 (retail)
};
extern CEngine* g_pEngine;                             // 0x01436EA4

struct CEngineRenderInfoAnimatedMesh {                 // retail +0xe0 in the primitive
    void*           Lights;                            // +0x00
    const C3DMesh2* Mesh;                              // +0x04
    unsigned long   LOD;                               // +0x08
    float           ObjectTransform[12];               // +0x0c
    float           Distance;                          // +0x3c
    void*           BoneTransforms;                    // +0x40
    long            RenderFrame;                       // +0x44
};

struct CEngineInternalPrimitiveAnimatedMesh {
    unsigned char                 _pad00[0x50];
    C3DMeshInfo*                  MeshInfo;            // +0x50
    unsigned char                 _pad54[0xe0 - 0x54];
    CEngineRenderInfoAnimatedMesh RenderInfo;          // +0xe0
    bool SetupMesh2D();
};

bool CEngineInternalPrimitiveAnimatedMesh::SetupMesh2D()
{
    long frame = g_pEngine->CurrentRenderFrame;
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

// ---- test doubles and driver ----
#include <cstdlib>
static bool g_armed = false; static void* g_deleted = 0; static int g_destroyed = 0;
void __cdecl operator delete(void* block) { if (g_armed) { g_deleted = block; return; } std::free(block); }
static void __fastcall DestroyMesh(void*) { ++g_destroyed; }
static CCPPointerInfo g_info;
static bool g_ok = true; static int g_getCalls = 0;
bool C3DMeshInfo::GetMesh(CCountedPointer<const C3DMesh2>* mesh, unsigned long lod, unsigned long flags)
{
    ++g_getCalls;
    if (lod != 0 || flags != 0x10 || mesh->Data != 0 || mesh->Info != 0) return false;
    mesh->Data = reinterpret_cast<const C3DMesh2*>(&g_info); mesh->Info = &g_info;
    g_info.References = 1; g_info.Destroy = DestroyMesh; g_info.Object = &g_info;
    return g_ok;
}
static CEngine g_engine;
CEngine* g_pEngine = &g_engine;
int main()
{
    static CEngineInternalPrimitiveAnimatedMesh m; static C3DMeshInfo info;
    m.MeshInfo = &info; g_engine.CurrentRenderFrame = 5;
    m.RenderInfo.Mesh = 0; m.RenderInfo.LOD = 3; m.RenderInfo.BoneTransforms = &m; m.RenderInfo.RenderFrame = 0;
    g_armed = true;
    bool r1 = m.SetupMesh2D();
    bool ok = r1 && g_getCalls == 1 && m.RenderInfo.Mesh == reinterpret_cast<const C3DMesh2*>(&g_info) && m.RenderInfo.LOD == 0
        && m.RenderInfo.BoneTransforms == 0 && m.RenderInfo.RenderFrame == 5 && g_destroyed == 1 && g_deleted == &g_info;
    bool r2 = m.SetupMesh2D();                         // same frame, mesh present: nothing fetched
    ok = ok && r2 && g_getCalls == 1;
    g_engine.CurrentRenderFrame = 6; g_ok = false; g_deleted = 0;
    bool r3 = m.SetupMesh2D();                         // new frame, fetch fails: false, the block still released
    ok = ok && !r3 && g_getCalls == 2 && g_destroyed == 2 && g_deleted == &g_info && m.RenderInfo.RenderFrame == 5;
    g_armed = false;
    if (!ok) { std::printf("00bcf430_TEST FAIL\n"); return 1; }
    std::printf("00bcf430_TEST PASS\n");
    return 0;
}