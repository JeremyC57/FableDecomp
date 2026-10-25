// CParticleProgram::SimulationStep(CParticleSimulation&, const CParticleContext&, const C3DAnimationBlendState&,
// C3DAnimationManager&, const C3DVector*, const C3DMesh2&) const -- for a non-empty simulation: dragging and forces,
// cling the particles to the animated mesh, then apply the constraints for the context's iteration count and time delta.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE2D20, length 97 bytes (padded to 112), `ret 18h`.
// Signatures (Ego_r.pdb): ?SimulationStep@CParticleProgram@@QBEXAAVCParticleSimulation@@ABVCParticleContext@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z,
//   callees ?ApplyDragging@ / ?ApplyForces@CParticleSimulation@@AAEXABVCParticleContext@@@Z (0x00ADF300 / 0x00ADF760),
//   ?ClingToMesh@CParticleProgram@@ABEXAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z
//   (0x00AE1320) and ?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z (0x00AE2510, `ret 0Ch`): the
//   context's slot 5 (float) is evaluated first as the third argument, slot 4 (no argument) gives the long second one.
// Leaf: ?SimulationStep@CParticleProgram@@QBEXAAVCParticleSimulation@@ABVCParticleContext@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z
class C4DVector {
public:
    float X, Y, Z, W;
};
class C3DVector;
class C3DAnimationBlendState;
class C3DAnimationManager;
class C3DMesh2;
class CMovableResourceMemoryPool;
class CParticleSimulationRenderData;

namespace NVectorMath {
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* a, unsigned long aStride,
                                  const C4DVector* b, unsigned long bStride, const float* t, unsigned long tStride,
                                  unsigned long count);   // 0x00A5BEF0
}

class CParticleContext {                    // Ego_r.pdb: sizeof 4 (vptr only); slot 0 is the virtual destructor (??_E)
public:
    virtual ~CParticleContext();
    virtual void Slot1() const;             // slots 1-3 unused here
    virtual void Slot2() const;
    virtual void Slot3() const;
    virtual long GetStepCount() const;      // slot 4 (+0x10), descriptive: constraint iterations for this step
    virtual float GetTimeDelta() const;     // slot 5 (+0x14), descriptive
};

class CMovableResource {                    // retail RTTI base of CParticleSimulation (vtable 0x0129F0C4) and
public:                                     // CParticleProgram (vtable 0x0129F0D8)
    virtual ~CMovableResource();
};

class CParticleSimulation : public CMovableResource {   // Ego_r.pdb layout (0x80 bytes; the base's vptr is +0x00)
    friend class CParticleProgram;
public:
    CParticleSimulationRenderData* RenderData[2];   // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                 // +0x0c
    bool          TimestepChanged;          // +0x10
    float         TimestepMultiplier;       // +0x14
    unsigned long Size;                     // +0x18
    C4DVector*    Positions;                // +0x1c
    C4DVector*    LastPositions;            // +0x20
    C4DVector*    SavedPositions;           // +0x24
    C4DVector*    Accelerations;            // +0x28
    float*        SimulationAlphas;         // +0x2c
    float         MaximumAdjustment;        // +0x30
    float         GravityStrength;          // +0x34
    float         WindStrength;             // +0x38
    bool          DraggingEnable;           // +0x3c
    bool          DraggingRotational;       // +0x3d
    float         DraggingStrength;         // +0x40
    bool          AccelerationEnable;       // +0x44
    float         GlobalDamping;            // +0x48
    double        CurrentTime;              // +0x50
    float         BoundingBox[6];           // +0x58 C3DBoundingBox
    bool          BoundingBoxValid;         // +0x70
    long          MemoryHandle;             // +0x74
    CMovableResourceMemoryPool* MemoryPool; // +0x78
private:
    void ApplyDragging(const CParticleContext& context);   // 0x00ADF300
    void ApplyForces(const CParticleContext& context);     // 0x00ADF760
};

class CParticleProgram : public CMovableResource {   // Ego_r.pdb: InitialSimulation +8 (8-aligned), sizeof 0x170
public:
    CParticleSimulation InitialSimulation;  // +0x08
    void SimulationStep(CParticleSimulation& simulation, const CParticleContext& context) const;
    void SimulationStep(CParticleSimulation& simulation, const CParticleContext& context,
                        const C3DAnimationBlendState& blendState, C3DAnimationManager& animationManager,
                        const C3DVector* position, const C3DMesh2& mesh) const;
private:
    void ApplyConstraints(CParticleSimulation& simulation, long iterations, float timeDelta) const;   // 0x00AE2510
    void ClingToMesh(CParticleSimulation& simulation, const C3DAnimationBlendState& blendState,
                     C3DAnimationManager& animationManager, const C3DVector* position,
                     const C3DMesh2& mesh) const;                                                      // 0x00AE1320
};

void CParticleProgram::SimulationStep(CParticleSimulation& simulation, const CParticleContext& context,
                                      const C3DAnimationBlendState& blendState, C3DAnimationManager& animationManager,
                                      const C3DVector* position, const C3DMesh2& mesh) const
{
    if (simulation.Size) {
        simulation.ApplyDragging(context);
        simulation.ApplyForces(context);
        ClingToMesh(simulation, blendState, animationManager, position, mesh);
        ApplyConstraints(simulation, context.GetStepCount(), context.GetTimeDelta());
    }
}
