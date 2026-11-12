// CParticleProgram::SaveStateToInterpolateFrom(CParticleSimulation& simulation) -- copy the simulation's current positions into its saved-positions buffer (Size C4DVectors), the state the next step interpolates from
// Leaf: ?SaveStateToInterpolateFrom@CParticleProgram@@QAEXAAVCParticleSimulation@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADF5B0, length 37 bytes (`ret 4`; `this` in ecx is never read).
// Name-fix 2026-11-12 (was the stand-in CParticleSimulationUser_00adf5b0::SavePositions(CParticleSimulation*); the older manifest
//   label COpinionPersonalityDef::operator= is propagated): Ego_r.pdb 00beb250 + FableWin.pdb 03337640
//   ?SaveStateToInterpolateFrom@CParticleProgram@@QAEXAAVCParticleSimulation@@@Z, the only CParticleProgram method taking a
//   CParticleSimulation& and returning void; declared in engine/CParticleProgram.h (FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION),
//   and the landed caller CEngineClothSimulationStaticMesh::RunSimulation 0x00B81F30 binds this address under that symbol.
// Body: an inline memcpy of `Size * 16` bytes from Positions (+0x1C) to SavedPositions (+0x24) (CParticleSimulation, friend
//   CParticleProgram); memcpy arguments evaluated right to left, the intrinsic expands to `rep movsd` + `rep movsb`.
#include <string.h>
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"

void CParticleProgram::SaveStateToInterpolateFrom(CParticleSimulation& simulation)
{
    memcpy(simulation.SavedPositions, simulation.Positions, simulation.Size * sizeof(C4DVector));
}
