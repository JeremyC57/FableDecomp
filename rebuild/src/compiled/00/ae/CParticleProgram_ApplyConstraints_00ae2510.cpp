// CParticleProgram::ApplyConstraints(CParticleSimulation& simulation, long iterations, float tolerance) const -- relax the cloth constraints: run the constraint program up to `iterations` times, stopping early once a pass moved no particle by more than `tolerance`
// Leaf: ?ApplyConstraints@CParticleProgram@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE2510, 61 bytes, __fastcall `ret 0Ch`.
// Identity: Ego_r.pdb `?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z` (private const: simulation&,
//   long, float) -- the only 3-argument CParticleProgram member that calls ExecuteProgram. Callers: the two SimulationStep
//   overloads 0x00AE2CB0 / 0x00AE2D20 (landed) and SetupSimulation 0x00AE2C7C, each pushing the context's float time step
//   first, then its iteration count and the simulation.
// Callees: CParticleProgram::ExecuteProgram(CParticleSimulation&) const 0x00AE1A90 (manifest name right).
// Layout: Ego_r.pdb CParticleSimulation -- MaximumAdjustment (float) at +0x30, reset before every pass and raised by the
//   constraint program.
// Label: catalog "ParticleProgram_ExecuteLooped" is descriptive.
// Shape: `for (i = 0; i < iterations; ++i) { simulation.MaximumAdjustment = 0; ExecuteProgram(simulation); if
//   (simulation.MaximumAdjustment <= tolerance) break; }` -- the `<=` test is `test ah,41h ; jnp` (out on less or equal).
class CParticleSimulation {
public:
    void*         __vftable;                // +0x00
    unsigned char RenderData[8];            // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                 // +0x0c
    bool          TimestepChanged;          // +0x10
    float         TimestepMultiplier;       // +0x14
    unsigned long Size;                     // +0x18
    void*         Positions;                // +0x1c C4DVector*
    void*         LastPositions;            // +0x20
    void*         SavedPositions;           // +0x24
    void*         Accelerations;            // +0x28
    float*        SimulationAlphas;         // +0x2c
    float         MaximumAdjustment;        // +0x30
};

class CParticleProgram {
private:
    void ExecuteProgram(CParticleSimulation& simulation) const;                                // 0x00AE1A90
    void ApplyConstraints(CParticleSimulation& simulation, long iterations, float tolerance) const;
};

void CParticleProgram::ApplyConstraints(CParticleSimulation& simulation, long iterations, float tolerance) const
{
    for (long i = 0; i < iterations; ++i) {
        simulation.MaximumAdjustment = 0.0f;
        ExecuteProgram(simulation);
        if (simulation.MaximumAdjustment <= tolerance)
            break;
    }
}