#include <cstdio>
// Behaviour model of CTCHeroCentreDoorMarker::~CTCHeroCentreDoorMarker (0x0077F5C0): kill a live particle and clear it.
struct Thing { bool dead; int killed; };
struct Marker { Thing* particle;
    void Destroy() { if (particle && !particle->dead) { ++particle->killed; particle = 0; } } };
int main()
{
    Thing t; t.dead = false; t.killed = 0;
    Marker m; m.particle = &t; m.Destroy();
    if (t.killed != 1 || m.particle) { std::printf("0077f5c0_TEST FAIL\n"); return 1; }
    t.dead = true; m.particle = &t; m.Destroy();
    if (t.killed != 1 || m.particle != &t) { std::printf("0077f5c0_TEST FAIL\n"); return 1; }
    m.particle = 0; m.Destroy();
    std::printf("0077f5c0_TEST PASS\n");
    return 0;
}
