// CEngineClothSimulationStaticMesh::RunSimulation() -- one simulation tick of a static mesh's cloth: update the cloth transform and the run state (a simulation that should not run drops its particle simulations); fetch the best loaded LOD mesh, and for this simulation's mesh primitive create one particle simulation per cloth primitive on first use (set up from the cloth's particle program in the particle memory pool; any failure clears them all), then save every simulation's state to interpolate from and step it with this object as the particle context
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81F30, length 455 bytes (`ret`, vtable 0x012A3998 slot 6).
// Name: Ego_r.pdb / FableWin.pdb ?RunSimulation@CEngineClothSimulationStaticMesh@@UAEXXZ (public virtual, no arguments).
// Callees: CEngineClothSimulationMeshBase::UpdateTransform 0x00B81050, own slot 8 (+0x20) CalcAndSetRunState (0x00B81180 for this class),
//   CEngineClothSimulationMeshBase::ClearParticleSims 0x00B81D60, C3DMeshInfo::GetMesh 0x00A25E70, the out-of-line
//   ~CCountedPointer<const C3DMesh2> COMDAT 0x00A36EC0 (manifest "_Dest_val<...COptimisedPrimitive>" is a byte-identical fold),
//   vector<CCountedPointer<CParticleSimulation> >::resize(n) 0x00B840A0 (Ego_r.pdb / FableWin.pdb; the manifest's "_Umove<EWeaponClass>"
//   label is a propagated one), operator new 0x00BFEA1A, CParticleSimulation::CParticleSimulation 0x00AE0600,
//   CCountedPointer<CParticleSimulation>::Reset 0x00B82A50, CParticleProgram::SetupSimulation(simulation, pool) 0x00AE0050,
//   CParticleProgram::SaveStateToInterpolateFrom 0x00ADF5B0, CParticleProgram::SimulationStep(simulation, context) 0x00AE2CB0,
//   operator delete 0x00BFE9BC. Global g_ParticleMemoryPool 0x013D2D70 (CMovableResourceMemoryPool*).
// Inline (expanded): FableWin.pdb C3DMeshInfo::GetBestCurrentDetailLevel / HasLOD (0x00B81F67..0x00B81F8E, the unrotated LOD loop),
//   the CCountedPointer<const C3DMesh2> constructor and destructor, STLport vector empty() / size(), CArray::operator[].
// Layout: CParticleContext (vtable 0x012A38E0, six slots) is the polymorphic base at +0 of CEngineSimulationBase (rtti_bases.tsv), so
//   `*this` is the SimulationStep context without adjustment; CEngineClothSimulationMeshBase: ParticleSims +0x0C (Ego_r.pdb
//   CArray<CCountedPointer<CParticleSimulation> >, the retail 12-byte vector), MeshInfo +0xA8, PrimitiveIndex +0xAC (PDB 172 / 176 less
//   the 4 bytes of the shorter retail CArray). C3DClothPrimitive::ParticleProgram +0x04 (Ego_r.pdb). No engine/ header exists for the
//   CEngine*Simulation* classes, CParticleContext or C3DClothPrimitive: modelled here; everything else is the shared headers.
// Shape: the mesh pointer is an ordinary local with plain early returns: VC7.1 expands its destructor at the GetMesh-failure return
//   (cloned test, then a jump into the final release at 0x00B820DA) and at the end, and calls the COMDAT at the empty-primitive return.
//   `this` is spilled at entry because esi is re-used for &ParticleSims. Set-up loop: unsigned index in a frame slot against the count
//   taken once (ebp); the simulation is named before the program. Step loop: the cloth primitive is named before the simulation and
//   its program is read once per call (0x00B8209D, 0x00B820B4).
// Leaf: ?RunSimulation@CEngineClothSimulationStaticMesh@@UAEXXZ
#include "fable_compiler.h"
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/CMatrix3x4.h"
#include "engine/CBankFile.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMeshDataBank.h"
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
class CMatrix3x4;
class C3DClothPrimitive { public:                          // Ego_r.pdb (landed 0x00B812F0 model)
    void*                             __vftable;           // +0x00
    CCountedPointer<CParticleProgram> ParticleProgram;     // +0x04
    unsigned long                     PrimitiveIndex;      // +0x0c
    unsigned long                     MaterialIndex;       // +0x10
};
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;   // 0x013D2D70
class CParticleContext {                                   // vtable 0x012A38E0 (slots 0..5; FableWin.pdb names of the overriders)
public:
    virtual ~CParticleContext();                                         // slot 0
    virtual C3DVector GetGravityVector() const = 0;                      // slot 1
    virtual C3DVector GetWindVector() const = 0;                         // slot 2
    virtual void GetDeltaTransform(CMatrix3x4& transform) const = 0;     // slot 3
    virtual long GetMaximumIterations() const = 0;                       // slot 4
    virtual float GetMaximumDistanceError() const = 0;                   // slot 5
};
class CEngineSimulationBase : public CParticleContext {    // vtable 0x012A38FC
public:
    virtual void RunSimulation() = 0;                      // slot 6
    virtual bool IsRunning() const = 0;                    // slot 7
protected:
    virtual bool CalcAndSetRunState() = 0;                 // slot 8 (+0x20)
    long          RunState;                                // +0x04 ERunState
    unsigned char SimStepOffset;                           // +0x08
};
class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    void ClearParticleSims();                              // 0x00B81D60
protected:
    void UpdateTransform();                                // 0x00B81050
    CArray<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0c
    CMatrix3x4 CurrentTransform;                           // +0x18 (Ego_r.pdb +28)
    CMatrix3x4 LastTransform;                              // +0x48 (Ego_r.pdb +76)
    CMatrix3x4 InverseTransform;                           // +0x78 (Ego_r.pdb +124)
    C3DMeshInfo* MeshInfo;                                 // +0xa8
    unsigned long      PrimitiveIndex;                     // +0xac
};
class CEngineClothSimulationStaticMesh : public CEngineClothSimulationMeshBase {
public:
    virtual void RunSimulation();                          // 0x00B81F30
protected:
    virtual bool CalcAndSetRunState();                     // 0x00B81180
};

void CEngineClothSimulationStaticMesh::RunSimulation()
{
    UpdateTransform();
    if (!CalcAndSetRunState()) {
        ClearParticleSims();
        return;
    }
    CCountedPointer<const C3DMesh2> mesh;
    if (!MeshInfo->GetMesh(mesh, MeshInfo->GetBestCurrentDetailLevel(), 4))
        return;
    C3DPrimitive2& primitive = mesh->Primitives[PrimitiveIndex];
    if (primitive.ClothPrimitives.empty())
        return;
    if (ParticleSims.empty()) {
        unsigned long count = primitive.ClothPrimitives.size();
        ParticleSims.resize(count);
        for (unsigned long i = 0; i < count; ++i) {
            ParticleSims[i].Reset(new CParticleSimulation);
            CParticleSimulation* simulation = ParticleSims[i].Data;
            CParticleProgram* program = primitive.ClothPrimitives[i]->ParticleProgram.Data;
            if (!program->SetupSimulation(*simulation, *g_ParticleMemoryPool)) {
                ClearParticleSims();
                break;
            }
        }
    }
    for (unsigned long i = 0; i < ParticleSims.size(); ++i) {
        C3DClothPrimitive* cloth = primitive.ClothPrimitives[i].Data;
        CParticleSimulation* simulation = ParticleSims[i].Data;
        cloth->ParticleProgram->SaveStateToInterpolateFrom(*simulation);
        cloth->ParticleProgram->SimulationStep(*simulation, *this);
    }
}