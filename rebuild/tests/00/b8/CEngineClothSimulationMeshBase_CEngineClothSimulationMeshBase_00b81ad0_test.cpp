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
// CEngineClothSimulationMeshBase::CEngineClothSimulationMeshBase(C3DMeshInfo*, unsigned long, CEngineInternalPrimitiveMeshBase*) -- constructor: CEngineSimulationBase (reference count 1, the next simulation id), an empty ParticleSims vector, the mesh/index/owner arguments, and the three transforms set to identity
// Leaf: ??0CEngineClothSimulationMeshBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B81AD0, 233 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0. Vtables: 0x012A38FC CEngineSimulationBase (stored by its
//   inline ctor, as landed 0x00B80900), 0x012A3924 CEngineClothSimulationMeshBase (RTTI). Global: the physics settings
//   0x01436E68 (simulation id counter byte +0x1c).
// Layout (retail; Ego_r.pdb CEngineClothSimulationMeshBase sits 4 bytes higher from +0x0c on because its CArray is 16 bytes
//   where the retail STLport vector is 12): CEngineSimulationBase {vptr, refs +4, id +8}, ParticleSims +0x0c {start, finish,
//   end_of_storage}, CurrentTransform +0x18, LastTransform +0x48, InverseTransform +0x78 (CMatrix3x4: 12 floats), MeshInfo
//   +0xa8, PrimitiveIndex +0xac, Owner +0xb0 (0xb4 bytes).
// Shape: the vector is a member object with an inline zeroing ctor; the three arguments are init-list members; the
//   matrices are identity-filled in the body (Current, Inverse, Last: retail's store order) by an inline SetIdentity (a chained off-diagonal zero assignment stored high to
//   low, then the diagonal m[8], m[4], m[0]) with 0.0f/1.0f held in eax/ecx.
struct C3DMeshInfo;
struct CEngineInternalPrimitiveMeshBase;

struct SPhysicsSettings {
    unsigned char _pad[0x1c];
    unsigned char m_simulationCounter;                 // +0x1c
    unsigned char NextSimulationId()
    {
        unsigned char id = m_simulationCounter;
        m_simulationCounter = id + 1;
        return id;
    }
};
extern SPhysicsSettings* g_pPhysicsSettings;           // 0x01436E68

struct CBaseClassNonCopyable {
    virtual void v00() {}
    CBaseClassNonCopyable();                           // 0x0099A2F0
};

struct CEngineSimulationBase : CBaseClassNonCopyable { // vtable 0x012A38FC
    long          m_references;                        // +0x04
    unsigned char m_id;                                // +0x08
    virtual void v00() {}
    CEngineSimulationBase() : m_references(1), m_id(g_pPhysicsSettings->NextSimulationId()) { }
};

struct CParticleSimulationPointer {                    // CCountedPointer<CParticleSimulation>
    void* m_pObject;
    void* m_pCount;
};
struct SParticleSimArray {                             // STLport vector<CCountedPointer<CParticleSimulation>> (12 bytes)
    CParticleSimulationPointer* _M_start;
    CParticleSimulationPointer* _M_finish;
    CParticleSimulationPointer* _M_end_of_storage;
    SParticleSimArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CMatrix3x4 {
    float m[12];                                       // rows X, Y, Z, Pos
    void SetIdentity()
    {
        m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = m[9] = m[10] = m[11] = 0.0f;
        m[0] = m[4] = m[8] = 1.0f;
    }
};

struct CEngineClothSimulationMeshBase : CEngineSimulationBase {   // vtable 0x012A3924
    SParticleSimArray                 ParticleSims;    // +0x0c
    CMatrix3x4                        CurrentTransform;   // +0x18
    CMatrix3x4                        LastTransform;      // +0x48
    CMatrix3x4                        InverseTransform;   // +0x78
    C3DMeshInfo*                      MeshInfo;        // +0xa8
    unsigned long                     PrimitiveIndex;  // +0xac
    CEngineInternalPrimitiveMeshBase* Owner;           // +0xb0
    virtual void v00() {}
    CEngineClothSimulationMeshBase(C3DMeshInfo* meshInfo, unsigned long primitiveIndex, CEngineInternalPrimitiveMeshBase* owner);
};

CEngineClothSimulationMeshBase::CEngineClothSimulationMeshBase(C3DMeshInfo* meshInfo, unsigned long primitiveIndex, CEngineInternalPrimitiveMeshBase* owner)
    : MeshInfo(meshInfo)
    , PrimitiveIndex(primitiveIndex)
    , Owner(owner)
{
    CurrentTransform.SetIdentity();
    InverseTransform.SetIdentity();
    LastTransform.SetIdentity();
}

// ---- test doubles and driver ----
static int g_baseCtors = 0;
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_baseCtors; }
static SPhysicsSettings g_settings;
SPhysicsSettings* g_pPhysicsSettings = &g_settings;
static bool IsIdentity(const CMatrix3x4& m)
{
    for (int i = 0; i < 12; ++i) if (m.m[i] != ((i == 0 || i == 4 || i == 8) ? 1.0f : 0.0f)) return false;
    return true;
}
int main()
{
    g_settings.m_simulationCounter = 41;
    C3DMeshInfo* info = reinterpret_cast<C3DMeshInfo*>(&g_settings);
    CEngineInternalPrimitiveMeshBase* owner = reinterpret_cast<CEngineInternalPrimitiveMeshBase*>(&g_baseCtors);
    CEngineClothSimulationMeshBase* a = new CEngineClothSimulationMeshBase(info, 7, owner);
    CEngineClothSimulationMeshBase* b = new CEngineClothSimulationMeshBase(0, 9, 0);
    bool ok = g_baseCtors == 2 && a->m_references == 1 && a->m_id == 41 && b->m_id == 42 && g_settings.m_simulationCounter == 43
        && a->ParticleSims._M_start == 0 && a->ParticleSims._M_finish == 0 && a->ParticleSims._M_end_of_storage == 0
        && a->MeshInfo == info && a->PrimitiveIndex == 7 && a->Owner == owner && b->PrimitiveIndex == 9
        && IsIdentity(a->CurrentTransform) && IsIdentity(a->LastTransform) && IsIdentity(a->InverseTransform);
    if (!ok) { std::printf("00b81ad0_TEST FAIL\n"); return 1; }
    std::printf("00b81ad0_TEST PASS\n");
    return 0;
}