// Behaviour test for 0x00B92300 CEngineSceneGridCell::CreateOcclusionGroup: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct Cell { void* groups[4]; unsigned long occluded; int parentPrims; };
static bool Create(Cell& c, int layer, void* group)
{
    if (c.groups[layer]) return false;
    unsigned long mask = 1u << (layer + 1);
    c.groups[layer] = group; c.parentPrims++; c.occluded |= mask;
    return true;
}
int main() {
    Cell c = { { 0, 0, 0, 0 }, 0, 0 }; int g1, g2;
    int ok = Create(c, 1, &g1) && c.occluded == 4 && !Create(c, 1, &g2) && c.groups[1] == &g1 && Create(c, 2, &g2) && c.occluded == 12 && c.parentPrims == 2;
    std::printf(ok ? "00b92300_TEST PASS\n" : "00b92300_TEST FAIL\n");
    return ok ? 0 : 1;
}
