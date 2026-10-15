#include <cstdio>
#include <cstring>
// Behaviour test for CParticleSimulationUser_00adf5b0::SavePositions @ 0x00ADF5B0
struct C4DVector { float X, Y, Z, W; };
class CParticleSimulation { public: unsigned long Size; C4DVector* Positions; C4DVector* LastPositions; C4DVector* SavedPositions; };
class CParticleSimulationUser_00adf5b0 {
public:
    void SavePositions(CParticleSimulation* simulation)
    { std::memcpy(simulation->SavedPositions, simulation->Positions, simulation->Size * sizeof(C4DVector)); }
};
int main()
{
    C4DVector pos[3], saved[4];
    for (int i = 0; i < 3; ++i) { pos[i].X = (float)i; pos[i].Y = 2.0f * i; pos[i].Z = 3.0f; pos[i].W = 1.0f; }
    for (int i = 0; i < 4; ++i) { saved[i].X = saved[i].Y = saved[i].Z = saved[i].W = -1.0f; }
    CParticleSimulation sim; sim.Size = 3; sim.Positions = pos; sim.LastPositions = 0; sim.SavedPositions = saved;
    CParticleSimulationUser_00adf5b0 user; user.SavePositions(&sim);
    if (saved[2].Y != 4.0f || saved[0].W != 1.0f || saved[3].X != -1.0f) { std::printf("00adf5b0_TEST FAIL\n"); return 1; }
    std::printf("00adf5b0_TEST PASS\n");
    return 0;
}
