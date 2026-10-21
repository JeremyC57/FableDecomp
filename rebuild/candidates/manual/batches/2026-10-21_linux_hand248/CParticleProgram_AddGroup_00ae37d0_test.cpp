// Behaviour test for 00ae37d0 (CParticleProgram::AddGroup): both group arrays gain one group carrying the source bone;
// every source blend is re-indexed through the particle and vertex remap tables, dropping out-of-range indices and
// entries of -1.
#include <cstdio>

struct Blend { unsigned long index; float value; };
struct Group { long bone; Blend blends[8]; unsigned long n; void Add(const Blend& b) { blends[n++] = b; } };
struct Program {
    long particle[4]; unsigned long nParticle; long vertex[4]; unsigned long nVertex;
    Group sim[4]; unsigned long nSim; Group nonSim[4]; unsigned long nNonSim;
    void AddGroup(const Group& g)
    {
        sim[nSim].n = 0; sim[nSim].bone = -1; ++nSim;
        nonSim[nNonSim].n = 0; nonSim[nNonSim].bone = -1; ++nNonSim;
        Group& s = sim[nSim - 1]; Group& ns = nonSim[nNonSim - 1];
        s.bone = g.bone; ns.bone = g.bone;
        for (unsigned long i = 0; i < g.n; ++i) {
            unsigned long v = g.blends[i].index; float value = g.blends[i].value;
            if (v < nParticle && particle[v] != -1) { Blend b = { (unsigned long)particle[v], value }; s.Add(b); }
            if (v < nVertex && vertex[v] != -1) { Blend b = { (unsigned long)vertex[v], value }; ns.Add(b); }
        }
    }
};
int main()
{
    Program p = {};
    p.particle[0] = 2; p.particle[1] = -1; p.particle[2] = 0; p.nParticle = 3;
    p.vertex[0] = -1; p.vertex[1] = 1; p.nVertex = 2;
    Group g = {}; g.bone = 7;
    Blend b0 = { 0, 0.5f }, b1 = { 1, 0.25f }, b2 = { 2, 1.0f }, b3 = { 9, 0.1f };
    g.Add(b0); g.Add(b1); g.Add(b2); g.Add(b3);
    p.AddGroup(g);
    int ok = p.nSim == 1 && p.nNonSim == 1 && p.sim[0].bone == 7 && p.nonSim[0].bone == 7 &&
             p.sim[0].n == 2 && p.sim[0].blends[0].index == 2 && p.sim[0].blends[1].index == 0 && p.sim[0].blends[1].value == 1.0f &&
             p.nonSim[0].n == 1 && p.nonSim[0].blends[0].index == 1 && p.nonSim[0].blends[0].value == 0.25f;
    if (!ok) { std::printf("00ae37d0_TEST FAIL\n"); return 1; }
    std::printf("00ae37d0_TEST PASS\n");
    return 0;
}
