// Grade: DIFFER(472v455) -- three shape gaps: retail's inlined best-LOD loop is not rotated (`cmp eax,4 ; jae` at the top, `jmp` back) where ours is; the empty-particle-systems exit calls the counted-pointer destructor out of line (0x00A36EC0) where ours inlines it; the first loop keeps i in a frame word and the count in ebp (ours the reverse)
// CEngineClothSimulationStaticMesh::RunSimulation() -- per frame: refresh the transform; when the simulation is disabled drop
//   the particle sims; otherwise fetch the best loaded LOD mesh, and for this primitive's particle systems create one particle
//   simulation each (first run; set-up failure drops them all) and then save positions + step every simulation.
// Leaf: RunSimulation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B81F30, 455 bytes (+int3).
// Callees: CEngineClothSimulationMeshBase::UpdateTransform 0x00B81050 / ClearParticleSims 0x00B81D60, own slot 8 (+0x20),
//   C3DMeshInfo::GetMesh 0x00A25E70, the CCountedPointer<const C3DMesh2> destructor 0x00A36EC0 (manifest label `_Dest_val<...
//   COptimisedPrimitive>` is a byte-identical instance), vector<CCountedPointer<CParticleSimulation>>::resize 0x00B840A0
//   (manifest `_Umove<EWeaponClass*>` propagated), operator new 0x00BFEA1A, CParticleSimulation ctor 0x00AE0600 (manifest
//   `GetDefaultVal<CPhysicalPrimitiveInit>` propagated), CCountedPointer::Reset 0x00B82A50, CParticleProgram::SetupSimulation
//   0x00AE0050 / SavePositions 0x00ADF5B0 (manifest `COpinionPersonalityDef::operator=` propagated) / SimulationStep 0x00AE2CB0,
//   operator delete 0x00BFE9BC; particle memory pool 0x013D2D70.
// Layout: Ego_r.pdb CEngineClothSimulationMeshBase less 4 past the 12-byte STLport ParticleSims (MeshInfo +0xa8, PrimitiveIndex
//   +0xac); C3DMeshInfo MeshData +0x0c / LODDataSizes +0x50 (PDB); the C3DMesh2 primitive array (+0x94, 0xa0-byte entries with
//   an 8-byte-entry particle-system vector at +0x20) is descriptive. No engine/ header covers these.
// Tried: for / while / for(;;) / goto / do-while / pointer-walk forms of the LOD loop (all rotated), #pragma optimize("s")
//   (frame pointer, nothing inlined).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

#include "engine/CCountedPointer.h"



struct CParticleContext;
struct CMovableResourceMemoryPool;
struct CParticleSimulation {
    CParticleSimulation();                             // 0x00AE0600
    unsigned char _body[0x80];
};
struct CParticleProgram {
    bool SetupSimulation(CParticleSimulation* simulation, CMovableResourceMemoryPool* pool);   // 0x00AE0050
    void SavePositions(CParticleSimulation* simulation);                                         // 0x00ADF5B0
    void SimulationStep(CParticleSimulation* simulation, CParticleContext* context);            // 0x00AE2CB0
};
struct CParticleSystemDef {
    void*             _pad00;
    CParticleProgram* Program;                         // +0x04
};
struct CParticleSystemEntry {                          // 8 bytes
    CParticleSystemDef* Def;
    void*               Info;
};
struct C3DMeshPrimitive {                              // 0xa0 bytes
    unsigned char         _pad00[0x20];
    CParticleSystemEntry* ParticleSystems_start;       // +0x20
    CParticleSystemEntry* ParticleSystems_finish;      // +0x24
    CParticleSystemEntry* ParticleSystems_eos;         // +0x28
    unsigned char         _pad2c[0xa0 - 0x2c];
    unsigned long ParticleSystemCount() const { return ParticleSystems_finish - ParticleSystems_start; }
};
struct C3DMesh2 {
    unsigned char     _pad00[0x94];
    C3DMeshPrimitive* Primitives;                      // +0x94
};
struct C3DMeshInfo {
    struct CMeshData {
        void* __vftable;
        long  RefCount;
        void* LODs[4];                                 // +0x08
    };
    unsigned char _pad00[0x0c];
    CMeshData*    MeshData;                            // +0x0c CIVCountedPointer<CMeshData>
    unsigned char _pad10[0x50 - 0x10];
    unsigned long LODDataSizes[4];                     // +0x50
    long GetBestLoadedLOD() const
    {
        if (MeshData) {
            for (unsigned long i = 0; i < 4 && LODDataSizes[i]; ++i)
                if (MeshData->LODs[i])
                    return i;
        }
        return -1;
    }
    bool GetMesh(CCountedPointer<const C3DMesh2>* mesh, unsigned long lod, unsigned long flags);   // 0x00A25E70
};

template <class T> struct CRetailVector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
    void resize(unsigned long n);                      // 0x00B840A0
};

extern CMovableResourceMemoryPool* g_pParticleMemoryPool;   // 0x013D2D70

struct CEngineClothSimulationMeshBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual bool IsSimulationEnabled();                // slot 8 (+0x20)
    unsigned char _pad04[0x08];
    CRetailVector<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0c
    unsigned char _pad18[0xa8 - 0x18];
    C3DMeshInfo*  MeshInfo;                            // +0xa8
    unsigned long PrimitiveIndex;                      // +0xac
    void UpdateTransform();                            // 0x00B81050
    void ClearParticleSims();                          // 0x00B81D60
};

struct CEngineClothSimulationStaticMesh : CEngineClothSimulationMeshBase {
    virtual void RunSimulation();
};

void CEngineClothSimulationStaticMesh::RunSimulation()
{
    UpdateTransform();
    if (!IsSimulationEnabled()) {
        ClearParticleSims();
        return;
    }
    CCountedPointer<const C3DMesh2> mesh;
    if (!MeshInfo->GetMesh(&mesh, MeshInfo->GetBestLoadedLOD(), 4))
        return;
    const C3DMeshPrimitive& primitive = mesh->Primitives[PrimitiveIndex];
    if (primitive.ParticleSystems_start == primitive.ParticleSystems_finish)
        return;
    if (ParticleSims.empty()) {
        unsigned long count = primitive.ParticleSystemCount();
        ParticleSims.resize(count);
        for (unsigned long i = 0; i < count; ++i) {
            ParticleSims[i].Reset(new CParticleSimulation);
            if (!primitive.ParticleSystems_start[i].Def->Program->SetupSimulation(ParticleSims[i].Data, g_pParticleMemoryPool)) {
                ClearParticleSims();
                break;
            }
        }
    }
    for (unsigned long j = 0; j < ParticleSims.size(); ++j) {
        CParticleSystemDef* def = primitive.ParticleSystems_start[j].Def;
        CParticleSimulation* simulation = ParticleSims[j].Data;
        def->Program->SavePositions(simulation);
        def->Program->SimulationStep(simulation, (CParticleContext*)this);
    }
}
