// CParticleProgram::SimulationStep(CParticleSimulation*, CParticleContext*, C3DAnimationBlendState*, C3DAnimationManager*, C3DVector*, C3DMesh2*) -- for a non-empty simulation: dragging and forces, cling the particles to the animated mesh, then run the program for the context's step count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE2D20, length 97 bytes (padded to 112).
// Leaf: ?SimulationStep@CParticleProgram@@QAEXPAUCParticleSimulation@@PAUCParticleContext@@PAUC3DAnimationBlendState
// Callees: CParticleSimulation::ApplyDragging / ApplyForces(CParticleContext*) @ 0x00ADF300 / 0x00ADF760, GFInterpolateVectors
// CParticleProgram::ClingToMesh @ 0x00AE1320 (in place of GFInterpolateVectors @ 0x00A5BEF0), the context's virtual slots 5 (time delta, float) and 4 (step count for a time), the program's
// looped execution @ 0x00AE2510.
// Layout: PDB CParticleSimulation (Size +0x18, Positions +0x1c, SimulationAlphas +0x2c); CParticleProgram's InitialSimulation at +8.
// Shape: the time delta is held in a DOUBLE local (the float return widened, no rounding store) and narrowed
// straight into the pushed argument slot (`push ecx ; fstp [esp]`) after the vtable re-read; a float local spills
// through its frame slot first, the nested call caches the vtable across the inner call.
struct C4DVector { float X, Y, Z, W; };
struct C3DVector;
struct C3DAnimationBlendState;
struct C3DAnimationManager;
struct C3DMesh2;
struct CParticleContext {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long GetStepCount(float time);  // slot 4 (+0x10)
    virtual float GetTimeDelta();           // slot 5 (+0x14)
};
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, C4DVector* a, unsigned long aStride,
                                  C4DVector* b, unsigned long bStride, float* t, unsigned long tStride,
                                  unsigned long count);   // 0x00A5BEF0
struct CParticleSimulation {                // Ego_r.pdb layout (0x80 bytes)
    void*         __vftable;                // +0x00
    unsigned char RenderData[8];            // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                 // +0x0c
    bool          TimestepChanged;          // +0x10
    float         TimestepMultiplier;       // +0x14
    unsigned long Size;                     // +0x18
    C4DVector*    Positions;                // +0x1c
    C4DVector*    LastPositions;            // +0x20
    C4DVector*    SavedPositions;           // +0x24
    C4DVector*    Accelerations;            // +0x28
    float*        SimulationAlphas;         // +0x2c
    unsigned char _rest[0x50];              // +0x30
    void ApplyDragging(CParticleContext* context);   // 0x00ADF300
    void ApplyForces(CParticleContext* context);     // 0x00ADF760
};
struct CParticleProgram {
    void*               __vftable;          // +0x00
    unsigned char       _base_0x04[4];      // +0x04
    CParticleSimulation InitialSimulation;  // +0x08
    void ExecuteLooped(CParticleSimulation* simulation, long steps);   // 0x00AE2510
    void ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animationManager,
                     C3DVector* position, C3DMesh2* mesh);              // 0x00AE1320
    void SimulationStep(CParticleSimulation* simulation, CParticleContext* context);
    void SimulationStep(CParticleSimulation* simulation, CParticleContext* context, C3DAnimationBlendState* blendState,
                        C3DAnimationManager* animationManager, C3DVector* position, C3DMesh2* mesh);
};
void CParticleProgram::SimulationStep(CParticleSimulation* simulation, CParticleContext* context, C3DAnimationBlendState* blendState,
                                      C3DAnimationManager* animationManager, C3DVector* position, C3DMesh2* mesh)
{
    if (simulation->Size) {
        simulation->ApplyDragging(context);
        simulation->ApplyForces(context);
        ClingToMesh(simulation, blendState, animationManager, position, mesh);
        double timeDelta = context->GetTimeDelta();
        ExecuteLooped(simulation, context->GetStepCount((float)timeDelta));
    }
}