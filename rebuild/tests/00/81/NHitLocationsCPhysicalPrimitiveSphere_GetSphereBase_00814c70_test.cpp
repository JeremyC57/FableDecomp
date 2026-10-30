// Behaviour test for 0x00814C70 NHitLocations::CPhysicalPrimitiveSphere::GetSphereBase (see src/00814c70.cpp): with a base
// name set and the named dummy found on the thing's appearance, the dummy position; otherwise the thing's physics position.
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Appearance { bool Has; Vec Dummy; bool GetDummy(Vec& out) const { if (!Has) return false; out = Dummy; return true; } };
struct Physics { Vec Position; };
struct Thing { Physics* PhysicsTC; Appearance* App; };
static Vec GetSphereBase(const char* baseName, const Thing& thing)
{
    const Physics* physics = thing.PhysicsTC;
    Vec pos;
    bool found = false;
    if (baseName[0] != 0) found = thing.App->GetDummy(pos);
    if (!found) pos = physics->Position;
    return pos;
}
int main()
{
    Physics ph = { { 1, 2, 3 } }; Appearance yes = { true, { 7, 8, 9 } }; Appearance no = { false, { 0, 0, 0 } };
    Thing a = { &ph, &yes }; Thing b = { &ph, &no };
    Vec r1 = GetSphereBase("Head", a), r2 = GetSphereBase("Head", b), r3 = GetSphereBase("", a);
    if (r1.X != 7 || r1.Z != 9 || r2.Y != 2 || r3.X != 1 || r3.Z != 3) { std::printf("00814c70_TEST FAIL\n"); return 1; }
    std::printf("00814c70_TEST PASS\n");
    return 0;
}