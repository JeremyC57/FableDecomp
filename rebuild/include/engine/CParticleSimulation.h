#ifndef FABLE_ENGINE_CPARTICLESIMULATION_H
#define FABLE_ENGINE_CPARTICLESIMULATION_H
// CParticleSimulation (Ego_r TPI, struct_layouts_egor.tsv; RTTI base CMovableResource): RenderData +0x04
// (CCountedPointer<CParticleSimulationRenderData>), Timestep +0x0C, TimestepChanged +0x10, TimestepMultiplier +0x14, Size +0x18,
// Positions +0x1C, LastPositions +0x20, SavedPositions +0x24, Accelerations +0x28, SimulationAlphas +0x2C, MaximumAdjustment +0x30,
// GravityStrength +0x34, WindStrength +0x38, DraggingEnable +0x3C, DraggingRotational +0x3D, DraggingStrength +0x40,
// AccelerationEnable +0x44, GlobalDamping +0x48, CurrentTime +0x50, BoundingBox +0x58, BoundingBoxValid +0x70, MemoryHandle +0x74,
// MemoryPool +0x78; sizeof 0x80 (no container members, so the Ego_r offsets are retail's; 0x00AE0070 reads Size +0x18 /
// Positions +0x1C of CParticleProgram::InitialSimulation at +0x08). Inline accessors (FableWin): GetParticleCount
// ?GetParticleCount@CParticleSimulation@@QBEKXZ, GetParticlePosition ?GetParticlePosition@CParticleSimulation@@QBE?AVC3DVector@@K@Z,
// PeekParticlePositions, SetParticlePosition. CParticleProgram reads the protected members (OptimiseSpringOrder, Size /
// SimulationAlphas): friend.
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCE
#endif
#include "engine/CMovableResource.h"
#include "engine/CCountedPointer.h"
#include "engine/C4DVector.h"
#include "engine/C3DBoundingBox.h"
class CParticleSimulationRenderData;
#ifdef FABLE_RETAIL_CPARTICLESIMULATION
class CParticleDistanceConstraint;
#endif
class CMovableResourceMemoryPool;
class CParticleSimulation : public CMovableResource {
    friend class CParticleProgram;
public:
    CParticleSimulation();
    CParticleSimulation(const CParticleSimulation&);
    virtual ~CParticleSimulation();
    virtual void RelocateData(long,void*,void*,long);
    unsigned long GetParticleCount() const { return Size; }
    C3DVector GetParticlePosition(unsigned long index) const { return Positions[index].ToC3DVector(); }
    const C4DVector* PeekParticlePositions() const { return Positions; }
    void SetParticlePosition(unsigned long index,const C3DVector& vect) { Positions[index].Initialise(vect,1.0f); }
#ifdef FABLE_RETAIL_CPARTICLESIMULATION
    // Chunk 748: FableWin.pdb ?CopyFrom@CParticleSimulation@@QAE_NABV1@AAVCMovableResourceMemoryPool@@@Z, retail 0x00ADF610
    // (called by CParticleProgram::SetupSimulation 0x00AE29D0 with the program's InitialSimulation).
    bool CopyFrom(const CParticleSimulation& other, CMovableResourceMemoryPool& pool);
private:
    // FableWin.pdb ?SatisfyDistanceConstraints@CParticleSimulation@@AAEXPBVCParticleDistanceConstraint@@K@Z, retail 0x00ADF960.
    void SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count);
public:
#endif
    CCountedPointer<CParticleSimulationRenderData> RenderData;
protected:
    float Timestep;
    bool TimestepChanged;
    float TimestepMultiplier;
    unsigned long Size;
    C4DVector* Positions;
    C4DVector* LastPositions;
    C4DVector* SavedPositions;
    C4DVector* Accelerations;
    float* SimulationAlphas;
    float MaximumAdjustment;
    float GravityStrength;
    float WindStrength;
    bool DraggingEnable;
    bool DraggingRotational;
    float DraggingStrength;
    bool AccelerationEnable;
    float GlobalDamping;
private:
    double CurrentTime;
    C3DBoundingBox BoundingBox;
    bool BoundingBoxValid;
    long MemoryHandle;
    CMovableResourceMemoryPool* MemoryPool;
};
FABLE_STATIC_ASSERT(sizeof(CParticleSimulation)==0x80);
#endif
