// Behaviour test for 0x00860AD0 CVillageTaskPatrol::IsCreatureMoreAppropriateForTask: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
static bool More(bool locked, float assignedDist, float creatureDist)
{
    if (locked) return false;
    return creatureDist * creatureDist * 1.5625f < assignedDist * assignedDist;
}
int main() {
    int ok = More(false, 10.0f, 5.0f) && !More(false, 10.0f, 9.0f) && !More(true, 10.0f, 1.0f) && More(false, 1.3f, 1.0f);
    std::printf(ok ? "00860ad0_TEST PASS\n" : "00860ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}
