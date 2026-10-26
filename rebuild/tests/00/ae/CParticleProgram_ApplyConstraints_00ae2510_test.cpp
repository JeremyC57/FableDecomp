// behaviour test for 0x00AE2510 CParticleProgram::ApplyConstraints: up to `iterations` passes, stop once the pass's
// maximum adjustment is within the tolerance
#include <cstdio>
class CParticleSimulation { public: float MaximumAdjustment; int Passes; float Adjust[8]; };
class CParticleProgram { public:
    void ExecuteProgram(CParticleSimulation& s) const { s.MaximumAdjustment = s.Adjust[s.Passes]; ++s.Passes; }
    void ApplyConstraints(CParticleSimulation& simulation, long iterations, float tolerance) const
    {
        for (long i = 0; i < iterations; ++i) {
            simulation.MaximumAdjustment = 0.0f;
            ExecuteProgram(simulation);
            if (simulation.MaximumAdjustment <= tolerance)
                break;
        }
    }
};
int main()
{
    CParticleProgram p;
    CParticleSimulation s;
    float adj[8] = { 1.0f, 0.5f, 0.05f, 0.01f, 0.0f, 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < 8; ++i) s.Adjust[i] = adj[i];
    s.Passes = 0;
    p.ApplyConstraints(s, 10, 0.1f);
    if (s.Passes != 3) { std::printf("00ae2510_TEST FAIL\n"); return 1; }
    s.Passes = 0;
    p.ApplyConstraints(s, 2, 0.1f);                 // the iteration cap wins
    if (s.Passes != 2) { std::printf("00ae2510_TEST FAIL\n"); return 1; }
    s.Passes = 0;
    p.ApplyConstraints(s, 0, 0.1f);                 // no passes at all
    if (s.Passes != 0) { std::printf("00ae2510_TEST FAIL\n"); return 1; }
    std::printf("00ae2510_TEST PASS\n");
    return 0;
}