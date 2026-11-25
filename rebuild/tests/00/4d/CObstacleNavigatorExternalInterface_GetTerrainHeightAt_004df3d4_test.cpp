// Behaviour test for 0x004DF3D4 CObstacleNavigatorExternalInterface_GetTerrainHeightAt: returns 0.0f (retail's pooled __real@00000000, 0x0122DEDC), ret 4.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include <cstdio>
float __stdcall CObstacleNavigatorExternalInterface_GetTerrainHeightAt(void* pos);
int main()
{
    int dummy = 5;
    float r = CObstacleNavigatorExternalInterface_GetTerrainHeightAt(&dummy);
    float r2 = CObstacleNavigatorExternalInterface_GetTerrainHeightAt(0);
    if (r != 0.0f || r2 != 0.0f) { std::printf("BAD %f %f\n", r, r2); return 1; }
    std::printf("OK_0x004df3d4\n");
    return 0;
}
