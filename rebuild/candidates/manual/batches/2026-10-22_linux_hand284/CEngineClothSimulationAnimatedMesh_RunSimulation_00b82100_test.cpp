// Behaviour test for CEngineClothSimulationAnimatedMesh::RunSimulation (0x00B82100): a cloth that should not run clears its
// particle simulations; without a mesh or without particle systems nothing is stepped; the first run creates one simulation per
// particle system (a failed set-up clears them all and stops), then every simulation has its positions saved and is stepped.
#include <cstdio>
#include <vector>

struct Sim { int saved; int steps; };
struct Cloth {
    bool shouldRun, haveMesh; int systems; int failSetupAt;
    std::vector<Sim*> sims; int clears; int setups; int transforms;
    Cloth() : shouldRun(true), haveMesh(true), systems(0), failSetupAt(-1), clears(0), setups(0), transforms(0) { }
    void ClearParticleSims() { for (unsigned long i = 0; i < sims.size(); ++i) delete sims[i]; sims.clear(); ++clears; }
    bool SetupSimulation(int index) { ++setups; return index != failSetupAt; }
    void RunSimulation()
    {
        ++transforms;
        if (!shouldRun) { ClearParticleSims(); return; }
        if (!haveMesh) return;
        if (systems == 0) return;
        if (sims.empty()) {
            unsigned long count = systems;
            sims.resize(count, (Sim*)0);
            for (unsigned long i = 0; i < count; ++i) {
                Sim* s = new Sim; s->saved = 0; s->steps = 0; sims[i] = s;
                if (!SetupSimulation((int)i)) { ClearParticleSims(); break; }
            }
        }
        for (unsigned long i = 0; i < sims.size(); ++i) { ++sims[i]->saved; ++sims[i]->steps; }
    }
};

int main()
{
    int ok = 1;
    Cloth a; a.systems = 3; a.RunSimulation();
    if (a.sims.size() != 3 || a.setups != 3 || a.sims[2]->steps != 1 || a.sims[0]->saved != 1) ok = 0;
    a.RunSimulation();
    if (a.setups != 3 || a.sims[1]->steps != 2) ok = 0;
    a.shouldRun = false; a.RunSimulation();
    if (!a.sims.empty() || a.clears != 1 || a.transforms != 3) ok = 0;
    Cloth b; b.systems = 4; b.failSetupAt = 1; b.RunSimulation();
    if (!b.sims.empty() || b.clears != 1 || b.setups != 2) ok = 0;
    Cloth c; c.systems = 2; c.haveMesh = false; c.RunSimulation();
    if (!c.sims.empty() || c.setups != 0 || c.clears != 0) ok = 0;
    Cloth d; d.systems = 0; d.RunSimulation();
    if (!d.sims.empty() || d.setups != 0) ok = 0;
    a.ClearParticleSims();
    std::printf(ok ? "00b82100_TEST PASS\n" : "00b82100_TEST FAIL\n");
    return ok ? 0 : 1;
}
