#include <cstdio>
// CAIStateGroup_MinionReactToNoise::Cleanup: RemoveMode(0x11), InvestigatingNoise = false
struct Modes { unsigned long removed; void RemoveMode(unsigned long m) { removed = m; } };
struct Group { Modes* modes; bool investigatingNoise; void Cleanup() { modes->RemoveMode(0x11); investigatingNoise = false; } };
int main()
{
    Modes m; m.removed = 0; Group g; g.modes = &m; g.investigatingNoise = true;
    g.Cleanup(); if (m.removed != 0x11 || g.investigatingNoise) { std::printf("0091cfa0_TEST FAIL\n"); return 1; }
    std::printf("0091cfa0_TEST PASS\n"); return 0;
}
