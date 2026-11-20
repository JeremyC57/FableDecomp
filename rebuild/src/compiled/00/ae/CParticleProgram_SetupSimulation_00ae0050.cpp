// CParticleProgram::SetupSimulation(CParticleSimulation& simulation, CMovableResourceMemoryPool& pool) const -- start a simulation as a
// copy of the program's initial simulation, allocating from the pool; returns CopyFrom's result. Retail 0x00AE0050.
// Leaf: ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@AAVCMovableResourceMemoryPool@@@Z
// Naming (NF173, 2026-11-20): q4885: ported off the local CParticleSimulation / CParticleProgram stand-ins onto the shared
//   engine/CParticleProgram.h (InitialSimulation +0x08; UNSKINNED_SIMULATION declares this overload) and engine/CParticleSimulation.h
//   (FABLE_RETAIL_CPARTICLESIMULATION declares CopyFrom). Callee 0x00ADF610 is Ego_r.pdb 0x00BEB340 / FableWin.pdb 0x03332650
//   ?CopyFrom@CParticleSimulation@@QAE_NABV1@AAVCMovableResourceMemoryPool@@@Z (was the stand-in Init(void*, pool*)); own leaf
//   respelled to Ego_r.pdb 0x00BEBB70 / FableWin.pdb 0x03334FC0 ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@AAVCMovableResourceMemoryPool@@@Z
//   (was the non-const pointer stand-in); its only landed referencer 0x00B81F30 already binds that spelling through the header.
// Inferred: the bool is normalised after the call (retail 0x00AE0062 `test al,al ; setne al`); the plain `return simulation.CopyFrom(...)`
//   grades DIFFER(21v26) (no normalisation), the `? true : false` spelling matches.
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"

bool CParticleProgram::SetupSimulation(CParticleSimulation& simulation, CMovableResourceMemoryPool& pool) const
{
    return simulation.CopyFrom(InitialSimulation, pool) ? true : false;
}
