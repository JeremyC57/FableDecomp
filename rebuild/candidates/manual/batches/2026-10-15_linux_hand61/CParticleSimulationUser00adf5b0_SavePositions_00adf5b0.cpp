// CParticleSimulationUser_00adf5b0::SavePositions(CParticleSimulation* simulation) -- copy a particle simulation's current positions into its saved-positions buffer (Size C4DVectors)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADF5B0, length 37 bytes (`ret 4`; `this` in ecx is never read).
// The manifest label (COpinionPersonalityDef::operator=) is propagated and wrong: the body is an inline memcpy of
// `Size * 16` bytes from the argument's Positions to its SavedPositions, and its only callers are the cloth simulations'
// RunSimulation (0x00B820A5 in CEngineClothSimulationStaticMesh, 0x00B822C8 in CEngineClothSimulationAnimatedMesh),
// which pass a simulation pointer with an unrelated object in ecx. Layout from the PDB (CParticleSimulation): Size +0x18,
// Positions +0x1C, LastPositions +0x20, SavedPositions +0x24 (C4DVector*, 16-byte elements). Shape: memcpy arguments
// evaluated right to left (size, source, destination), the intrinsic expands to `rep movsd` + `rep movsb`; a
// __thiscall member that ignores `this` still returns with `ret 4`.
// Leaf: SavePositions
#include <string.h>

struct C4DVector {
    float X;
    float Y;
    float Z;
    float W;
};

class CParticleSimulation {
public:
    unsigned char _pad00[0x18];
    unsigned long Size;                            // +0x18
    C4DVector*    Positions;                       // +0x1C
    C4DVector*    LastPositions;                   // +0x20
    C4DVector*    SavedPositions;                  // +0x24
};

class CParticleSimulationUser_00adf5b0 {
public:
    void SavePositions(CParticleSimulation* simulation);
};

void CParticleSimulationUser_00adf5b0::SavePositions(CParticleSimulation* simulation)
{
    memcpy(simulation->SavedPositions, simulation->Positions, simulation->Size * sizeof(C4DVector));
}
