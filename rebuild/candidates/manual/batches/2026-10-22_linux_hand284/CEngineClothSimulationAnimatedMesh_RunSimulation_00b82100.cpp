// CEngineClothSimulationAnimatedMesh::RunSimulation() -- update the cloth transform and run state (clearing the particle simulations when it should not run); fetch the best loaded LOD mesh, and for the mesh primitive with particle systems create one particle simulation per system on first use (set up against the owner blend state, animation manager and bone scalings; any failure clears them all), then save positions and step every simulation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B82100, length 558 bytes (`ret`, virtual ?RunSimulation@CEngineClothSimulationAnimatedMesh@@UAEXXZ).
// Callees: UpdateTransform 0x00B81050, own slot 8 (+0x20) CalcAndSetRunState, ClearParticleSims 0x00B81D60, C3DMeshInfo::GetMesh 0x00A25E70,
//   the out-of-line counted-pointer destructor body 0x00A36EC0 (manifest "_Dest_val<...COptimisedPrimitive>" is a byte-identical COMDAT),
//   CEngineInternalPrimitiveAnimatedMesh::GetBoneScalings 0x00B83750, the simulation vector resize 0x00B840A0, operator new 0x00BFEA1A,
//   CParticleSimulation ctor 0x00AE0600 (manifest "GetDefaultVal<CPhysicalPrimitiveInit>"), CCountedPointer::Reset 0x00B82A50,
//   CParticleProgram::SetupSimulation 0x00AE29D0 / SavePositions 0x00ADF5B0 / SimulationStep 0x00AE2D20, operator delete 0x00BFE9BC.
//   Global g_ParticleMemoryPool 0x013D2D70.
// Layout: retail CEngineClothSimulationMeshBase (ParticleSims 12-byte vector +0xc, MeshInfo +0xa8, PrimitiveIndex +0xac, Owner +0xb0) as the landed
//   CalcAndSetRunState 0x00B81230; C3DMeshInfo as GetMesh 0x00A25E70; C3DMesh2 primitives +0x94 (0xa0-byte entries, particle systems vector +0x20);
//   the owner blend state +0x7c and animation +0x50 -> +4 -> +0x3c0 manager. Modelled locally.
// Shape: the first failure exit releases the mesh pointer inline, the empty-primitive exit calls the out-of-line destructor; the set-up loop names
//   the simulation and the program before the call (a direct chained call interleaves the pushes and tail-merges the two inline releases).
// Leaf: RunSimulation
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);
struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};
struct C3DVector;
struct C3DAnimationBlendState;
struct C3DAnimationManager;
struct CMovableResourceMemoryPool;
struct CParticleContext;
struct CParticleSimulation {
    CParticleSimulation();                              // 0x00AE0600
    unsigned char _body[0x80];
};
struct C3DMesh2;
struct CMeshPointer {                                   // CCountedPointer<const C3DMesh2>
    C3DMesh2* Object;
    SCountBlock* Info;
    void Destroy();                                     // 0x00A36EC0 (out-of-line destructor body)
    __forceinline void Release()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
};
struct CParticleProgram {
    void SavePositions(CParticleSimulation* sim);       // 0x00ADF5B0
    bool SetupSimulation(CParticleSimulation* sim, C3DAnimationBlendState* blend, C3DAnimationManager* manager, C3DVector* boneScalings,
                         C3DMesh2* mesh, CMovableResourceMemoryPool* pool, CParticleContext* context);   // 0x00AE29D0
    void SimulationStep(CParticleSimulation* sim, CParticleContext* context, C3DAnimationBlendState* blend, C3DAnimationManager* manager,
                        C3DVector* boneScalings, C3DMesh2* mesh);   // 0x00AE2D20
};
struct CMeshParticleSystem {
    void* m_00;
    CParticleProgram* Program;                          // +4
};
struct CParticleSystemPointer { CMeshParticleSystem* Object; SCountBlock* Info; };
struct C3DMeshPrimitive {
    unsigned char _pad00[0x20];
    CParticleSystemPointer* ParticleSystemsStart;       // +0x20
    CParticleSystemPointer* ParticleSystemsFinish;      // +0x24
    unsigned char _pad28[0xa0 - 0x28];
};
struct C3DMesh2 {
    unsigned char _pad00[0x94];
    C3DMeshPrimitive* Primitives;                       // +0x94
};
struct C3DMeshLODInfo;
struct CMeshData {
    void* __vftable;
    unsigned long IVRefCount;                           // +4
    C3DMeshLODInfo* LODMeshes[4];                       // +8
};
class C3DMeshInfo {
public:
    virtual void v00();
    long m_04, m_08;
    CMeshData* MeshData;                                // +0xc
    unsigned char _10[0x40];
    unsigned long LODDataSizes[4];                      // +0x50
    bool IsLODPresent(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    long GetLoadedLOD() const
    {
        if (MeshData) {
            for (unsigned long i = 0; IsLODPresent(i); ++i)
                if (MeshData->LODMeshes[i])
                    return i;
        }
        return -1;
    }
    bool GetMesh(CMeshPointer& out, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
};
struct C3DAnimationOwner { unsigned char _pad[0x3c0]; C3DAnimationManager* Manager; };   // +0x3c0
struct C3DAnimationHolder { void* m_00; C3DAnimationOwner* Owner; };                     // +4
class CEngineInternalPrimitiveAnimatedMesh {
public:
    unsigned char _pad00[0x50];
    C3DAnimationHolder* Animation;                      // +0x50
    unsigned char _pad54[0x7c - 0x54];
    unsigned char BlendState[4];                        // +0x7c (C3DAnimationBlendState)
    C3DVector* GetBoneScalings(C3DMesh2* mesh);         // 0x00B83750
};
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;   // 0x013D2D70
struct CParticleSimPointer {                             // CCountedPointer<CParticleSimulation>
    CParticleSimulation* Object;
    SCountBlock* Info;
    void Reset(CParticleSimulation* object);            // 0x00B82A50
};
struct CParticleSimArray {                               // 12-byte STLport vector
    CParticleSimPointer* _M_start;
    CParticleSimPointer* _M_finish;
    CParticleSimPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    void resize(unsigned long n);                       // 0x00B840A0
};
class CEngineSimulationBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual bool CalcAndSetRunState();                  // slot 8 (+0x20)
    long          RunState;                             // +0x04
    unsigned char SimStepOffset;                        // +0x08
};
class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    CParticleSimArray ParticleSims;                     // +0x0c
    unsigned char _pad18[0xa8 - 0x18];
    C3DMeshInfo*  MeshInfo;                             // +0xa8
    unsigned long PrimitiveIndex;                       // +0xac
    CEngineInternalPrimitiveAnimatedMesh* Owner;        // +0xb0
    void UpdateTransform();                             // 0x00B81050
    void ClearParticleSims();                           // 0x00B81D60
    CParticleContext* GetContext() { return (CParticleContext*)this; }
};
class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase {
public:
    virtual void RunSimulation();
};

void CEngineClothSimulationAnimatedMesh::RunSimulation()
{
    UpdateTransform();
    if (!CalcAndSetRunState()) {
        ClearParticleSims();
        return;
    }
    CMeshPointer mesh;
    mesh.Object = 0;
    mesh.Info = 0;
    if (!MeshInfo->GetMesh(mesh, MeshInfo->GetLoadedLOD(), 4)) {
        mesh.Release();
        return;
    }
    C3DMeshPrimitive& primitive = mesh.Object->Primitives[PrimitiveIndex];
    if (primitive.ParticleSystemsStart == primitive.ParticleSystemsFinish) {
        mesh.Destroy();
        return;
    }
    CEngineInternalPrimitiveAnimatedMesh* owner = Owner;
    C3DAnimationBlendState* blend = (C3DAnimationBlendState*)owner->BlendState;
    C3DVector* boneScalings = owner->GetBoneScalings(mesh.Object);
    C3DAnimationManager* manager = owner->Animation->Owner->Manager;
    if (ParticleSims.empty()) {
        unsigned long count = primitive.ParticleSystemsFinish - primitive.ParticleSystemsStart;
        ParticleSims.resize(count);
        for (unsigned long i = 0; i < count; ++i) {
            ParticleSims._M_start[i].Reset(new CParticleSimulation);
            CParticleSimulation* sim = ParticleSims._M_start[i].Object;
            CParticleProgram* program = primitive.ParticleSystemsStart[i].Object->Program;
            if (!program->SetupSimulation(sim, blend, manager,
                    boneScalings, mesh.Object, g_ParticleMemoryPool, GetContext())) {
                ClearParticleSims();
                break;
            }
        }
    }
    for (unsigned long i = 0; i < ParticleSims.size(); ++i) {
        CMeshParticleSystem* system = primitive.ParticleSystemsStart[i].Object;
        CParticleSimulation* sim = ParticleSims._M_start[i].Object;
        system->Program->SavePositions(sim);
        system->Program->SimulationStep(sim, GetContext(), blend, manager, boneScalings, mesh.Object);
    }
    mesh.Release();
}
