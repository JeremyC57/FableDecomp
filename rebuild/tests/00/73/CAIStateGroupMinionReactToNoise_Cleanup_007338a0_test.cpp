#include <cstdio>
// creature-mode cleanup (catalogued as CAIStateGroup_MinionReactToNoise::Cleanup): RemoveMode(12), clear two flags
struct Modes { unsigned long removed; void RemoveMode(unsigned long m) { removed = m; } };
struct Mode { Modes* modes; bool flag62; bool flag63; void Cleanup() { modes->RemoveMode(12); flag63 = false; flag62 = false; } };
int main()
{
    Modes m; m.removed = 0; Mode x; x.modes = &m; x.flag62 = true; x.flag63 = true;
    x.Cleanup(); if (m.removed != 12 || x.flag62 || x.flag63) { std::printf("007338a0_TEST FAIL\n"); return 1; }
    std::printf("007338a0_TEST PASS\n"); return 0;
}